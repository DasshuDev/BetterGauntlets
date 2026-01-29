#include "../GauntletLayer.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

void RedesignedGauntletLayer::editForceGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 42;

    editGauntletLayer("Force", ccc3(53, 50, 64), ccc3(130, 112, 193), ccc3(160, 19, 74));
	GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // rings
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "10a-1a2a0.45a4a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a360a0a1a0.2a0a1a0a0.2a0.1a7500a0a0a360a1a0a0a0a0.811765a0a0.05a0a0a0a0.6a0.2a0a0a0a0a0a0a0a2a1a0a0a0a176a0a0a0a0a0a0a0a0a0a0a5a1.5a5a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // aura
        auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "10a-1a2a0.45a4a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a360a0a1a0.2a0a1a0a0.2a0.1a10000a0a0a360a1a0a0a0a0.811765a0a0.05a0a0a0a0.45a0.15a0a0a0a0a0a0a0a2a1a0a0a0a192a0a0a0a0a0a0a0a0a0a0a3.75a1a5a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}