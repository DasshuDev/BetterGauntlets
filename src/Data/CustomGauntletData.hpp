#pragma once
#include <Geode/Geode.hpp>
#include <string>
// #include <vector>

using namespace geode::prelude;

// Per-level entry inside a custom gauntlet
struct CustomSlotLevel {
    int         id      = 0;
    std::string name;
    std::string creator;
    int         stars   = 0;
};

// Full server-side data for a published custom gauntlet
struct CustomGauntletData {
    int         id           = 0;
    std::string name;
    std::string description;
    std::string iconURL;

    ccColor3B   nameColor    = {255, 255, 255};
    ccColor3B   nodeColor    = {255, 255, 255};
    ccColor3B   bgColor      = {34,  34,  34 };
    ccColor3B   accentColor1 = {255, 255, 255};
    ccColor3B   accentColor2 = {255, 255, 255};

    int         bgIndex      = 1;

    std::array<CustomSlotLevel, 5> levels;

    std::string infoDate;
    std::string infoVersion;
    std::string infoSuggester;
    int         infoAccID    = 0;

    bool        featured     = false;
    int         rewardCoins  = 0;
};