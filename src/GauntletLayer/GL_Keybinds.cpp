#include <Geode/Geode.hpp>
#include <Geode/loader/Loader.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS
#include <geode.custom-keybinds/include/Keybinds.hpp>
using namespace keybinds;
#endif

#ifdef GEODE_IS_WINDOWS
$execute {
	BindManager::get()->registerBindable({
		"first-gauntlet-level"_spr,
		"Gauntlet Level 1",
		"View the <cg>first</c> gauntlet level.",
		{ Keybind::create(KEY_One) },
		"Global/Better Gauntlets"
	});
	BindManager::get()->registerBindable({
		"second-gauntlet-level"_spr,
		"Gauntlet Level 2",
		"View the <cg>second</c> gauntlet level.",
		{ Keybind::create(KEY_Two) },
		"Global/Better Gauntlets"
	});
	BindManager::get()->registerBindable({
		"third-gauntlet-level"_spr,
		"Gauntlet Level 3",
		"View the <cg>third</c> gauntlet level.",
		{ Keybind::create(KEY_Three) },
		"Global/Better Gauntlets"
	});
	BindManager::get()->registerBindable({
		"fourth-gauntlet-level"_spr,
		"Gauntlet Level 4",
		"View the <cg>fourth</c> gauntlet level.",
		{ Keybind::create(KEY_Four) },
		"Global/Better Gauntlets"
	});
	BindManager::get()->registerBindable({
		"fifth-gauntlet-level"_spr,
		"Gauntlet Level 5",
		"View the <cg>fifth</c> gauntlet level.",
		{ Keybind::create(KEY_Five) },
		"Global/Better Gauntlets"
	});
}
#endif