#include "GauntletManagerPopup.hpp"
#include "GauntletEditPopup.hpp"
#include "../Data/CustomGauntletData.hpp"
#include <argon/argon.hpp>

GauntletManagerPopup* GauntletManagerPopup::create() {
    auto ret = new GauntletManagerPopup();
    if (ret && ret->init(400, 270, "GJ_square05.png")) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GauntletManagerPopup::init(float width, float height, char const* bg) {
    if (!Popup::init(width, height, bg)) return false;

    // Title
    auto title = CCLabelBMFont::create("Gauntlet Manager", "goldFont.fnt");
    title->setPosition(m_size.width / 2, m_size.height - 20);
    title->setScale(0.75);
    m_mainLayer->addChild(title);

    // Loading circle
    m_loadingCircle = LoadingCircle::create();
    m_loadingCircle->setPosition(m_size / 2);
    m_loadingCircle->setVisible(false);
    m_loadingCircle->ignoreAnchorPointForPosition(false);
    m_mainLayer->addChild(m_loadingCircle, 10);

    if (GauntletManagerAPI::get()->isLoggedIn()) {
        buildPanelView();
        fetchGauntlets();
    } else {
        startArgonAuth();
    }

    return true;
}

void GauntletManagerPopup::startArgonAuth() {
    m_loadingCircle->setVisible(true);

    m_argonHolder.spawn(
        argon::startAuth(),
           [this](Result<std::string> result) {
            m_loadingCircle->setVisible(false);

            if (auto label = m_mainLayer->getChildByID("auth-label"))
                label->removeFromParent();

            if (!result.isOk()) {
                Notification::create(
                    fmt::format("Auth failed: {}", result.unwrapErr()),
                    NotificationIcon::Error
                )->show();
                this->onClose(nullptr);
                return;
            }

            auto token = std::move(result).unwrap();
            auto accountID = GJAccountManager::sharedState()->m_accountID;
            m_loadingCircle->setVisible(true);

            m_fetchHolder.spawn(
                web::WebRequest()
                    .header("Authorization", "Bearer " + token)
                    .header("X-Account-Id", std::to_string(accountID))
                    .get("https://bettergauntlets.dev/manage"),
                [this, token](web::WebResponse res) {
                    m_loadingCircle->setVisible(false);

                    if (res.code() == 403) {
                        Notification::create(
                            "You are not a manager.", NotificationIcon::Error
                        )->show();
                        this->onClose(nullptr);
                        return;
                    }
                    if (!res.ok()) {
                        Notification::create(
                            fmt::format("Server error {}", res.code()), NotificationIcon::Error
                        )->show();
                        this->onClose(nullptr);
                        return;
                    }

                    GauntletManagerAPI::get()->setToken(token);

                    auto json = res.json().unwrapOr(matjson::Value());
                    m_gauntlets.clear();
                    if (json.contains("gauntlets") && json["gauntlets"].isArray()) {
                        for (auto const& g : json["gauntlets"]) {
                            CustomGauntletData data;
                            data.id      = g["id"].asInt().unwrapOr(0);
                            data.name    = g["name"].asString().unwrapOr("");
                            data.iconURL = g["icon_url"].asString().unwrapOr("");
                            data.color   = {
                                (GLubyte)g["color_r"].asInt().unwrapOr(255),
                                (GLubyte)g["color_g"].asInt().unwrapOr(255),
                                (GLubyte)g["color_b"].asInt().unwrapOr(255)
                            };
                            m_gauntlets.push_back(data);
                        }
                    }
                    buildPanelView();
                    buildGauntletList();
                }
            );
        }
    );
}

void GauntletManagerPopup::buildPanelView() {
    m_panelLayer = CCLayer::create();
    m_panelLayer->setPosition({0, 0});
    m_panelLayer->setContentSize(m_mainLayer->getContentSize());
    m_panelLayer->setID("panel");
    m_mainLayer->addChild(m_panelLayer);

    // New gauntlet button — top right of popup
    auto createMenu = CCMenu::create();
    createMenu->setPosition({m_size.width - 55, m_size.height - 20});
    m_panelLayer->addChild(createMenu);

    auto createSpr = ButtonSprite::create("Create", "bigFont.fnt", "GJ_button_01.png");
    createSpr->setScale(0.55f);
    auto createBtn = CCMenuItemExt::createSpriteExtra(
        createSpr,
        [this](CCMenuItemSpriteExtra*) { onCreateNew(nullptr); }
    );
    createMenu->addChild(createBtn);

    // List container with dark background
    m_listBG = NineSlice::create("square02b_001.png");
    m_listBG->setContentSize({m_size.width - 40, m_size.height - 60});
    m_listBG->setPosition({m_size.width / 2, m_size.height / 2 - 10});
    m_listBG->setColor({ 0, 0, 0 });
    m_listBG->setOpacity(80);
    m_panelLayer->addChild(m_listBG);

    m_listLayer = CCLayer::create();
    m_listLayer->setPosition({20, 20});
    m_listLayer->setID("list");
    m_listLayer->setContentSize(m_listBG->getContentSize());
    m_panelLayer->addChild(m_listLayer);
}

void GauntletManagerPopup::fetchGauntlets() {
    m_loadingCircle->setVisible(true);

    m_fetchHolder.spawn(
        GauntletManagerAPI::get()->fetchAll(),
        [this](web::WebResponse res) {
            m_loadingCircle->setVisible(false);

            // Token expired — clear it and re-authenticate
            if (res.code() == 401 || res.code() == 403) {
                GauntletManagerAPI::get()->setToken("");
                startArgonAuth();
                return;
            }

            if (!res.ok()) {
                Notification::create(
                    fmt::format("Failed to load gauntlets. ({})", res.code()),
                    NotificationIcon::Error
                )->show();
                return;
            }

            auto json = res.json().unwrapOr(matjson::Value());
            m_gauntlets.clear();

            if (json.contains("gauntlets") && json["gauntlets"].isArray()) {
                for (auto const& g : json["gauntlets"]) {
                    CustomGauntletData data;
                    data.id      = g["id"].asInt().unwrapOr(0);
                    data.name    = g["name"].asString().unwrapOr("");
                    data.iconURL = g["icon_url"].asString().unwrapOr("");
                    data.color   = {
                        (GLubyte)g["color_r"].asInt().unwrapOr(255),
                        (GLubyte)g["color_g"].asInt().unwrapOr(255),
                        (GLubyte)g["color_b"].asInt().unwrapOr(255)
                    };
                    m_gauntlets.push_back(data);
                }
            }

            buildGauntletList();
        }
    );
}

void GauntletManagerPopup::buildGauntletList() {
    if (!m_listLayer) return;
    m_listLayer->removeAllChildren();

    m_gauntletList = CCMenu::create();
    m_gauntletList->setID("gauntlet-list");
    m_gauntletList->setContentSize(m_listLayer->getContentSize());
    m_gauntletList->setPosition({m_size.width / 2 - 20, m_size.height / 2 - 30});
    m_gauntletList->setLayout(ColumnLayout::create()
        ->setGap(0)
        ->setAxisReverse(true)
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::End)
    );
    m_listLayer->addChild(m_gauntletList);
    // float yPos = m_size.height - 55;

    // Staged rows first
    for (int i = 0; i < (int)m_staged.size(); i++) {
        buildStagedRow(m_staged[i], i);
    }

    // Then server rows
    for (auto const& g : m_gauntlets) {
        buildGauntletRow(g);
    }

    m_gauntletList->updateLayout();

    if (m_gauntlets.empty() && m_staged.empty()) {
        auto label = CCLabelBMFont::create("No gauntlets yet.\nCreate one!", "bigFont.fnt");
        label->setScale(0.45);
        label->setAlignment(kCCTextAlignmentCenter);
        label->setColor({180, 180, 180});
        label->setPosition({m_size.width / 2, m_size.height / 2 - 10});
        m_panelLayer->addChild(label);
        return;
    }

    // for (auto const& g : m_gauntlets) {
    //     buildGauntletRow(g, yPos);
    //     yPos -= 44.f;
    // }
}

void GauntletManagerPopup::buildGauntletRow(CustomGauntletData const& g) {
    auto row = CCNode::create();
    row->setContentSize({m_gauntletList->getContentWidth(), 60});

    // Background
    auto rowBg = CCSprite::createWithSpriteFrameName("d_largeSquare_01_001.png");
    rowBg->setContentSize(row->getContentSize());
    rowBg->setOpacity(60);
    rowBg->setAnchorPoint({0, 0});
    row->addChild(rowBg);

    // Color swatch
    auto swatch = CCScale9Sprite::create("square02_001.png");
    swatch->setContentSize({10, 10});
    swatch->setColor(g.color);
    swatch->setPosition({12, row->getContentHeight() / 2});
    row->addChild(swatch);

    // Name
    auto nameLabel = CCLabelBMFont::create(
        fmt::format("{} Gauntlet", g.name).c_str(), "bigFont.fnt");
    nameLabel->setScale(0.38f);
    nameLabel->setAnchorPoint({0, 0.5f});
    nameLabel->limitLabelWidth(180.f, 0.38f, 0.1f);
    nameLabel->setPosition({22, row->getContentHeight() / 2 + 6});
    row->addChild(nameLabel);

    // Buttons
    int gid = g.id;
    m_actionMenu = CCMenu::create();
    m_actionMenu->setPosition({row->getContentWidth() - 45, row->getContentHeight() / 2});
    m_actionMenu->setLayout(RowLayout::create()
        ->setGap(5)
        ->setAxisAlignment(AxisAlignment::End)
    );
    row->addChild(m_actionMenu);

    auto editSpr = CCSprite::createWithSpriteFrameName("GR_editBtn_001.png"_spr);
    editSpr->setScale(0.45f);
    auto editBtn = CCMenuItemExt::createSpriteExtra(editSpr,
        [this, gid](CCMenuItemSpriteExtra*) { onEdit(gid); }
    );
    editBtn->setPositionX(-25);
    
    auto delSpr = CCSprite::createWithSpriteFrameName("GR_deleteBtn_001.png"_spr);
    delSpr->setScale(0.45f);
    auto delBtn = CCMenuItemExt::createSpriteExtra(delSpr,
        [this, gid](CCMenuItemSpriteExtra*) { onDelete(gid); }
    );
    delBtn->setPositionX(25);
        
    m_actionMenu->addChild(editBtn);
    m_actionMenu->addChild(delBtn);

    m_actionMenu->updateLayout();

    m_gauntletList->addChild(row);
}

void GauntletManagerPopup::onEdit(int gauntletId) {
    auto it = std::find_if(m_gauntlets.begin(), m_gauntlets.end(),
        [gauntletId](auto const& g) { return g.id == gauntletId; });
    if (it == m_gauntlets.end()) return;

    GauntletEditData data;
    data.id      = it->id;
    data.name    = it->name;
    data.iconURL = it->iconURL;
    data.bgColor = it->color;

    GauntletEditPopup::create(data, [this](GauntletEditData const& updated) {
        fetchGauntlets();
    })->show();
}

void GauntletManagerPopup::onCreateNew(CCObject*) {
    GauntletEditData empty;
    GauntletEditPopup::create(empty, [this](GauntletEditData const& data) {
        m_staged.push_back(data);
        buildGauntletList();
        Notification::create(
            fmt::format("\"{}\" staged.", data.name),
            NotificationIcon::Success
        )->show();
    })->show();
}

void GauntletManagerPopup::onEditStaged(int index) {
    if (index < 0 || index >= (int)m_staged.size()) return;
    auto existing = m_staged[index];
    GauntletEditPopup::create(existing, [this, index](GauntletEditData const& data) {
        m_staged[index] = data;
        buildGauntletList();
    })->show();
}

void GauntletManagerPopup::buildStagedRow(GauntletEditData const& g, int index) {
    auto row = CCNode::create();
    row->setContentSize({m_gauntletList->getContentWidth(), 38});

    // Accent 1 background
    auto rowBg = CCScale9Sprite::create("square02_001.png");
    rowBg->setContentSize(row->getContentSize());
    rowBg->setColor(g.accentColor1);
    rowBg->setOpacity(120);
    rowBg->setAnchorPoint({0, 0});
    row->addChild(rowBg);

    // Accent 2 overlay on right half
    auto accent2 = CCScale9Sprite::create("square02_001.png");
    accent2->setContentSize({row->getContentWidth() / 2, row->getContentHeight()});
    accent2->setColor(g.accentColor2);
    accent2->setOpacity(120);
    accent2->setAnchorPoint({1, 0});
    accent2->setPosition({row->getContentWidth(), 0});
    row->addChild(accent2);

    // STAGED badge
    auto badge = CCLabelBMFont::create("STAGED", "bigFont.fnt");
    badge->setScale(0.25f);
    badge->setColor({255, 220, 50});
    badge->setAnchorPoint({0, 0.5f});
    badge->setPosition({6, row->getContentHeight() / 2 + 8});
    row->addChild(badge);

    // Name
    auto nameLabel = CCLabelBMFont::create(
        fmt::format("{} Gauntlet", g.name).c_str(), "bigFont.fnt");
    nameLabel->setScale(0.35f);
    nameLabel->setColor(g.nameColor);
    nameLabel->setAnchorPoint({0, 0.5f});
    nameLabel->limitLabelWidth(160.f, 0.35f, 0.1f);
    nameLabel->setPosition({6, row->getContentHeight() / 2 - 6});
    row->addChild(nameLabel);

    // Buttons
    auto rowMenu = CCMenu::create();
    rowMenu->setPosition({row->getContentWidth() - 55, row->getContentHeight() / 2});
    row->addChild(rowMenu);

    auto deleteSpr = ButtonSprite::create("Del", "bigFont.fnt", "GJ_button_06.png");
    deleteSpr->setScale(0.4f);
    auto deleteBtn = CCMenuItemExt::createSpriteExtra(deleteSpr,
        [this, index](CCMenuItemSpriteExtra*) {
            m_staged.erase(m_staged.begin() + index);
            buildGauntletList();
        });
    deleteBtn->setPositionX(-38);
    rowMenu->addChild(deleteBtn);

    auto editSpr = ButtonSprite::create("Edit", "bigFont.fnt", "GJ_button_02.png");
    editSpr->setScale(0.4f);
    auto editBtn = CCMenuItemExt::createSpriteExtra(editSpr,
        [this, index](CCMenuItemSpriteExtra*) { onEditStaged(index); });
    editBtn->setPositionX(0);
    rowMenu->addChild(editBtn);

    auto pushSpr = ButtonSprite::create("Push", "bigFont.fnt", "GJ_button_01.png");
    pushSpr->setScale(0.4f);
    auto pushBtn = CCMenuItemExt::createSpriteExtra(pushSpr,
        [this, index](CCMenuItemSpriteExtra*) { onPushStaged(index); });
    pushBtn->setPositionX(38);
    rowMenu->addChild(pushBtn);

    m_gauntletList->addChild(row);
}

void GauntletManagerPopup::onPushStaged(int index) {
    if (index < 0 || index >= (int)m_staged.size()) return;
    auto data = m_staged[index];

    createQuickPopup(
        "Push Gauntlet",
        fmt::format("Push <cy>{}</c> to the server?", data.name).c_str(),
        "Cancel", "Push",
        [this, data, index](FLAlertLayer*, bool confirmed) {
            if (!confirmed) return;
            m_loadingCircle->setVisible(true);

            auto future = data.id == 0
                ? GauntletManagerAPI::get()->create(data)
                : GauntletManagerAPI::get()->update(data);

            m_pushHolder.spawn(std::move(future), [this, index](web::WebResponse res) {
                m_loadingCircle->setVisible(false);
                if (!res.ok()) {
                    Notification::create(
                        fmt::format("Push failed: HTTP {}", res.code()),
                        NotificationIcon::Error
                    )->show();
                    return;
                }
                m_staged.erase(m_staged.begin() + index);
                Notification::create("Gauntlet pushed!", NotificationIcon::Success)->show();
                fetchGauntlets(); // refresh server list
            });
        }
    );
}

void GauntletManagerPopup::onDelete(int gauntletId) {
    createQuickPopup(
        "Delete Gauntlet",
        "Are you sure? This <cr>cannot</c> be undone.",
        "Cancel", "Delete",
        [this, gauntletId](FLAlertLayer*, bool confirmed) {
            if (!confirmed) return;
            m_loadingCircle->setVisible(true);
            m_deleteHolder.spawn(
                GauntletManagerAPI::get()->remove(gauntletId),
                [this](web::WebResponse res) {
                    m_loadingCircle->setVisible(false);
                    if (!res.ok()) {
                        Notification::create("Delete failed.", NotificationIcon::Error)->show();
                        return;
                    }
                    Notification::create("Gauntlet deleted.", NotificationIcon::Success)->show();
                    fetchGauntlets();
                }
            );
        }
    );
}