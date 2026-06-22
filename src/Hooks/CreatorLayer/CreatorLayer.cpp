#include "CreatorLayer.hpp"
#include "../../GauntletSelectLayer/GauntletSelectLayer.hpp"

using namespace geode::prelude;

bool MyCreatorLayer::init() {
    if (!CreatorLayer::init()) return false;

    auto GDUtils = Loader::get()->getLoadedMod("gdutilsdevs.gdutils");
    if (GDUtils) {
        auto settingVal = GDUtils->getSettingValue<bool>("gauntletDesign");
        if (settingVal) {
            GDUtils->setSettingValue<bool>("gauntletDesign", false);
            log::info("GDUtils' gauntlet designs disabled");
        }
    }

    // Find the gauntlets button and swap its callback to open our standalone layer
    auto gauntletsBtn = static_cast<CCMenuItemSpriteExtra*>(
        this->getChildByIDRecursive("gauntlets-button")
    );
    if (gauntletsBtn) {
        gauntletsBtn->setTarget(this, menu_selector(MyCreatorLayer::onGauntlets));
        log::info("Gauntlets button callback overwritten");
    } else {
        log::warn("Could not find gauntlets-button");
    }

    return true;
}

void MyCreatorLayer::onGauntlets(CCObject* sender) {
    auto scene = BetterGauntletSelectLayer::scene();
    if (scene) {
        CCDirector::get()->pushScene(scene);
    } else {
        CreatorLayer::onGauntlets(sender);
    }
}
