#include <Geode/Geode.hpp>
#include <Geode/Loader.hpp>
#include <Geode/ui/Layout.hpp>
#include <Geode/ui/SimpleAxisLayout.hpp>
#include <argon/argon.hpp>
#include <alphalaneous.alphas-ui-pack/include/API.hpp>
#include "GauntletSelectLayer.hpp"
#include "../Managers/GauntletManagerPopup.hpp"

using namespace geode::prelude;

cocos2d::CCNode* RedesignedGauntletSelectLayer::getChildBySpriteFrameNameRecursive(cocos2d::CCNode* parent, char const* name) {
    return findFirstChildRecursive<cocos2d::CCNode>(parent, [=](auto* spr) {
        return isSpriteFrameName(spr, name);
    });
}

bool RedesignedGauntletSelectLayer::init(int gauntletType) {

    if (!GauntletSelectLayer::init(gauntletType)) return false;
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto director = CCDirector::sharedDirector();

    auto textureCache = CCTextureCache::sharedTextureCache();
    auto spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();

    auto GDUtils = Loader::get()->getLoadedMod("gdutilsdevs.gdutils");
    if (GDUtils) {
        auto settingVal = GDUtils->getSettingValue<bool>("gauntletDesign");
        if (settingVal) {
            GDUtils->setSettingValue<bool>("gauntletDesign", false);
        }
    }

    if (PlatformToolbox::isControllerConnected()) {
        auto controllerBtn = getChildByID("controller-back-hint");
        controllerBtn->setZOrder(1);
    }
    auto TRMenu = this->getChildByIDRecursive("top-right-menu");
    if (!TRMenu) return false;
    TRMenu->setPosition(director->getScreenRight() - 24, 254.5);
    TRMenu->setContentHeight(125);

    auto BLMenu = getChildByID("bottom-left-menu");
    if (!BLMenu) return false;
    BLMenu->setAnchorPoint({0.5, 0});
    BLMenu->setPosition({director->getScreenLeft() + 30, 12.5});
    BLMenu->setLayout(ColumnLayout::create()
        ->setAutoGrowAxis(true)
    );

    auto discordIcon = CCSprite::create("GR_discord_02_001.png"_spr);
    discordIcon->setScale(0.65);

    auto discordSpr = CircleButtonSprite::create(
        discordIcon,
        CircleBaseColor::Green,
        CircleBaseSize::Medium
    );
    discordSpr->setScale(0.75);

    auto discordBtn = CCMenuItemSpriteExtra::create(
        discordSpr,
        this,
        menu_selector(RedesignedGauntletSelectLayer::onDiscord)
    );
    discordBtn->setID("discord-button"_spr);

    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoBtn_001.png");
    infoSpr->setScale(0.75);

    CCMenuItemSpriteExtra* infoBtn = CCMenuItemSpriteExtra::create(
        infoSpr,
        this,
        menu_selector(RedesignedGauntletSelectLayer::onNewInfo)
    );
    infoBtn->setID("info-button"_spr);

    BLMenu->addChild(infoBtn);
    BLMenu->addChild(discordBtn);
    BLMenu->removeChildByID("info-button");
    BLMenu->updateLayout();

    auto BRMenu = getChildByID("bottom-right-menu");
    if (!BRMenu) return false;
    BRMenu->setAnchorPoint({0.5, 0});
    BRMenu->setPosition({director->getScreenRight() - 30, 12.5});
    BRMenu->setLayout(
        ColumnLayout::create()
        ->setAxisReverse(false)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setGap(10.0)
        ->setAutoGrowAxis(true)
    );

    BRMenu->updateLayout();

    auto topMenu = CCMenu::create();
    if (!topMenu) return false;
    topMenu->setPosition({winSize.width / 2, director->getScreenTop() - 39});
    topMenu->setID("top-menu"_spr);
    this->addChild(topMenu, 1);

    auto title = this->getChildByID("title");
    auto TLCorner = this->getChildByID("top-left-corner");
    auto TRCorner = this->getChildByID("top-right-corner");
    auto BLCorner = this->getChildByID("bottom-left-corner");
    auto BRCorner = this->getChildByID("bottom-right-corner");
    if (title &&TLCorner && TRCorner && BLCorner && BRCorner) {
        title->setVisible(false);
        TLCorner->setVisible(false);
        TRCorner->setVisible(false);
        BLCorner->setVisible(false);
        BRCorner->setVisible(false);
    }

    auto refreshSpr = Mod::get()->getSettingValue<double>("rescale-refresh-spr");
    if (refreshSpr) {
        auto loadCircle = getChildByIDRecursive("loading-circle");
        loadCircle->setPositionY(-18.5);
        loadCircle->setScale(refreshSpr);
    }
    auto floor = CCSprite::createWithSpriteFrameName("gauntletGround_001.png"_spr);
    if (floor) {
        floor->setID("menu-floor"_spr);
        floor->setPositionY(35);
        
        auto floorSize = floor->getContentSize();
        
        float scaleX = winSize.width / floorSize.width;
        float scaleY = winSize.height / floorSize.height;
        float scale = std::min(scaleX, scaleY);
        
        floor->setScale(scale);
        floor->setPosition(ccp(winSize.width / 2, floorSize.height * scale / 2));
        floor->setAnchorPoint(ccp(0.5, 0.5));
        floor->setZOrder(-2);
        floor->setColor(ccc3(175, 175, 175));
        this->addChild(floor);
    }
    auto TLCornerNew = CCSpriteGrayscale::createWithSpriteFrameName("GJ_sideArt_001.png");
    if (TLCornerNew) {
        TLCornerNew->setID("top-left-corner"_spr);
        TLCornerNew->setPosition(ccp(director->getScreenLeft() - 1, director->getScreenTop() + 1));
        TLCornerNew->setAnchorPoint(ccp(0 , 1));
        TLCornerNew->setFlipY(true);
        TLCornerNew->setColor(ccc3(67, 67, 67));
        this->addChild(TLCornerNew);
    }
    auto TRCornerNew = CCSpriteGrayscale::createWithSpriteFrameName("GJ_sideArt_001.png");
    if (TRCornerNew) {
        TRCornerNew->setID("top-right-corner"_spr);
        TRCornerNew->setPosition(ccp(director->getScreenRight() + 1, director->getScreenTop() + 1));
        TRCornerNew->setAnchorPoint(ccp(1 , 1));
        TRCornerNew->setFlipY(true);
        TRCornerNew->setFlipX(true);
        TRCornerNew->setColor(ccc3(67, 67, 67));
        this->addChild(TRCornerNew);
    }
    auto backgroundColor = static_cast<CCSprite*>(this->getChildByID("background"));
    if (backgroundColor) {
        backgroundColor->setColor(ccc3(34, 34, 34));
        backgroundColor->setZOrder(-99);
    }
    m_fields->m_vanillaTitle = CCSprite::create("GR_gauntletTitle_01_001.png"_spr);
    if (m_fields->m_vanillaTitle) {
        m_fields->m_vanillaTitle->setID("title"_spr);
        m_fields->m_vanillaTitle->setAnchorPoint(ccp(0.5, 0.5));
        m_fields->m_vanillaTitle->setZOrder(10);
        m_fields->m_vanillaTitle->setScale(0.575);
        topMenu->addChild(m_fields->m_vanillaTitle);
    }
    m_fields->m_betterTitle = CCSprite::create("GR_gauntletTitle_02_001.png"_spr);
    if (m_fields->m_betterTitle) {
        m_fields->m_betterTitle->setID("better-title"_spr);
        m_fields->m_betterTitle->setAnchorPoint(ccp(0.5, 0.5));
        m_fields->m_betterTitle->setZOrder(10);
        m_fields->m_betterTitle->setScale(0.575);
        m_fields->m_betterTitle->setVisible(false);
        topMenu->addChild(m_fields->m_betterTitle);
    }
    // if (Mod::get()->getVersion() >= VersionInfo::parse("2.0.0").unwrap()) {
    //     auto lockBtn = CCMenuItemSpriteExtra::create(
    //         CCSpriteGrayscale::createWithSpriteFrameName("GJ_lock_001.png"),
    //         this,
    //         menu_selector(RedesignedGauntletSelectLayer::onLock)
    //     );
    //     lockBtn->setID("secret-button"_spr);
    //     lockBtn->setPosition({115, 15});
    //     lockBtn->m_baseScale = 0.75;
    //     lockBtn->setScale(0.75);
    //     lockBtn->setOpacity(80);
    //     topMenu->addChild(lockBtn);
    // }

    auto toggleOff = CCSpriteGrayscale::create("GR_gauntletStar_001.png"_spr);
    toggleOff->setOpacity(80);
    toggleOff->setColor(ccc3(128, 128, 128));
    auto toggleOn = CCSprite::create("GR_gauntletStar_001.png"_spr);

    auto toggleSpr = CCMenuItemToggler::create(
        toggleOff,
        toggleOn,
        this,
        menu_selector(RedesignedGauntletSelectLayer::toggleList)
    );
    toggleSpr->setID("toggle-list-button");
    toggleSpr->setPosition({102, 17.5});
    toggleSpr->setScale(0.65);
    toggleSpr->setOpacity(80);
    topMenu->addChild(toggleSpr);

    auto decoParentNode = CCNode::create();
    if (decoParentNode) {
        decoParentNode->setID("background-decoration"_spr);
        decoParentNode->setPosition(0, 0);
        
        decoParentNode->setZOrder(-1);
        this->addChild(decoParentNode);
    
        auto chainParentNode = CCNode::create();
        if (chainParentNode) {
            chainParentNode->setID("chain-parent"_spr);
            chainParentNode->setPosition(0, 0);
            chainParentNode->setZOrder(-1);
            decoParentNode->addChild(chainParentNode);

            for (int c = 0; c < 4; c++) {
                auto chain = CCSprite::createWithSpriteFrameName("chain_01_001.png");
                chain->setID(fmt::format("chain-{}", c + 1));
                chain->setColor(ccc3(20, 20, 20));
                chain->setPosition(ccp(0, 0));
                chain->setRotation(180);
                chainParentNode->addChild(chain);
            }
            CCSprite* chain1 = static_cast<CCSprite*>(getChildByIDRecursive("chain-1"));
            chain1->setScale(2.0);
            chain1->setOpacity(255);
            chain1->setPosition(ccp(director->getScreenLeft() + 50, director->getScreenTop() - 70));

            CCSprite* chain2 = static_cast<CCSprite*>(getChildByIDRecursive("chain-2"));
            chain2->setScale(1.65);
            chain2->setOpacity(200);
            chain2->setPosition(ccp(director->getScreenRight() - 40, director->getScreenTop() - 55));
            
            CCSprite* chain3 = static_cast<CCSprite*>(getChildByIDRecursive("chain-3"));
            chain3->setOpacity(127);
            chain3->setPosition(ccp(director->getScreenRight() - 100, director->getScreenTop() - 26));
            
            CCSprite* chain4 = static_cast<CCSprite*>(getChildByIDRecursive("chain-4"));
            chain4->setScale(0.75);
            chain4->setOpacity(80);
            chain4->setPosition(ccp(director->getScreenLeft() + 90, director->getScreenTop() - 24));
        }
    }
    
    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
    if (enableParticles) {
        auto bgParticleNodeA = CCParticleSystemQuad::create();
        if (bgParticleNodeA) {
            CCParticleSystemQuad* bgParticlesA = GameToolbox::particleFromString(
                "100a-1a4a2a16a90a90a0a0a300a0a128a-25a0a60a0a0a400a0a90a60a0a0a0a0a0a0a0.05a0.02a400a0a90a67a0a0a0a0a0a0a0a0.05a1a0a0.5a0a0a0a0a0a0a0a0a2a0a0a0a0a182a0a1.8a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticlesA->setPosition(ccp(winSize.width / 2, director->getScreenBottom() + 10));
            bgParticlesA->setZOrder(-2);
            this->addChild(bgParticlesA);
        }
        auto bgParticleNodeB = CCParticleSystemQuad::create();
        if (bgParticleNodeB) {
            CCParticleSystemQuad* bgParticlesB = GameToolbox::particleFromString(
                "100a-1a1.75a0a20a90a90a0a0a300a0a0a-77a0a0a0a0a5a3a0a60a0.0941176a0a0.0941176a0a0.0941176a0a1a0a2a0a0a67a0.0941176a0a0.0941176a0a0.0941176a0a1a0a0a0a0.2a0a0a0a0a0a0a0a0a2a0a0a0a0a0a0a-1a0a0a0a0a0a0a0a0a0a0a0a0;",
                NULL,
                false
            );
            bgParticlesB->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 10));
            bgParticlesB->setZOrder(-2);
            this->addChild(bgParticlesB);
        }
    }

    CCNode* exitAdjust = this->getChildByIDRecursive("back-menu");
    if (!exitAdjust) return false;
    exitAdjust->setPosition(24, 254.5);
    exitAdjust->setContentWidth(32.5);
    exitAdjust->setContentHeight(125);
    exitAdjust->updateLayout();

    setupNavigation();

    // // Check if this player is a manager and show the button if so
    // auto accountID = GJAccountManager::get()->m_accountID;
    // m_fields->m_managerCheckHolder.spawn(
    //     web::WebRequest().get(fmt::format(
    //         "https://bettergauntlets.dev/ismanager?accountId={}",
    //         Mod::get()->getSettingValue<std::string>("server-url-base"),
    //         accountID
    //     )),
    //     [this](web::WebResponse res) {
    //         if (!res.ok()) return;
    //         auto json       = res.json().unwrapOr(matjson::Value());
    //         bool isManager  = json["isManager"].asBool().unwrapOr(false);
    //         if (!isManager) log::info("User is a manager");

    //         auto BLMenu = this->getChildByIDRecursive("bottom-left-menu");
    //         if (!BLMenu) return;

    //         auto manageSpr = CCSprite::createWithSpriteFrameName("GJ_editBtn_001.png");
    //         manageSpr->setScale(0.7f);

    //         auto manageBtn = CCMenuItemExt::createSpriteExtra(manageSpr, [](CCMenuItemSpriteExtra*) {
    //             GauntletManagerPopup::create()->show();
    //         });
    //         manageBtn->setID("manager-button"_spr);
    //         static_cast<CCMenu*>(BLMenu)->addChild(manageBtn);
    //         static_cast<CCMenu*>(BLMenu)->updateLayout();
    //     }
    // );
    
    return true;
}

void RedesignedGauntletSelectLayer::updateDots() {

    if (m_fields->m_dots.empty()) {
        log::warn("updateDots: no dots exist");
        return;
    }

    if (!m_scrollLayer) {
        log::warn("updateDots: no scroll layer");
        return;
    }

    log::info("Dots: {}", m_fields->m_dots.size());

    auto sfc = CCSpriteFrameCache::sharedSpriteFrameCache();

    for(CCMenuItemSpriteExtra* btn : m_fields->m_dots) {
        auto btnIdx = std::find(m_fields->m_dots.begin(), m_fields->m_dots.end(), btn) - m_fields->m_dots.begin();

        int newPage = m_scrollLayer->m_page;
        if (newPage <= -1) newPage = m_fields->m_dots.size() + newPage;
        if (newPage >= m_fields->m_dots.size()) newPage = newPage % m_fields->m_dots.size();

        if (btnIdx == newPage)
            static_cast<CCSprite*>(btn->getNormalImage())->setDisplayFrame(sfc->spriteFrameByName("gj_navDotBtn_on_001.png"));
        else
            static_cast<CCSprite*>(btn->getNormalImage())->setDisplayFrame(sfc->spriteFrameByName("gj_navDotBtn_off_001.png"));
    }
}

void RedesignedGauntletSelectLayer::loadLevelsFinished(CCArray* gauntlets, char const* key, int type) {
    GauntletSelectLayer::loadLevelsFinished(gauntlets, key, type);
    
    setupNavigation();
}

void RedesignedGauntletSelectLayer::loadLevelsFailed(char const* key, int type) {
    GauntletSelectLayer::loadLevelsFailed(key, type);
    
    auto tryAgainText = getChildByID("try-again-text");
    if (tryAgainText) {
        tryAgainText->setPositionY(-1000);
    }
    
    Notification::create(
        "Gauntlets failed to load",
        NotificationIcon::Error,
        1.5
    )->show();
}
 
void RedesignedGauntletSelectLayer::setupNavigation() {
    if (!m_scrollLayer) return;
    
    if (m_scrollLayer->getTotalPages() == 0) return;
    
    auto scroll = Mod::get()->getSettingValue<bool>("gauntlet-scroll");
    
    if (scroll) {
        setupScrollMode();
        log::info("Using scroll navigation");
    }
    if (!scroll) {
        setupDotMode();
        log::info("Using dot navigation");
    }
    
    styleGauntletButtons();
}
 
void RedesignedGauntletSelectLayer::setupScrollMode() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
 
    if (m_fields->m_customScrollLayer && m_fields->m_customScrollLayer->getParent()) {
        return;
    }
 
    auto menu = getChildByIDRecursive("gauntlet-menu");
    auto dots = getChildByIDRecursive("page-buttons");
    auto arrows = getChildByIDRecursive("scroll-buttons-menu");
 
    if (menu) menu->setVisible(false);
    if (dots) dots->setVisible(false);
    if (arrows) arrows->setVisible(false);
 
    if (m_fields->m_customScrollLayer) {
        m_fields->m_customScrollLayer->removeFromParent();
        m_fields->m_customScrollLayer = nullptr;
    }
    if (m_fields->m_customScrollBar) {
        m_fields->m_customScrollBar->removeFromParent();
        m_fields->m_customScrollBar = nullptr;
    }
    if (m_fields->m_gauntletBtnContainer) {
        m_fields->m_gauntletBtnContainer->removeFromParent();
        m_fields->m_gauntletBtnContainer = nullptr;
    }
 
    m_fields->m_gauntletBtnContainer = CCMenu::create();
    m_fields->m_gauntletBtnContainer->setAnchorPoint({0, 0.5});
    m_fields->m_gauntletBtnContainer->setPositionX(0);
    m_fields->m_gauntletBtnContainer->setID("gauntlet-btns");
    m_fields->m_gauntletBtnContainer->setLayout(
        RowLayout::create()
        ->setAxisReverse(false)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setGap(3)
        ->setAutoGrowAxis(true)
        ->setPadding({60, 0, 60, 0})
    );
 
    m_fields->m_customScrollLayer = alpha::ui::AdvancedScrollLayer::create(
        m_fields->m_gauntletBtnContainer->getContentSize()
    );
    m_fields->m_customScrollLayer->setHorizontalScroll(true);
    m_fields->m_customScrollLayer->setVerticalScroll(false);
    m_fields->m_customScrollLayer->setPosition(winSize.width / 2, winSize.height / 2 - 19);
    m_fields->m_customScrollLayer->setContentWidth(winSize.width);
    m_fields->m_customScrollLayer->setID("gauntlet-buttons"_spr);
    m_fields->m_customScrollLayer->ignoreAnchorPointForPosition(false);
    this->addChild(m_fields->m_customScrollLayer);
 
    m_fields->m_customScrollBar = alpha::ui::AdvancedScrollBar::create(
        m_fields->m_customScrollLayer, 
        alpha::ui::ScrollOrientation::HORIZONTAL
    );
    m_fields->m_customScrollBar->setPosition({
        winSize.width / 2, 
        m_fields->m_customScrollLayer->getPositionY() - 126
    });
    m_fields->m_customScrollBar->setContentSize({12, winSize.height + 125});
    m_fields->m_customScrollBar->setID("gauntlet-scrollbar"_spr);
    this->addChild(m_fields->m_customScrollBar);
    
    std::vector<CCNode*> gauntletBtns;
    
    for (int p = 0; p < m_scrollLayer->getTotalPages(); p++) {
        auto page = getChildByIDRecursive(fmt::format("gauntlet-page-{}", p + 1));
        if (!page) continue;
        
        auto pageMenu = page->getChildByIDRecursive("gauntlet-menu");
        if (!pageMenu) continue;
        
        for (int b = 0; b < 3; b++) {
            auto gauntletBtn = pageMenu->getChildByIDRecursive(fmt::format("gauntlet-button-{}", b + 1));
            if (gauntletBtn) {
                gauntletBtns.push_back(gauntletBtn);
            }
        }
    }
    
    for (auto gauntletBtn : gauntletBtns) {
        gauntletBtn->retain();
        gauntletBtn->removeFromParentAndCleanup(false);
        m_fields->m_gauntletBtnContainer->addChild(gauntletBtn);
        gauntletBtn->release();
    }
    
    m_fields->m_gauntletBtnContainer->updateLayout();
 
    m_fields->m_customScrollLayer->getContentLayer()->addChild(m_fields->m_gauntletBtnContainer);
    m_fields->m_customScrollLayer->getContentLayer()->setContentWidth(m_fields->m_gauntletBtnContainer->getContentWidth());

    m_fields->m_sliderLabel = CCLabelBMFont::create("", "chatFont.fnt");
    m_fields->m_sliderLabel->setScale(0.5f);
    m_fields->m_sliderLabel->setPosition({winSize.width / 2, director->getScreenBottom() + 6.5f});
    this->addChild(m_fields->m_sliderLabel);

    styleGauntletButtons();
    loadScrollPos();
}


void RedesignedGauntletSelectLayer::setupDotMode() {

    log::info("Entering dot mode");
    log::info("Pages: {}", m_scrollLayer ? m_scrollLayer->getTotalPages() : -1);

    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

    if (auto menu = getChildByIDRecursive("gauntlet-menu")) menu->setVisible(true);
    else {
        log::warn("Could not find gauntlet-menu to show");
        return;
    }
    if (auto dots = getChildByIDRecursive("page-buttons")) dots->setVisible(true);
    else {
        log::warn("Could not find page-buttons to show");
        return;
    }
    if (auto arrows = getChildByIDRecursive("scroll-buttons-menu")) arrows->setVisible(true);
    else {
        log::warn("Could not find scroll-buttons-menu to show");
        return;
    }

    if (m_fields->m_dotsMenu && m_fields->m_dotsMenu->getParent()) {
        return;
    }
 
    if (m_fields->m_customScrollLayer) {
        m_fields->m_customScrollLayer->removeFromParent();
        m_fields->m_customScrollLayer = nullptr;
    }
    if (m_fields->m_customScrollBar) {
        m_fields->m_customScrollBar->removeFromParent();
        m_fields->m_customScrollBar = nullptr;
    }
    if (m_fields->m_gauntletBtnContainer) {
        m_fields->m_gauntletBtnContainer->removeFromParent();
        m_fields->m_gauntletBtnContainer = nullptr;
    }
 
    m_fields->m_dots.clear();
    if (m_fields->m_dotsMenu) {
        m_fields->m_dotsMenu->removeFromParent();
        m_fields->m_dotsMenu = nullptr;
    }
 
    m_fields->m_dotsMenu = CCMenu::create();
    m_fields->m_dotsMenu->setLayout(AxisLayout::create());
    m_fields->m_dotsMenu->setPositionY(director->getScreenBottom() + 15);
    m_fields->m_dotsMenu->setContentWidth(winSize.width - 200);
    m_fields->m_dotsMenu->setID("page-navigation"_spr);
    addChild(m_fields->m_dotsMenu);
 
    for (int i = 0; i < m_scrollLayer->getTotalPages(); i++) {
        auto spr = CCSprite::createWithSpriteFrameName("gj_navDotBtn_off_001.png");
        spr->setScale(0.8);
 
        auto dot = CCMenuItemSpriteExtra::create(
            spr,
            this,
            menu_selector(RedesignedGauntletSelectLayer::onDot)
        );
 
        m_fields->m_dotsMenu->addChild(dot);
        m_fields->m_dots.push_back(dot);
    }
 
    updateDots();
    m_fields->m_dotsMenu->updateLayout();
 
    if (const auto pageButtons = m_scrollLayer->m_dots) {
        findCurrentGauntletPageUsing(pageButtons);
    }

    styleGauntletButtons();
}


void RedesignedGauntletSelectLayer::onDot(CCObject* sender) {
    auto btnIdx = std::find(m_fields->m_dots.begin(), m_fields->m_dots.end(), sender) - m_fields->m_dots.begin();
    m_scrollLayer->instantMoveToPage(btnIdx);
    m_scrollLayer->updatePages();
    m_scrollLayer->repositionPagesLooped();
}

void RedesignedGauntletSelectLayer::findCurrentGauntletPageUsing(CCArray* pageButtons) {
    log::info("pageButtons count = {}", pageButtons->count());

    int i = 0;

    for (auto obj : CCArrayExt<CCObject*>(pageButtons)) {
        auto ccSprite = typeinfo_cast<CCSprite*>(obj);

        if (!ccSprite) {
            log::error("Dot {} is not a CCSprite", i);
            continue;
        }

        if (ccSprite->getDisplayedColor() == ccc3(255, 255, 255)) {
            m_fields->currentGauntletPage = i + 1;
            break;
        }

        i++;
    }
}

void RedesignedGauntletSelectLayer::pressGauntlet(int desiredGauntlet) {
    if (const auto theGauntletPage = getChildByIDRecursive(fmt::format("gauntlet-page-{}", m_fields->currentGauntletPage))) {
        if (const auto theGauntlet = theGauntletPage->getChildByIDRecursive(fmt::format("gauntlet-button-{}", desiredGauntlet))) {
            GauntletSelectLayer::onPlay(theGauntlet);
        }
    }
}

#ifndef GEODE_IS_ANDROID
void RedesignedGauntletSelectLayer::scrollLayerWillScrollToPage(BoomScrollLayer* p0, int p1) {
    GauntletSelectLayer::scrollLayerWillScrollToPage(p0, p1);
    if (const auto pageButtons = m_scrollLayer->m_dots) {
        RedesignedGauntletSelectLayer::findCurrentGauntletPageUsing(pageButtons);
    }
}

void RedesignedGauntletSelectLayer::scrollLayerScrolledToPage(BoomScrollLayer* p0, int p1) {
    GauntletSelectLayer::scrollLayerScrolledToPage(p0, p1);
    updateDots();
}
#else
void RedesignedGauntletSelectLayer::updateArrows() {
    GauntletSelectLayer::updateArrows();
    if (!m_scrollLayer) return;
    if (const auto pageButtons = m_scrollLayer->m_dots) {
        RedesignedGauntletSelectLayer::findCurrentGauntletPageUsing(pageButtons);
    }
    updateDots();
}
#endif

void RedesignedGauntletSelectLayer::onNewInfo(CCObject* sender) {
    MDPopup* popup = MDPopup::create(
        "The Lost Gauntlets",
        "<cy>The Lost Gauntlets</c> are a series of themed collections of five (5) rated levels that grant a <cg>special reward</c> upon completion. They are made by <co>RobTop</c> based on which levels fit the theme, or are placed in by official <cj>Creator Contests</c>.",
        "OK"
    );
    popup->show();
}

void RedesignedGauntletSelectLayer::onDiscord(CCObject* sender) {
    createQuickPopup(
        "Join the Community",
        "Come join and talk about the <cc>Gauntlets</c>, <cg>Geometry Dash</c>, or <cy>anything at all</c>!\n\nWe're happy to have you!",
        "Decline",
        "Accept",
        [](FLAlertLayer*, bool accepted) {
            if (accepted) {
                web::openLinkInBrowser("https://discord.gg/fuz6JDpncq");
            }
        }
    );
}

void RedesignedGauntletSelectLayer::styleGauntletButtons() {

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    
    int btn = 1;
    int total = 0;

    auto pageDots = static_cast<CCSpriteBatchNode*>(getChildByIDRecursive("page-buttons"));
    if (!pageDots) {
        log::error("page-buttons missing");
        return;
    }

    auto scrollLayer = static_cast<alpha::ui::AdvancedScrollLayer*>(getChildByIDRecursive("gauntlet-buttons"_spr));

    if (scrollLayer) {
    
        std::vector<CCSprite*> gauntletBtns;
        
        auto btnNode = scrollLayer->getContentLayer()->getChildByID("gauntlet-btns");
        if (!btnNode) { log::error("gauntlet-btns missing"); return; }
        auto btnContainer = btnNode->getChildren();

        auto obj = nullptr;

        for (auto obj : CCArrayExt(btnContainer)) {
            auto gauntletBtn = typeinfo_cast<CCNode*>(obj);

            if (!gauntletBtn) {
                btn++;
                log::warn("node {} invalid", btn);
                continue;
            }

            auto btnNode = static_cast<CCSprite*>(gauntletBtn->getChildByIDRecursive("background"));

            if (btnNode) btnNode->setContentSize({110, 220});

            auto infoNode = static_cast<CCSprite*>(gauntletBtn->getChildByIDRecursive("gauntlet-info-node"));
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
            total++;
        }
    }
    else {
        auto gauntletPages = getChildByIDRecursive("gauntlet-pages")->getChildren();

        for (int p = 0; p < gauntletPages->count(); p++) {
            auto page = getChildByIDRecursive(fmt::format("gauntlet-page-{}", p + 1));
            if (!page) continue;

            for (int b = 0; b < 3; b++) {
                auto gauntletBtn = page->getChildByIDRecursive(fmt::format("gauntlet-button-{}", b + 1));

                auto btnNode = static_cast<CCSprite*>(gauntletBtn->getChildByIDRecursive("background"));
                if (btnNode) btnNode->setContentSize({110, 220});

                auto infoNode = static_cast<CCSprite*>(gauntletBtn->getChildByIDRecursive("gauntlet-info-node"));

                auto GDUtils = Loader::get()->getLoadedMod("gdutilsdevs.gdutils");
                if (GDUtils) {
                    auto settingVal = GDUtils->getSettingValue<bool>("gauntletDesign");
                    if (!settingVal) infoNode->setPositionY(-2.5);
                }
                
                else infoNode->setPositionY(-2.5);

                auto nameLabel = static_cast<CCLabelBMFont*>(gauntletBtn->getChildByIDRecursive("gauntlet-name-label"));
                if (nameLabel) nameLabel->setPositionY(84);

                auto gauntletLabel = static_cast<CCLabelBMFont*>(gauntletBtn->getChildByIDRecursive("gauntlet-label"));
                if (gauntletLabel) gauntletLabel->setPositionY(nameLabel->getPositionY() - 15);

                auto nameShadowLabel = static_cast<CCLabelBMFont*>(gauntletBtn->getChildByIDRecursive("gauntlet-name-shadow-label"));
                if (nameShadowLabel) nameShadowLabel->setPosition(ccp(nameLabel->getPositionX() + 2, nameLabel->getPositionY() - 2));

                auto gauntletShadowLabel = static_cast<CCLabelBMFont*>(gauntletBtn->getChildByIDRecursive("gauntlet-shadow-label"));
                if (gauntletShadowLabel) gauntletShadowLabel->setPosition(ccp(gauntletLabel->getPositionX() + 2, gauntletLabel->getPositionY() - 2));

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
                if (rewardLabel && rewardShadowLabel) {
                    rewardLabel->setPositionY(chestSpr->getPositionY() - 13.5);
                    rewardLabel->setScale(0.5);
                    rewardLabel->setZOrder(3);
                    rewardShadowLabel->setPosition(ccp(rewardLabel->getPositionX() + 2, rewardLabel->getPositionY() - 2));
                    rewardShadowLabel->setScale(0.5);
                }

                btn++;
                total++;
            }
        }
    }
}

// BETTER GAUNTLETS SERVER LIST

void RedesignedGauntletSelectLayer::toggleList(CCObject* sender) {
    m_fields->showingCustomList = !m_fields->showingCustomList;

    // Vanilla UI visibility
    if (m_fields->m_customScrollLayer)
        m_fields->m_customScrollLayer->setVisible(!m_fields->showingCustomList);
    if (m_fields->m_customScrollBar)
        m_fields->m_customScrollBar->setVisible(!m_fields->showingCustomList);
    if (m_fields->m_vanillaTitle)
        m_fields->m_vanillaTitle->setVisible(!m_fields->showingCustomList);

    if (m_fields->showingCustomList) {

        log::debug("build custom list");

        m_refreshButton->setVisible(false);
        m_fields->m_vanillaTitle->setVisible(false);
        m_fields->m_betterTitle->setVisible(true);

        startAuth();

        // Check if this player is a manager and show the button if so
        auto accountID = GJAccountManager::get()->m_accountID;

        log::debug("Checking Manager access...");

        m_fields->m_managerCheckHolder.spawn(
            web::WebRequest().get(fmt::format(
                "https://bettergauntlets.dev/ismanager?accountId={}",
                accountID
            )),
            [this](web::WebResponse res) {
                if (!res.ok()) return;
                auto json = res.json().unwrapOr(matjson::Value());
                bool isManager = json["isManager"].asBool().unwrapOr(false);
                if (!isManager) {
                    log::info("User is not a Gauntlet manager");
                    return;
                }
                log::info("User is a Gauntlet manager!");

                auto BLMenu = this->getChildByIDRecursive("bottom-left-menu");
                if (!BLMenu) return;

                auto manageSpr = CCSprite::create("GR_gauntletStar_001.png"_spr);
                manageSpr->setScale(0.7);

                auto managerBtnSpr = CircleButtonSprite::createWithSprite(
                    "GR_gauntletStar_001.png"_spr,
                    1,
                    CircleBaseColor::DarkPurple,
                    CircleBaseSize::Medium
                );
                managerBtnSpr->setScale(0.75);

                auto manageBtn = CCMenuItemExt::createSpriteExtra(managerBtnSpr, [](CCMenuItemSpriteExtra*) {
                    GauntletManagerPopup::create()->show();
                });
                manageBtn->setID("manager-button"_spr);
                static_cast<CCMenu*>(BLMenu)->addChild(manageBtn);
                static_cast<CCMenu*>(BLMenu)->updateLayout();
            }
        );

    } else {

        if (auto existing = getChildByIDRecursive("custom-gauntlet-scroll"_spr))
            existing->removeFromParent();
        if (auto existing = getChildByIDRecursive("custom-gauntlet-bar"_spr))
            existing->removeFromParent();
        if (auto existing = getChildByIDRecursive("manager-button"_spr))
            existing->removeFromParent();

        log::debug("build vanilla list");

        m_refreshButton->setVisible(true);
        m_fields->m_vanillaTitle->setVisible(true);
        m_fields->m_betterTitle->setVisible(false);
    }
}

void RedesignedGauntletSelectLayer::startAuth() {
    m_fields->m_listener.spawn(
        argon::startAuth(),
        [](Result<std::string> result) {
            if (result.isOk()) {
                auto token = std::move(result).unwrap();
                log::info("Got token: {}", token);
            } else {
                log::error("Failed to authenticate: {}", result.unwrapErr());
            }
        }
    );
}

/*
void RedesignedGauntletSelectLayer::buildCustomList() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    if (CustomGauntletManager::get()->hasCached()) {
        populateCustomList(CustomGauntletManager::get()->getCached());
        return;
    }

    auto loadingCircle = LoadingCircle::create();
    loadingCircle->setID("custom-list-loading"_spr);
    loadingCircle->setPosition(winSize / 2);
    loadingCircle->ignoreAnchorPointForPosition(false);
    loadingCircle->show();
    this->addChild(loadingCircle, 10);

    m_loadingCircle = loadingCircle;

    m_fields->m_fetchHolder.spawn(
        CustomGauntletManager::get()->fetchGauntlets(),
        [this, loadingCircle](web::WebResponse res) {
            if (loadingCircle->getParent()) this->removeChild(loadingCircle);

            if (!res.ok()) {
                Notification::create(
                    fmt::format("Failed to load custom gauntlets: HTTP {}", res.code()),
                    NotificationIcon::Error, 2.f
                )->show();
                m_fields->showingCustomList = false;
                if (m_fields->m_customScrollLayer)
                    m_fields->m_customScrollLayer->setVisible(true);
                if (m_fields->m_customScrollBar)
                    m_fields->m_customScrollBar->setVisible(true);
                if (m_refreshButton)
                    m_refreshButton->setVisible(false);
                return;
            }

            auto body = res.string().unwrapOr("");
            if (body.empty() || body == "-1") {
                Notification::create(
                    "Server returned an empty response.",
                    NotificationIcon::Error, 2.f
                )->show();
                return;
            }

            auto gauntlets = CustomGauntletManager::get()->parse(body);
            populateCustomList(gauntlets);
        }
    );
}

void RedesignedGauntletSelectLayer::populateCustomList(
    std::vector<CustomGauntletData> const& gauntlets
) {
    if (!m_fields->showingCustomList) return;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    if (auto scrollLayer = getChildByIDRecursive("custom-gauntlet-scroll"_spr)) scrollLayer->removeFromParent();
    if (auto scrollBar = getChildByIDRecursive("custom-gauntlet-bar"_spr)) scrollBar->removeFromParent();
    if (m_loadingCircle) m_loadingCircle->removeFromParent();

    auto container = CCMenu::create();
    container->setAnchorPoint({0, 0.5});
    container->setLayout(
        RowLayout::create()
        ->setAxisAlignment(AxisAlignment::Start)
        ->setGap(3)
        ->setAutoGrowAxis(true)
        ->setPadding({60, 0, 60, 0})
    );
    container->setID("custom-gauntlet-btns"_spr);

    for (auto const& data : gauntlets) {
        auto node = CustomGauntletNode::create(data, [](CustomGauntletData const& tapped) {
            auto scene = CustomGauntletLayer::scene(tapped);
            CCDirector::sharedDirector()->pushScene(
                CCTransitionFade::create(0.5f, scene)
            );
        });
        if (node) container->addChild(node);
    }

    container->updateLayout();

    auto scrollLayer = alpha::ui::AdvancedScrollLayer::create(container->getContentSize());
    scrollLayer->setHorizontalScroll(true);
    scrollLayer->setVerticalScroll(false);
    scrollLayer->setPosition(winSize.width / 2, winSize.height / 2 - 19);
    scrollLayer->setContentWidth(winSize.width);
    scrollLayer->setID("custom-gauntlet-scroll"_spr);
    scrollLayer->ignoreAnchorPointForPosition(false);
    this->addChild(scrollLayer);

    scrollLayer->getContentLayer()->addChild(container);
    scrollLayer->getContentLayer()->setContentWidth(container->getContentWidth());

    auto scrollBar = alpha::ui::AdvancedScrollBar::create(
        scrollLayer, alpha::ui::ScrollOrientation::HORIZONTAL
    );
    scrollBar->setPosition({winSize.width / 2, scrollLayer->getPositionY() - 126});
    scrollBar->setContentSize({12, winSize.height + 125});
    scrollBar->setID("custom-gauntlet-bar"_spr);
    this->addChild(scrollBar, 1);
}
*/

// SAVE AND LOAD SCROLL POSITION

void RedesignedGauntletSelectLayer::saveScrollPos() {
    if (m_fields->m_customScrollLayer) {
        float max = m_fields->m_customScrollLayer->getHorizontalMax();
        if (max > 0.f) {
            m_scrollLocation = m_fields->m_customScrollLayer->getScrollPoint().x;
        }
    }
}

void RedesignedGauntletSelectLayer::loadScrollPos() {

    if (m_scrollLocation > 0.f) {
        // Clamp in case gauntlet count changed since last save
        float restoredX = m_scrollLocation;
        m_fields->m_customScrollLayer->setScrollX(restoredX, false);
        log::debug("restored scrollPoint.x: {:.4f} of max {:.4f}", restoredX, m_fields->m_customScrollLayer->getHorizontalMax());
    }

    if (Mod::get()->getSettingValue<bool>("advscroll-pos-label") && m_fields->m_sliderLabel) {
        m_fields->m_sliderLabel->setString(
            fmt::format("{:.4f} / {:.4f}", m_scrollLocation, m_fields->m_customScrollLayer->getHorizontalMax()).c_str()
        );
    }

    styleGauntletButtons();

}

// VANILLA CALLBACKS

void RedesignedGauntletSelectLayer::onPlay(CCObject* sender) {
    saveScrollPos();
    GauntletSelectLayer::onPlay(sender);
}

void RedesignedGauntletSelectLayer::onBack(CCObject* sender) {
    m_scrollLocation = 0.f;
    m_fields->currentGauntletPage = 0;
    GauntletSelectLayer::onBack(sender);
}

void RedesignedGauntletSelectLayer::onRefresh(CCObject* sender) {

    saveScrollPos();

    if (m_fields->m_sliderLabel) {
        m_fields->m_sliderLabel->removeFromParent();
        m_fields->m_sliderLabel = nullptr;
    }

    if (m_fields->m_customScrollLayer) {
        m_fields->m_customScrollLayer->removeFromParent();
        m_fields->m_customScrollLayer = nullptr;
    }
    if (m_fields->m_customScrollBar) {
        m_fields->m_customScrollBar->removeFromParent();
        m_fields->m_customScrollBar = nullptr;
    }
    if (m_fields->m_gauntletBtnContainer) {

        m_fields->m_gauntletBtnContainer = nullptr;
    }

    if (m_fields->m_dotsMenu) {
        m_fields->m_dotsMenu->removeFromParent();
        m_fields->m_dotsMenu = nullptr;
    }
    m_fields->m_dots.clear();

    GauntletSelectLayer::onRefresh(sender);
}

