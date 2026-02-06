#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace CustomGauntlets {
    
    // Set any custom Gauntlets to an ID over
    // 1000, as to not collide with any existing
    // Gauntlet ID for the foreseeable future.

    constexpr int Star = 1001;

    std::string getName(int type); // <--------- Display Name
    std::string getSprite(int type); // <------- Custom GauntletSprite
    std::vector<int> getLevels(int type); // <-- Level IDs
    std::string getDescription(int type); // <-- Custom Description
    ccColor3B getBGColor(int type); // <-------- Gauntlet Btn BG Color
    ccColor3B getNameColor(int type); // <------ Gauntlet Label Color
    bool isCustomGauntlet(int type); // <------- "Is this Gauntlet vanilla or modded?"
}