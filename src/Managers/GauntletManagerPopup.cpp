#include "GauntletManagerPopup.hpp"
#include "GauntletEditPopup.hpp"
#include "../Data/CustomGauntletManager.hpp"

GauntletManagerPopup* GauntletManagerPopup::create() {
    auto ret = new GauntletManagerPopup();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GauntletManagerPopup::init() {
    // Use the simple opacity-only init that your bindings have
    if (!FLAlertLayer::init(150)) return false;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    // Dim background
    auto bg = CCLayerColor::create({0, 0, 0, 150});
    this->addChild(bg, -1);

    // Popup background
    auto popupBg = CCScale9Sprite::create("GJ_square05.png");
    popupBg->setContentSize({330, 180});
    popupBg->setPosition(winSize / 2);
    this->addChild(popupBg);

    // Title
    auto title = CCLabelBMFont::create("Gauntlet Manager", "goldFont.fnt");
    title->setScale(0.7f);
    title->setPosition({winSize.width / 2, winSize.height / 2 + 145});
    this->addChild(title);

    // Close button
    auto closeMenu = CCMenu::create();
    closeMenu->setPosition({winSize.width / 2 - 220, winSize.height / 2 + 145});
    this->addChild(closeMenu);

    auto closeBtn = CCMenuItemExt::createSpriteExtra(
        CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"),
        [this](CCMenuItemSpriteExtra*) { onClose(nullptr); }
    );
    closeMenu->addChild(closeBtn);

    // Loading circle
    m_loadingCircle = LoadingCircle::create();
    m_loadingCircle->setPosition(winSize / 2);
    m_loadingCircle->setVisible(false);
    this->addChild(m_loadingCircle, 10);

    this->registerWithTouchDispatcher();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(
        this, -500, true
    );
    this->setZOrder(100);

    if (GauntletManagerAPI::get()->isLoggedIn()) {
        buildPanelView();
        fetchGauntlets();
    } else {
        buildLoginView();
    }

    return true;
}

void GauntletManagerPopup::buildLoginView() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto bg      = m_mainLayer;

    m_loginLayer = CCLayer::create();
    m_loginLayer->setPosition({0, 0});
    bg->addChild(m_loginLayer, 1);

    auto subtitle = CCLabelBMFont::create("Manager Login", "goldFont.fnt");
    subtitle->setScale(0.6f);
    subtitle->setPosition({winSize.width / 2, winSize.height / 2 + 80});
    m_loginLayer->addChild(subtitle);

    // Username
    auto userBg = CCScale9Sprite::create("square02_001.png");
    userBg->setContentSize({280, 32});
    userBg->setOpacity(100);
    userBg->setPosition({winSize.width / 2, winSize.height / 2 + 30});
    m_loginLayer->addChild(userBg);

    m_usernameInput = CCTextInputNode::create(260, 28, "Username", "chatFont.fnt");
    m_usernameInput->setPosition({winSize.width / 2, winSize.height / 2 + 30});
    m_usernameInput->setDelegate(this);
    m_loginLayer->addChild(m_usernameInput);

    // Password
    auto passBg = CCScale9Sprite::create("square02_001.png");
    passBg->setContentSize({280, 32});
    passBg->setOpacity(100);
    passBg->setPosition({winSize.width / 2, winSize.height / 2 - 15});
    m_loginLayer->addChild(passBg);

    m_passwordInput = CCTextInputNode::create(260, 28, "Password", "chatFont.fnt");
    m_passwordInput->setPosition({winSize.width / 2, winSize.height / 2 - 15});
    m_passwordInput->setDelegate(this);
    m_loginLayer->addChild(m_passwordInput);

    // Login button
    auto loginMenu = CCMenu::create();
    loginMenu->setPosition({winSize.width / 2, winSize.height / 2 - 65});
    m_loginLayer->addChild(loginMenu);

    auto loginSpr = ButtonSprite::create("Sign In", "goldFont.fnt", "geode.loader/GE_button_04.png");
    auto loginBtn = CCMenuItemSpriteExtra::create(
        loginSpr, this, menu_selector(GauntletManagerPopup::onLogin)
    );
    loginMenu->addChild(loginBtn);
}

void GauntletManagerPopup::buildPanelView() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto bg      = m_mainLayer;

    if (m_loginLayer) {
        m_loginLayer->removeFromParent();
        m_loginLayer = nullptr;
    }

    m_panelLayer = CCLayer::create();
    m_panelLayer->setPosition({0, 0});
    bg->addChild(m_panelLayer);

    // Create button
    auto createMenu = CCMenu::create();
    createMenu->setPosition({winSize.width / 2 + 160, winSize.height / 2 + 140});
    m_panelLayer->addChild(createMenu);

    auto createSpr = ButtonSprite::create("+ New", "bigFont.fnt", "GJ_button_01.png");
    createSpr->setScale(0.7f);
    auto createBtn = CCMenuItemSpriteExtra::create(
        createSpr, this, menu_selector(GauntletManagerPopup::onCreateNew)
    );
    createMenu->addChild(createBtn);

    // List area
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

            // Iterate using contains + index access
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

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    float yPos   = winSize.height / 2 + 110.f;

    if (m_gauntlets.empty()) {
        auto emptyLabel = CCLabelBMFont::create(
            "No gauntlets yet. Create one!", "bigFont.fnt"
        );
        emptyLabel->setScale(0.45f);
        emptyLabel->setColor({180, 180, 180});
        emptyLabel->setPosition({winSize.width / 2, winSize.height / 2});
        m_listLayer->addChild(emptyLabel);
        return;
    }

    for (auto const& g : m_gauntlets) {
        buildGauntletRow(g, yPos);
        yPos -= 46.f;
    }
}

void GauntletManagerPopup::buildGauntletRow(CustomGauntletData const& g, float yPos) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    // Row background
    auto rowBg = CCScale9Sprite::create("square02_001.png");
    rowBg->setContentSize({400, 40});
    rowBg->setOpacity(60);
    rowBg->setPosition({winSize.width / 2, yPos});
    m_listLayer->addChild(rowBg);

    // Color swatch
    auto swatch = CCScale9Sprite::create("square02_001.png");
    swatch->setContentSize({16, 16});
    swatch->setColor(g.color);
    swatch->setPosition({winSize.width / 2 - 185, yPos});
    m_listLayer->addChild(swatch);

    // Name
    auto nameLabel = CCLabelBMFont::create(g.name.c_str(), "bigFont.fnt");
    nameLabel->setScale(0.45f);
    nameLabel->setAnchorPoint({0, 0.5f});
    nameLabel->limitLabelWidth(220.f, 0.45f, 0.2f);
    nameLabel->setPosition({winSize.width / 2 - 165, yPos});
    m_listLayer->addChild(nameLabel);

    // Edit + Delete buttons
    auto rowMenu = CCMenu::create();
    rowMenu->setPosition({winSize.width / 2 + 120, yPos});
    m_listLayer->addChild(rowMenu);

    auto editSpr = ButtonSprite::create("Edit", "bigFont.fnt", "GJ_button_02.png");
    editSpr->setScale(0.5f);
    int gid = g.id;
    auto editBtn = CCMenuItemExt::createSpriteExtra(editSpr, [this, gid](CCMenuItemSpriteExtra*) {
        onEdit(gid);
    });
    editBtn->setPositionX(-36);
    rowMenu->addChild(editBtn);

    auto delSpr = ButtonSprite::create("Del", "bigFont.fnt", "GJ_button_06.png");
    delSpr->setScale(0.5f);
    auto delBtn = CCMenuItemExt::createSpriteExtra(delSpr, [this, gid](CCMenuItemSpriteExtra*) {
        onDelete(gid);
    });
    delBtn->setPositionX(36);
    rowMenu->addChild(delBtn);
}

void GauntletManagerPopup::onLogin(CCObject*) {
    auto username = std::string(m_usernameInput->getString());
    auto password = std::string(m_passwordInput->getString());

    if (username.empty() || password.empty()) {
        Notification::create("Enter username and password.", NotificationIcon::Warning)->show();
        return;
    }

    m_loadingCircle->setVisible(true);

    m_loginHolder.spawn(
        GauntletManagerAPI::get()->login(username, password),
        [this](web::WebResponse res) {
            m_loadingCircle->setVisible(false);

            if (!res.ok()) {
                Notification::create("Invalid credentials.", NotificationIcon::Error)->show();
                return;
            }

            auto json  = res.json().unwrapOr(matjson::Value());
            auto token = json["token"].asString().unwrapOr("");
            if (token.empty()) {
                Notification::create("No token returned.", NotificationIcon::Error)->show();
                return;
            }

            GauntletManagerAPI::get()->setToken(token);
            buildPanelView();
            fetchGauntlets();
        }
    );
}

void GauntletManagerPopup::onCreateNew(CCObject*) {
    GauntletEditData empty;
    auto popup = GauntletEditPopup::create(empty, [this] {
        fetchGauntlets();
    });
    popup->show();
}

void GauntletManagerPopup::onEdit(int gauntletId) {
    // Find the gauntlet data
    auto it = std::find_if(m_gauntlets.begin(), m_gauntlets.end(),
        [gauntletId](auto const& g) { return g.id == gauntletId; });
    if (it == m_gauntlets.end()) return;

    GauntletEditData data;
    data.id      = it->id;
    data.name    = it->name;
    data.iconURL = it->iconURL;
    data.bgColor = it->color;

    auto popup = GauntletEditPopup::create(data, [this] {
        fetchGauntlets();
    });
    popup->show();
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
                        Notification::create(
                            "Delete failed.", NotificationIcon::Error
                        )->show();
                        return;
                    }
                    Notification::create(
                        "Gauntlet deleted.", NotificationIcon::Success
                    )->show();
                    fetchGauntlets();
                }
            );
        }
    );
}

void GauntletManagerPopup::onClose(CCObject*) {
    this->removeFromParent();
    this->keyBackClicked();
}