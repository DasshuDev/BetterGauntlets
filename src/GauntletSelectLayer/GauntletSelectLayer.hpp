#pragma once

#include <Geode/modify/GauntletSelectLayer.hpp>

using namespace geode::prelude;

class $modify(RedesignedGauntletSelectLayer, GauntletSelectLayer) {

    bool init(int type);
    
    void toggleList(CCObject* sender);
};