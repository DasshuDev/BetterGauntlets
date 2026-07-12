#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(GRPlayLayer, PlayLayer) {
    struct Fields {
        bool m_isGauntletAttempt = false;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects);
    void levelComplete();
};
