#include "GauntletManagerPopup.hpp"
#include "GauntletEditPopup.hpp"
#include "../Data/CustomGauntletData.hpp"
#include <argon/argon.hpp>

static CustomGauntletData parseGauntletJson(matjson::Value const& g) {
    CustomGauntletData data;
    data.id          = g["id"].asInt().unwrapOr(0);
    data.name        = g["name"].asString().unwrapOr("");
    data.description = g["description"].asString().unwrapOr("");
    data.iconURL     = g["icon_url"].asString().unwrapOr("");
    data.bgIndex     = g["background"].asInt().unwrapOr(1);
    data.nameColor   = {
        (GLubyte)g["name_color_r"].asInt().unwrapOr(255),
        (GLubyte)g["name_color_g"].asInt().unwrapOr(255),
        (GLubyte)g["name_color_b"].asInt().unwrapOr(255)
    };
    data.nodeColor   = {
        (GLubyte)g["node_color_r"].asInt().unwrapOr(255),
        (GLubyte)g["node_color_g"].asInt().unwrapOr(255),
        (GLubyte)g["node_color_b"].asInt().unwrapOr(255)
    };
    data.bgColor     = {
        (GLubyte)g["color_r"].asInt().unwrapOr(255),
        (GLubyte)g["color_g"].asInt().unwrapOr(255),
        (GLubyte)g["color_b"].asInt().unwrapOr(255)
    };
    data.accentColor1 = {
        (GLubyte)g["accent_color1_r"].asInt().unwrapOr(255),
        (GLubyte)g["accent_color1_g"].asInt().unwrapOr(255),
        (GLubyte)g["accent_color1_b"].asInt().unwrapOr(255)
    };
    data.accentColor2 = {
        (GLubyte)g["accent_color2_r"].asInt().unwrapOr(255),
        (GLubyte)g["accent_color2_g"].asInt().unwrapOr(255),
        (GLubyte)g["accent_color2_b"].asInt().unwrapOr(255)
    };
    data.infoDate      = g["info_date"].asString().unwrapOr("");
    data.infoVersion   = g["info_version"].asString().unwrapOr("");
    data.infoSuggester = g["info_suggester"].asString().unwrapOr("");
    data.infoAccID     = g["info_acc_id"].asInt().unwrapOr(0);

    if (g.contains("levels") && g["levels"].isArray()) {
        int i = 0;
        for (auto const& lvl : g["levels"]) {
            if (i >= 5) break;
            log::debug("parseGauntletJson: level[{}] raw json: {}", i, lvl.dump());
            data.levels[i].id      = lvl["level_id"].asInt().unwrapOr(0);
            data.levels[i].name    = lvl["level_name"].asString().unwrapOr("");
            data.levels[i].creator = lvl["creator"].asString().unwrapOr("");
            data.levels[i].stars   = lvl["stars"].asInt().unwrapOr(0);
            i++;
        }
    } else {
        log::debug("parseGauntletJson: '{}' has no levels array (contains={})", data.name, g.contains("levels"));
    }
    return data;
}

static std::vector<CustomGauntletData> parseGauntletsResponse(matjson::Value const& json) {
    std::vector<CustomGauntletData> result;

    // Server returns levels as a separate top-level array keyed by gauntlet_id.
    // Group them so parseGauntletJson can find them nested under each gauntlet.
    std::unordered_map<int, std::vector<matjson::Value const*>> levelsByGauntlet;
    if (json.contains("levels") && json["levels"].isArray()) {
        for (auto const& lvl : json["levels"]) {
            int gid = lvl["gauntlet_id"].asInt().unwrapOr(0);
            if (gid) levelsByGauntlet[gid].push_back(&lvl);
        }
    }

    if (json.contains("gauntlets") && json["gauntlets"].isArray()) {
        for (auto const& g : json["gauntlets"]) {
            int gid = g["id"].asInt().unwrapOr(0);
            auto copy = g;
            if (levelsByGauntlet.count(gid)) {
                auto arr = matjson::Value::array();
                for (auto* lvl : levelsByGauntlet[gid]) {
                    arr.push(*lvl);
                }
                copy["levels"] = arr;
            }
            result.push_back(parseGauntletJson(copy));
        }
    }
    return result;
}

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
        loadStaged();
        buildPanelView();
        fetchGauntlets();
    } else {
        loadStaged();
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
                    m_gauntlets = parseGauntletsResponse(json);
                    buildPanelView();
                    buildGauntletList();
                }
            );
        }
    );
}

void GauntletManagerPopup::saveStaged() {
    auto arr = matjson::Value::array();
    for (auto const& g : m_staged) {
        auto levels = matjson::Value::array();
        for (auto const& slot : g.levels) {
            auto lvl = matjson::Value();
            lvl["id"]      = slot.id;
            lvl["name"]    = slot.name;
            lvl["creator"] = slot.creator;
            lvl["stars"]   = slot.stars;
            levels.push(lvl);
        }
        auto obj = matjson::Value();
        obj["id"]              = g.id;
        obj["name"]            = g.name;
        obj["description"]     = g.description;
        obj["iconURL"]         = g.iconURL;
        obj["nameColor_r"]     = (int)g.nameColor.r;
        obj["nameColor_g"]     = (int)g.nameColor.g;
        obj["nameColor_b"]     = (int)g.nameColor.b;
        obj["nodeColor_r"]     = (int)g.nodeColor.r;
        obj["nodeColor_g"]     = (int)g.nodeColor.g;
        obj["nodeColor_b"]     = (int)g.nodeColor.b;
        obj["bgColor_r"]       = (int)g.bgColor.r;
        obj["bgColor_g"]       = (int)g.bgColor.g;
        obj["bgColor_b"]       = (int)g.bgColor.b;
        obj["bgIndex"]         = g.bgIndex;
        obj["accentColor1_r"]  = (int)g.accentColor1.r;
        obj["accentColor1_g"]  = (int)g.accentColor1.g;
        obj["accentColor1_b"]  = (int)g.accentColor1.b;
        obj["accentColor2_r"]  = (int)g.accentColor2.r;
        obj["accentColor2_g"]  = (int)g.accentColor2.g;
        obj["accentColor2_b"]  = (int)g.accentColor2.b;
        obj["infoDate"]        = g.infoDate;
        obj["infoVersion"]     = g.infoVersion;
        obj["infoSuggester"]   = g.infoSuggester;
        obj["infoAccID"]       = g.infoAccID;
        obj["levels"]          = levels;
        arr.push(obj);
    }
    Mod::get()->setSavedValue("staged-gauntlets", arr);
}

void GauntletManagerPopup::loadStaged() {
    m_staged.clear();
    auto saved = Mod::get()->getSavedValue<matjson::Value>(
        "staged-gauntlets", matjson::Value::array()
    );
    if (!saved.isArray()) return;

    for (auto const& obj : saved) {
        GauntletEditData g;
        g.id          = obj["id"].asInt().unwrapOr(0);
        g.name        = obj["name"].asString().unwrapOr("");
        g.description = obj["description"].asString().unwrapOr("");
        g.iconURL     = obj["iconURL"].asString().unwrapOr("");
        g.nameColor   = {
            (GLubyte)obj["nameColor_r"].asInt().unwrapOr(255),
            (GLubyte)obj["nameColor_g"].asInt().unwrapOr(255),
            (GLubyte)obj["nameColor_b"].asInt().unwrapOr(255)
        };
        g.nodeColor   = {
            (GLubyte)obj["nodeColor_r"].asInt().unwrapOr(255),
            (GLubyte)obj["nodeColor_g"].asInt().unwrapOr(255),
            (GLubyte)obj["nodeColor_b"].asInt().unwrapOr(255)
        };
        g.bgColor     = {
            (GLubyte)obj["bgColor_r"].asInt().unwrapOr(255),
            (GLubyte)obj["bgColor_g"].asInt().unwrapOr(255),
            (GLubyte)obj["bgColor_b"].asInt().unwrapOr(255)
        };
        g.bgIndex     = obj["bgIndex"].asInt().unwrapOr(1);
        g.accentColor1 = {
            (GLubyte)obj["accentColor1_r"].asInt().unwrapOr(255),
            (GLubyte)obj["accentColor1_g"].asInt().unwrapOr(255),
            (GLubyte)obj["accentColor1_b"].asInt().unwrapOr(255)
        };
        g.accentColor2 = {
            (GLubyte)obj["accentColor2_r"].asInt().unwrapOr(255),
            (GLubyte)obj["accentColor2_g"].asInt().unwrapOr(255),
            (GLubyte)obj["accentColor2_b"].asInt().unwrapOr(255)
        };
        g.infoDate      = obj["infoDate"].asString().unwrapOr("");
        g.infoVersion   = obj["infoVersion"].asString().unwrapOr("");
        g.infoSuggester = obj["infoSuggester"].asString().unwrapOr("");
        g.infoAccID     = obj["infoAccID"].asInt().unwrapOr(0);

        if (obj.contains("levels") && obj["levels"].isArray()) {
            int i = 0;
            for (auto const& lvl : obj["levels"]) {
                if (i >= 5) break;
                g.levels[i].id      = lvl["id"].asInt().unwrapOr(0);
                g.levels[i].name    = lvl["name"].asString().unwrapOr("");
                g.levels[i].creator = lvl["creator"].asString().unwrapOr("");
                g.levels[i].stars   = lvl["stars"].asInt().unwrapOr(0);
                i++;
            }
        }
        m_staged.push_back(g);
    }
}

void GauntletManagerPopup::buildPanelView() {
    if (m_panelLayer) {
        m_panelLayer->removeFromParent();
        m_panelLayer = nullptr;
        m_listLayer = nullptr;
        m_listBG = nullptr;
        m_gauntletList = nullptr;
    }

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

    auto listOutline = NineSlice::create("GJ_square07.png");
    listOutline->setContentSize({m_size.width - 38, m_size.height - 54});
    listOutline->setPosition({m_size.width / 2, m_size.height / 2 - 10});
    listOutline->setColor({67, 67, 67});
    m_panelLayer->addChild(listOutline, 1);

    auto listClip = CCClippingNode::create(m_listBG);
    listClip->setAlphaThreshold(0);
    m_panelLayer->addChild(listClip);

    m_listLayer = CCLayer::create();
    m_listLayer->setPosition({20, 20});
    m_listLayer->setID("list");
    m_listLayer->setContentSize(m_listBG->getContentSize());
    listClip->addChild(m_listLayer);
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
            log::debug("fetchGauntlets response: {}", json.dump());
            m_gauntlets = parseGauntletsResponse(json);

            buildGauntletList();
        }
    );
}

void GauntletManagerPopup::buildGauntletList() {
    if (!m_listLayer) return;
    m_rowIconHolders.clear();
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

    auto scroll = ScrollLayer::create(m_gauntletList->getContentSize(), true, true);
    scroll->m_contentLayer->addChild(m_gauntletList);

    m_listLayer->addChild(scroll);
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
    //    buildGauntletRow(g, yPos);
    //    yPos -= 44.f;
    // }
}

// Already added Gauntlets

void GauntletManagerPopup::buildGauntletRow(CustomGauntletData const& g) {
    auto row = CCNode::create();
    row->setContentSize({m_gauntletList->getContentWidth(), 60});

    // Background with gauntlet's bg color
    auto accent1 = CCScale9Sprite::create("square.png");
    accent1->setContentSize(row->getContentSize());
    accent1->setColor(g.bgColor);
    accent1->setAnchorPoint({0, 0});
    row->addChild(accent1);

    // Gradient overlay with name color
    auto accent2 = CCSprite::createWithSpriteFrameName("GR_pureGradient_001.png"_spr);
    accent2->setScaleX(2.5);
    accent2->setScaleY(0.475);
    accent2->setColor(g.nameColor);
    accent2->setOpacity(120);
    accent2->setAnchorPoint({1, 0});
    accent2->setPosition({row->getContentWidth(), 0});
    row->addChild(accent2);

    // Icon placeholder — filled async below
    auto iconNode = CCNode::create();
    iconNode->setID("icon-node");
    iconNode->setContentSize({50, 50});
    iconNode->setPosition({30, row->getContentHeight() / 2});
    iconNode->setAnchorPoint({0.5f, 0.5f});

    auto loadingIcon = LoadingCircle::create();
    loadingIcon->setID("loading-circle");
    loadingIcon->setPosition(iconNode->getContentSize() / 2);
    loadingIcon->setContentSize({50, 50});
    loadingIcon->setOpacity(120);
    loadingIcon->setAnchorPoint({0.5f, 0.5f});
    iconNode->addChild(loadingIcon);
    row->addChild(iconNode);

    // Name with gauntlet's name color
    auto nameLabel = CCLabelBMFont::create(fmt::format("{} Gauntlet", g.name).c_str(), "bigFont.fnt");
    nameLabel->setScale(0.575);
    nameLabel->setColor(g.nameColor);
    nameLabel->setAnchorPoint({0, 0.5f});
    nameLabel->setPosition({58, row->getContentHeight() / 2 + 17.5f});
    row->addChild(nameLabel, 1);

    auto nameShadow = CCLabelBMFont::create(fmt::format("{} Gauntlet", g.name).c_str(), "bigFont.fnt");
    nameShadow->setScale(0.575);
    nameShadow->setID("gauntlet-name-shadow");
    nameShadow->setColor({0, 0, 0});
    nameShadow->setOpacity(60);
    nameShadow->setAnchorPoint({0, 0.5});
    nameShadow->setPosition({nameLabel->getPositionX() + 2, nameLabel->getPositionY() - 2});
    row->addChild(nameShadow, 0);

    auto descBox = NineSlice::create("square02b_small.png");
    descBox->setContentSize({185, 25});
    descBox->setAnchorPoint({0, 0});
    descBox->setPosition(nameLabel->getPositionX(), row->getContentHeight() / 2 - 22.5);
    descBox->setColor({0, 0, 0});
    descBox->setOpacity(80);
    row->addChild(descBox);

    auto description = TextArea::create(
        g.description,
        "chatFont.fnt",
        0.5,
        160,
        {0, 0.5},
        7.5,
        false
    );
    description->setAnchorPoint({0, 0});
    description->setContentSize({180, 20});
    description->setPosition({descBox->getPositionX() + 2.5f, (descBox->getPositionY() + descBox->getContentHeight() / 2)});
    row->addChild(description);

    // Buttons — local menu, not m_actionMenu
    int gid = g.id;
    auto actionMenu = CCMenu::create();
    actionMenu->setAnchorPoint({1, 0.5f});
    actionMenu->setPosition({row->getContentWidth() - 12, row->getContentHeight() / 2});
    actionMenu->setLayout(RowLayout::create()->setGap(5)->setAxisAlignment(AxisAlignment::End));

    auto editSpr = CCSprite::createWithSpriteFrameName("GR_editBtn_001.png"_spr);
    editSpr->setScale(0.85f);
    auto editBtn = CCMenuItemExt::createSpriteExtra(editSpr,
        [this, gid](CCMenuItemSpriteExtra*) { onEdit(gid); });
    actionMenu->addChild(editBtn);

    auto delSpr = CCSprite::createWithSpriteFrameName("GR_deleteBtn_001.png"_spr);
    delSpr->setScale(0.85f);
    auto delBtn = CCMenuItemExt::createSpriteExtra(delSpr,
        [this, gid](CCMenuItemSpriteExtra*) { onDelete(gid); });
    actionMenu->addChild(delBtn);

    actionMenu->updateLayout();
    row->addChild(actionMenu);

    m_gauntletList->addChild(row);

    // Async icon load
    if (!g.iconURL.empty()) {
        m_rowIconHolders.emplace_back();
        m_rowIconHolders.back().spawn(
            web::WebRequest().get(g.iconURL),
            [iconNode](web::WebResponse res) {
                if (!res.ok()) return;
                auto bytes = res.data();
                queueInMainThread([iconNode, bytes]() {
                    if (!iconNode) return;
                    auto img = new CCImage();
                    if (!img->initWithImageData(
                            const_cast<unsigned char*>(bytes.data()), bytes.size())) {
                        delete img; return;
                    }
                    auto tex = new CCTexture2D();
                    tex->initWithImage(img);
                    delete img;
                    auto icon = CCSprite::createWithTexture(tex);
                    auto iconShadow = CCSprite::createWithTexture(tex);
                    tex->release();
                    icon->setScale(iconNode->getContentHeight() * 1.15 / icon->getContentHeight() * 1.15);
                    icon->setPosition(iconNode->getContentSize() / 2);
                    icon->setAnchorPoint({0.5f, 0.5f});
                    iconShadow->setScaleX(icon->getScaleX());
                    iconShadow->setScaleY(icon->getScaleY() * 1.2);
                    iconShadow->setPosition({icon->getPositionX(), icon->getPositionY() - 5});
                    iconShadow->setAnchorPoint({0.5f, 0.5f});
                    iconShadow->setColor({0, 0, 0});
                    iconShadow->setOpacity(50);
                    if (auto ph = iconNode->getChildByID("icon-placeholder"))
                        ph->removeFromParent();
                    iconNode->addChild(icon, 1);
                    iconNode->addChild(iconShadow, 0);
                });
            }
        );
    }
}

void GauntletManagerPopup::onEdit(int gauntletId) {
    auto it = std::find_if(m_gauntlets.begin(), m_gauntlets.end(),
        [gauntletId](auto const& g) { return g.id == gauntletId; });
    if (it == m_gauntlets.end()) return;

    GauntletEditData data;
    data.id            = it->id;
    data.name          = it->name;
    data.description   = it->description;
    data.iconURL       = it->iconURL;
    data.bgIndex       = it->bgIndex;
    data.nameColor     = it->nameColor;
    data.nodeColor     = it->nodeColor;
    data.bgColor       = it->bgColor;
    data.accentColor1  = it->accentColor1;
    data.accentColor2  = it->accentColor2;
    data.infoDate      = it->infoDate;
    data.infoVersion   = it->infoVersion;
    data.infoSuggester = it->infoSuggester;
    data.infoAccID     = it->infoAccID;
    for (int i = 0; i < 5; i++) {
        data.levels[i].id      = it->levels[i].id;
        data.levels[i].name    = it->levels[i].name;
        data.levels[i].creator = it->levels[i].creator;
        data.levels[i].stars   = it->levels[i].stars;
    }
    log::debug("onEdit: source levels (CustomGauntletData):");
    for (int i = 0; i < 5; i++) {
        log::debug("  src slot {}: id={}", i, it->levels[i].id);
    }
    log::debug("onEdit: copied levels (GauntletEditData):");
    for (int i = 0; i < 5; i++) {
        log::debug("  dst slot {}: id={}", i, data.levels[i].id);
    }

    GauntletEditPopup::create(data, [this](GauntletEditData const& updated) {
        fetchGauntlets();
    })->show();
}

void GauntletManagerPopup::onCreateNew(CCObject*) {
    GauntletEditData empty;
    GauntletEditPopup::create(empty, [this](GauntletEditData const& data) {
        m_staged.push_back(data);
        saveStaged();
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
        saveStaged();
        buildGauntletList();
    })->show();
}

void GauntletManagerPopup::buildStagedRow(GauntletEditData const& g, int index) {
    auto gauntletListItem = CCNode::create();
    gauntletListItem->setContentSize({m_gauntletList->getContentWidth(), 60});

    // Accent 1 background
    auto accent1 = CCScale9Sprite::create("square.png");
    accent1->setContentSize(gauntletListItem->getContentSize());
    accent1->setColor(g.bgColor);
    // accent1->setOpacity(120);
    accent1->setAnchorPoint({0, 0});
    gauntletListItem->addChild(accent1);

    // Accent 2 gradient overlay
    auto accent2 = CCSprite::createWithSpriteFrameName("GR_pureGradient_001.png"_spr);
    accent2->setScaleX(2.5);
    accent2->setScaleY(0.475);
    accent2->setColor(g.nameColor);
    accent2->setOpacity(120);
    // accent2->setOpacity(120);
    accent2->setAnchorPoint({1, 0});
    accent2->setPosition({gauntletListItem->getContentWidth(), 0});
    gauntletListItem->addChild(accent2);

    // Icon placeholder — filled async below
    auto iconNode = CCNode::create();
    iconNode->setID("icon-node");
    iconNode->setContentSize({50, 50});
    iconNode->setPosition({30, gauntletListItem->getContentHeight() / 2});
    iconNode->setAnchorPoint({0.5, 0.5});

    auto loadingIcon = LoadingCircle::create();
    loadingIcon->setID("loading-circle");
    loadingIcon->setPosition(iconNode->getContentSize() / 2);
    loadingIcon->setContentSize({50, 50});
    loadingIcon->setOpacity(120);
    loadingIcon->setAnchorPoint({0.5, 0.5});
    iconNode->addChild(loadingIcon);
    gauntletListItem->addChild(iconNode);

    // STAGED badge
    auto badge = CCSprite::createWithSpriteFrameName("GR_stagedTag_001.png"_spr);
    // auto badge = CCLabelBMFont::create("STAGED", "bigFont.fnt");
    badge->setScale(0.35);
    // badge->setColor({255, 220, 50});
    badge->setAnchorPoint({0.5, 1});
    badge->setPosition({10, gauntletListItem->getContentHeight()});
    gauntletListItem->addChild(badge);

    // Name
    auto nameLabel = CCLabelBMFont::create(fmt::format("{} Gauntlet", g.name).c_str(), "bigFont.fnt");
    nameLabel->setScale(0.575);
    nameLabel->setID("gauntlet-name");
    nameLabel->setColor(g.nameColor);
    nameLabel->setAnchorPoint({0, 0.5});
    nameLabel->setPosition({58, gauntletListItem->getContentHeight() / 2 + 17.5f});
    gauntletListItem->addChild(nameLabel, 1);

    auto nameShadow = CCLabelBMFont::create(fmt::format("{} Gauntlet", g.name).c_str(), "bigFont.fnt");
    nameShadow->setScale(0.575);
    nameShadow->setID("gauntlet-name-shadow");
    nameShadow->setColor({0, 0, 0});
    nameShadow->setOpacity(60);
    nameShadow->setAnchorPoint({0, 0.5});
    nameShadow->setPosition({nameLabel->getPositionX() + 2, nameLabel->getPositionY() - 2});
    gauntletListItem->addChild(nameShadow, 0);

    auto descBox = NineSlice::create("square02b_small.png");
    descBox->setContentSize({185, 25});
    descBox->setAnchorPoint({0, 0});
    descBox->setPosition(nameLabel->getPositionX(), gauntletListItem->getContentHeight() / 2 - 22.5);
    descBox->setColor({0, 0, 0});
    descBox->setOpacity(80);
    gauntletListItem->addChild(descBox);

    auto description = TextArea::create(
        g.description,
        "chatFont.fnt",
        0.5,
        160,
        {0, 0.5},
        7.5,
        false
    );
    description->setAnchorPoint({0, 0});
    description->setContentSize({180, 20});
    description->setPosition({descBox->getPositionX() + 2.5f, (descBox->getPositionY() + descBox->getContentHeight() / 2)});
    gauntletListItem->addChild(description);

    // Buttons — local menu
    auto actionMenu = CCMenu::create();
    actionMenu->setAnchorPoint({1, 0.5});
    actionMenu->setPosition({gauntletListItem->getContentWidth() - 12, gauntletListItem->getContentHeight() / 2});
    actionMenu->setLayout(RowLayout::create()->setGap(5)->setAxisAlignment(AxisAlignment::End));

    auto pushSpr = CCSprite::createWithSpriteFrameName("GR_addBtn_001.png"_spr);
    pushSpr->setScale(0.85f);
    auto pushBtn = CCMenuItemExt::createSpriteExtra(pushSpr,
        [this, index](CCMenuItemSpriteExtra*) { onPushStaged(index); });
    actionMenu->addChild(pushBtn);

    auto editSpr = CCSprite::createWithSpriteFrameName("GR_editBtn_001.png"_spr);
    editSpr->setScale(0.85f);
    auto editBtn = CCMenuItemExt::createSpriteExtra(editSpr,
        [this, index](CCMenuItemSpriteExtra*) { onEditStaged(index); });
    actionMenu->addChild(editBtn);

    auto deleteSpr = CCSprite::createWithSpriteFrameName("GR_deleteBtn_001.png"_spr);
    deleteSpr->setScale(0.85f);
    auto deleteBtn = CCMenuItemExt::createSpriteExtra(deleteSpr,
        [this, index](CCMenuItemSpriteExtra*) { onDelete(index); });
        actionMenu->addChild(deleteBtn);

    actionMenu->updateLayout();
    gauntletListItem->addChild(actionMenu);

    m_gauntletList->addChild(gauntletListItem);

    // Async icon load
    if (!g.iconURL.empty()) {
        m_rowIconHolders.emplace_back();
        m_rowIconHolders.back().spawn(
            web::WebRequest().get(g.iconURL),
            [iconNode](web::WebResponse res) {
                if (!res.ok()) return;
                auto bytes = res.data();
                queueInMainThread([iconNode, bytes]() {
                    if (!iconNode) return;
                    auto img = new CCImage();
                    if (!img->initWithImageData(
                            const_cast<unsigned char*>(bytes.data()), bytes.size())) {
                        delete img; return;
                    }
                    auto tex = new CCTexture2D();
                    tex->initWithImage(img);
                    delete img;
                    auto icon = CCSprite::createWithTexture(tex);
                    auto iconShadow = CCSprite::createWithTexture(tex);
                    tex->release();
                    icon->setScale(iconNode->getContentHeight() * 1.15 / icon->getContentHeight() * 1.15);
                    icon->setPosition(iconNode->getContentSize() / 2);
                    icon->setAnchorPoint({0.5, 0.5});
                    iconShadow->setScaleX(icon->getScaleX());
                    iconShadow->setScaleY(icon->getScaleY() * 1.2);
                    iconShadow->setPosition({icon->getPositionX(), icon->getPositionY() - 5});
                    iconShadow->setAnchorPoint({0.5, 0.5});
                    iconShadow->setColor({0, 0, 0});
                    iconShadow->setOpacity(50);
                    if (auto ph = iconNode->getChildByID("icon-placeholder"))
                        ph->removeFromParent();
                    iconNode->addChild(icon, 1);
                    iconNode->addChild(iconShadow, 0);
                });
            }
        );
    }
}

void GauntletManagerPopup::onPushStaged(int index) {
    if (index < 0 || index >= (int)m_staged.size()) return;
    auto data = m_staged[index];

    createQuickPopup(
        "Push Gauntlet",
        fmt::format("Push the <cy>{} Gauntlet</c> to the server?", data.name).c_str(),
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
                    log::error("Push failed: HTTP {} - {}", res.code(), res.string().unwrapOr(""));
                    Notification::create(
                        fmt::format("Push failed: HTTP {}", res.code()),
                        NotificationIcon::Error
                    )->show();
                    return;
                }
                m_staged.erase(m_staged.begin() + index);
                saveStaged();
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
                        log::error("Delete failed: HTTP {} - {}", res.code(), res.string().unwrapOr(""));
                        Notification::create(fmt::format("Delete failed. Err {}", res.code()), NotificationIcon::Error)->show();
                        return;
                    }
                    Notification::create("Gauntlet deleted.", NotificationIcon::Success)->show();
                    fetchGauntlets();
                    saveStaged();
                    buildGauntletList();
                }
            );
        }
    );
}