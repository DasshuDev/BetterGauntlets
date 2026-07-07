#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Detects when a level completion finishes off one of our custom gauntlets
// and, if so, queues the crystal reward that GRCurrencyRewardLayer will pick
// up when the game's own reward popup appears for this same completion.
//
// A completion only counts if this PlayLayer was entered via the gauntlet
// screen (CustomGauntletManager::markPendingGauntletAttempt) - finishing the
// same level some other way (search, saved levels, etc.) grants nothing.
class $modify(GRPlayLayer, PlayLayer) {
    struct Fields {
        bool m_isGauntletAttempt = false;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects);
    void levelComplete();
};
