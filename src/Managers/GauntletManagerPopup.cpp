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
    title->setScale(0.75f);
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
    m_mainLayer->addChild(m_panelLayer);

    // New gauntlet button — top right of popup
    auto createMenu = CCMenu::create();
    createMenu->setPosition({m_size.width - 55, m_size.height - 20});
    m_panelLayer->addChild(createMenu);

    auto createSpr = ButtonSprite::create("+ New", "bigFont.fnt", "GJ_button_01.png");
    createSpr->setScale(0.55f);
    auto createBtn = CCMenuItemExt::createSpriteExtra(
        createSpr,
        [this](CCMenuItemSpriteExtra*) { onCreateNew(nullptr); }
    );
    createMenu->addChild(createBtn);

    // List container with dark background
    auto listBg = CCScale9Sprite::create("square02b_001.png");
    listBg->setContentSize({m_size.width - 20, m_size.height - 60});
    listBg->setPosition({m_size.width / 2, m_size.height / 2 - 10});
    listBg->setColor({0, 0, 0});
    listBg->setOpacity(80);
    m_panelLayer->addChild(listBg);

    m_listLayer = CCLayer::create();
    m_listLayer->setPosition({0, 0});
    m_panelLayer->addChild(m_listLayer);
}

void GauntletManagerPopup::fetchGauntlets() {
    m_loadingCircle->setVisible(true);

    m_fetchHolder.spawn(
        GauntletManagerAPI::get()->fetchAll(),
        [this](web::WebResponse res) {
            m_loadingCircle->setVisible(false);
            if (!res.ok()) {
                Notification::create(
                    "Failed to load gauntlets.", NotificationIcon::Error
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

    // Start from near the top of the popup content area
    float yPos = m_size.height - 55.f;

    if (m_gauntlets.empty()) {
        auto label = CCLabelBMFont::create(
            "No gauntlets yet.\nCreate one!", "bigFont.fnt"
        );
        label->setScale(0.45f);
        label->setAlignment(kCCTextAlignmentCenter);
        label->setColor({180, 180, 180});
        label->setPosition({m_size.width / 2, m_size.height / 2 - 10});
        m_listLayer->addChild(label);
        return;
    }

    for (auto const& g : m_gauntlets) {
        buildGauntletRow(g, yPos);
        yPos -= 44.f;
    }
}

void GauntletManagerPopup::buildGauntletRow(CustomGauntletData const& g, float yPos) {
    // Row background
    auto rowBg = CCScale9Sprite::create("square02_001.png");
    rowBg->setContentSize({m_size.width - 30, 38});
    rowBg->setOpacity(60);
    rowBg->setPosition({m_size.width / 2, yPos});
    m_listLayer->addChild(rowBg);

    // Color swatch
    auto swatch = CCScale9Sprite::create("square02_001.png");
    swatch->setContentSize({14, 14});
    swatch->setColor(g.color);
    swatch->setPosition({22, yPos});
    m_listLayer->addChild(swatch);

    // Name
    auto nameLabel = CCLabelBMFont::create(g.name.c_str(), "bigFont.fnt");
    nameLabel->setScale(0.42f);
    nameLabel->setAnchorPoint({0, 0.5f});
    nameLabel->limitLabelWidth(240.f, 0.42f, 0.15f);
    nameLabel->setPosition({35, yPos});
    m_listLayer->addChild(nameLabel);

    // Edit + Delete buttons
    auto rowMenu = CCMenu::create();
    rowMenu->setPosition({m_size.width - 60, yPos});
    m_listLayer->addChild(rowMenu);

    auto editSpr = ButtonSprite::create("Edit", "bigFont.fnt", "GJ_button_02.png");
    editSpr->setScale(0.45f);
    int gid = g.id;
    auto editBtn = CCMenuItemExt::createSpriteExtra(editSpr, [this, gid](CCMenuItemSpriteExtra*) {
        onEdit(gid);
    });
    editBtn->setPositionX(-30);
    rowMenu->addChild(editBtn);

    auto delSpr = ButtonSprite::create("Del", "bigFont.fnt", "GJ_button_06.png");
    delSpr->setScale(0.45f);
    auto delBtn = CCMenuItemExt::createSpriteExtra(delSpr, [this, gid](CCMenuItemSpriteExtra*) {
        onDelete(gid);
    });
    delBtn->setPositionX(30);
    rowMenu->addChild(delBtn);
}

void GauntletManagerPopup::onCreateNew(CCObject*) {
    GauntletEditData empty;
    GauntletEditPopup::create(empty, [this] {
        fetchGauntlets();
    })->show();
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

    GauntletEditPopup::create(data, [this] {
        fetchGauntlets();
    })->show();
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