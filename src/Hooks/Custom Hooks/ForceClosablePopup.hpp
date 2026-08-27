#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct ForceClosablePopup {
    virtual void forceCloseFromReward(cocos2d::CCObject* sender) = 0;
};

// marker only, lets the shared reward hook know whether to reskin orb/diamond icons to coins/crystals for this popup
struct CoinCrystalStylablePopup {
    virtual ~CoinCrystalStylablePopup() = default;
};
