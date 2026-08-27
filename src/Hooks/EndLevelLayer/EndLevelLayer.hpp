#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/EndLevelLayer.hpp>

using namespace geode::prelude;

class $modify(GREndLevelLayer, EndLevelLayer) {
    void customSetup();
};
