#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/EndLevelLayer.hpp>

using namespace geode::prelude;

// Shows the crystal reward popup (queued by GRPlayLayer::levelComplete())
// once the level-complete results screen is up.
class $modify(GREndLevelLayer, EndLevelLayer) {
    void customSetup();
};
