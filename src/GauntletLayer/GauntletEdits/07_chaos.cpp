#include "../GauntletLayer.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

void RedesignedGauntletLayer::editChaosGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 14;

    editGauntletLayer("Chaos", ccc3(33, 12, 6), ccc3(255, 145, 0), ccc3(255, 18, 0));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // aura
		auto bgParticleNode = CCParticleSystemQuad::create();
        if (bgParticleNode) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "60a-1a2a0.45a5a-90a0a107a0a350a0a0a-500a0a0a0a0a750a5a0a47a0.811765a0a0.278431a0a0.211765a0a0.5a0a750a0a0a47a0.337255a0a0a0a0a0a0.4a0a0.25a0a0.4a0a0a0a0a0a0a0a0a2a1a0a0a0a192a0a4.5a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 200));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // sparks
        auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "95a-1a2a0.45a53a-90a90a13a0a300a0a-44a-134a0a0a0a87a15a10a0a40a1a0a0.501961a0a0.501961a0a1a0.15a0a0a0a25a1a0a0.25a0a0a0a0.5a0.15a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a0.6a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}