#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct ForceClosablePopup {
    virtual void forceCloseFromReward(cocos2d::CCObject* sender) = 0;
};

// Marker only (no members) - lets the single shared RewardUnlockLayer hook
// know whether its parent popup wants the orb/diamond summary icons it
// builds mid-reveal reskinned to coins/crystals. Only CustomGauntletCompletionPopup
// implements this; the vanilla GauntletCompletionPopup doesn't, since it shows
// real vanilla rewards that should keep their real icons.
struct CoinCrystalStylablePopup {
    virtual ~CoinCrystalStylablePopup() = default;
};
