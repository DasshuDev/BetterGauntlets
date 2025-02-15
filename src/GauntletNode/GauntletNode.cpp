#include "GauntletNode.hpp"

using namespace geode::prelude;

gd::string RedesignedGauntletNode::frameForType(GauntletType type) {
    switch(type) {
        case GauntletType::Discord: return "island_discord_002.png"_spr;
        default: return GauntletNode::frameForType(type);
    }
}