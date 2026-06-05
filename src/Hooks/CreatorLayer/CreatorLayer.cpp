#include "CreatorLayer.hpp"

using namespace geode::prelude;

bool MyCreatorLayer::init() {
    if (!CreatorLayer::init()) return false;

    log::info("welcome");

    auto GDUtils = Loader::get()->getLoadedMod("gdutilsdevs.gdutils");
    if (GDUtils) {
        auto settingVal = GDUtils->getSettingValue<bool>("gauntletDesign");
        if (settingVal) {
            GDUtils->setSettingValue<bool>("gauntletDesign", false);
            log::info("gauntlet design disabled");
        }
        else log::error("couldnt disable, twin");
    }

    return true;
}