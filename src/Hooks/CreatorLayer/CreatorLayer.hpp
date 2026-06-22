#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/CreatorLayer.hpp>

using namespace geode::prelude;

class $modify(MyCreatorLayer, CreatorLayer) {
    bool init();
    void onGauntlets(cocos2d::CCObject* sender);
};
