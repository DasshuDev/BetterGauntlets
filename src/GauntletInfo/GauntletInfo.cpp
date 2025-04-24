#include <Geode/Geode.hpp>
#include "GauntletInfo.hpp"

using namespace geode::prelude;

std::string getGauntletVer(GauntletType gauntletType) {
    switch (gauntletType) {
        default: {
            return ("{}", Loader::get()->getGameVersion());
        }
        case GauntletType::Fire: {
            return "2.1";
        }
        case GauntletType::Ice: {
            return "2.1";
        }
        case GauntletType::Poison: {
            return "2.1";
        }
        case GauntletType::Shadow: {
            return "2.1";
        }
        case GauntletType::Lava: {
            return "2.1";
        }
        case GauntletType::Bonus: {
            return "2.1";
        }
        case GauntletType::Chaos: {
            return "2.1";
        }
        case GauntletType::Demon: {
            return "2.1";
        }
        case GauntletType::Time: {
            return "2.11";
        }
        case GauntletType::Crystal: {
            return "2.11";
        }
        case GauntletType::Magic: {
            return "2.11";
        }
        case GauntletType::Spike: {
            return "2.11";
        }
        case GauntletType::Monster: {
            return "2.11";
        }
        case GauntletType::Doom: {
            return "2.11";
        }
        case GauntletType::Death: {
            return "2.11";
        }
        case GauntletType::Forest: {
            return "2.2";
        }
        // case GauntletType::Rune: {
        //     return "2.2";
        // }
        case GauntletType::Force: {
            return "2.2";
        }
        // case GauntletType::Spooky: {
        //     return "2.2";
        // }
        // case GauntletType::Dragon: {
        //     return "2.2";
        // }
        case GauntletType::Water: {
            return "2.2";
        }
        case GauntletType::Haunted: {
            return "2.2";
        }
        // case GauntletType::Acid: {
        //     return "2.2";
        // }
        // case GauntletType::Witch: {
        //     return "2.2";
        // }
        case GauntletType::Power: {
            return "2.2";
        }
        // case GauntletType::Potion: {
        //     return "TBA";
        // }
        // case GauntletType::Snake: {
        //     return "TBA";
        // }
        // case GauntletType::Toxic: {
        //     return "TBA7";
        // }
        case GauntletType::Halloween: {
            return "2.2";
        }
        // case GauntletType::Treasure: {
        //     return "TBA";
        // }
        // case GauntletType::Ghost: {
        //     return "TBA7";
        // }
        // case GauntletType::Spider: {
        //     return "TBA";
        // }
        // case GauntletType::Gem: {
        //     return "TBA";
        // }
        case GauntletType::Inferno: {
            return "2.2";
        }
        case GauntletType::Portal: {
            return "2.2";
        }
        case GauntletType::Strange: {
            return "2.2";
        }
        case GauntletType::Fantasy: {
            return "2.2";
        }
        case GauntletType::Christmas: {
            return "2.2";
        }
        // case GauntletType::Surprise: {
        //     return "TBA";
        // }
        case GauntletType::Mystery: {
            return "2.2";
        }
        case GauntletType::Cursed: {
            return "2.2";
        }
        case GauntletType::Cyborg: {
            return "2.2";
        }
        case GauntletType::Castle: {
            return "2.2";
        }
        // case GauntletType::Grave: {
        //     return "TBA";
        // }
        // case GauntletType::Temple: {
        //     return "TBA";
        // }
        case GauntletType::World: {
            return "2.2";
        }
        case GauntletType::Galaxy: {
            return "2.2";
        }
        case GauntletType::Universe: {
            return "2.2";
        }
        case GauntletType::Discord: {
            return "2.2";
        }
        case GauntletType::Split: {
            return "2.2";
        }
        case GauntletType::NCS: {
            return "2.207";
        }
        case GauntletType::NCS2: {
            return "2.207";
        }
        case GauntletType::Space: {
            return "2.207";
        }
        case GauntletType::Cosmos: {
            return "2.207";
        }
    }
}