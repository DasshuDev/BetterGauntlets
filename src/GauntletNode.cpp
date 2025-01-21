#include "GauntletNode.hpp"

using namespace geode::prelude;

gd::string RedesignedGauntletNode::frameForType(GauntletType type) {
    switch(type) {
        // case GauntletType::Fire: return "island_lvl_fire_001.png"_spr;
        // case GauntletType::Ice: return "island_lvl_ice_001.png"_spr;
        // case GauntletType::Poison: return "island_lvl_poison_001.png"_spr;
        default: return GauntletNode::frameForType(type);
    }
}