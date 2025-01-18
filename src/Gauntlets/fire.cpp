#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/GameToolbox.hpp>
#include "fmt/format.h"
#include "fire.hpp"
#include "editGauntletSingle.hpp"

using namespace cocos2d;
using namespace geode::prelude;

void editFireGauntlet(CCNode* parent) {
    if (!parent) return;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    GauntletType::Fire; {
        editGauntletSingle::editGauntletSingle("Fire", ccc3(63, 29, 9), ccc3(255, 255, 0), ccc3(231, 79, 0));
        GameManager::get()->loadBackground(14);

        auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_14_001.png");
        auto bgSprite = static_cast<cocos2d::CCSprite*>(parent->getChildByID("background"));
        if (bgSprite) {
            bgSprite->setTexture(bgTexture);
        }

        auto bgParticleNode = CCParticleSystemQuad::create();
        if (bgParticleNode) {
            auto bgParticles = GameToolbox::particleFromString(
                "50a-1a2a0.45a20a90a90a0a0a300a0a-44a140a0a0a0a196a25a15a0a40a1a0a0.75a0a0a0a1a0.05a0a0a0a25a1a0a0.25a0a0a0a0.5a0.15a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a0.6a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
            bgParticles->setZOrder(-2);
            parent->addChild(bgParticles);
        }
    }

}
