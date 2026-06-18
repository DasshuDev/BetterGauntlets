#pragma once
#include <string>
#include <array>

struct CustomGauntletLevel {
    int id = 0;
    std::string name;
    std::string creator;
    int stars = 0;
};

struct CustomGauntletData {
    int id = 0;
    std::string name;
    std::string description;
    std::string iconURL;
    int bgIndex = 1;
    cocos2d::ccColor3B nameColor  = {255, 255, 255};
    cocos2d::ccColor3B nodeColor  = {255, 255, 255};
    cocos2d::ccColor3B bgColor    = {255, 255, 255};
    cocos2d::ccColor3B accentColor1 = {255, 255, 255};
    cocos2d::ccColor3B accentColor2 = {255, 255, 255};
    std::array<CustomGauntletLevel, 5> levels;

    std::string infoDate;
    std::string infoVersion;
    std::string infoSuggester;
    int infoAccID = 0;

    // Legacy compat for code that just needs a single color
    cocos2d::ccColor3B color = {255, 255, 255};
};