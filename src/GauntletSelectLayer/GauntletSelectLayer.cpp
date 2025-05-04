#include <Geode/Geode.hpp>
#include <Geode/Loader.hpp>
#include <Geode/modify/GauntletSelectLayer.hpp>
#include <Geode/modify/GauntletNode.hpp>
#include "cocos-ext.h"

using namespace geode::prelude;

#ifndef GEODE_IS_IOS
	#include <geode.custom-keybinds/include/Keybinds.hpp>
	using namespace keybinds;
#endif

class $modify(GauntletSelectLayerHook, GauntletSelectLayer) {

    cocos2d::CCNode* getChildBySpriteFrameNameRecursive(cocos2d::CCNode* parent, char const* name) {
        return findFirstChildRecursive<cocos2d::CCNode>(parent, [=](auto* spr) {
            return isSpriteFrameName(spr, name);
        });
    }

    bool init(int gauntletType) {
        if (!GauntletSelectLayer::init(gauntletType)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto director = CCDirector::sharedDirector();

        if (PlatformToolbox::isControllerConnected()) {
            auto controllerBtn = getChildByID("controller-back-hint");
            controllerBtn->setZOrder(1);
        }

        auto refreshBtn = getChildByIDRecursive("refresh-button");
        if (refreshBtn) {
            refreshBtn->setVisible(true);
            refreshBtn->setScale(1.25);
        }
        auto BRMenu = getChildByID("bottom-right-menu");
        if (BRMenu) {
            BRMenu->setContentSize(ccp(23.25, 103));
        }
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
        auto floor = CCSprite::createWithSpriteFrameName("gauntletGround_001.png"_spr);
        if (floor) {
            floor->setID("menu-floor"_spr);
            floor->setPositionY(35.f);
            
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

        auto backgroundColor = static_cast<CCSprite*>(this->getChildByID("background"));
        if (backgroundColor) {
            backgroundColor->setColor(ccc3(34, 34, 34));
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
                chain1->setPosition(ccp(director->getScreenLeft() + 50.f, director->getScreenTop() - 70));

                // second chain pair //
                auto chain2 = getChildByIDRecursive("chain-2");
                chain2->setScale(1.65);
                chain2->setPosition(ccp(director->getScreenRight() - 40.f, director->getScreenTop() - 55));
                
                // third chain pair //
                auto chain3 = getChildByIDRecursive("chain-3");
                // chain3->setScale(1.0);
                chain3->setPosition(ccp(director->getScreenRight() - 100.f, director->getScreenTop() - 26));
                
                // fourth chain pair //
                auto chain4 = getChildByIDRecursive("chain-4");
                chain3->setScale(0.75);
                chain4->setPosition(ccp(director->getScreenLeft() + 90.f, director->getScreenTop() - 34));
            }
        }

        auto topRight = this->getChildByID("top-right-menu");
        if (topRight) {
            topRight->setPositionY(266.75f);
        }
        
        auto bgParticleNode = CCParticleSystemQuad::create();
        if (bgParticleNode) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "100a-1a4a2a16a90a90a0a0a300a0a128a-25a0a60a0a0a400a0a90a60a0a0a0a0a0a0a0.05a0.02a400a0a90a67a0a0a0a0a0a0a0a0.05a1a0a0.5a0a0a0a0a0a0a0a0a2a0a0a0a0a182a0a1.8a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
                );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() + 10));
            bgParticles->setZOrder(-1);
            this->addChild(bgParticles);
        }

        return true;
    }
};

class $modify(RedesignedGauntletSelectLayer, GauntletSelectLayer) {
    struct Fields {
        std::vector<CCMenuItemSpriteExtra*> m_dots = {};
        CCMenu* m_dotsMenu = nullptr;
        int currentGauntletPage = 0;
    };

    void onRefresh() {
        auto refreshBtn = getChildByIDRecursive("refresh-button");
        if (refreshBtn) {
            refreshBtn->setVisible(true);
            refreshBtn->setScale(1.25);
        }
    }
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
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        m_fields->m_dotsMenu = CCMenu::create();
        m_fields->m_dotsMenu->setLayout(AxisLayout::create());
        m_fields->m_dotsMenu->setPositionY(director->getScreenBottom() + 15.f);
        m_fields->m_dotsMenu->setContentWidth(winSize.width - 100.f);
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

        onRefresh();

        // thank you ery :3
        if (const auto pageButtons = m_scrollLayer->m_dots) {
            RedesignedGauntletSelectLayer::findCurrentGauntletPageUsing(pageButtons);
        }
        #ifndef GEODE_IS_IOS
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

        for (int p = 0; p < m_scrollLayer->getTotalPages(); p++) {

            auto gauntletPages = static_cast<CCMenu*>(getChildByIDRecursive(fmt::format("gauntlet-page-{}", p + 1)));
            if (!gauntletPages) nullptr;

            auto gauntletMenu = gauntletPages->getChildByIDRecursive("gauntlet-menu");
            if (!gauntletMenu) nullptr;

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
                for (int i = 0; i < buttonCount; i++) {
                    float posY = winSize.height / 2 - 17.5;
                    float startX = winSize.width / 2 - (buttonCount - 1) * 57.5;
                    gauntletButtons[i]->setPosition(ccp(startX + i * 115, posY));

                    auto GDUtils = Loader::get()->getLoadedMod("gdutilsdevs.gdutils");
                    if (GDUtils) {
                        auto settingVal = GDUtils->getSettingValue<bool>("gauntletDesign");
                        if (settingVal) {
                            float startX = winSize.width / 2 - (buttonCount - 1) * 69;
                            gauntletButtons[i]->setPosition(ccp(startX + i * 138, posY));
                        }
                    }
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

    #ifndef GEODE_IS_IOS
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