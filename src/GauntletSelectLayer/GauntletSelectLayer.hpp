#pragma once

#include <Geode/modify/GauntletSelectLayer.hpp>
#include <alphalaneous.alphas-ui-pack/include/API.hpp>

using namespace geode::prelude;

class $modify(RedesignedGauntletSelectLayer, GauntletSelectLayer) {
    struct Fields {
        std::vector<CCMenuItemSpriteExtra*> m_dots = {};
        CCMenu* m_dotsMenu = nullptr;
        int currentGauntletPage = 0;
        bool showingCustomList = false;
        int m_dialogIndex = 0;
        int m_dialogSprite = 0;
        alpha::ui::AdvancedScrollLayer* m_customScrollLayer = nullptr;
        alpha::ui::AdvancedScrollBar* m_customScrollBar = nullptr;
        CCMenu* m_gauntletBtnContainer = nullptr;
    };

    cocos2d::CCNode* getChildBySpriteFrameNameRecursive(cocos2d::CCNode* parent, char const* name);
    bool init(int gauntletType);
    void updateDots();
    void loadLevelsFinished(CCArray* gauntlets, char const* key, int type);
    void setupScrollMode();
    void setupDotMode();
    void styleGauntletButtons();
    void onDot(CCObject* sender);
    void findCurrentGauntletPageUsing(CCArray* pageButtons);
    void pressGauntlet(int desiredGauntlet);
    
    #ifndef GEODE_IS_ANDROID
    void scrollLayerWillScrollToPage(BoomScrollLayer* p0, int p1);
    void scrollLayerScrolledToPage(BoomScrollLayer* p0, int p1);
    #else
    void updateArrows();
    #endif
    
    void onBack(CCObject* sender);
    void onLock(CCObject* sender);
    void onNewInfo(CCObject* sender);
    void refreshUI();
};