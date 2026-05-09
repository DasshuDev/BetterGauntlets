#include <Geode/Geode.hpp>
#include <Geode/Loader.hpp>
#include <Geode/ui/Layout.hpp>
#include <Geode/ui/SimpleAxisLayout.hpp>
#include <alphalaneous.alphas-ui-pack/include/API.hpp>
#include "../Hooks/DialogIcons/DialogIcons.h"
#include "GauntletSelectLayer.hpp"

using namespace geode::prelude;

cocos2d::CCNode* RedesignedGauntletSelectLayer::getChildBySpriteFrameNameRecursive(cocos2d::CCNode* parent, char const* name) {
    return findFirstChildRecursive<cocos2d::CCNode>(parent, [=](auto* spr) {
        return isSpriteFrameName(spr, name);
    });
}

bool RedesignedGauntletSelectLayer::init(int gauntletType) {
    if (!GauntletSelectLayer::init(gauntletType)) {
        auto tryAgainText = getChildByID("try-again-text");
        if (tryAgainText) {
            tryAgainText->setPositionY(-1000);
        }
        
        Notification::create(
            "Gauntlets failed to load",
            NotificationIcon::Error,
            1.5
        )->show();
        
        return false;
    }
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto director = CCDirector::sharedDirector();

    auto textureCache = CCTextureCache::sharedTextureCache();
    auto spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();

    if (PlatformToolbox::isControllerConnected()) {
        auto controllerBtn = getChildByID("controller-back-hint");
        controllerBtn->setZOrder(1);
    }
    auto BRMenu = getChildByID("bottom-right-menu");
    if (!BRMenu) return false;

    BRMenu->setContentSize(ccp(23.25, 103));
    BRMenu->setLayout(
        ColumnLayout::create()
        ->setAxisReverse(false)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setGap(10.0)
    );

    BRMenu->updateLayout();

    auto BLMenu = getChildByID("bottom-left-menu");
    if (!BLMenu) return false;

    auto infoBtnSpr = CCSprite::createWithSpriteFrameName("GJ_infoBtn_001.png");
    infoBtnSpr->setScale(0.75);

    CCMenuItemSpriteExtra* infoBtn = CCMenuItemSpriteExtra::create(
        infoBtnSpr,
        this,
        menu_selector(RedesignedGauntletSelectLayer::onNewInfo)
    );
    infoBtn->setID("info-button"_spr);

    BLMenu->addChild(infoBtn);
    BLMenu->removeChildByID("info-button");
    BLMenu->updateLayout();

    auto refreshSpr = Mod::get()->getSettingValue<double>("rescale-refresh-spr");
    if (refreshSpr) {
        auto loadCircle = getChildByIDRecursive("loading-circle");
        loadCircle->setPositionY(-18.5);
        loadCircle->setScale(refreshSpr);
    }
    auto title = this->getChildByID("title");
    if (title) {
        title->setVisible(false);
    }
    auto TLCorner = this->getChildByID("top-left-corner");
    if (TLCorner) {
        TLCorner->setVisible(false);
    }
    auto TRCorner = this->getChildByID("top-right-corner");
    if (TRCorner) {
        TRCorner->setVisible(false);
    }
    auto BLCorner = this->getChildByID("bottom-left-corner");
    if (BLCorner) {
        BLCorner->setVisible(false);
    }
    auto BRCorner = this->getChildByID("bottom-right-corner");
    if (BRCorner) {
        BRCorner->setVisible(false);
    }
    auto topMenu = CCMenu::create();
    if (topMenu) {
        topMenu->setPosition({winSize.width / 2, director->getScreenTop() - 39});
        topMenu->setID("top-menu"_spr);
        this->addChild(topMenu);
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
    auto titleRedesign = CCSprite::create("gauntletTitle_001.png"_spr);
    if (titleRedesign) {
        titleRedesign->setID("title"_spr);
        titleRedesign->setPosition(ccp(winSize.width / 2 + 2, director->getScreenTop() - 39));
        titleRedesign->setAnchorPoint(ccp(0.5, 0.5));
        titleRedesign->setZOrder(10);
        this->addChild(titleRedesign);
    }
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
            auto chain1 = getChildByIDRecursive("chain-1");
            chain1->setScale(2.0);
            chain1->setPosition(ccp(director->getScreenLeft() + 50, director->getScreenTop() - 70));

            auto chain2 = getChildByIDRecursive("chain-2");
            chain2->setScale(1.65);
            chain2->setPosition(ccp(director->getScreenRight() - 40, director->getScreenTop() - 55));
            
            auto chain3 = getChildByIDRecursive("chain-3");
            chain3->setPosition(ccp(director->getScreenRight() - 100, director->getScreenTop() - 26));
            
            auto chain4 = getChildByIDRecursive("chain-4");
            chain3->setScale(0.75);
            chain4->setPosition(ccp(director->getScreenLeft() + 90, director->getScreenTop() - 34));
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

    CCNode* topRightMenu = this->getChildByIDRecursive("top-right-menu");
    if (!topRightMenu) return false;
    topRightMenu->setPosition(director->getScreenRight() - 24, 254.5);
    topRightMenu->setContentHeight(125);

    return true;
}

void RedesignedGauntletSelectLayer::updateDots() {
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

void RedesignedGauntletSelectLayer::setupGauntlets() {
    GauntletSelectLayer::setupGauntlets();

    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

    auto scroll = Mod::get()->getSettingValue<bool>("gauntlet-scroll");
    auto menu = getChildByIDRecursive("gauntlet-menu");
    auto dots = getChildByIDRecursive("page-buttons");
    auto nav = getChildByIDRecursive("page-navigation"_spr);
    auto arrows = getChildByIDRecursive("scroll-buttons-menu");

    if (scroll && menu && dots && nav && arrows) {
        menu->setVisible(false);
        dots->setVisible(false);
        nav->setVisible(false);
        arrows->setVisible(false);

        auto btnContainer = CCMenu::create();
        btnContainer->setAnchorPoint({0, 0.5});
        btnContainer->setPositionX(0);
        btnContainer->setID("gauntlet-btns");
        btnContainer->setLayout(
            RowLayout::create()
            ->setAxisReverse(false)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setGap(3)
            ->setAutoGrowAxis(true)
            ->setPadding({60, 0, 60, 0})
        );

        auto scrollLayer = alpha::ui::AdvancedScrollLayer::create(btnContainer->getContentSize());
        scrollLayer->setHorizontalScroll(true);
        scrollLayer->setVerticalScroll(false);
        scrollLayer->setPosition(winSize.width / 2, winSize.height / 2 - 19);
        scrollLayer->setContentWidth(winSize.width);
        scrollLayer->setID("gauntlet-buttons"_spr);
        scrollLayer->ignoreAnchorPointForPosition(false);
        this->addChild(scrollLayer);

        auto scrollBar = alpha::ui::AdvancedScrollBar::create(scrollLayer, alpha::ui::ScrollOrientation::HORIZONTAL);
        scrollBar->setPosition({winSize.width / 2, scrollLayer->getPositionY() - 126});
        scrollBar->setContentSize({12, winSize.height + 50});
        scrollBar->setID("gauntlet-scrollbar"_spr);
        this->addChild(scrollBar);
        
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

            for (int g = 0; g < gauntletBtns.size(); g++) {
                auto gauntletBtn = pageMenu->getChildByIDRecursive(fmt::format("gauntlet-button-{}", g + 1));
                if (!gauntletBtn) continue;
                gauntletBtn->setID(fmt::format("gauntlet-button-{}"_spr, g + 1));
            }
        }
        
        for (auto gauntletBtn : gauntletBtns) {
            gauntletBtn->retain();
            gauntletBtn->removeFromParentAndCleanup(false);
            btnContainer->addChild(gauntletBtn);
            gauntletBtn->release();
        }
        
        btnContainer->updateLayout();

        scrollLayer->getContentLayer()->addChild(btnContainer);
        scrollLayer->getContentLayer()->setContentWidth(btnContainer->getContentWidth());

        editGauntletButton();
    }
    
    else {

        m_fields->m_dots.clear();
        if (m_fields->m_dotsMenu)
            m_fields->m_dotsMenu->removeFromParent();

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
            RedesignedGauntletSelectLayer::findCurrentGauntletPageUsing(pageButtons);
        }

        editGauntletButton();
    }
}

void RedesignedGauntletSelectLayer::onDot(CCObject* sender) {
    auto btnIdx = std::find(m_fields->m_dots.begin(), m_fields->m_dots.end(), sender) - m_fields->m_dots.begin();
    m_scrollLayer->instantMoveToPage(btnIdx);
    m_scrollLayer->updatePages();
    m_scrollLayer->repositionPagesLooped();
}

void RedesignedGauntletSelectLayer::findCurrentGauntletPageUsing(CCArray* pageButtons) {
    int i = 0;
    for (CCSprite* ccSprite : CCArrayExt<CCSprite*>(pageButtons)) {
        if (ccSprite->getDisplayedColor() == ccColor3B({255, 255, 255})) {
            m_fields->currentGauntletPage = (i + 1);
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

void RedesignedGauntletSelectLayer::onBack(cocos2d::CCObject* sender) {
    m_fields->currentGauntletPage = 0;
    GauntletSelectLayer::onBack(sender);
}

void RedesignedGauntletSelectLayer::onLock(CCObject* sender) {
    CCArray* DialogResponses = CCArray::create();
    switch (m_fields->m_dialogIndex) {
        case 0: {
            m_fields->m_dialogSprite = 1;
            DialogObject* response01A = DialogObject::create(
                "The Gauntlet Keeper",
                "<cr>HALT</c>! Who goes there!?",
                m_fields->m_dialogSprite,
                1,
                true,
                ccWHITE
            );

            DialogObject* response01B = DialogObject::create(
                "The Gauntlet Keeper",
                "Oh. It's just a player.",
                m_fields->m_dialogSprite,
                1,
                false,
                ccWHITE
            );

            DialogObject* response01C = DialogObject::create(
                "The Gauntlet Keeper",
                "Unfortunately for you, this is as far as you go.",
                m_fields->m_dialogSprite,
                1,
                false,
                ccWHITE
            );
            DialogObject* response01D = DialogObject::create(
                "The Gauntlet Keeper",
                "These next gauntlets are not <cy>meant for you</c>.",
                m_fields->m_dialogSprite,
                1,
                false,
                ccWHITE
            );

            DialogResponses->addObject(response01A);
            DialogResponses->addObject(response01B);
            DialogResponses->addObject(response01C);
            DialogResponses->addObject(response01D);

            m_fields->m_dialogIndex++;

            break;
        }
        
        case 1: {
            DialogObject* response02A = DialogObject::create(
                "The Gauntlet Keeper",
                "You again?",
                m_fields->m_dialogSprite,
                1,
                true,
                ccWHITE
            );

            DialogObject* response02B = DialogObject::create(
                "The Gauntlet Keeper",
                "I already told you, these gauntlets are locked.",
                m_fields->m_dialogSprite,
                1,
                false,
                ccWHITE
            );

            DialogObject* response02C = DialogObject::create(
                "The Gauntlet Keeper",
                "<cr>Go away</c>.",
                m_fields->m_dialogSprite,
                1,
                false,
                ccWHITE
            );

            DialogResponses->addObject(response02A);
            DialogResponses->addObject(response02B);

            m_fields->m_dialogIndex++;

            break;
        }
    }

    DialogLayer* dialog = DialogLayer::createWithObjects(DialogResponses, 5);
    dialog->addToMainScene();
    dialog->animateInRandomSide();
    DialogIcon::setDialogObjectCustomIcon(dialog, "GauntletKeeper_1.png"_spr);
}

void RedesignedGauntletSelectLayer::onNewInfo(CCObject* sender) {
    MDPopup* popup = MDPopup::create(
        "The Lost Gauntlets",
        "<cy>The Lost Gauntlets</c> are a series of themed collections of five (5) rated levels that grant a <cg>special reward</c> upon completion. They are made by <co>RobTop</c> based on which levels fit the theme, or are placed in by official <cj>Creator Contests</c>.",
        "OK"
    );
    popup->show();
}

void RedesignedGauntletSelectLayer::editGauntletButton() {

    log::info("called editGauntletButton");

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    // for (int p = 0; p < m_scrollLayer->getTotalPages(); p++) {
        // CCMenu* pages = static_cast<CCMenu*>(getChildByIDRecursive(fmt::format("gauntlet-page-{}", p + 1)));
        // CCNode* menu = pages->getChildByIDRecursive("gauntlet-menu");
        CCNode* nav = getChildByIDRecursive("page-navigation"_spr);
        CCSpriteBatchNode* pageDots = static_cast<CCSpriteBatchNode*>(getChildByIDRecursive("page-buttons"));
        // CCMenu* gauntlets = static_cast<CCMenu*>(getChildByIDRecursive("gauntlet-buttons"_spr));

        if (!nav || !pageDots) {
            log::error(" ---- One or more nodes not found: nav {}, pageDots {}", nav != nullptr, pageDots != nullptr);
            return;
        }

        std::vector<CCSprite*> gauntletBtns;
        for (int b = 0; b < 3; b++) {
            auto gauntletBtn = static_cast<CCSprite*>(getChildByIDRecursive(fmt::format("gauntlet-button-{}", b + 1)));
            if (gauntletBtn) {
                gauntletBtns.push_back(gauntletBtn);
                auto btnNode = static_cast<CCSprite*>(getChildByIDRecursive("background"));

                log::info("found gauntlet button {}", b + 1);

                if (btnNode) {
                    btnNode->setContentSize({110, 220});
                    log::debug(" ---- btnNode set to 110x220");
                }

                auto infoNode = static_cast<CCSprite*>(getChildByIDRecursive("gauntlet-info-node"));

                auto GDUtils = Loader::get()->getLoadedMod("gdutilsdevs.gdutils");
                if (GDUtils) {
                    auto settingVal = GDUtils->getSettingValue<bool>("gauntletDesign");
                    if (!settingVal) {
                        infoNode->setPositionY(-2.5);
                    }
                }
                
                else {
                    infoNode->setPositionY(-2.5);
                }

                auto nameLabel = static_cast<CCSprite*>(getChildByIDRecursive("gauntlet-name-label"));
                if (nameLabel) {
                    nameLabel->setPositionY(84);
                    log::debug(" ---- nameLabel position set to 84");
                }

                auto gauntletLabel = static_cast<CCSprite*>(getChildByIDRecursive("gauntlet-label"));
                if (gauntletLabel) {
                    gauntletLabel->setPositionY(nameLabel->getPositionY() - 15);
                    log::debug(" ---- gauntletLabel position set to nameLabel position - 15");
                }

                auto nameShadowLabel = static_cast<CCSprite*>(getChildByIDRecursive("gauntlet-name-shadow-label"));
                if (nameShadowLabel) {
                    nameShadowLabel->setPosition(ccp(nameLabel->getPositionX() + 2, nameLabel->getPositionY() - 2));
                    log::debug(" ---- nameShadowLabel position set to nameLabel position + (2, -2)");
                }

                auto gauntletShadowLabel = static_cast<CCSprite*>(getChildByIDRecursive("gauntlet-shadow-label"));
                if (gauntletShadowLabel) {
                    gauntletShadowLabel->setPosition(ccp(gauntletLabel->getPositionX() + 2, gauntletLabel->getPositionY() - 2));
                    log::debug(" ---- gauntletShadowLabel position set to gauntletLabel position + (2, -2)");
                }

                auto chestSpr = static_cast<CCSprite*>(getChildByIDRecursive("chest-sprite"));
                if (chestSpr) {
                    chestSpr->setPositionY(-64.5);
                    chestSpr->setScale(0.3);
                    log::debug(" ---- chestSpr position set to -64.5 and scale set to 0.3");
                }

                auto chestShadowSpr = static_cast<CCSprite*>(getChildByIDRecursive("chest-shadow-sprite"));
                if (chestShadowSpr) {
                    chestShadowSpr->setPosition(ccp(chestSpr->getPositionX() + 2, chestSpr->getPositionY() - 2));
                    chestShadowSpr->setScale(0.3);
                    log::debug(" ---- chestShadowSpr position set to chestSpr position + (2, -2) and scale set to 0.3");
                }

                auto rewardLabel = static_cast<CCSprite*>(getChildByIDRecursive("reward-label"));
                if (rewardLabel) {
                    rewardLabel->setPositionY(chestSpr->getPositionY() - 13.5);
                    rewardLabel->setScale(0.5);
                    rewardLabel->setZOrder(3);
                    log::debug(" ---- rewardLabel position set to chestSpr position - 13.5, scale set to 0.5, and zOrder set to 3");
                }

                auto rewardShadowLabel = static_cast<CCSprite*>(getChildByIDRecursive("reward-shadow-label"));
                if (rewardShadowLabel) {
                    rewardShadowLabel->setPosition(ccp(rewardLabel->getPositionX() + 2, rewardLabel->getPositionY() - 2));
                    rewardShadowLabel->setScale(0.5);
                    log::debug(" ---- rewardShadowLabel set position to rewardShadowLabel position + (2, -2), scale set to 0.5");
                }

                if (!btnNode || !infoNode || !nameLabel || !gauntletLabel || !nameShadowLabel || !gauntletShadowLabel || !chestSpr || !chestShadowSpr || !rewardLabel || !rewardShadowLabel) {
                    log::error(" ---- One or more nodes not found for gauntlet button {}", b + 1);
                }
            }

            if (!gauntletBtn) {
                log::error(" ---- gauntletBtn not found for button {}", b + 1);
            }
        }

        if (gauntletBtns.empty()) {
            log::error("no buttons found!");
        }

        // int buttonCount = gauntletBtns.size();
        // if (buttonCount > 0) {
        //     for (int i = 0; i < buttonCount; i++) {
        //         float posY = winSize.height / 2 - 17.5;
        //         float startX = winSize.width / 2 - (buttonCount - 1) * 57.5;
        //         gauntletBtns[i]->setPosition(ccp(startX + i * 115, posY));
        //     }
        // }
    }
// }