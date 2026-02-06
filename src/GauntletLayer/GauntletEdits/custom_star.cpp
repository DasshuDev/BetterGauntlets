#include "../GauntletLayer.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

void RedesignedGauntletLayer::editStarGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 1;

    editGauntletLayer("Star", ccc3(25, 25, 50), ccc3(255, 255, 100), ccc3(255, 215, 0));
    GameManager::get()->loadBackground(backgroundValue);

    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue))); 

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
    if (enableParticles) {
        // Sparkle particles
        auto starParticles1 = CCParticleSystemQuad::create();
        if (starParticles1) {
            CCParticleSystemQuad* particles = GameToolbox::particleFromString(
                "50a-1a2a0.3a10a90a90a20a0a200a0a0a80a0a0a0a0a80a10a5a30a1a1a1a0a1a0.8a0.2a150a5a0a90a1a1a0.8a0a1a1a0.8a0a0.2a0a0.5a0a0a0a0a0a0a0a0a2a1a0a0a0a20a0a1.5a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            particles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            particles->setZOrder(-97);
            this->addChild(particles);
        }
    }
}