#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct ForceClosablePopup {
    virtual void forceCloseFromReward(cocos2d::CCObject* sender) = 0;
};
