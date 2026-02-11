#pragma once

#include <Geode/modify/GauntletSelectLayer.hpp>

using namespace geode::prelude;

class $modify(RedesignedGauntletSelectLayer, GauntletSelectLayer) {
    struct Fields {
        
        std::vector<CCMenuItemSpriteExtra*> m_dots = {};
        CCMenu* m_dotsMenu = nullptr;
        int currentGauntletPage = 0;
        CCMenuItemSpriteExtra* settingsButton;
        int m_retryCount = 0;  // <-- ADD THIS
    };

    bool init(int type);
    void onCustomGauntlet(CCObject* sender);

    CCMenuItemSpriteExtra* createCustomGauntletButton(int gauntletType, int buttonIndex);
};