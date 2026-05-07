#pragma once

#include <Geode/modify/GauntletSelectLayer.hpp>

using namespace geode::prelude;

class $modify(RedesignedGauntletSelectLayer, GauntletSelectLayer) {
    struct Fields {
        std::vector<CCMenuItemSpriteExtra*> m_dots = {};
        CCMenu* m_dotsMenu = nullptr;
        int currentGauntletPage = 0;
        bool showingCustomList = false;
        int m_dialogIndex = 0;
        int m_dialogSprite = 0;
    };

    cocos2d::CCNode* getChildBySpriteFrameNameRecursive(cocos2d::CCNode* parent, char const* name);
    bool init(int gauntletType);
    void updateDots();
    void setupGauntlets();
    void onDot(CCObject* sender);
    void findCurrentGauntletPageUsing(CCArray* pageButtons);
    void pressGauntlet(int desiredGauntlet);
    
    #ifndef GEODE_IS_ANDROID
    void scrollLayerWillScrollToPage(BoomScrollLayer* p0, int p1);
    void scrollLayerScrolledToPage(BoomScrollLayer* p0, int p1);
    #else
    void updateArrows();
    #endif
    
    void onBack(cocos2d::CCObject* sender);
    void onLock(CCObject* sender);
    void onNewInfo(CCObject* sender);
};