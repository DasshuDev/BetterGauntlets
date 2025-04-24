#include <Geode/Geode.hpp>
#include "GauntletInfo.hpp"

using namespace geode::prelude;

std::string getGauntletInfo(GauntletType gauntletType) {
    switch (gauntletType) {
        default: {
            return "No info available for this gauntlet.";
        }
        case GauntletType::Fire: {
            return "A huge <co>fiery</c> landscape where the skies are filled with <cy>intense flames</c>, and the charred ground radiates an unusual <cy>warmth</c>.\n\n<cr>Do you think you can handle the heat?</c>";
        }
        case GauntletType::Ice: {
            return "These desolate snow-covered lands present many dangers! Its jagged ice formations rise like sharp teeth in the sky, and the chilling temperatures certainly don’t help things. Just make sure you don’t catch a cold!";
        }
        case GauntletType::Poison: {
            return "In these poisonous parts, pools of murky, bubbling sludge cover the ground. The air is thick with an overwhelming stench of chemical fumes, making every breath feel like it could be your last…";
        }
        case GauntletType::Shadow: {
            return "A realm shrouded in darkness, where shadows dance and flicker. The eerie silence is only broken by the whispers of unseen entities. Dare to step into the unknown?";
        }
        case GauntletType::Lava: {
            return "A molten landscape where rivers of lava flow freely. The air is thick with heat and the ground trembles with volcanic activity. Can you withstand the fiery wrath?";
        }
        case GauntletType::Bonus: {
            return "A land of unexpected rewards and hidden treasures. Every corner holds a surprise, but not all are pleasant. Are you ready to take the risk for the ultimate reward?";
        }
        case GauntletType::Chaos: {
            return "A chaotic world where the laws of nature are twisted. Gravity shifts, time warps, and nothing is as it seems. Can you navigate through the madness?";
        }
        case GauntletType::Demon: {
            return "A hellish domain ruled by fearsome demons. The air is thick with the scent of brimstone and the ground is scorched by infernal flames. Do you have the courage to face the demons?";
        }
        case GauntletType::Time: {
            return "A dimension where time flows differently. Ancient ruins and futuristic landscapes coexist, and every step could take you to a different era. Can you master the flow of time?";
        }
        case GauntletType::Crystal: {
            return "A dazzling world of shimmering crystals and sparkling gems. The beauty is mesmerizing, but the sharp edges and hidden traps make it perilous. Can you navigate through the crystal maze?";
        }
        case GauntletType::Magic: {
            return "A mystical realm where magic is alive. Enchanted forests, floating islands, and magical creatures abound. Can you harness the power of magic to overcome the challenges?";
        }
        case GauntletType::Spike: {
            return "A treacherous land filled with deadly spikes and traps. Every step must be taken with caution, as danger lurks around every corner. Can you survive the spike-filled gauntlet?";
        }
        case GauntletType::Monster: {
            return "A terrifying world inhabited by monstrous creatures. The roars and growls echo through the air, and the ground shakes with their footsteps. Can you defeat the monsters and emerge victorious?";
        }
        case GauntletType::Doom: {
            return "A desolate wasteland where doom is imminent. The sky is dark, the ground is barren, and the air is filled with a sense of impending disaster. Can you escape the doom that awaits?";
        }
        case GauntletType::Death: {
            return "A grim realm where death is ever-present. The air is cold, the ground is littered with bones, and the shadows seem to move on their own. Can you cheat death and survive?";
        }
        case GauntletType::Forest: {
            return "A dense forest teeming with life and mystery. The canopy is thick, the undergrowth is lush, and the sounds of wildlife fill the air. Can you find your way through the forest?";
        }
        // case GauntletType::Rune: {
        //     return "Add the gauntlet info here.";
        // }
        case GauntletType::Force: {
            return "A powerful domain where the forces of nature are unleashed. Thunderstorms, earthquakes, and tornadoes are common occurrences. Can you harness the forces and conquer the gauntlet?";
        }
        // case GauntletType::Spooky: {
        //     return "Add the gauntlet info here.";
        // }
        // case GauntletType::Dragon: {
        //     return "Add the gauntlet info here.";
        // }
        case GauntletType::Water: {
            return "A vast underwater world where the ocean's depths hold many secrets. The water is clear, the marine life is abundant, and the currents are strong. Can you navigate through the watery gauntlet?";
        }
        case GauntletType::Haunted: {
            return "A spooky realm where ghosts and ghouls roam freely. The air is filled with eerie sounds and the atmosphere is chilling. Can you brave the haunted gauntlet?";
        }
        // case GauntletType::Acid: {
        //     return "Add the gauntlet info here.";
        // }
        // case GauntletType::Witch: {
        //     return "Add the gauntlet info here.";
        // }
        case GauntletType::Power: {
            return "A domain where raw power is the key to survival. The air crackles with energy, and the ground trembles with every step. Can you harness the power and conquer the gauntlet?";
        }
        // case GauntletType::Potion: {
        //     return "Add the gauntlet info here.";
        // }
        // case GauntletType::Snake: {
        //     return "Add the gauntlet info here.";
        // }
        // case GauntletType::Toxic: {
        //     return "Add the gauntlet info here.";
        // }
        case GauntletType::Halloween: {
            return "A festive yet frightening world where Halloween never ends. Pumpkins, skeletons, and spooky decorations are everywhere. Can you survive the tricks and treats of the Halloween gauntlet?";
        }
        // case GauntletType::Treasure: {
        //     return "Add the gauntlet info here.";
        // }
        // case GauntletType::Ghost: {
        //     return "Add the gauntlet info here.";
        // }
        // case GauntletType::Spider: {
        //     return "Add the gauntlet info here.";
        // }
        // case GauntletType::Gem: {
        //     return "Add the gauntlet info here.";
        // }
        case GauntletType::Inferno: {
            return "A blazing inferno where the heat is unbearable. The ground is scorched, the air is filled with smoke, and the flames are relentless. Can you withstand the inferno and emerge unscathed?";
        }
        case GauntletType::Portal: {
            return "A mysterious realm where portals to other dimensions are abundant. Each portal leads to a new and unknown world. Can you navigate through the portals and find your way?";
        }
        case GauntletType::Strange: {
            return "A bizarre world where nothing makes sense. The landscape is surreal, the creatures are odd, and the rules of reality are twisted. Can you make sense of the strange gauntlet?";
        }
        case GauntletType::Fantasy: {
            return "A fantastical realm where dreams and reality blend. Castles, dragons, and magical creatures are everywhere. Can you live out your fantasy and conquer the gauntlet?";
        }
        case GauntletType::Christmas: {
            return "A festive wonderland where Christmas is eternal. Snow-covered landscapes, twinkling lights, and holiday cheer abound. Can you spread the joy and conquer the Christmas gauntlet?";
        }
        // case GauntletType::Surprise: {
        //     return "Add the gauntlet info here.";
        // }
        case GauntletType::Mystery: {
            return "A puzzling world where mysteries are waiting to be solved. Clues are hidden everywhere, and nothing is as it seems. Can you unravel the mysteries and conquer the gauntlet?";
        }
        case GauntletType::Cursed: {
            return "A cursed land where misfortune is rampant. The air is thick with a sense of dread, and the ground is littered with cursed objects. Can you break the curse and survive the gauntlet?";
        }
        case GauntletType::Cyborg: {
            return "A futuristic world where technology and biology merge. The landscape is filled with advanced machinery and cybernetic creatures. Can you navigate through the cyborg gauntlet?";
        }
        case GauntletType::Castle: {
            return "A grand castle filled with hidden secrets and ancient treasures. The halls are vast, the rooms are numerous, and danger lurks around every corner. Can you conquer the castle gauntlet?";
        }
        // case GauntletType::Grave: {
        //     return "Add the gauntlet info here.";
        // }
        // case GauntletType::Temple: {
        //     return "Add the gauntlet info here.";
        // }
        case GauntletType::World: {
            return "A diverse world where every biome is represented. From deserts to jungles, mountains to oceans, this gauntlet has it all. Can you conquer the world gauntlet?";
        }
        case GauntletType::Galaxy: {
            return "A vast galaxy where the stars are your guide. Planets, asteroids, and cosmic phenomena are everywhere. Can you navigate through the galaxy gauntlet?";
        }
        case GauntletType::Universe: {
            return "An expansive universe where the possibilities are endless. Galaxies, black holes, and nebulae are just the beginning. Can you conquer the universe gauntlet?";
        }
        case GauntletType::Discord: {
            return "A chaotic realm where discord reigns supreme. The landscape is ever-changing, and the rules are constantly shifting. Can you navigate through the discord gauntlet?";
        }
        case GauntletType::Split: {
            return "A fractured world where reality is split in two. Each side holds different challenges and dangers. Can you navigate through the split gauntlet?";
        }
        case GauntletType::NCS: {
            return "A vibrant world filled with music and rhythm. The landscape pulses with energy, and the beat never stops. Can you keep up with the rhythm and conquer the NCS gauntlet?";
        }
        case GauntletType::NCS2: {
            return "A continuation of the vibrant NCS world, with even more music and rhythm. The challenges are tougher, and the beat is faster. Can you conquer the NCS2 gauntlet?";
        }
        case GauntletType::Space: {
            return "A boundless expanse filled with stars, planets, and cosmic wonders. The silence of the void is both serene and haunting, while meteors and black holes pose constant threats. Can you navigate the vastness of space and uncover its secrets?";
        }
        case GauntletType::Cosmos: {
            return "An awe-inspiring expanse where the mysteries of the universe unfold. Nebulae shimmer with vibrant colors, stars are born and die in cosmic dances, and black holes twist the very fabric of reality. The beauty is breathtaking, but the dangers are immense. Can you traverse the infinite wonders of the cosmos and uncover its secrets?";
        }
    }
}

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