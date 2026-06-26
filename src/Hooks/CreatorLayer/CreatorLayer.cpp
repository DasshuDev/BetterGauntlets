#include "CreatorLayer.hpp"
#include "../../GauntletSelectLayer/GauntletSelectLayer.hpp"

using namespace geode::prelude;

bool MyCreatorLayer::init() {
    if (!CreatorLayer::init()) return false;

    // Find the gauntlets button and swap its callback to open our standalone layer
    auto gauntletsBtn = static_cast<CCMenuItemSpriteExtra*>(
        this->getChildByIDRecursive("gauntlets-button")
    );
    if (gauntletsBtn) {
        gauntletsBtn->setTarget(this, menu_selector(MyCreatorLayer::onGauntlets));
    } else {
        log::warn("Could not find gauntlets-button");
    }

    return true;
}

void MyCreatorLayer::onGauntlets(CCObject* sender) {
    auto scene = BetterGauntletSelectLayer::scene();
    if (scene) {
        CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f, scene));
    } else {
        CreatorLayer::onGauntlets(sender);
    }
}
