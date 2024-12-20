#include <Geode/Geode.hpp>
#include <Geode/modify/GauntletSelectLayer.hpp>
#include <Geode/modify/GauntletNode.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS
#include <geode.custom-keybinds/include/Keybinds.hpp>

using namespace keybinds;
#endif

class $modify(GauntletSelectLayerHook, GauntletSelectLayer) {
    bool init(int gauntletType) {
        if (!GauntletSelectLayer::init(gauntletType)) return false;

        auto title = this->getChildByID("title");
        if (title) {
            title->setVisible(false);
        }

        auto BLCorner = this->getChildByID("bottom-left-corner");
        if (BLCorner) {
            BLCorner->setVisible(false);
        }

        auto BRCorner = this->getChildByID("bottom-right-corner");
        if (BRCorner) {
            BRCorner->setVisible(false);
        }
        return true;
    }

    void onEnter() {
        GauntletSelectLayer::onEnter();

        auto title = this->getChildByID("title");
        if (title) {
            title->setVisible(false);
        }
        
        auto BLCorner = this->getChildByID("bottom-left-corner");
        if (BLCorner) {
            BLCorner->setVisible(false);
        }

        auto BRCorner = this->getChildByID("bottom-right-corner");
        if (BRCorner) {
            BRCorner->setVisible(false);
        }
    }
};

class $modify(RedesignedGauntletSelectLayer, GauntletSelectLayer) {
    struct Fields {
        std::vector<CCMenuItemSpriteExtra*> m_dots = {};
        CCMenu* m_dotsMenu = nullptr;
        int currentGauntletPage = 0;
    };

    void updateDots() {
        auto sfc = CCSpriteFrameCache::sharedSpriteFrameCache();

        for(CCMenuItemSpriteExtra* btn : m_fields->m_dots) {
            auto btnIdx = std::find(m_fields->m_dots.begin(), m_fields->m_dots.end(), btn) - m_fields->m_dots.begin();

            int newPage = m_scrollLayer->m_page;
            if (newPage <= -1) newPage = m_fields->m_dots.size() + newPage;
            if (newPage >= m_fields->m_dots.size()) newPage = newPage % m_fields->m_dots.size();
            // log::info("{}, {}", btnIdx, newPage);

            if (btnIdx == newPage)
                static_cast<CCSprite*>(btn->getNormalImage())->setDisplayFrame(sfc->spriteFrameByName("gj_navDotBtn_on_001.png"));
            else
                static_cast<CCSprite*>(btn->getNormalImage())->setDisplayFrame(sfc->spriteFrameByName("gj_navDotBtn_off_001.png"));
        }
    }

    void setupGauntlets() {
        GauntletSelectLayer::setupGauntlets();

        m_fields->m_dots.clear();
        if (m_fields->m_dotsMenu) {
            m_fields->m_dotsMenu->removeFromParent();
            m_fields->m_dotsMenu = nullptr;
        }
        
        auto director = CCDirector::sharedDirector();

        m_fields->m_dotsMenu = CCMenu::create();
        m_fields->m_dotsMenu->setLayout(AxisLayout::create());
        m_fields->m_dotsMenu->setPositionY(director->getScreenBottom() + 15.f);
        m_fields->m_dotsMenu->setID("page-navigation"_spr);
        addChild(m_fields->m_dotsMenu);

        for (int i = 0; i < m_scrollLayer->getTotalPages(); i++) {
            auto spr = CCSprite::createWithSpriteFrameName("gj_navDotBtn_off_001.png");
            spr->setScale(0.8f);

            CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(RedesignedGauntletSelectLayer::onDot));
            m_fields->m_dotsMenu->addChild(btn);
            m_fields->m_dots.push_back(btn);
        }

        auto dotsArray = CCArrayExt<CCSprite*>(m_scrollLayer->m_dots);

        for(CCSprite* dot : dotsArray) {
            dot->setVisible(false);
        }
        m_scrollLayer->m_dotsVisible = false;

        updateDots();

        m_fields->m_dotsMenu->updateLayout();

        // thank you ery :3
        if (const auto pageButtons = m_scrollLayer->m_dots) {
            RedesignedGauntletSelectLayer::findCurrentGauntletPageUsing(pageButtons);
        }
        #ifdef GEODE_IS_WINDOWS
        this->defineKeybind("next-gauntlet"_spr, [this]() {
            GauntletSelectLayer::onNext(nullptr); // default: right arrow key
        });
        this->defineKeybind("previous-gauntlet"_spr, [this]() {
            GauntletSelectLayer::onPrev(nullptr); // default: left arrow key
        });
        this->defineKeybind("first-visible-gauntlet"_spr, [this]() {
            RedesignedGauntletSelectLayer::pressGauntlet(1); // default: numrow 1
        });
        this->defineKeybind("second-visible-gauntlet"_spr, [this]() {
            RedesignedGauntletSelectLayer::pressGauntlet(2); // default: numrow 2
        });
        this->defineKeybind("third-visible-gauntlet"_spr, [this]() {
            RedesignedGauntletSelectLayer::pressGauntlet(3); // default: numrow 3
        }); 
        #endif
        
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        //log::debug("winSize = {}px, {}px", winSize.width, winSize.height);

        auto main = getChildByID("main-layer");

        auto floor = CCSprite::create("gauntletGround_001.png"_spr);
        if (floor) {
            floor->setID("floor");
            floor->setColor(ccc3(37, 37, 37));

            auto floorSize = floor->getContentSize();

            float scaleX = winSize.width / floorSize.width;
            float scaleY = winSize.height / floorSize.height;

            float scale = std::min(scaleX, scaleY);

            floor->setScale(scale);
            floor->setPosition(ccp(winSize.width / 2, floorSize.height * scale / 2));
            floor->setAnchorPoint(ccp(0.5, 0.5));
            floor->setZOrder(-1);
            this->addChild(floor);
        }

        auto decorationParentNode = CCNode::create();
        if (decorationParentNode) {
            decorationParentNode->setID("background-decoration-node");
            decorationParentNode->setPosition(0, 0);
            decorationParentNode->setZOrder(-1);
            this->addChild(decorationParentNode);
        
            auto chainParentNode = CCNode::create();
            if (chainParentNode) {
                chainParentNode->setID("chain-parent-node");
                chainParentNode->setPosition(0, 0);
                chainParentNode->setZOrder(-1);
                decorationParentNode->addChild(chainParentNode);

                for (int c = 0; c < 3; c++) {

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
                chain1->setPosition(ccp(director->getScreenLeft() + 50.f, director->getScreenTop() - 70));

                // second chain pair //
                auto chain2 = getChildByIDRecursive("chain-2");
                chain2->setScale(1.0);
                chain2->setPosition(ccp(director->getScreenRight() - 60.f, director->getScreenTop() - 35));

                // third chain pair //
                auto chain3 = getChildByIDRecursive("chain-3");
                chain3->setScale(1.65);
                chain3->setPosition(ccp(director->getScreenRight() - 40.f, director->getScreenTop() - 55));
            }
        }

        auto dust = CCParticleSystemQuad::create();
        if (dust) {
            CCParticleSystemQuad* dustParticles = GameToolbox::particleFromString("30a-1a1.85a1.5a3a90a90a0a0a250a0a0a-300a0a0a0a0a15a5a0a40a0a0a0a0a0a0a0.26a0.05a0a0a0a25a0a0a0a0a0a0a0.1a0.05a0.25a0a0.25a0a0a0a0a0a0a0a0a2a0a0a0a0a183a0a1.8a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            dustParticles->setPosition(winSize.width / 2, director->getScreenTop() + 10);
            dustParticles->setZOrder(-2);
            this->addChild(dustParticles);
        }

        for (int p = 0; p < m_scrollLayer->getTotalPages(); p++) {

            auto gauntletPage = static_cast<CCMenu*>(this->getChildByIDRecursive(fmt::format("gauntlet-page-{}", p + 1)));
            if (gauntletPage) {

                auto gauntletMenu = gauntletPage->getChildByIDRecursive("gauntlet-menu");
                if (gauntletMenu) {
                    gauntletMenu->setScale(0.9);
                    gauntletMenu->setPosition(ccp(0, 0));

                    std::vector<CCSprite*> gauntletButtons;
                    for (int b = 0; b < 3; b++) {
                        auto gauntletButton = static_cast<CCSprite*>(gauntletMenu->getChildByIDRecursive(fmt::format("gauntlet-button-{}", b + 1)));
                        if (gauntletButton) {
                            gauntletButtons.push_back(gauntletButton);
                        }
                    }

                    int buttonCount = gauntletButtons.size();
                    if (buttonCount > 0) {
                        float startX = winSize.width / 2 - (buttonCount - 1) * 57.5;
                        float posY = winSize.height / 2 - 17.5;
                        for (int i = 0; i < buttonCount; i++) {
                            gauntletButtons[i]->setPosition(ccp(startX + i * 115, posY));
                        }
                    }
                }
            }
        }

        const char* titleA = "The Lost";
        const char* titleB = "Gauntlets";

        auto titleParentNode = CCNode::create();
        if (titleParentNode) {
            titleParentNode->setID("title-parent-node");
            titleParentNode->setPosition(0, 0);
            titleParentNode->setZOrder(-1);
            this->addChild(titleParentNode);
        }

        // order SHOULD be Shadow -> Outline -> Base -> Detail //
        auto GRTitleShadowSML = CCLabelBMFont::create(titleA, "GR_OxygeneFontOutline.fnt"_spr);
        GRTitleShadowSML->setID("title-small-shadow");
        GRTitleShadowSML->setScale(0.50);
        GRTitleShadowSML->setPosition(ccp(winSize.width / 2 + 0.75, winSize.height - 19));
        GRTitleShadowSML->setColor(ccc3(0, 0, 0));
        GRTitleShadowSML->setOpacity(100);
        titleParentNode->addChild(GRTitleShadowSML);

        auto GRTitleOutlineSML = CCLabelBMFont::create(titleA, "GR_OxygeneFontOutline.fnt"_spr);
        GRTitleOutlineSML->setID("title-small-outline");
        GRTitleOutlineSML->setScale(0.50);
        GRTitleOutlineSML->setPosition(ccp(winSize.width / 2 - 1, winSize.height - 17.5));
        GRTitleOutlineSML->setColor(ccc3(255, 255, 255));
        titleParentNode->addChild(GRTitleOutlineSML);

        auto GRTitleSML = CCLabelBMFont::create(titleA, "GR_OxygeneFont.fnt"_spr);
        GRTitleSML->setID("title-small-base");
        GRTitleSML->setScale(0.50);
        GRTitleSML->setPosition(ccp(winSize.width / 2, winSize.height - 17.5));
        GRTitleSML->setColor(ccc3(255, 255, 105));
        titleParentNode->addChild(GRTitleSML);

        auto GRTitleHighlightSML = CCLabelBMFont::create(titleA, "GR_OxygeneFontHighlight.fnt"_spr);
        GRTitleHighlightSML->setID("title-small-highlight");
        GRTitleHighlightSML->setScale(0.50);
        GRTitleHighlightSML->setPosition(ccp(winSize.width / 2 + 1.25, winSize.height - 17.5));
        GRTitleHighlightSML->setColor(ccc3(255, 123, 0));
        titleParentNode->addChild(GRTitleHighlightSML);

        // Large title children //
        auto GRTitleShadowLRG = CCLabelBMFont::create(titleB, "GR_OxygeneFontOutline.fnt"_spr);
        GRTitleShadowLRG->setID("title-large-shadow");
        GRTitleShadowLRG->setScale(0.90);
        GRTitleShadowLRG->setPosition(ccp(winSize.width / 2 + 1.65, winSize.height - 48));
        GRTitleShadowLRG->setColor(ccc3(0, 0, 0));
        GRTitleShadowLRG->setOpacity(100);
        titleParentNode->addChild(GRTitleShadowLRG);

        auto GRTitleOutlineLRG = CCLabelBMFont::create(titleB, "GR_OxygeneFontOutline.fnt"_spr);
        GRTitleOutlineLRG->setID("title-large-outline");
        GRTitleOutlineLRG->setScale(0.90);
        GRTitleOutlineLRG->setPosition(ccp(winSize.width / 2 - 1.65, winSize.height - 45));
        GRTitleOutlineLRG->setColor(ccc3(255, 255, 255));
        titleParentNode->addChild(GRTitleOutlineLRG);

        auto GRTitleLRG = CCLabelBMFont::create(titleB, "GR_OxygeneFont.fnt"_spr);
        GRTitleLRG->setID("title-large-base");
        GRTitleLRG->setScale(0.90);
        GRTitleLRG->setPosition(ccp(winSize.width / 2, winSize.height - 45));
        GRTitleLRG->setColor(ccc3(255, 255, 105));
        titleParentNode->addChild(GRTitleLRG);

        auto GRTitleHighlightLRG = CCLabelBMFont::create(titleB, "GR_OxygeneFontHighlight.fnt"_spr);
        GRTitleHighlightLRG->setID("title-large-highlight");
        GRTitleHighlightLRG->setScale(0.90);
        GRTitleHighlightLRG->setPosition(ccp(winSize.width / 2 + 2.75, winSize.height - 45));
        GRTitleHighlightLRG->setColor(ccc3(255, 123, 0));
        titleParentNode->addChild(GRTitleHighlightLRG);
    }

    void onDot(CCObject* sender) {
        auto btnIdx = std::find(m_fields->m_dots.begin(), m_fields->m_dots.end(), sender) - m_fields->m_dots.begin();

        m_scrollLayer->instantMoveToPage(btnIdx);
        m_scrollLayer->updatePages();
        m_scrollLayer->repositionPagesLooped();
    }

    #ifdef GEODE_IS_WINDOWS
    void defineKeybind(const char* id, std::function<void()> callback) {
        this->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {
            if (event->isDown()) {
                callback();
            }
            return ListenerResult::Propagate;
        }, id);
    }
    #endif

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

// class $modify(GauntletNode) {

// public:
//     void setupGauntletNode() {
//         auto updateBackground = Mod::get()->getSettingValue<bool>("update-backgrounds");
//         if (updateBackground) {
//             // Get the background node color
//             auto backgroundNode = static_cast<CCSprite*>(this->getChildByIDRecursive("background"));
//             ccColor3B backgroundColor = backgroundNode ? backgroundNode->getColor() : ccc3(128, 128, 128); // Default: grey
//             if (backgroundNode) {
//                 backgroundNode->setVisible(false);
//             }

//             // Create parent node
//             auto portalParent = CCNode::create();
//             if (portalParent) {
//                 portalParent->setID("gauntlet-portal-parent-node");
//                 portalParent->setPosition(ccp(this->getContentSize().width / 2 - 3, this->getContentSize().height / 2));
//                 this->addChild(portalParent);
//             }

//             createParticleParentNode();
//             repositionLabels();
//             setupInfoNode();
//             setupPortalSprites(portalParent, backgroundColor);
//         }
//     }

// private:
//     void createParticleParentNode() {
//         auto particleParentNode = CCNode::create();
//         if (particleParentNode) {
//             particleParentNode->setID("particle-parent-node");
//             particleParentNode->setZOrder(5);
//             this->addChild(particleParentNode);

//             for (int p = 0; p < 1; p++) {
//                 auto portalParticle1 = CCParticleSystemQuad::create();
//                 if (portalParticle1) {
//                     CCParticleSystemQuad* particle1 = GameToolbox::particleFromString("50a-1a2a0.35a7a0a180a0a0a20a40a0a0a-19a0a0a0a2a1a0a40a1a0a1a0a1a0a0.7a0.3a0a0a0a25a1a0a1a0a1a0a0.4a0.05a0.6a0a1a0a50a0a0a10a-30a15a1a2a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
//                     particle1->setPosition(ccp(0, 0));
//                     particle1->setZOrder(2);
//                     particle1->setScale(1.7f);
//                     particleParentNode->addChild(particle1);
//                 }

//                 auto portalParticle2 = CCParticleSystemQuad::create();
//                 if (portalParticle2) {
//                     CCParticleSystemQuad* particle2 = GameToolbox::particleFromString("20a-1a2a1a6a180a0a0a0a25a50a0a5a-8a0a0a10a0a0a0a360a1a0a1a0a1a0a0.08a0.04a100a30a0a180a1a0a1a0a1a0a0.02a0a0.6a0a1a0a0a0a0a0a0a0a1a2a1a0a0a0a190a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
//                     particle2->setPosition(ccp(0, 0));
//                     particle2->setZOrder(2);
//                     particle2->setScaleX(1.25f);
//                     particle2->setScaleY(2.5f);
//                     particleParentNode->addChild(particle2);
//                 }

//                 auto portalParticle3 = CCParticleSystemQuad::create();
//                 if (portalParticle3) {
//                     CCParticleSystemQuad* particle3 = GameToolbox::particleFromString("20a-1a2a0a2a180a0a0a0a25a50a0a5a-8a0a0a10a0a0a0a171a1a0a1a0a1a0a0.1a0.05a85a10a0a97a1a0a1a0a1a0a0.02a0a0a0a0.35a0a0a0a0a0a0a0a1a2a1a0a0a0a179a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
//                     particle3->setPosition(ccp(0, 0));
//                     particle3->setZOrder(2);
//                     particle3->setScaleX(1.25f);
//                     particle3->setScaleY(2.5f);
//                     particleParentNode->addChild(particle3);
//                 }
//             }
//         }
//     }

//     void repositionLabels() {
//         auto nameLabelRepos = static_cast<GauntletNode*>(this->getChildByIDRecursive("gauntlet-name-label"));
//         nameLabelRepos->setScale(0.50);
//         nameLabelRepos->setPosition(ccp(0, 90));

//         auto nameLabelShadowRepos = static_cast<GauntletNode*>(this->getChildByIDRecursive("gauntlet-name-shadow-label"));
//         nameLabelShadowRepos->setScale(nameLabelRepos->getScale());
//         nameLabelShadowRepos->setPosition(ccp(nameLabelRepos->getPositionX() + 1, nameLabelRepos->getPositionY() - 1.5));

//         auto gauntletLabel = static_cast<GauntletNode*>(this->getChildByIDRecursive("gauntlet-label"));
//         gauntletLabel->setScale(0.35);
//         gauntletLabel->setPosition(ccp(0, nameLabelRepos->getPositionY() - 10));

//         auto gauntletShadowLabel = static_cast<GauntletNode*>(this->getChildByIDRecursive("gauntlet-shadow-label"));
//         gauntletShadowLabel->setScale(gauntletLabel->getScale());
//         gauntletShadowLabel->setPosition(ccp(gauntletLabel->getPositionX() + 1, gauntletLabel->getPositionY() - 1.5));
//     }

//     void setupInfoNode() {
//         auto infoNode = this->getChildByIDRecursive("gauntlet-info-node");

//         auto gauntletSprite = infoNode->getChildByIDRecursive("gauntlet-sprite");
//         if (gauntletSprite) {
//             gauntletSprite->setPosition(ccp(0, -92.5));
//             gauntletSprite->setScale(0.65);
//         }

//         auto chestSprite = infoNode->getChildByIDRecursive("chest-sprite");
//         if (chestSprite) {
//             chestSprite->setPosition(ccp(0, -120.5));
//             chestSprite->setScale(0.25);
//         }

//         auto chestShadowSprite = infoNode->getChildByIDRecursive("chest-shadow-sprite");
//         if (chestShadowSprite) {
//             chestShadowSprite->setPosition(ccp(chestSprite->getPositionX() + 2, chestSprite->getPositionY() - 3));
//             chestShadowSprite->setScale(0.25);
//         }

//         auto rewardLabel = infoNode->getChildByIDRecursive("reward-label");
//         if (rewardLabel) {
//             rewardLabel->setPosition(ccp(0, -130));
//             rewardLabel->setZOrder(3);
//         }

//         auto rewardShadowLabel = infoNode->getChildByIDRecursive("reward-shadow-label");
//         if (rewardShadowLabel) {
//             rewardShadowLabel->setPosition(ccp(rewardLabel->getPositionX() + 2, rewardLabel->getPositionY() - 3));
//         }

//         auto progressLabel = infoNode->getChildByIDRecursive("gauntlet-progress-label");
//         if (progressLabel) {
//             progressLabel->setPosition(ccp(0, 55));
//         }

//         auto progressShadowLabel = infoNode->getChildByIDRecursive("gauntlet-progress-shadow-label");
//         if (progressShadowLabel) {
//             progressShadowLabel->setPosition(ccp(progressLabel->getPositionX() + 2, progressLabel->getPositionY() - 3));
//         }
//     }

//     void setupPortalSprites(CCNode* portalParent, ccColor3B backgroundColor) {
        
//     // Main Gauntlet Portal //

//         // Base portal sprite
//         auto portalBase = CCSprite::create("gauntletPortal_001_01.png"_spr);
//         portalBase->setID("portal-base");
//         portalBase->setColor(backgroundColor);
//         portalBase->setPosition(ccp(1, 0));
//         portalBase->setZOrder(0);
//         portalParent->addChild(portalBase);

//         // Portal outline
//         auto portalOutline = CCSprite::create("gauntletPortal_002.png"_spr);
//         portalOutline->setID("portal-outline");
//         portalOutline->setPosition(ccp(3, -1.75));
//         portalOutline->setZOrder(-3);
//         portalOutline->setColor(backgroundColor);
//         portalParent->addChild(portalOutline);

//         // Portal inline
//         auto portalInline = CCSprite::create("gauntletPortal_003.png"_spr);
//         portalInline->setID("portal-inline");
//         portalInline->setPosition(ccp(2.5, 1.50));
//         portalInline->setZOrder(-1);
//         ccBlendFunc portalInlineBlend = { GL_SRC_ALPHA, GL_ONE };
//         portalInline->setBlendFunc(portalInlineBlend);
//         portalParent->addChild(portalInline);

//         // Portal color
//         auto portalColor = CCSprite::create("gauntletPortal_004_02.png"_spr);
//         portalColor->setID("portal-color");
//         portalColor->setPosition(ccp(1.25, 0.3));
//         portalColor->setZOrder(-1);
//         portalColor->setColor(backgroundColor);
//         portalParent->addChild(portalColor);

//         // Portal background
//         auto portalBG = CCSprite::create("gauntletPortal_005.png"_spr);
//         portalBG->setID("portal-background");
//         portalBG->setPosition(ccp(0, 0));
//         portalBG->setScale(1.25);
//         portalBG->setZOrder(-2);
//         portalBG->setColor(backgroundColor);
//         portalParent->addChild(portalBG);

//         auto portalBGGlow = CCSprite::create("gauntletPortal_006.png"_spr);
//         portalBGGlow->setID("portal-background-glow");
//         portalBGGlow->setPosition(ccp(2.5, 1));
//         portalBGGlow->setZOrder(-1);
//         // portalBGGlow->setOpacity(150);
//         ccBlendFunc portalBGGlowBlend = { GL_SRC_ALPHA, GL_ONE };
//         portalBGGlow->setBlendFunc(portalBGGlowBlend);
//         portalParent->addChild(portalBGGlow);

//     // Gauntlet Island Plaques //

//         // Island plaque
//         auto islandPlaque = CCSprite::create("gauntletPlaque_001.png"_spr);
//         islandPlaque->setID("island-plaque");
//         islandPlaque->setPosition(ccp(3, -95));
//         islandPlaque->setZOrder(1);
//         islandPlaque->setColor(backgroundColor);
//         portalParent->addChild(islandPlaque);

//         // Island plaque outline
//         auto islandPlaqueOutline = CCSprite::create("gauntletPlaque_002.png"_spr);
//         islandPlaqueOutline->setID("island-plaque-outline");
//         islandPlaqueOutline->setPosition(ccp(islandPlaque->getPositionX() + 1.5, islandPlaque->getPositionY() - 1));
//         islandPlaqueOutline->setZOrder(-3);
//         portalParent->addChild(islandPlaqueOutline);
        
//     // Gauntlet Name Plaque //

//         // Name plaque
//         auto namePlaque = CCSprite::create("gauntletPlaque_003.png"_spr);
//         namePlaque->setID("gauntlet-name-plaque");
//         namePlaque->setPosition(ccp(3, 84));
//         namePlaque->setZOrder(1);
//         namePlaque->setScale(0.65);
//         namePlaque->setColor(backgroundColor);
//         portalParent->addChild(namePlaque);

//         // Name plaque outline
//         auto namePlaqueOutline = CCSprite::create("gauntletPlaque_004_1.png"_spr);
//         namePlaqueOutline->setID("portal-name-plaque-outline");
//         namePlaqueOutline->setPosition(ccp(namePlaque->getPositionX() + 0.75, namePlaque->getPositionY() - 0.75));
//         namePlaqueOutline->setZOrder(-3);
//         namePlaqueOutline->setScale(namePlaque->getScale());
//         portalParent->addChild(namePlaqueOutline);
//     }
// };

#ifdef GEODE_IS_WINDOWS
$execute {
    BindManager::get()->registerBindable({
        "previous-gauntlet"_spr,
        "Previous Gauntlet Page",
        "Use this keybind (set to Left by default) to view the previous gauntlet page.",
        { Keybind::create(KEY_Left) },
        "Global/Better Gauntlets"
    });
    BindManager::get()->registerBindable({
        "next-gauntlet"_spr,
        "Next Gauntlet Page",
        "Use this keybind (set to Right by default) to view the next gauntlet page.",
        { Keybind::create(KEY_Right) },
        "Global/Better Gauntlets"
    });
    BindManager::get()->registerBindable({
        "first-visible-gauntlet"_spr,
        "First Gauntlet",
        "Enters the <cg>first</c> gauntlet on the current Gauntlet Page.\n\n<cy>(Disclaimer: Results may not be accurate.)</c>",
        { Keybind::create(KEY_One) },
        "Global/Better Gauntlets"
    });
    BindManager::get()->registerBindable({
        "second-visible-gauntlet"_spr,
        "Second Gauntlet",
        "Enters the <cg>second</c> gauntlet on the current Gauntlet Page.\n\n<cy>(Disclaimer: Results may not be accurate.)</c>",
        { Keybind::create(KEY_Two) },
        "Global/Better Gauntlets"
    });
    BindManager::get()->registerBindable({
        "third-visible-gauntlet"_spr,
        "Third Gauntlet",
        "Enters the <cg>third</c> gauntlet on the current Gauntlet Page.\n\n<cy>(Disclaimer: Results may not be accurate.)</c>",
        { Keybind::create(KEY_Three) },
        "Global/Better Gauntlets"
    });
}
#endif
