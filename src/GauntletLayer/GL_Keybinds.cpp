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
		"View the <cg>first</c> gauntlet level.",
		{ Keybind::create(KEY_One) },
		"Global/Better Gauntlets"
	});
	BindManager::get()->registerBindable({
		"second-gauntlet-level"_spr,
		"Second Gauntlet Level",
		"View the <cg>second</c> gauntlet level.",
		{ Keybind::create(KEY_Two) },
		"Global/Better Gauntlets"
	});
	BindManager::get()->registerBindable({
		"third-gauntlet-level"_spr,
		"Third Gauntlet Level",
		"View the <cg>third</c> gauntlet level.",
		{ Keybind::create(KEY_Three) },
		"Global/Better Gauntlets"
	});
	BindManager::get()->registerBindable({
		"fourth-gauntlet-level"_spr,
		"Fourth Gauntlet Level",
		"View the <cg>fourth</c> gauntlet level.",
		{ Keybind::create(KEY_Four) },
		"Global/Better Gauntlets"
	});
	BindManager::get()->registerBindable({
		"fifth-gauntlet-level"_spr,
		"Fifth Gauntlet Level",
		"View the <cg>fifth</c> gauntlet level.",
		{ Keybind::create(KEY_Five) },
		"Global/Better Gauntlets"
	});
}
#endif