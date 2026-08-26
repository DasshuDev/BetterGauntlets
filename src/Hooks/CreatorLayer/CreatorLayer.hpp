#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/CreatorLayer.hpp>

using namespace geode::prelude;

class $modify(BGCreatorLayer, CreatorLayer) {
    bool init();
    void onGauntlets(cocos2d::CCObject* sender);
};
