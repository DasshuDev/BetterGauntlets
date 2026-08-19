#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// Lets a single RewardUnlockLayer close hook bridge into whichever popup
// (vanilla or custom gauntlet completion) is actually its parent, without
// either popup depending on the other's concrete type.
struct ForceClosablePopup {
    virtual void forceCloseFromReward(cocos2d::CCObject* sender) = 0;
};
