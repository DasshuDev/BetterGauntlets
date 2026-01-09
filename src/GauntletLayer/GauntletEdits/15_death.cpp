#include "../GauntletLayer.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

void RedesignedGauntletLayer::editDeathGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 16;

    editGauntletSingle("Death", ccc3(22, 22, 22), ccc3(100, 100, 100), ccc3(48, 48, 48));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // ghouls
		auto bgParticleNode = CCParticleSystemQuad::create();
        if (bgParticleNode) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "8a-1a2a0.45a3a90a20a155a40a300a0a0a0a0a0a0a39a30a15a0a0a1a0a1a0a1a0a0.75a0.15a3a0a0a0a1a0a1a0a1a0a0a0a0a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a1a40a0a1a0.3a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 20));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // skulls
        auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "8a-1a2a0.45a3a90a20a155a40a300a0a0a0a0a0a0a39a30a15a0a0a1a0a1a0a1a0a0.3a0.15a3a0a0a0a1a0a1a0a1a0a0a0a0a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a1a34a0a1a0.3a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 20));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // aura 1
        auto bgParticleNode3 = CCParticleSystemQuad::create();
        if (bgParticleNode3) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "60a-1a2a0.45a5a-90a0a107a77a350a0a0a500a0a0a0a0a750a5a90a47a0.243137a0a0a0a0.00392157a0a0.5a0a750a0a90a47a0.164706a0a0a0a0a0a0.4a0a0.25a0a0.4a0a0a0a0a0a0a0a0a2a1a0a0a0a192a0a4.5a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 90));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // aura 2
        auto bgParticleNode4 = CCParticleSystemQuad::create();
        if (bgParticleNode4) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "60a-1a2a0.45a5a-90a0a107a77a350a0a0a500a0a0a0a0a750a5a90a47a0.243137a0a0a0a0.00392157a0a0.5a0a750a0a90a47a0.164706a0a0a0a0a0a0.4a0a0.25a0a0.4a0a0a0a0a0a0a0a0a2a1a0a0a0a26a0a4.5a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 175));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}