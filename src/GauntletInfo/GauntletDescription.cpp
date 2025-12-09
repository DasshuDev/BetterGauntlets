#include "GauntletInfo.hpp"

std::string getGauntletInfo(GauntletType gauntletType) {
    switch (gauntletType) {
        default: {
            return "No info available for this gauntlet yet.";
        }
        case GauntletType::Fire: {
            return "A fiery landscape where the skies are filled with intense embers, and the charred ground radiates an unusual warmth.";
        }
        case GauntletType::Ice: {
            return "These desolate snow-covered lands present many dangers! Its jagged ice formations rise like sharp teeth in the sky, and the chilling temperatures certainly don’t help things.";
        }
        case GauntletType::Poison: {
            return "In these poisonous parts, pools of murky, bubbling sludge cover the ground. The air is thick with an overwhelming stench of chemical fumes, making every breath feel like it could be your last…";
        }
        case GauntletType::Shadow: {
            return "A realm shrouded in darkness, where shadows dance and flicker. The eerie silence is only broken by the whispers of unseen entities.";
        }
        case GauntletType::Lava: {
            return "A molten landscape where rivers of lava flow freely. The air is thick with heat and the ground trembles with volcanic activity.";
        }
        case GauntletType::Bonus: {
            return "A land of unexpected rewards and hidden treasures. Every corner holds a surprise, but not all are pleasant.";
        }
        case GauntletType::Chaos: {
            return "A chaotic world where the laws of nature are twisted. Gravity shifts, time warps, and nothing is as it seems.";
        }
        case GauntletType::Demon: {
            return "A hellish domain ruled by fearsome demons. The air is thick with the scent of brimstone and the ground is scorched by infernal flames.";
        }
        case GauntletType::Time: {
            return "A dimension where time flows differently. Ancient ruins and futuristic landscapes coexist, and every step could take you to a different era.";
        }
        case GauntletType::Crystal: {
            return "A dazzling world of shimmering crystals and sparkling gems. The beauty is mesmerizing, but the sharp edges and hidden traps make it perilous.";
        }
        case GauntletType::Magic: {
            return "A mystical realm where magic is alive. Enchanted forests, floating islands, and magical creatures abound.";
        }
        case GauntletType::Spike: {
            return "A treacherous land filled with deadly spikes and traps. Every step must be taken with caution, as danger lurks around every corner.";
        }
        case GauntletType::Monster: {
            return "A terrifying world inhabited by monstrous creatures. The roars and growls echo through the air, and the ground shakes with their footsteps.";
        }
        case GauntletType::Doom: {
            return "A desolate wasteland where doom is imminent. The sky is dark, the ground is barren, and the air is filled with a sense of impending disaster.";
        }
        case GauntletType::Death: {
            return "A grim realm where death is ever-present. The air is cold, the ground is littered with bones, and the shadows seem to move on their own.";
        }
        case GauntletType::Forest: {
            return "A dense forest teeming with life and mystery. The canopy is thick, the undergrowth is lush, and the sounds of wildlife fill the air.";
        }
        case GauntletType::Rune: {
            return "A mystical land where ancient runes hold great power. The air is thick with magic, and the ground is covered in glowing symbols.";
        }
        case GauntletType::Force: {
            return "A powerful domain where the forces of nature are unleashed. Thunderstorms, earthquakes, and tornadoes are common occurrences.";
        }
        case GauntletType::Spooky: {
            return "A chilling realm where eerie sounds and ghostly apparitions abound. The atmosphere is thick with suspense, and every shadow seems to hide a lurking danger.";
        }
        case GauntletType::Dragon: {
            return "A majestic world ruled by dragons. The skies are filled with their mighty roars, and the ground is scorched by their fiery breath.";
        }
        case GauntletType::Water: {
            return "A vast underwater world where the ocean's depths hold many secrets. The water is clear, the marine life is abundant, and the currents are strong.";
        }
        case GauntletType::Haunted: {
            return "A spooky realm where ghosts and ghouls roam freely. The air is filled with eerie sounds and the atmosphere is chilling.";
        }
        case GauntletType::Acid: {
            return "A corrosive landscape where pools of acid bubble and steam. The air is thick with toxic fumes, and the ground is unstable.";
        }
        case GauntletType::Witch: {
            return "A mystical world where witches and warlocks practice their craft. Enchanted forests, bubbling cauldrons, and spellbinding potions are everywhere.";
        }
        case GauntletType::Power: {
            return "A domain where raw power is the key to survival. The air crackles with energy, and the ground trembles with every step.";
        }
        case GauntletType::Potion: {
            return "A whimsical world where magical potions abound. The air is filled with enchanting aromas, and the landscape is dotted with colorful bottles and bubbling cauldrons.";
        }
        case GauntletType::Snake: {
            return "A slithering world where snakes of all kinds reign supreme. The ground is covered in scales, and the air is filled with hisses and rattles.";
        }
        case GauntletType::Toxic: {
            return "A hazardous environment where toxic substances are rampant. The air is thick with noxious fumes, and the ground is littered with dangerous waste.";
        }
        case GauntletType::Halloween: {
            return "A festive yet frightening world where Halloween never ends. Pumpkins, skeletons, and spooky decorations are everywhere.";
        }
        case GauntletType::Treasure: {
            return "A glittering realm where treasure is abundant. Gold, jewels, and precious artifacts are hidden throughout the landscape.";
        }
        case GauntletType::Ghost: {
            return "A spectral world where ghosts and spirits roam freely. The air is filled with eerie whispers, and the atmosphere is thick with mystery.";
        }
        case GauntletType::Spider: {
            return "A web-filled domain where spiders of all sizes spin their intricate webs. The ground is sticky, and the air is filled with the sound of skittering legs.";
        }
        case GauntletType::Gem: {
            return "A sparkling world where precious gems and minerals abound. The landscape glitters with every color imaginable, but beware of hidden dangers among the beauty.";
        }
        case GauntletType::Inferno: {
            return "A blazing inferno where the heat is unbearable. The ground is scorched, the air is filled with smoke, and the flames are relentless.";
        }
        case GauntletType::Portal: {
            return "A mysterious realm where portals to other dimensions are abundant. Each portal leads to a new and unknown world.";
        }
        case GauntletType::Strange: {
            return "A bizarre world where nothing makes sense. The landscape is surreal, the creatures are odd, and the rules of reality are twisted.";
        }
        case GauntletType::Fantasy: {
            return "A fantastical realm where dreams and reality blend. Castles, dragons, and magical creatures are everywhere.";
        }
        case GauntletType::Christmas: {
            return "A festive wonderland where Christmas is eternal. Snow-covered landscapes, twinkling lights, and holiday cheer abound.";
        }
        case GauntletType::Surprise: {
            return "A whimsical world full of unexpected delights and surprises. Every turn brings a new wonder, from playful creatures to magical phenomena.";
        }
        case GauntletType::Mystery: {
            return "A puzzling world where mysteries are waiting to be solved. Clues are hidden everywhere, and nothing is as it seems.";
        }
        case GauntletType::Cursed: {
            return "A cursed land where misfortune is rampant. The air is thick with a sense of dread, and the ground is littered with cursed objects.";
        }
        case GauntletType::Cyborg: {
            return "A futuristic world where technology and biology merge. The landscape is filled with advanced machinery and cybernetic creatures.";
        }
        case GauntletType::Castle: {
            return "A grand castle filled with hidden secrets and ancient treasures. The halls are vast, the rooms are numerous, and danger lurks around every corner.";
        }
        case GauntletType::Grave: {
            return "A somber graveyard where the dead rest in peace. The air is heavy with sorrow, and the ground is dotted with tombstones.";
        }
        case GauntletType::Temple: {
            return "An ancient temple filled with traps and treasures. The air is thick with mystery, and the walls are adorned with intricate carvings.";
        }
        case GauntletType::World: {
            return "A diverse world where every biome is represented. From deserts to jungles, mountains to oceans, this gauntlet has it all.";
        }
        case GauntletType::Galaxy: {
            return "A vast galaxy where the stars are your guide. Planets, asteroids, and cosmic phenomena are everywhere.";
        }
        case GauntletType::Universe: {
            return "An expansive universe where the possibilities are endless. Galaxies, black holes, and nebulae are just the beginning.";
        }
        case GauntletType::Discord: {
            return "A chaotic realm where discord reigns supreme. The landscape is ever-changing, and the rules are constantly shifting.";
        }
        case GauntletType::Split: {
            return "A fractured world where reality is split. Each side holds different challenges and dangers.";
        }
        case GauntletType::NCS: {
            return "A vibrant world filled with music and rhythm. The landscape pulses with energy, and the beat never stops.";
        }
        case GauntletType::NCS2: {
            return "A continuation of the vibrant NCS world, with even more music and rhythm. The challenges are tougher, and the beat is faster.";
        }
        case GauntletType::Space: {
            return "A boundless expanse filled with stars, planets, and cosmic wonders. The silence of the void is both serene and haunting, while meteors and black holes pose constant threats.";
        }
        case GauntletType::Cosmos: {
            return "An awe-inspiring expanse where the mysteries of the universe unfold. Nebulae shimmer with vibrant colors, stars are born and die in cosmic dances, and black holes twist the very fabric of reality. The beauty is breathtaking, but the dangers are immense.";
        }
        // case GauntletType::Random: {
        //     return "An ever-changing gauntlet assembled from a mix of biomes and hazards, ensuring every run feels unique and unpredictable.";
        // }
        // case GauntletType::Chance: {
        //     return "A gauntlet governed by luck, where rewards and dangers appear unpredictably and fortunes shift with every step.";
        // }
    }
}