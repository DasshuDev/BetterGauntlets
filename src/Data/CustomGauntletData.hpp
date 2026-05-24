#pragma once
#include <string>
#include <vector>

struct CustomGauntletData {
    int id = 0;
    std::string name;
    std::string iconURL;
    std::vector<int> levelIDs; // parsed from colon-separated GD gauntlet format
    cocos2d::ccColor3B color = {255, 255, 255};
};