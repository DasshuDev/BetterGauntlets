#include <Geode/Geode.hpp>
#include <Geode/Loader.hpp>
#include <Geode/ui/Layout.hpp>
#include <Geode/ui/SimpleAxisLayout.hpp>
#include <argon/argon.hpp>
#include <alphalaneous.alphas-ui-pack/include/API.hpp>
#include "GauntletSelectLayer.hpp"
#include "../Managers/GauntletManagerPopup.hpp"
#include "../Managers/GauntletManagerCache.hpp"
#include "../Data/CustomGauntletManager.hpp"
#include "../CustomGauntlets/CustomGauntletNode.hpp"
#include "../CustomGauntlets/CustomGauntletLayer.hpp"
#include "../Managers/LeaderboardPopup.hpp"
#include "GauntletInfoPopup.hpp"

using namespace geode::prelude;

// create / scene 

CCScene* BetterGauntletSelectLayer::scene() {
    auto layer = BetterGauntletSelectLayer::create();
    if (!layer) return nullptr;
    auto sc = CCScene::create();
    sc->addChild(layer);
    return sc;
}

BetterGauntletSelectLayer::~BetterGauntletSelectLayer() {
    auto glm = GameLevelManager::get();
    if (glm->m_levelManagerDelegate == this)
        glm->m_levelManagerDelegate = nullptr;
    if (m_gauntletPacks) { m_gauntletPacks->release(); m_gauntletPacks = nullptr; }
}

BetterGauntletSelectLayer* BetterGauntletSelectLayer::create() {
    auto ret = new BetterGauntletSelectLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

// init 

bool BetterGauntletSelectLayer::init() {
    if (!CCLayer::init()) return false;

    setKeypadEnabled(true);
    setKeyboardEnabled(true);

    GauntletManagerCache::get()->refresh();

    auto GDUtils = Loader::get()->getLoadedMod("gdutilsdevs.gdutils");
    if (GDUtils) {
        auto settingVal = GDUtils->getSettingValue<bool>("gauntletDesign");
        if (settingVal) GDUtils->setSettingValue<bool>("gauntletDesign", false);
    }

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("GauntletSheet.plist");

    buildBackground();
    buildDecorations();
    buildMenus();

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto spinner = CCSprite::create("loadingCircle.png");
    spinner->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
    spinner->runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360.0f)));
    spinner->setPosition(winSize / 2);
    spinner->setID("loading-circle");
    m_loadingCircle = spinner;
    this->addChild(m_loadingCircle, 10);

    // Fetch gauntlets from RobTop servers
    GameLevelManager::get()->m_levelManagerDelegate = this;
    GameLevelManager::get()->getGauntlets();

    buildCustomList();

    checkManagerStatus();

    return true;
}

// Background 

void BetterGauntletSelectLayer::buildBackground() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto bg = CCSprite::create("GJ_gradientBG.png");
    if (bg) {
        bg->setID("background");
        bg->setColor(ccc3(34, 34, 34));
        bg->setPosition({0, 0});
        bg->setAnchorPoint({0, 0});
        float scaleX = winSize.width / bg->getContentWidth();
        float scaleY = winSize.height / bg->getContentHeight();
        bg->setScale(std::max(scaleX, scaleY));
        this->addChild(bg, -99);
    }

    // Floor
    auto floor = CCSprite::createWithSpriteFrameName("gauntletGround_001.png"_spr);
    if (floor) {
        floor->setID("menu-floor"_spr);
        auto floorSize = floor->getContentSize();
        float scaleX = winSize.width / floorSize.width;
        float scaleY = winSize.height / floorSize.height;
        float scale = std::min(scaleX, scaleY);
        floor->setScale(scale);
        floor->setPosition(ccp(winSize.width / 2, floorSize.height * scale / 2));
        floor->setAnchorPoint(ccp(0.5, 0.5));
        floor->setColor({175, 175, 175});
        this->addChild(floor, -2);
    }

    // Corners
    auto TLCornerNew = CCSpriteGrayscale::createWithSpriteFrameName("GJ_sideArt_001.png");
    if (TLCornerNew) {
        TLCornerNew->setID("top-left-corner"_spr);
        TLCornerNew->setPosition(ccp(director->getScreenLeft() - 1, director->getScreenTop() + 1));
        TLCornerNew->setAnchorPoint(ccp(0, 1));
        TLCornerNew->setFlipY(true);
        TLCornerNew->setColor({67, 67, 67});
        this->addChild(TLCornerNew);
    }
    auto TRCornerNew = CCSpriteGrayscale::createWithSpriteFrameName("GJ_sideArt_001.png");
    if (TRCornerNew) {
        TRCornerNew->setID("top-right-corner"_spr);
        TRCornerNew->setPosition(ccp(director->getScreenRight() + 1, director->getScreenTop() + 1));
        TRCornerNew->setAnchorPoint(ccp(1, 1));
        TRCornerNew->setFlipY(true);
        TRCornerNew->setFlipX(true);
        TRCornerNew->setColor({67, 67, 67});
        this->addChild(TRCornerNew);
    }
}

// Decorations 

void BetterGauntletSelectLayer::buildDecorations() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    // Chains
    auto decoParentNode = CCNode::create();
    decoParentNode->setID("background-decoration"_spr);
    decoParentNode->setPosition(0, 0);
    this->addChild(decoParentNode, -1);

    auto chainParentNode = CCNode::create();
    chainParentNode->setID("chain-parent"_spr);
    chainParentNode->setPosition(0, 0);
    decoParentNode->addChild(chainParentNode);

    for (int c = 0; c < 4; c++) {
        auto chain = CCSprite::createWithSpriteFrameName("chain_01_001.png");
        chain->setID(fmt::format("chain-{}", c + 1));
        chain->setColor({ 10, 10, 10 });
        chain->setPosition(ccp(0, 0));
        chain->setRotation(180);
        chainParentNode->addChild(chain);
    }

    if (auto chain1 = chainParentNode->getChildByID("chain-1")) {
        chain1->setScale(2.0);
        static_cast<CCSprite*>(chain1)->setOpacity(255);
        chain1->setPosition(ccp(director->getScreenLeft() + 50, director->getScreenTop() - 70));
    }
    if (auto chain2 = chainParentNode->getChildByID("chain-2")) {
        chain2->setScale(1.65);
        static_cast<CCSprite*>(chain2)->setOpacity(200);
        chain2->setPosition(ccp(director->getScreenRight() - 40, director->getScreenTop() - 55));
    }
    if (auto chain3 = chainParentNode->getChildByID("chain-3")) {
        static_cast<CCSprite*>(chain3)->setOpacity(127);
        chain3->setPosition(ccp(director->getScreenRight() - 100, director->getScreenTop() - 26));
    }
    if (auto chain4 = chainParentNode->getChildByID("chain-4")) {
        chain4->setScale(0.75);
        static_cast<CCSprite*>(chain4)->setOpacity(80);
        chain4->setPosition(ccp(director->getScreenLeft() + 90, director->getScreenTop() - 24));
    }

    // Background particles
    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
    if (enableParticles) {
        CCParticleSystemQuad* bgParticlesA = GameToolbox::particleFromString(
            "100a-1a4a2a16a90a90a0a0a300a0a128a-25a0a60a0a0a400a0a90a60a0a0a0a0a0a0a0.05a0.02a400a0a90a67a0a0a0a0a0a0a0a0.05a1a0a0.5a0a0a0a0a0a0a0a0a2a0a0a0a0a182a0a1.8a0a0a0a0a0a0a0a0a0a0a0a0",
            NULL, false
        );
        if (bgParticlesA) {
            bgParticlesA->setPosition(ccp(winSize.width / 2, director->getScreenBottom() + 10));
            bgParticlesA->setZOrder(-2);
            this->addChild(bgParticlesA);
        }

        CCParticleSystemQuad* bgParticlesB = GameToolbox::particleFromString(
            "100a-1a1.75a0a20a90a90a0a0a300a0a0a-77a0a0a0a0a5a3a0a60a0a0a0a0a0a0a1a0a2a0a0a67a0a0a0a0a0a0a1a0a0a0a0.2a0a0a0a0a0a0a0a0a2a0a0a0a0a0a0a-1a0a0a0a0a0a0a0a0a0a0a0a0",
            NULL, false
        );
        if (bgParticlesB) {
            bgParticlesB->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 10));
            bgParticlesB->setZOrder(-2);
            this->addChild(bgParticlesB);
        }
    }
}

// Menus 

void BetterGauntletSelectLayer::buildMenus() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    // Back button 
    auto backMenu = CCMenu::create();
    backMenu->setID("back-menu");
    backMenu->setPosition(24, director->getScreenTop() - 23);
    backMenu->setContentWidth(32.5);
    backMenu->setContentHeight(125);
    this->addChild(backMenu, 1);

    auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    auto backBtn = CCMenuItemSpriteExtra::create(
        backSpr, this, menu_selector(BetterGauntletSelectLayer::onBack)
    );
    backMenu->addChild(backBtn);

    if (PlatformToolbox::isControllerConnected()) {
        auto controllerBtn = CCSprite::createWithSpriteFrameName("controllerBtn_B_001.png");
        controllerBtn->setID("controller-back-hint");
        controllerBtn->setPosition({backBtn->getPositionX(), backBtn->getPositionY() - 30});
        controllerBtn->setScale(0.7);
        controllerBtn->setZOrder(1);
        backMenu->addChild(controllerBtn);
    }

    // Top-right menu
    auto TRMenu = CCMenu::create();
    TRMenu->setID("top-right-menu");
    TRMenu->setPosition(director->getScreenRight() - 24, 254.5);
    TRMenu->setContentHeight(125);
    this->addChild(TRMenu, 1);

    // Bottom-left menu (info + discord) 
    auto BLMenu = CCMenu::create();
    BLMenu->setID("bottom-left-menu");
    BLMenu->setAnchorPoint({0.5, 0});
    BLMenu->setPosition({director->getScreenLeft() + 30, 12.5});
    BLMenu->setLayout(ColumnLayout::create()->setAutoGrowAxis(true));
    this->addChild(BLMenu, 1);

    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoBtn_001.png");
    infoSpr->setScale(0.75);
    auto infoBtn = CCMenuItemSpriteExtra::create(
        infoSpr, this, menu_selector(BetterGauntletSelectLayer::onNewInfo)
    );
    infoBtn->setID("info-button"_spr);
    BLMenu->addChild(infoBtn);

    auto discordIcon = CCSprite::create("GR_discord_02_001.png"_spr);
    discordIcon->setScale(0.65);
    auto discordSpr = CircleButtonSprite::create(discordIcon, CircleBaseColor::Green, CircleBaseSize::Medium);
    discordSpr->setScale(0.75);
    auto discordBtn = CCMenuItemSpriteExtra::create(
        discordSpr, this, menu_selector(BetterGauntletSelectLayer::onDiscord)
    );
    discordBtn->setID("discord-button"_spr);
    BLMenu->addChild(discordBtn);
    BLMenu->updateLayout();

    // Bottom-right menu (refresh) 
    auto BRMenu = CCMenu::create();
    BRMenu->setID("bottom-right-menu");
    BRMenu->setAnchorPoint({0.5, 0});
    BRMenu->setPosition({director->getScreenRight() - 30, 12.5});
    BRMenu->setLayout(
        ColumnLayout::create()
        ->setAxisReverse(false)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setGap(5.0)
        ->setAutoGrowAxis(true)
    );
    this->addChild(BRMenu, 1);

    auto refreshSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
    refreshSpr->setScale(0.75);
    m_refreshButton = CCMenuItemSpriteExtra::create(
        refreshSpr, this, menu_selector(BetterGauntletSelectLayer::onRefresh)
    );
    m_refreshButton->setID("refresh-button");
    BRMenu->addChild(m_refreshButton);

    auto leaderboardSpr = CCSprite::createWithSpriteFrameName("GJ_levelLeaderboardBtn_001.png");
    leaderboardSpr->setScale(0.75);
    m_leaderboardButton = CCMenuItemSpriteExtra::create(
        leaderboardSpr, this, menu_selector(BetterGauntletSelectLayer::onLeaderboard)
    );
    m_leaderboardButton->setID("leaderboard-button");
    BRMenu->addChild(m_leaderboardButton);

    BRMenu->updateLayout();

    // Top menu (title + toggle) 
    auto topMenu = CCMenu::create();
    topMenu->setPosition({winSize.width / 2, director->getScreenTop() - 39});
    topMenu->setID("top-menu"_spr);
    this->addChild(topMenu, 1);

    m_vanillaTitle = CCSprite::create("GR_gauntletTitle_01_001.png"_spr);
    if (m_vanillaTitle) {
        m_vanillaTitle->setID("title"_spr);
        m_vanillaTitle->setAnchorPoint(ccp(0.5, 0.5));
        m_vanillaTitle->setZOrder(10);
        m_vanillaTitle->setScale(0.575);
        topMenu->addChild(m_vanillaTitle);
    }

    m_betterTitle = CCSprite::create("GR_gauntletTitle_02_001.png"_spr);
    if (m_betterTitle) {
        m_betterTitle->setID("better-title"_spr);
        m_betterTitle->setAnchorPoint(ccp(0.5, 0.5));
        m_betterTitle->setZOrder(10);
        m_betterTitle->setScale(0.575);
        m_betterTitle->setVisible(false);
        topMenu->addChild(m_betterTitle);
    }

    auto toggleOff = CCSpriteGrayscale::create("GR_gauntletStar_001.png"_spr);
    toggleOff->setOpacity(80);
    toggleOff->setColor(ccc3(128, 128, 128));
    auto toggleOn = CCSprite::create("GR_gauntletStar_001.png"_spr);

    auto toggleSpr = CCMenuItemToggler::create(
        toggleOff, toggleOn,
        this, menu_selector(BetterGauntletSelectLayer::toggleList)
    );
    toggleSpr->setID("toggle-list-button");
    toggleSpr->setPosition({102, 17.5});
    toggleSpr->setScale(0.65);
    toggleSpr->setOpacity(80);
    topMenu->addChild(toggleSpr);

    // Crystal counter
    m_crystalLabel = CCLabelBMFont::create("0", "bigFont.fnt");
    m_crystalLabel->setID("crystal-counter-label"_spr);
    m_crystalLabel->setAnchorPoint({0.5, 0.5});
    m_crystalLabel->setAlignment(kCCTextAlignmentLeft);
    m_crystalLabel->setScale(0.6f);
    topMenu->addChild(m_crystalLabel);
    updateCrystalLabel();
}

// Level loading delegates 

void BetterGauntletSelectLayer::loadLevelsFinished(CCArray* levels, char const* key, int type) {
    auto glm = GameLevelManager::get();
    if (glm->m_levelManagerDelegate == this)
        glm->m_levelManagerDelegate = nullptr;

    if (m_loadingCircle) {
        m_loadingCircle->removeFromParent();
        m_loadingCircle = nullptr;
    }

    if (!levels || levels->count() == 0) {
        Notification::create("No gauntlets returned from server.", NotificationIcon::Error)->show();
        return;
    }

    m_gauntletPacks = levels;
    m_gauntletPacks->retain();

    buildGauntletNodes(levels);
}

void BetterGauntletSelectLayer::loadLevelsFailed(char const* key, int type) {
    auto glm = GameLevelManager::get();
    if (glm->m_levelManagerDelegate == this)
        glm->m_levelManagerDelegate = nullptr;

    if (m_loadingCircle) {
        m_loadingCircle->removeFromParent();
        m_loadingCircle = nullptr;
    }

    Notification::create("Gauntlets failed to load", NotificationIcon::Error, 1.5)->show();
}

// Build gauntlet nodes from server data 

void BetterGauntletSelectLayer::buildGauntletNodes(CCArray* gauntlets) {
    // Sort gauntlets by pack ID to ensure consistent ordering
    std::vector<GJMapPack*> packs;
    for (auto* obj : CCArrayExt<GJMapPack*>(gauntlets)) {
        packs.push_back(obj);
    }
    std::sort(packs.begin(), packs.end(), [](GJMapPack* a, GJMapPack* b) {
        return a->m_packID < b->m_packID;
    });

    setupScrollMode();

    if (!m_gauntletBtnContainer) return;

    for (auto* pack : packs) {
        auto gauntletNode = GauntletNode::create(pack);
        if (!gauntletNode) continue;

        for (auto* child : CCArrayExt<CCNode*>(gauntletNode->getChildren())) {
            if (auto menu = typeinfo_cast<CCMenu*>(child)) {
                menu->setEnabled(false);
                menu->setTouchEnabled(false);
            }
        }

        auto btn = CCMenuItemSpriteExtra::create(
            gauntletNode, this,
            menu_selector(BetterGauntletSelectLayer::onPlay)
        );
        btn->setUserObject(pack);
        btn->setTag(pack->m_packID);
        btn->setContentSize({110, 220});
        btn->m_scaleMultiplier = 1.05;
        gauntletNode->setPosition(btn->getContentSize() / 2);

        m_gauntletBtnContainer->addChild(btn);
    }

    if (m_gauntletBtnContainer->getChildrenCount() < 5) {
        m_gauntletBtnContainer->setAnchorPoint({0.0, 0.5});
        m_gauntletBtnContainer->setPositionX(58.5);
    }
    else {
        m_gauntletBtnContainer->setAnchorPoint({0, 0.5});
        m_gauntletBtnContainer->setPositionX(0);
    }

    m_gauntletBtnContainer->updateLayout();

    m_customScrollLayer->getContentLayer()->setContentWidth(m_gauntletBtnContainer->getContentWidth());

    styleGauntletButtons();
    loadScrollPos();

    if (s_showCustomList) {
        toggleList(nullptr);
    }
}

// Scroll mode

void BetterGauntletSelectLayer::setupScrollMode() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

    if (m_customScrollLayer && m_customScrollLayer->getParent()) return;

    // Clean up if re-entering
    if (m_customScrollLayer) { m_customScrollLayer->removeFromParent(); m_customScrollLayer = nullptr; }
    if (m_customScrollBar) { m_customScrollBar->removeFromParent(); m_customScrollBar = nullptr; }
    if (m_gauntletBtnContainer) { m_gauntletBtnContainer->removeFromParent(); m_gauntletBtnContainer = nullptr; }

    m_gauntletBtnContainer = CCMenu::create();
    m_gauntletBtnContainer->setAnchorPoint({0, 0.5});
    m_gauntletBtnContainer->setID("gauntlet-btns");
    m_gauntletBtnContainer->setLayout(
        RowLayout::create()
        ->setAxisReverse(false)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setGap(3)
        ->setAutoGrowAxis(true)
        ->setPadding({60, 0, 60, 0})
    );

    m_customScrollLayer = alpha::ui::AdvancedScrollLayer::create(
        m_gauntletBtnContainer->getContentSize()
    );
    m_customScrollLayer->setHorizontalScroll(true);
    m_customScrollLayer->setVerticalScroll(false);
    m_customScrollLayer->setPosition(winSize.width / 2, winSize.height / 2 - 19);
    m_customScrollLayer->setContentWidth(winSize.width);
    m_customScrollLayer->setID("gauntlet-buttons"_spr);
    m_customScrollLayer->ignoreAnchorPointForPosition(false);
    this->addChild(m_customScrollLayer);

    m_customScrollBar = alpha::ui::AdvancedScrollBar::create(
        m_customScrollLayer, alpha::ui::ScrollOrientation::HORIZONTAL
    );
    m_customScrollBar->setPosition({
        winSize.width / 2,
        m_customScrollLayer->getPositionY() - 126
    });
    m_customScrollBar->setContentSize({12, winSize.height + 125});
    m_customScrollBar->setID("gauntlet-scrollbar"_spr);
    this->addChild(m_customScrollBar);

    m_customScrollLayer->getContentLayer()->addChild(m_gauntletBtnContainer);

    m_sliderLabel = CCLabelBMFont::create("", "chatFont.fnt");
    m_sliderLabel->setScale(0.5f);
    m_sliderLabel->setPosition({winSize.width / 2, director->getScreenBottom() + 6.5f});
    this->addChild(m_sliderLabel);
}

void BetterGauntletSelectLayer::styleGauntletButtons() {
    auto scrollLayer = m_customScrollLayer;
    if (!scrollLayer) return;

    auto btnNode = scrollLayer->getContentLayer()->getChildByID("gauntlet-btns");
    if (!btnNode) return;
    auto btnContainer = btnNode->getChildren();
    if (!btnContainer) return;

    int btn = 1;
    for (auto obj : CCArrayExt(btnContainer)) {
        auto gauntletBtn = typeinfo_cast<CCNode*>(obj);
        if (!gauntletBtn) { btn++; continue; }

        auto bgNode = static_cast<CCSprite*>(gauntletBtn->getChildByIDRecursive("background"));
        if (bgNode) bgNode->setContentSize({110, 220});

        auto infoNode = gauntletBtn->getChildByIDRecursive("gauntlet-info-node");
        if (infoNode) infoNode->setPositionY(-2.5);

        auto nameLabel = static_cast<CCLabelBMFont*>(gauntletBtn->getChildByIDRecursive("gauntlet-name-label"));
        if (nameLabel) nameLabel->setPositionY(84);

        auto gauntletLabel = static_cast<CCLabelBMFont*>(gauntletBtn->getChildByIDRecursive("gauntlet-label"));
        if (gauntletLabel && nameLabel) gauntletLabel->setPositionY(nameLabel->getPositionY() - 15);

        auto nameShadowLabel = static_cast<CCLabelBMFont*>(gauntletBtn->getChildByIDRecursive("gauntlet-name-shadow-label"));
        if (nameShadowLabel && nameLabel) nameShadowLabel->setPosition(ccp(nameLabel->getPositionX() + 2, nameLabel->getPositionY() - 2));

        auto gauntletShadowLabel = static_cast<CCLabelBMFont*>(gauntletBtn->getChildByIDRecursive("gauntlet-shadow-label"));
        if (gauntletShadowLabel && gauntletLabel) gauntletShadowLabel->setPosition(ccp(gauntletLabel->getPositionX() + 2, gauntletLabel->getPositionY() - 2));

        auto chestSpr = static_cast<CCSprite*>(gauntletBtn->getChildByIDRecursive("chest-sprite"));
        auto chestShadowSpr = static_cast<CCSprite*>(gauntletBtn->getChildByIDRecursive("chest-shadow-sprite"));
        if (chestSpr && chestShadowSpr) {
            chestSpr->setPositionY(-64.5);
            chestSpr->setScale(0.3);
            chestShadowSpr->setPosition(ccp(chestSpr->getPositionX() + 2, chestSpr->getPositionY() - 2));
            chestShadowSpr->setScale(0.3);
        }

        auto rewardLabel = static_cast<CCLabelBMFont*>(gauntletBtn->getChildByIDRecursive("reward-label"));
        auto rewardShadowLabel = static_cast<CCLabelBMFont*>(gauntletBtn->getChildByIDRecursive("reward-shadow-label"));
        if (rewardLabel && rewardShadowLabel && chestSpr) {
            rewardLabel->setPositionY(chestSpr->getPositionY() - 13.5);
            rewardLabel->setScale(0.5);
            rewardLabel->setZOrder(3);
            rewardShadowLabel->setPosition(ccp(rewardLabel->getPositionX() + 2, rewardLabel->getPositionY() - 2));
            rewardShadowLabel->setScale(0.5);
        }

        gauntletBtn->setID(fmt::format("gauntlet-button-{}", btn));
        btn++;
    }
}

// Scroll position 

void BetterGauntletSelectLayer::saveScrollPos() {
    if (m_customScrollLayer) {
        float max = m_customScrollLayer->getHorizontalMax();
        if (max > 0.f)
            s_scrollLocation = m_customScrollLayer->getScrollPoint().x;
    }
}

void BetterGauntletSelectLayer::loadScrollPos() {
    if (s_scrollLocation > 0.f && m_customScrollLayer) {
        m_customScrollLayer->setScrollX(s_scrollLocation, false);
    }

    if (Mod::get()->getSettingValue<bool>("advscroll-pos-label") && m_sliderLabel && m_customScrollLayer) {
        m_sliderLabel->setString(
            fmt::format("{:.4f} / {:.4f}", s_scrollLocation, m_customScrollLayer->getHorizontalMax()).c_str()
        );
    }
}

// Callbacks 

void BetterGauntletSelectLayer::onEnterTransitionDidFinish() {
    CCLayer::onEnterTransitionDidFinish();
    m_exiting = false;

    updateCrystalLabel();
}

void BetterGauntletSelectLayer::updateCrystalLabel() {
    if (!m_crystalLabel) return;
    m_crystalLabel->setString(fmt::format("{}", CustomGauntletManager::get()->getCrystalTotal()).c_str());
}

void BetterGauntletSelectLayer::onPlay(CCObject* sender) {
    if (m_exiting || m_playBlocked) return;
    GameManager* gm = GameManager::sharedState();

    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto pack = static_cast<GJMapPack*>(btn->getUserObject());
    if (!pack) return;

    GauntletType type = static_cast<GauntletType>(pack->m_packID);

    if (type == GauntletType::Chaos) {
        m_locked = !gm->getUGV("19"); // Spoken to Demon Guardian
    } else if (type == GauntletType::Demon) {
        m_locked = !gm->getUGV("13"); // Demon Guardian freed
    } else {
        m_locked = false;
    }

    if (m_locked) {
        m_playBlocked = true;
        Notification::create("This Gauntlet is locked!", NotificationIcon::Error, 1.5)->show();
        this->runAction(CCSequence::create(
            CCDelayTime::create(1.5f),
            CCCallFunc::create(this, callfunc_selector(BetterGauntletSelectLayer::unblockPlay)),
            nullptr
        ));
        return;
    }

    m_exiting = true;
    saveScrollPos();

    logMembers(pack);

    auto scene = BetterGauntletLayer::scene(type);
    if (scene) {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
    } else {
        m_exiting = false;
    }
}

void BetterGauntletSelectLayer::unblockPlay() {
    m_playBlocked = false;
}

void BetterGauntletSelectLayer::logMembers(GJMapPack* pack) {
    log::debug(
        "[GauntletSelectLayer] --- opening gauntlet packID={} ---",
        pack ? pack->m_packID : -1
    );

    log::debug("  m_customScrollLayer: {}", (void*)m_customScrollLayer);
    log::debug("  m_customScrollBar: {}", (void*)m_customScrollBar);
    log::debug("  m_gauntletBtnContainer: {}", (void*)m_gauntletBtnContainer);
    log::debug("  m_sliderLabel: {}", (void*)m_sliderLabel);
    log::debug("  m_vanillaTitle: {}", (void*)m_vanillaTitle);
    log::debug("  m_betterTitle: {}", (void*)m_betterTitle);
    log::debug("  m_loadingCircle: {}", (void*)m_loadingCircle);
    log::debug("  m_refreshButton: {}", (void*)m_refreshButton);
    log::debug("  m_crystalLabel: {}", (void*)m_crystalLabel);
    log::debug("  m_customGauntletScrollLayer: {}", (void*)m_customGauntletScrollLayer);
    log::debug("  m_customGauntletScrollBar: {}", (void*)m_customGauntletScrollBar);
    log::debug("  m_customListLoadingCircle: {}", (void*)m_customListLoadingCircle);
    log::debug("  m_managerButton: {}", (void*)m_managerButton);

    log::debug("  m_showingCustomList: {}", m_showingCustomList);
    log::debug("  m_exiting: {}", m_exiting);
    log::debug("  m_playBlocked: {}", m_playBlocked);
    log::debug("  m_locked: {}", m_locked);

    log::debug(
        "  m_gauntletPacks: {} (count={})",
        (void*)m_gauntletPacks, m_gauntletPacks ? m_gauntletPacks->count() : 0
    );
    log::debug("  m_fetchHolder.isPending(): {}", m_fetchHolder.isPending());

    log::debug("  s_scrollLocation (static): {}", s_scrollLocation);
    log::debug("  s_showCustomList (static): {}", s_showCustomList);
}

void BetterGauntletSelectLayer::keyBackClicked() {
    onBack(nullptr);
}

void BetterGauntletSelectLayer::onBack(CCObject* sender) {
    if (m_exiting) return;
    m_exiting = true;
    s_scrollLocation = 0.f;
    s_showCustomList = false;
    auto scene = CreatorLayer::scene();
    CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f, scene));
}

void BetterGauntletSelectLayer::onRefresh(CCObject* sender) {
    if (m_showingCustomList) {
        if (m_customGauntletScrollLayer) { m_customGauntletScrollLayer->removeFromParent(); m_customGauntletScrollLayer = nullptr; }
        if (m_customGauntletScrollBar) { m_customGauntletScrollBar->removeFromParent(); m_customGauntletScrollBar = nullptr; }
        if (m_customListLoadingCircle) { m_customListLoadingCircle->removeFromParent(); m_customListLoadingCircle = nullptr; }

        CustomGauntletManager::get()->clearCache();
        buildCustomList();
    } else {
        saveScrollPos();

        if (m_sliderLabel) { m_sliderLabel->removeFromParent(); m_sliderLabel = nullptr; }
        if (m_customScrollLayer) { m_customScrollLayer->removeFromParent(); m_customScrollLayer = nullptr; }
        if (m_customScrollBar) { m_customScrollBar->removeFromParent(); m_customScrollBar = nullptr; }
        if (m_gauntletBtnContainer) { m_gauntletBtnContainer->removeFromParent(); m_gauntletBtnContainer = nullptr; }
        if (m_gauntletPacks) { m_gauntletPacks->release(); m_gauntletPacks = nullptr; }

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto spinner = CCSprite::create("loadingCircle.png");
        spinner->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
        spinner->runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360.0f)));
        spinner->setPosition(winSize / 2);
        spinner->setID("loading-circle");
        m_loadingCircle = spinner;
        this->addChild(m_loadingCircle, 10);

        GameLevelManager::get()->m_levelManagerDelegate = this;
        GameLevelManager::get()->getGauntlets();
    }
}

void BetterGauntletSelectLayer::onNewInfo(CCObject* sender) {
    std::vector<GauntletInfoPopup::InfoPage> pages = {
        {
            "The Lost Gauntlets",
            "<cy>The Lost Gauntlets</c> are a series of themed collections "
            "of five (5) rated levels that grant a <cg>special reward</c> upon completion. "
            "They are made by <co>RobTop</c> based on which levels fit the theme, or are placed "
            "in by official <cj>Creator Contests</c>. Make sure to check in regularly for new "
            "Gauntlet Creator Contests on <co>RobTop's</c> <cr>social medias</c>!"
        },
        {
            "The Better Gauntlets",
            "<cc>The Better Gauntlets</c> list is a collection made up of <cl>custom Gauntlets</c>. "
            "These Gauntlets are collections of five (5) rated levels which grant a <cg>custom reward</c> once "
            "completed. These Gauntlets are made by <co>Gauntlet Managers</c>. The themes of the "
            "levels should fit the theme of the Gauntlet itself, or may be placed in via "
            "<cj>Creator Contests</c>."
        },
        {
            "Credits",
            "hi chat"
        }
    };

    GauntletInfoPopup::create(pages)->show();
}

void BetterGauntletSelectLayer::onDiscord(CCObject* sender) {
    createQuickPopup(
        "Join the Community",
        "Come join the community Discord and talk about current and upcoming <cc>Gauntlets</c>, stuff about <cg>Geometry Dash</c>, or <cy>anything at all</c>!\n\nWe'd be happy to have you!",
        "Decline",
        "Accept",
        [](FLAlertLayer*, bool accepted) {
            if (accepted) web::openLinkInBrowser("https://discord.gg/fuz6JDpncq");
        }
    );
}

void BetterGauntletSelectLayer::onLeaderboard(CCObject* sender) {
    LeaderboardPopup::create()->show();
}

// Custom gauntlets toggle

void BetterGauntletSelectLayer::toggleList(CCObject* sender) {
    m_showingCustomList = !m_showingCustomList;
    s_showCustomList = m_showingCustomList;

    if (m_customScrollLayer) m_customScrollLayer->setVisible(!m_showingCustomList);
    if (m_customScrollBar) m_customScrollBar->setVisible(!m_showingCustomList);
    if (m_vanillaTitle) m_vanillaTitle->setVisible(!m_showingCustomList);
    if (m_betterTitle) m_betterTitle->setVisible(m_showingCustomList);
    if (m_customGauntletScrollLayer) m_customGauntletScrollLayer->setVisible(m_showingCustomList);
    if (m_customGauntletScrollBar) m_customGauntletScrollBar->setVisible(m_showingCustomList);
    if (m_customListLoadingCircle) m_customListLoadingCircle->setVisible(m_showingCustomList);
    buildCustomList();
}

void BetterGauntletSelectLayer::buildCustomList() {
    // Already built, or a fetch is already in flight - safe to call repeatedly.
    if (m_customGauntletScrollLayer || m_customListLoadingCircle) return;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    if (CustomGauntletManager::get()->hasCached()) {
        populateCustomList(CustomGauntletManager::get()->getCached());
        return;
    }

    auto loadingCircle = CCSprite::create("loadingCircle.png");
    loadingCircle->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
    loadingCircle->runAction(CCRepeatForever::create(CCRotateBy::create(1.0f, 360.0f)));
    loadingCircle->setID("custom-list-loading"_spr);
    loadingCircle->setPosition(winSize / 2);
    loadingCircle->setVisible(m_showingCustomList);
    this->addChild(loadingCircle, 10);
    m_customListLoadingCircle = loadingCircle;

    m_fetchHolder.spawn(
        CustomGauntletManager::get()->fetchAll(),
        [this](web::WebResponse res) {
            if (m_customListLoadingCircle) {
                m_customListLoadingCircle->removeFromParent();
                m_customListLoadingCircle = nullptr;
            }

            if (!res.ok()) {
                if (m_showingCustomList) {
                    Notification::create(
                        fmt::format("Failed to load custom gauntlets: HTTP {}", res.code()),
                        NotificationIcon::Error, 2.f
                    )->show();
                    m_showingCustomList = false;
                    s_showCustomList = false;
                    if (m_customScrollLayer) m_customScrollLayer->setVisible(true);
                    if (m_customScrollBar) m_customScrollBar->setVisible(true);
                    if (m_vanillaTitle) m_vanillaTitle->setVisible(true);
                    if (m_betterTitle) m_betterTitle->setVisible(false);
                }
                return;
            }

            auto body = res.string().unwrapOr("");
            if (body.empty() || body == "-1") {
                if (m_showingCustomList) {
                    Notification::create("There are no Gauntlets yet!", NotificationIcon::Warning, 2.f)->show();
                }
                return;
            }

            auto gauntlets = CustomGauntletManager::get()->parse(body);
            populateCustomList(gauntlets);
        }
    );
}

void BetterGauntletSelectLayer::populateCustomList(std::vector<CustomGauntletData> const& gauntlets) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto container = CCMenu::create();
    container->setLayout(
        RowLayout::create()
        ->setAxisAlignment(AxisAlignment::Start)
        ->setGap(3)
        ->setAutoGrowAxis(true)
        ->setPadding({60, 0, 60, 0})
    );
    container->setPositionY(117);
    container->setID("custom-gauntlet-btns"_spr);

    for (auto const& data : gauntlets) {
        auto node = CustomGauntletNode::create(data,
            [](CustomGauntletData const& tapped) {
                auto sc = CustomGauntletLayer::scene(tapped);
                if (sc) CCDirector::sharedDirector()->replaceScene(
                    CCTransitionFade::create(0.5f, sc));
            }
        );
        if (!node) continue;

        if (data.featured) {
            auto glow = NineSlice::create("GR_featureGlow_001.png"_spr);
            glow->setContentSize({85, 160});
            glow->setScale(1.475);
            glow->setColor({255, 200, 67});
            glow->setPosition(node->getContentSize() / 2);
            glow->setID("feature-glow"_spr);
            node->addChild(glow, -1);

            auto stars = GameToolbox::particleFromString(
                "8a-1a1.75a0.25a4a90a180a0a20a45a100a0a0a0a0a0a0a50a0a90a53a0.964706a0a0.847059a0a0.329412a0a0.5a0a10a0a90a53a0.321569a0a0.192157a0a0a0a0.25a0a0.35a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a169a0a0a0.5a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            stars->setPosition(node->getContentSize() / 2);
            stars->setID("stars");
            node->addChild(stars, 1);
        }

        container->addChild(node);
    }

    if (container->getChildrenCount() < 5) container->setAnchorPoint({0.5, 0.5});
    else {
        container->setAnchorPoint({0, 0.5});
        container->setPositionX(0);
    }

    container->updateLayout();

    auto scrollLayer = alpha::ui::AdvancedScrollLayer::create(container->getContentSize());
    scrollLayer->setHorizontalScroll(true);
    scrollLayer->setVerticalScroll(false);
    scrollLayer->setPosition(winSize.width / 2, winSize.height / 2 - 19);
    scrollLayer->setContentWidth(winSize.width);
    scrollLayer->setID("custom-gauntlet-scroll"_spr);
    scrollLayer->ignoreAnchorPointForPosition(false);
    scrollLayer->setVisible(m_showingCustomList);
    this->addChild(scrollLayer);
    m_customGauntletScrollLayer = scrollLayer;

    scrollLayer->getContentLayer()->addChild(container);
    scrollLayer->getContentLayer()->setContentWidth(container->getContentWidth());

    auto scrollBar = alpha::ui::AdvancedScrollBar::create(
        scrollLayer, alpha::ui::ScrollOrientation::HORIZONTAL
    );
    scrollBar->setPosition({winSize.width / 2, scrollLayer->getPositionY() - 126});
    scrollBar->setContentSize({12, winSize.height + 125});
    scrollBar->setID("custom-gauntlet-bar"_spr);
    scrollBar->setVisible(m_showingCustomList);
    this->addChild(scrollBar, 1);
    m_customGauntletScrollBar = scrollBar;
}

void BetterGauntletSelectLayer::checkManagerStatus() {
    auto accountID = GJAccountManager::get()->m_accountID;
    Ref<BetterGauntletSelectLayer> self(this);
    GauntletManagerCache::get()->isManager(accountID, [self](bool isManager) {
        if (!isManager) log::info("User is not a Gauntlet Manager");
        if (!self->getParent()) log::info("BetterGauntletSelectLayer is not in the scene graph");
        if (self->m_managerButton) log::info("Manager button already exists");

        auto BLMenu = self->getChildByIDRecursive("bottom-left-menu");
        if (!BLMenu) return;

        auto managerBtnSpr = CircleButtonSprite::createWithSprite(
            "GR_gauntletStar_001.png"_spr, 1,
            CircleBaseColor::DarkPurple, CircleBaseSize::Medium
        );
        managerBtnSpr->setScale(0.75);

        auto manageBtn = CCMenuItemExt::createSpriteExtra(managerBtnSpr, [](CCMenuItemSpriteExtra*) {
            GauntletManagerPopup::create()->show();
        });
        manageBtn->setID("manager-button"_spr);
        self->m_managerButton = manageBtn;
        BLMenu->addChild(manageBtn);
        BLMenu->updateLayout();
    });
}
