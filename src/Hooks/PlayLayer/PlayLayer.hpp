#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(GRPlayLayer, PlayLayer) {
    void levelComplete();
};
