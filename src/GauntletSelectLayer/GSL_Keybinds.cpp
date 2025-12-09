#include <Geode/Geode.hpp>
#include <Geode/modify/GauntletSelectLayer.hpp>
#include "GauntletSelectLayer.cpp"

using namespace geode::prelude;

#ifndef GEODE_IS_IOS
	#include <geode.custom-keybinds/include/Keybinds.hpp>
	using namespace keybinds;

    $execute {
        BindManager::get()->registerBindable({
            "previous-gauntlet"_spr,
            "Previous Gauntlet Page",
            "Views the <cg>previous</c> page of gauntlets.",
            { Keybind::create(KEY_Left) },
            "Better Gauntlets/GauntletSelectLayer"
        });
        BindManager::get()->registerBindable({
            "next-gauntlet"_spr,
            "Next Gauntlet Page",
            "Views the <cg>next</c> page of gauntlets.",
            { Keybind::create(KEY_Right) },
            "Better Gauntlets/GauntletSelectLayer"
        });
        BindManager::get()->registerBindable({
            "first-visible-gauntlet"_spr,
            "First Gauntlet",
            "Enters the <cg>first</c> gauntlet on the current Gauntlet Page.\n\n<cy>(Disclaimer: Results may not always be accurate.)</c>",
            { Keybind::create(KEY_One) },
            "Better Gauntlets/GauntletSelectLayer"
        });
        BindManager::get()->registerBindable({
            "second-visible-gauntlet"_spr,
            "Second Gauntlet",
            "Enters the <cg>second</c> gauntlet on the current Gauntlet Page.\n\n<cy>(Disclaimer: Results may not always be accurate.)</c>",
            { Keybind::create(KEY_Two) },
            "Better Gauntlets/GauntletSelectLayer"
        });
        BindManager::get()->registerBindable({
            "third-visible-gauntlet"_spr,
            "Third Gauntlet",
            "Enters the <cg>third</c> gauntlet on the current Gauntlet Page.\n\n<cy>(Disclaimer: Results may not always be accurate.)</c>",
            { Keybind::create(KEY_Three) },
            "Better Gauntlets/GauntletSelectLayer"
        });
    }
#endif