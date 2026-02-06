// CustomGauntletPage.hpp
#pragma once
#include <Geode/Geode.hpp>
#include "CustomGauntlets.hpp"

using namespace geode::prelude;

namespace CustomGauntletPage {
    CCMenuItemSpriteExtra* createCustomGauntletButton(int gauntletType, int buttonIndex, cocos2d::SEL_MenuHandler callback, CCObject* target);
    CCMenu* addCustomGauntletsPage(BoomScrollLayer* scrollLayer, cocos2d::SEL_MenuHandler callback, CCObject* target, CCSize winSize);
    
    void positionCustomGauntletButtons(CCMenu* customPage, CCSize winSize);
}