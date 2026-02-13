#include <vector>
#include <Geode/Geode.hpp>
#include <Geode/Loader.hpp>
#include <Geode/ui/Layout.hpp>
#include <Geode/ui/SimpleAxisLayout.hpp>
#include <Geode/modify/GauntletSelectLayer.hpp>

using namespace geode::prelude;

// #ifndef GEODE_IS_IOS
// 	#include <geode.custom-keybinds/include/Keybinds.hpp>
// 	using namespace keybinds;
// #endif

class $modify(RedesignedGauntletSelectLayer, GauntletSelectLayer) {
    struct Fields {
        std::vector<CCMenuItemSpriteExtra*> m_dots = {};
        CCMenu* m_dotsMenu = nullptr;
        int currentGauntletPage = 0;
        bool showingCustomList = false;
    };

    cocos2d::CCNode* getChildBySpriteFrameNameRecursive(cocos2d::CCNode* parent, char const* name) {
        return findFirstChildRecursive<cocos2d::CCNode>(parent, [=](auto* spr) {
            return isSpriteFrameName(spr, name);
        });
    }

    bool init(int gauntletType) {
    
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
            ->setGap(5.0)
        );

        CCMenuItemToggler* toggleSpr = CCMenuItemToggler::create(
            CCSpriteGrayscale::createWithSpriteFrameName("GJ_bigStar_001.png"),
            CCSprite::createWithSpriteFrameName("GJ_bigStar_001.png"),
            this, menu_selector(RedesignedGauntletSelectLayer::toggleList)
        );
        toggleSpr->setID("toggle-list-button");
        BRMenu->addChild(toggleSpr);

        BRMenu->updateLayout();

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
        auto titleRedesign = CCSprite::create("gauntletTitle.png"_spr);
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
                // first chain pair //
                auto chain1 = getChildByIDRecursive("chain-1");
                chain1->setScale(2.0);
                chain1->setPosition(ccp(director->getScreenLeft() + 50, director->getScreenTop() - 70));

                // second chain pair //
                auto chain2 = getChildByIDRecursive("chain-2");
                chain2->setScale(1.65);
                chain2->setPosition(ccp(director->getScreenRight() - 40, director->getScreenTop() - 55));
                
                // third chain pair //
                auto chain3 = getChildByIDRecursive("chain-3");
                // chain3->setScale(1.0);
                chain3->setPosition(ccp(director->getScreenRight() - 100, director->getScreenTop() - 26));
                
                // fourth chain pair //
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

    void toggleList(CCObject* sender) {
        CCNode* gauntletList = getChildByIDRecursive("gauntlets-list");
        CCNode* serverList = getChildByIDRecursive("server-gauntlets-list"_spr);
        CCNode* dotsMenu = getChildByIDRecursive("page-navigation"_spr);
        CCNode* arrowBtns = getChildByIDRecursive("scroll-buttons-menu");
        
        if (gauntletList && serverList) {
            bool showingVanilla = !gauntletList->isVisible();
            
            gauntletList->setVisible(showingVanilla);
            dotsMenu->setVisible(showingVanilla);
            arrowBtns->setVisible(showingVanilla);
            serverList->setVisible(!showingVanilla);
            
            if (m_scrollLayer) {
                m_scrollLayer->setTouchEnabled(showingVanilla);
            }
            
            m_fields->showingCustomList = !showingVanilla;
        }
    }
    
    void updateDots() {
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

    void setupGauntlets() {
        GauntletSelectLayer::setupGauntlets();

        CCDirector* director = CCDirector::sharedDirector();
        CCSize winSize = director->getWinSize();

        m_fields->m_dots.clear();
        if (m_fields->m_dotsMenu)
            m_fields->m_dotsMenu->removeFromParent();

        m_fields->m_dotsMenu = CCMenu::create();
        m_fields->m_dotsMenu->setLayout(AxisLayout::create());
        m_fields->m_dotsMenu->setPositionY(director->getScreenBottom() + 15);
        m_fields->m_dotsMenu->setContentWidth(winSize.width - 100);
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

        // thank you ery :3
        if (const auto pageButtons = m_scrollLayer->m_dots) {
            RedesignedGauntletSelectLayer::findCurrentGauntletPageUsing(pageButtons);
        }
        //

        CCMenu* serverList = CCMenu::create();
        serverList->setID("server-gauntlets-list"_spr);
        serverList->setPosition({winSize.width / 2, winSize.height / 2 - 17.5f});

        serverList->setLayout(geode::RowLayout::create()
            ->setGap(71)
            ->setAxisAlignment(AxisAlignment::Center)
        );

        serverList->setVisible(false);
        this->addChild(serverList);
        
        for (int btn = 0; btn < 3; btn++) {
            CCMenuItemSpriteExtra* serverGauntletBtn = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("GJ_safeBtn_001.png"),
                this, nullptr
            );
            serverGauntletBtn->setID(fmt::format("server-gauntlet-button-{}", btn + 1));
            serverList->addChild(serverGauntletBtn);
            serverList->updateLayout();
        }

        if (m_fields->showingCustomList) {
            if (m_scrollLayer) {
                m_scrollLayer->setVisible(false);
                m_scrollLayer->setTouchEnabled(false);  // Disable touch
            }
            if (m_fields->m_dotsMenu) m_fields->m_dotsMenu->setVisible(false);
            
            auto serverList = getChildByID("server-gauntlets-list");
            if (serverList) serverList->setVisible(true);
            
        } else {
            if (m_scrollLayer) {
                m_scrollLayer->setVisible(true);
                m_scrollLayer->setTouchEnabled(true);  // Enable touch
            }
            if (m_fields->m_dotsMenu) m_fields->m_dotsMenu->setVisible(true);
            
            auto serverList = getChildByID("server-gauntlets-list");
            if (serverList) serverList->setVisible(false);
        }
        
        // #ifndef GEODE_IS_IOS
        //     this->defineKeybind("next-gauntlet"_spr, [this]() {
        //         GauntletSelectLayer::onNext(nullptr); // default: right arrow
        //     });
        //     this->defineKeybind("previous-gauntlet"_spr, [this]() {
        //         GauntletSelectLayer::onPrev(nullptr); // default: left arrow
        //     });
        //     this->defineKeybind("first-visible-gauntlet"_spr, [this]() {
        //         RedesignedGauntletSelectLayer::pressGauntlet(1); // default: numkey 1
        //     });
        //     this->defineKeybind("second-visible-gauntlet"_spr, [this]() {
        //         RedesignedGauntletSelectLayer::pressGauntlet(2); // default: numkey 2
        //     });
        //     this->defineKeybind("third-visible-gauntlet"_spr, [this]() {
        //         RedesignedGauntletSelectLayer::pressGauntlet(3); // default: numkey 3
        //     });
        // #endif

        for (int p = 0; p < m_scrollLayer->getTotalPages(); p++) {            
            CCMenu* pages = static_cast<CCMenu*>(getChildByIDRecursive(fmt::format("gauntlet-page-{}", p + 1)));
            CCSpriteBatchNode* dots = static_cast<CCSpriteBatchNode*>(getChildByIDRecursive("page-buttons"));
            CCNode* menu = pages->getChildByIDRecursive("gauntlet-menu");
            
            if (!pages || !dots || !menu) continue;

            menu->setPosition(ccp(0, -3));
            dots->setVisible(false);

            auto GDUtils = Loader::get()->getLoadedMod("gdutilsdevs.gdutils");
            if (GDUtils) {
                auto settingVal = GDUtils->getSettingValue<bool>("gauntletDesign");
                if (settingVal) {
                    menu->setScale(0.85);
                }
            }
            std::vector<CCSprite*> gauntletBtns;
            for (int b = 0; b < 3; b++) {
                auto gauntletBtn = static_cast<CCSprite*>(menu->getChildByIDRecursive(fmt::format("gauntlet-button-{}", b + 1)));
                if (gauntletBtn) {
                    gauntletBtns.push_back(gauntletBtn);
                    auto btnNode = static_cast<CCSprite*>(gauntletBtn->getChildByIDRecursive("background"));
                    if (btnNode) {
                        btnNode->setContentSize({110, 220});
                    }
                    // // Gauntlet Info Nodes // //
                    // Info Node
                    auto infoNode = static_cast<CCSprite*>(gauntletBtn->getChildByIDRecursive("gauntlet-info-node"));
                    auto GDUtils = Loader::get()->getLoadedMod("gdutilsdevs.gdutils");
                    if (GDUtils) {
                        auto settingVal = GDUtils->getSettingValue<bool>("gauntletDesign");
                        if (!settingVal) {
                            infoNode->setPositionY(-2.5);
                        }
                    } else {
                        infoNode->setPositionY(-2.5);
                    }
                    // Name Label
                    auto nameLabel = static_cast<CCSprite*>(gauntletBtn->getChildByIDRecursive("gauntlet-name-label"));
                    if (nameLabel) {
                        nameLabel->setPositionY(84);
                    }
                    // Gauntlet Label
                    auto gauntletLabel = static_cast<CCSprite*>(gauntletBtn->getChildByIDRecursive("gauntlet-label"));
                    if (gauntletLabel) {
                        gauntletLabel->setPositionY(nameLabel->getPositionY() - 15);
                    }
                    // Name Shadow Label
                    auto nameShadowLabel = static_cast<CCSprite*>(gauntletBtn->getChildByIDRecursive("gauntlet-name-shadow-label"));
                    if (nameShadowLabel) {
                        nameShadowLabel->setPosition(ccp(nameLabel->getPositionX() + 2, nameLabel->getPositionY() - 2));
                    }
                    // Gauntlet Shadow Label
                    auto gauntletShadowLabel = static_cast<CCSprite*>(gauntletBtn->getChildByIDRecursive("gauntlet-shadow-label"));
                    if (gauntletShadowLabel) {
                        gauntletShadowLabel->setPosition(ccp(gauntletLabel->getPositionX() + 2, gauntletLabel->getPositionY() - 2));
                    }
                    // // Reward Nodes // //
                    // Chest Sprite
                    auto chestSpr = static_cast<CCSprite*>(gauntletBtn->getChildByIDRecursive("chest-sprite"));
                    if (chestSpr) {
                        chestSpr->setPositionY(-64.5);
                        chestSpr->setScale(0.3);
                    }
                    // Chest Shadow Sprite
                    auto chestShadowSpr = static_cast<CCSprite*>(gauntletBtn->getChildByIDRecursive("chest-shadow-sprite"));
                    if (chestShadowSpr) {
                        chestShadowSpr->setPosition(ccp(chestSpr->getPositionX() + 2, chestSpr->getPositionY() - 2));
                        chestShadowSpr->setScale(0.3);
                    }
                    // Reward Label
                    auto rewardLabel = static_cast<CCSprite*>(gauntletBtn->getChildByIDRecursive("reward-label"));
                    if (rewardLabel) {
                        rewardLabel->setPositionY(chestSpr->getPositionY() - 13.5);
                        rewardLabel->setScale(0.5);
                        rewardLabel->setZOrder(3);
                    }
                    // Reward Shadow Label
                    auto rewardShadowLabel = static_cast<CCSprite*>(gauntletBtn->getChildByIDRecursive("reward-shadow-label"));
                    if (rewardShadowLabel) {
                        rewardShadowLabel->setPosition(ccp(rewardLabel->getPositionX() + 2, rewardLabel->getPositionY() - 2));
                        rewardShadowLabel->setScale(0.5);
                    }
                }
            }
            int buttonCount = gauntletBtns.size();
            if (buttonCount > 0) {
                for (int i = 0; i < buttonCount; i++) {
                    float posY = winSize.height / 2 - 17.5;
                    float startX = winSize.width / 2 - (buttonCount - 1) * 57.5;
                    gauntletBtns[i]->setPosition(ccp(startX + i * 115, posY));
                }
            }
        }
    }
    
    void onDot(CCObject* sender) {
        auto btnIdx = std::find(m_fields->m_dots.begin(), m_fields->m_dots.end(), sender) - m_fields->m_dots.begin();

        m_scrollLayer->instantMoveToPage(btnIdx);
        m_scrollLayer->updatePages();
        m_scrollLayer->repositionPagesLooped();
    }

    // #ifndef GEODE_IS_IOS
    //     void defineKeybind(const char* id, std::function<void()> callback) {
    //         this->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {
    //             if (event->isDown()) {
    //                 callback();
    //             }
    //             return ListenerResult::Propagate;
    //         }, id);
    //     }
    // #endif

    void findCurrentGauntletPageUsing(CCArray* pageButtons) {
        int i = 0;
        for (CCSprite* ccSprite : CCArrayExt<CCSprite*>(pageButtons)) {
            if (ccSprite->getDisplayedColor() == ccColor3B({255, 255, 255})) {
                m_fields->currentGauntletPage = (i + 1);
                break;
            }
            i++;
        }
    }

    void pressGauntlet(int desiredGauntlet) {
        if (const auto theGauntletPage = getChildByIDRecursive(fmt::format("gauntlet-page-{}", m_fields->currentGauntletPage))) {
            if (const auto theGauntlet = theGauntletPage->getChildByIDRecursive(fmt::format("gauntlet-button-{}", desiredGauntlet))) {
                GauntletSelectLayer::onPlay(theGauntlet);
            }
        }
    }

    #ifndef GEODE_IS_ANDROID

    void scrollLayerWillScrollToPage(BoomScrollLayer* p0, int p1) {
        GauntletSelectLayer::scrollLayerWillScrollToPage(p0, p1);
        if (const auto pageButtons = m_scrollLayer->m_dots) {
            RedesignedGauntletSelectLayer::findCurrentGauntletPageUsing(pageButtons);
        }
    }

    void scrollLayerScrolledToPage(BoomScrollLayer* p0, int p1) {
        GauntletSelectLayer::scrollLayerScrolledToPage(p0, p1);
        updateDots();
    }
    #else
    void updateArrows() {
        GauntletSelectLayer::updateArrows();
        if (!m_scrollLayer) return;
        if (const auto pageButtons = m_scrollLayer->m_dots) {
            RedesignedGauntletSelectLayer::findCurrentGauntletPageUsing(pageButtons);
        }
        updateDots();
    }

    #endif

    void onBack(cocos2d::CCObject* sender){
        m_fields->currentGauntletPage = 0;
        GauntletSelectLayer::onBack(sender);
    }
};