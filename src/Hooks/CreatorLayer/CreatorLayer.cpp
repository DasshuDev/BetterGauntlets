#include "CreatorLayer.hpp"
#include "../../GauntletSelectLayer/GauntletSelectLayer.hpp"

using namespace geode::prelude;

bool BGCreatorLayer::init() {
    if (!CreatorLayer::init()) return false;

    auto gauntletsBtn = static_cast<CCMenuItemSpriteExtra*>(
        this->getChildByIDRecursive("gauntlets-button")
    );

    auto newSprite = CCSprite::create("GR_gauntletBtn_001.png"_spr);
    newSprite->setScale(0.8);

    if (gauntletsBtn) {
        gauntletsBtn->setSprite(newSprite);
        gauntletsBtn->setTarget(this, menu_selector(BGCreatorLayer::onGauntlets));
    } else {
        log::warn("Could not find gauntlets-button");
    }

    return true;
}

void BGCreatorLayer::onGauntlets(CCObject* sender) {
    auto scene = BetterGauntletSelectLayer::scene();
    if (scene) {
        auto node = typeinfo_cast<CCNode*>(sender);
        if (node && (node->getTag() == 20260814 || node->getUserFlag("please-set-from-redash"_spr))) {
            CCDirector::get()->pushScene(CCTransitionFade::create(0.5, scene));
        } else {
            CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, scene));
        }
    } else {
        CreatorLayer::onGauntlets(sender);
    }
}