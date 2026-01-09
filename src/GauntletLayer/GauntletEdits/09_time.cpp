#include "../GauntletLayer.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

void RedesignedGauntletLayer::editTimeGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 13;

    editGauntletSingle("Time", ccc3(0, 17, 75), ccc3(61, 152, 255), ccc3(0, 20, 61));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // vortex
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "10a-1a2a0.45a4a0a0a0a0a0a0a0a0a0a0a0a0a1000a0a0a360a0a1a0.2a0a1a0a0.2a0.1a0a0a0a360a1a0a1a0a1a0a0.05a0a0.6a0.2a0a0a0a0a0a0a0a0a0a2a1a0a0a0a190a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // pulses
		auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "2a-1a2a0.45a1a0a0a0a0a0a0a0a0a0a0a0a0a1500a0a0a100a0a0a0.2a0a1a0a0.25a0.05a0a0a0a100a1a0a1a0a1a0a0.1a0.05a1a0a0a0a0a0a0a0a0a0a0a2a1a0a0a0a95a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // swirling lines
		auto bgParticleNode3 = CCParticleSystemQuad::create();
        if (bgParticleNode3) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "30a-1a1a0.3a30a0a180a29a0a11a0a0a0a0a0a0a0a50a0a0a0a1a0a1a0a1a0a0.5a0.15a0a0a0a0a1a0a1a0a1a0a0.25a0.1a0a0a0a0a400a0a0a0a-150a0a1a2a1a0a0a1a32a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // lights
		auto bgParticleNode4 = CCParticleSystemQuad::create();
        if (bgParticleNode4) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "30a-1a1a0.3a30a0a180a29a0a11a0a0a0a0a0a0a0a1000a0a0a0a0a0a1a0a0.937255a0a0.15a0.05a0a0a0a0a0a0a0a0a1a0a0a0a0.4a0a0.4a0a400a0a0a0a0a0a1a2a1a0a0a1a25a0a0a0a0a0a0a0a0a0a0a0a0a0a0;",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-99);
            this->addChild(bgParticles);
        }
    }
}