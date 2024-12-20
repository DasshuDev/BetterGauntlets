#include <Geode/Geode.hpp>
#include <Geode/modify/GauntletLayer.hpp>
#include "GauntletLayer.cpp"

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS
#include <geode.custom-keybinds/include/Keybinds.hpp>
using namespace keybinds;
#endif

#ifdef GEODE_IS_WINDOWS
$execute {
	BindManager::get()->registerBindable({
		"first-gauntlet-level"_spr,
		"First Gauntlet Level",
		"Use this keybind (set to 1 by default) to view the first gauntlet level.",
		{ Keybind::create(KEY_One) },
		"Global/Better Gauntlets"
	});
	BindManager::get()->registerBindable({
		"second-gauntlet-level"_spr,
		"Second Gauntlet Level",
		"Use this keybind (set to 2 by default) to view the second gauntlet level.",
		{ Keybind::create(KEY_Two) },
		"Global/Better Gauntlets"
	});
	BindManager::get()->registerBindable({
		"third-gauntlet-level"_spr,
		"Third Gauntlet Level",
		"Use this keybind (set to 3 by default) to view the third gauntlet level.",
		{ Keybind::create(KEY_Three) },
		"Global/Better Gauntlets"
	});
	BindManager::get()->registerBindable({
		"fourth-gauntlet-level"_spr,
		"Fourth Gauntlet Level",
		"Use this keybind (set to 4 by default) to view the fourth gauntlet level.",
		{ Keybind::create(KEY_Four) },
		"Global/Better Gauntlets"
	});
	BindManager::get()->registerBindable({
		"fifth-gauntlet-level"_spr,
		"Fifth Gauntlet Level",
		"Use this keybind (set to 5 by default) to view the fifth gauntlet level.",
		{ Keybind::create(KEY_Five) },
		"Global/Better Gauntlets"
	});
}
#endif