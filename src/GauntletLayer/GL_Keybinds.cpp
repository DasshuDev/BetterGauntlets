#include <Geode/Geode.hpp>
#include <Geode/loader/Loader.hpp>

using namespace geode::prelude;

#ifndef GEODE_IS_IOS
	#include <geode.custom-keybinds/include/Keybinds.hpp>
	using namespace keybinds;

	$execute {
		BindManager::get()->registerBindable({
			"first-gauntlet-level"_spr,
			"Gauntlet Level 1",
			"View the <cg>first</c> gauntlet level.",
			{ Keybind::create(KEY_One) },
            "Better Gauntlets/GauntletLayer"
		});
		BindManager::get()->registerBindable({
			"second-gauntlet-level"_spr,
			"Gauntlet Level 2",
			"View the <cg>second</c> gauntlet level.",
			{ Keybind::create(KEY_Two) },
            "Better Gauntlets/GauntletLayer"
		});
		BindManager::get()->registerBindable({
			"third-gauntlet-level"_spr,
			"Gauntlet Level 3",
			"View the <cg>third</c> gauntlet level.",
			{ Keybind::create(KEY_Three) },
            "Better Gauntlets/GauntletLayer"
		});
		BindManager::get()->registerBindable({
			"fourth-gauntlet-level"_spr,
			"Gauntlet Level 4",
			"View the <cg>fourth</c> gauntlet level.",
			{ Keybind::create(KEY_Four) },
            "Better Gauntlets/GauntletLayer"
		});
		BindManager::get()->registerBindable({
			"fifth-gauntlet-level"_spr,
			"Gauntlet Level 5",
			"View the <cg>fifth</c> gauntlet level.",
			{ Keybind::create(KEY_Five) },
            "Better Gauntlets/GauntletLayer"
		});
	}
#endif