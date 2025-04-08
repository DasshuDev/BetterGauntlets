#include "GauntletInfo.hpp"

std::string getGauntletDate(GauntletType gauntletType) {
    switch (gauntletType) {
        default: {
            return "TBA";
        }
        case GauntletType::Fire: {
            return "February 5, 2017";
        }
        case GauntletType::Ice: {
            return "February 5, 2017";
        }
        case GauntletType::Poison: {
            return "February 6, 2017";
        }
        case GauntletType::Shadow: {
            return "February 6, 2017";
        }
        case GauntletType::Lava: {
            return "February 7, 2017";
        }
        case GauntletType::Bonus: {
            return "February 7, 2017";
        }
        case GauntletType::Chaos: {
            return "February 8, 2017";
        }
        case GauntletType::Demon: {
            return "February 8, 2017";
        }
        case GauntletType::Time: {
            return "March 28, 2018";
        }
        case GauntletType::Crystal: {
            return "November 16, 2017";
        }
        case GauntletType::Magic: {
            return "December 28, 2017";
        }
        case GauntletType::Spike: {
            return "December 28, 2017";
        }
        case GauntletType::Monster: {
            return "April 11, 2018";
        }
        case GauntletType::Doom: {
            return "November 18, 2017";
        }
        case GauntletType::Death: {
            return "April 11, 2018";
        }
        case GauntletType::Forest: {
            return "July 18, 2024";
        }
        // case GauntletType::Rune: {
        //     return "TBA";
        // }
        case GauntletType::Force: {
            return "December 2, 2024";
        }
        // case GauntletType::Spooky: {
        //     return "TBA";
        // }
        // case GauntletType::Dragon: {
        //     return "TBA";
        // }
        case GauntletType::Water: {
            return "March 18, 2024";
        }
        case GauntletType::Haunted: {
            return "October 31, 2024";
        }
        // case GauntletType::Acid: {
        //     return "TBA";
        // }
        // case GauntletType::Witch: {
        //     return "TBA";
        // }
        case GauntletType::Power: {
            return "January 26, 2025";
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
            return "October 31, 2024";
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
            return "September 12, 2024";
        }
        case GauntletType::Portal: {
            return "March 3, 2024";
        }
        case GauntletType::Strange: {
            return "April 11, 2024";
        }
        case GauntletType::Fantasy: {
            return "May 5, 2024";
        }
        case GauntletType::Christmas: {
            return "December 25, 2024";
        }
        // case GauntletType::Surprise: {
        //     return "TBA";
        // }
        case GauntletType::Mystery: {
            return "June 20, 2024";
        }
        case GauntletType::Cursed: {
            return "May 19, 2024";
        }
        case GauntletType::Cyborg: {
            return "August 16, 2024";
        }
        case GauntletType::Castle: {
            return "January 25, 2024";
        }
        // case GauntletType::Grave: {
        //     return "TBA";
        // }
        // case GauntletType::Temple: {
        //     return "TBA";
        // }
        case GauntletType::World: {
            return "December 19, 2023";
        }
        case GauntletType::Galaxy: {
            return "December 19, 2023";
        }
        case GauntletType::Universe: {
            return "December 19, 2023";
        }
        case GauntletType::Discord: {
            return "December 19, 2023";
        }
        case GauntletType::Split: {
            return "December 19, 2023";
        }
        case GauntletType::NCS: {
            return "October 5, 2024";
        }
        case GauntletType::NCS2: {
            return "October 5, 2024";
        }
        case GauntletType::Space: {
            return "April 5, 2025";
        }
        case GauntletType::Cosmos: {
            return "April 5, 2025";
        }
    }
}