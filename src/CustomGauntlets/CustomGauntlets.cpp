#include "CustomGauntlets.hpp"

using namespace geode::prelude;

namespace CustomGauntlets {

    std::string getName(int type) {
        switch(type) {
            case Star:
                return "Star";
            default:
                return "Unknown";
        }
    }

    ccColor3B getBGColor(int type) {
        switch(type) {
            case Star:
                return ccc3(255, 208, 0);
            default:
                return ccc3(255, 255, 255);
        }
    }

    ccColor3B getNameColor(int type) {
        switch(type) {
            case Star:
                return ccc3(255, 255, 128);
            default:
                return ccc3(255, 255, 255);
        }
    }

    std::string getSprite(int type) {
        switch(type) {
            case Star:
                return "island_star_001.png"_spr;
            default:
                return "gauntletLock_001.png";
        }
    }

    std::vector<int> getLevels(int type) {
        switch(type) {
            case Star:
                return {94385545, 94385545, 94385545, 94385545, 94385545};
            default:
                return {128, 128, 128, 128, 128};
        }
    }

    std::string getDescription(int type) {
        switch(type) {
            case Star:
                return "A stellar collection of challenging levels!";
            default:
                return "Custom gauntlet levels added by Better Gauntlets.";
        }
    }

    bool isCustomGauntlet(int type) {
        return type >= 1001; // All custom gauntlets use IDs >= 1000
    }

}