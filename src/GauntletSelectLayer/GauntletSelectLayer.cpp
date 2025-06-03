#include <vector>
#include <cstdlib>
#include <ctime>
#include <Geode/Geode.hpp>
#include <Geode/Loader.hpp>
#include <Geode/ui/Layout.hpp>
#include <Geode/ui/GeodeUI.hpp>
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

    struct Fields {
        CCMenuItemSpriteExtra* settingsButton;
    };

    bool init(int gauntletType) {
        if (!GauntletSelectLayer::init(gauntletType)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto director = CCDirector::sharedDirector();

        CCSprite* settingsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
        settingsSpr->setScale(1.25f);
        m_fields->settingsButton = CCMenuItemSpriteExtra::create(settingsSpr, this, menu_selector(GauntletSelectLayerHook::onSettingsButton));

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
            BRMenu->addChild(m_fields->settingsButton);
            BRMenu->setLayout(
                ColumnLayout::create()
                ->setAxisReverse(false)
                ->setAxisAlignment(AxisAlignment::Start)
                ->setGap(-15.0f)
            );
            BRMenu->updateLayout();
        }
        CCArray* settingsSetup = BRMenu->getChildren();
        if (settingsSetup && settingsSetup->count() > 0) {
            auto button = static_cast<CCSprite*>(settingsSetup->objectAtIndex(1));
            button->setID("settings-button");
            button->setContentSize({23, 23.75});
            if (button) {
                CCArray* setupBtn = button->getChildren();
                if (setupBtn && setupBtn->count() > 0) {
                    auto settingsBtn = static_cast<CCSprite*>(setupBtn->objectAtIndex(0));
                    if (settingsBtn) {
                        settingsBtn->setScale(0.5);
                        settingsBtn->setPosition(ccp(button->getContentWidth() / 2, button->getContentHeight() / 2));
                    }
                }
            }
        }
        auto refreshSpr = Mod::get()->getSettingValue<double>("rescale-refresh-spr");
        if (refreshSpr) {
            auto loadCircle = getChildByIDRecursive("loading-circle");
            loadCircle->setPositionY(-18.5f);
            loadCircle->setScale(refreshSpr);
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

        auto squareBSpr = CCScale9Sprite::create("square04_001.png");
        squareBSpr->setID("gauntlet-backdrop"_spr);
        squareBSpr->setColor(ccc3(0, 0, 0));
        squareBSpr->setOpacity(128);
        squareBSpr->setZOrder(-1);
        squareBSpr->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 16.f));
        squareBSpr->setContentSize(CCSize(324, 214));

        squareBSpr->setPosition(winSize.width / 2, winSize.height / 2 - 18.5f);
        this->addChild(squareBSpr);
        
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

        auto textNode = getChildByIDRecursive("try-again-text");
        if (textNode) {
            textNode->setContentSize({0, 0});
            textNode->setPosition(ccp(squareBSpr->getPositionX(), squareBSpr->getPositionY()));
            auto children = textNode->getChildren();
            if (children) {
                for (int i = 0; i < children->count(); i++) {
                    auto child = children->objectAtIndex(i);

                    auto multiline = typeinfo_cast<MultilineBitmapFont*>(child);
                    if (multiline) {
                        multiline->removeFromParent();
                        log::debug("MultilineBitmapFont removed from try-again-text");

                    }
                }
            }

            std::srand(static_cast<unsigned int>(std::time(nullptr)));

            std::vector<std::string> headers = {
                "Oops!",
                "Uh-oh!",
                "Wait, what happened?",
                "Whoops!",
                "Yikes...",
                "Try Again!",
                "404",
                "Disconnected",
                "Get sogged",
                "Get Better IMO",
                "Womp womp",
                ":(",
                "Well, this is awkward..."
            };

            std::string chosenHeader = headers[std::rand() % headers.size()];

            auto reloadHeader = CCLabelBMFont::create(chosenHeader.c_str(), "goldFont.fnt");
            reloadHeader->setPositionY(30.f);
            reloadHeader->setID("error-header"_spr);
            
            auto reloadBody = CCLabelBMFont::create("Couldn't connect to the servers,\nreload the page and try again.", "bigFont.fnt");
            reloadBody->setAlignment(kCCTextAlignmentCenter);
            reloadBody->setPositionY(-15.f);
            reloadBody->setScale(0.6f);
            reloadBody->setID("error-body"_spr);

            textNode->addChild(reloadHeader);
            textNode->addChild(reloadBody);

        }
        return true;
    }

    void onSettingsButton(CCObject* sender) {
        geode::openSettingsPopup(Mod::get(), false);
    }
};

class $modify(RedesignedGauntletSelectLayer, GauntletSelectLayer) {
    struct Fields {
        std::vector<CCMenuItemSpriteExtra*> m_dots = {};
        CCMenu* m_dotsMenu = nullptr;
        int currentGauntletPage = 0;
    };    

    void onRefresh() {
        auto refreshNode = getChildByIDRecursive("refresh-button");
        if (refreshNode) {
            refreshNode->setVisible(true);
        }
        auto textNode = getChildByIDRecursive("try-again-text");
        if (textNode && textNode->isVisible()) {
            refreshNode->setVisible(true);
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
                GauntletSelectLayer::onNext(nullptr); // default: right arrow
            });
            this->defineKeybind("previous-gauntlet"_spr, [this]() {
                GauntletSelectLayer::onPrev(nullptr); // default: left arrow
            });
            this->defineKeybind("first-visible-gauntlet"_spr, [this]() {
                RedesignedGauntletSelectLayer::pressGauntlet(1); // default: numkey 1
            });
            this->defineKeybind("second-visible-gauntlet"_spr, [this]() {
                RedesignedGauntletSelectLayer::pressGauntlet(2); // default: numkey 2
            });
            this->defineKeybind("third-visible-gauntlet"_spr, [this]() {
                RedesignedGauntletSelectLayer::pressGauntlet(3); // default: numkey 3
            });
        #endif

        for (int p = 0; p < m_scrollLayer->getTotalPages(); p++) {

            auto gauntletPages = static_cast<CCMenu*>(getChildByIDRecursive(fmt::format("gauntlet-page-{}", p + 1)));
            if (!gauntletPages) nullptr;

            auto gauntletMenu = gauntletPages->getChildByIDRecursive("gauntlet-menu");
            if (!gauntletMenu) nullptr;

            gauntletMenu->setPosition(ccp(0, -3));
            gauntletMenu->setScale(0.9);

            auto GDUtils = Loader::get()->getLoadedMod("gdutilsdevs.gdutils");
            if (GDUtils) {
                auto settingVal = GDUtils->getSettingValue<bool>("gauntletDesign");
                if (settingVal) {
                    auto gauntletMenu = gauntletPages->getChildByIDRecursive("gauntlet-menu");
                    gauntletMenu->setScale(0.725f);
                }
            }
            
            std::vector<CCSprite*> gauntletButtons;
            for (int b = 0; b < 3; b++) {
                auto gauntletButton = static_cast<CCSprite*>(gauntletMenu->getChildByIDRecursive(fmt::format("gauntlet-button-{}", b + 1)));
                if (gauntletButton) {
                    gauntletButtons.push_back(gauntletButton);
                    auto gauntletNode = static_cast<CCSprite*>(gauntletButton->getChildByIDRecursive("background"));
                    if (gauntletNode) {
                        gauntletNode->setContentSize({110, 220});
                    }

                    // // Gauntlet Info Nodes // //

                    // Info Node
                    auto infoNode = static_cast<CCSprite*>(gauntletButton->getChildByIDRecursive("gauntlet-info-node"));
                    auto GDUtils = Loader::get()->getLoadedMod("gdutilsdevs.gdutils");
                    if (GDUtils) {
                        auto settingVal = GDUtils->getSettingValue<bool>("gauntletDesign");
                        if (!settingVal) {
                            infoNode->setPositionY(-2.5f);
                        }
                    }

                    // // Name Nodes // //

                    // Name Label
                    auto nameLabel = static_cast<CCSprite*>(gauntletButton->getChildByIDRecursive("gauntlet-name-label"));
                    if (nameLabel) {
                        nameLabel->setPositionY(84);
                    }
                    // Gauntlet Label
                    auto gauntletLabel = static_cast<CCSprite*>(gauntletButton->getChildByIDRecursive("gauntlet-label"));
                    if (gauntletLabel) {
                        gauntletLabel->setPositionY(nameLabel->getPositionY() - 15);
                    }
                    // Name Shadow Label
                    auto nameShadowLabel = static_cast<CCSprite*>(gauntletButton->getChildByIDRecursive("gauntlet-name-shadow-label"));
                    if (nameShadowLabel) {
                        nameShadowLabel->setPosition(ccp(nameLabel->getPositionX() + 2, nameLabel->getPositionY() - 2));
                    }
                    // Gauntlet Shadow Label
                    auto gauntletShadowLabel = static_cast<CCSprite*>(gauntletButton->getChildByIDRecursive("gauntlet-shadow-label"));
                    if (gauntletShadowLabel) {
                        gauntletShadowLabel->setPosition(ccp(gauntletLabel->getPositionX() + 2, gauntletLabel->getPositionY() - 2));
                    }

                    // // Reward Nodes // //

                    // Chest Sprite
                    auto chestSpr = static_cast<CCSprite*>(gauntletButton->getChildByIDRecursive("chest-sprite"));
                    if (chestSpr) {
                        chestSpr->setPositionY(-64.5f);
                        chestSpr->setScale(0.3f);
                    }
                    // Chest Shadow Sprite
                    auto chestShadowSpr = static_cast<CCSprite*>(gauntletButton->getChildByIDRecursive("chest-shadow-sprite"));
                    if (chestShadowSpr) {
                        chestShadowSpr->setPosition(ccp(chestSpr->getPositionX() + 2, chestSpr->getPositionY() - 2));
                        chestShadowSpr->setScale(0.3f);
                    }
                    // Reward Label
                    auto rewardLabel = static_cast<CCSprite*>(gauntletButton->getChildByIDRecursive("reward-label"));
                    if (rewardLabel) {
                        rewardLabel->setPositionY(chestSpr->getPositionY() - 13.5f);
                        rewardLabel->setScale(0.5f);
                        rewardLabel->setZOrder(3);
                    }
                    // Reward Shadow Label
                    auto rewardShadowLabel = static_cast<CCSprite*>(gauntletButton->getChildByIDRecursive("reward-shadow-label"));
                    if (rewardShadowLabel) {
                        rewardShadowLabel->setPosition(ccp(rewardLabel->getPositionX() + 2, rewardLabel->getPositionY() - 2));
                        rewardShadowLabel->setScale(0.5f);
                    }

                    // // Progression Label // //

                    // Progress Label
                    auto progressLabel = infoNode->getChildByIDRecursive("gauntlet-progress-label");
                    if (progressLabel) {
                        auto children = progressLabel->getChildren();
                        if (children && children->count() > 0) {
                            for (int i = 0; i < children->count(); i++) {
                                auto label = typeinfo_cast<CCLabelBMFont*>(children->objectAtIndex(i));
                                if (label) {
                                    std::string progressText = label->getString();
                                    if (progressText == "5/5") {
                                        label->setColor(ccc3(113, 211, 39)); // ✅ Apply to actual label
                                        log::info("Gauntlet is completed!");
                                    }
                                }
                            }
                        }
                    } else {
                        log::warn("gauntlet-progress-label not found");
                    }
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