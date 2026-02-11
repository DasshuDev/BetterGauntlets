#pragma once

#include <Geode/modify/GauntletSelectLayer.hpp>

using namespace geode::prelude;

class $modify(RedesignedGauntletSelectLayer, GauntletSelectLayer) {
    struct Fields {
        
        std::vector<CCMenuItemSpriteExtra*> m_dots = {};
        CCMenu* m_dotsMenu = nullptr;
        int currentGauntletPage = 0;
    };

    bool init(int type);
    void onCustomGauntlet(CCObject* sender);

    CCMenuItemSpriteExtra* createCustomGauntletButton(int gauntletType, int buttonIndex);
};