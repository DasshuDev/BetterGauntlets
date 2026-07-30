#include "CreatorLayer.hpp"
#include "../../GauntletSelectLayer/GauntletSelectLayer.hpp"

using namespace geode::prelude;

bool MyCreatorLayer::init() {
    if (!CreatorLayer::init()) return false;

    // Find the gauntlets button and swap its callback to open our standalone layer
    auto gauntletsBtn = static_cast<CCMenuItemSpriteExtra*>(
        this->getChildByIDRecursive("gauntlets-button")
    );
    auto newSprite = CCSprite::create("GR_gauntletBtn_001.png"_spr);
    newSprite->setScale(0.8);
    if (gauntletsBtn) {
        gauntletsBtn->setSprite(newSprite);
        gauntletsBtn->setTarget(this, menu_selector(MyCreatorLayer::onGauntlets));
    } else {
        log::warn("Could not find gauntlets-button");
    }

    return true;
}

void MyCreatorLayer::onGauntlets(CCObject* sender) {
    auto scene = BetterGauntletSelectLayer::scene();
    if (scene) {
        CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, scene));
    } else {
        CreatorLayer::onGauntlets(sender);
    }
}
