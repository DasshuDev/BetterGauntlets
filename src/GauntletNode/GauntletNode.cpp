#include "GauntletNode.hpp"

using namespace geode::prelude;

gd::string RedesignedGauntletNode::frameForType(GauntletType type) {
    switch(type) {
        case GauntletType::Discord: return "island_discord_002.png"_spr;
        default: return GauntletNode::frameForType(type);
    }
}

bool RedesignedGauntletNode::init(GauntletType* type) {
    if (!GauntletNode::init(type))
        return false;

    log::info("GauntletNode init for type {}", static_cast<int>(type));

    return true;
}