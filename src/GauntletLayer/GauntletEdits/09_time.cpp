#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editTimeGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Time", ccc3(0, 17, 75), ccc3(61, 152, 255), ccc3(0, 20, 61));
    GameManager::get()->loadBackground(13);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(36)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // vortex
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("10a-1a2a0.45a4a0a0a0a0a0a0a0a0a0a0a0a0a1500a0a0a360a0a0a0.2a0a1a0a0.55a0.05a0a0a0a360a1a0a1a0a1a0a0.4a0.15a0.6a0.2a0a0a0a0a0a0a0a0a0a2a1a0a0a0a180a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // lines
		auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("2a-1a2a0.45a1a0a0a0a0a0a0a0a0a0a0a0a0a1250a0a0a360a0a0a0.2a0a1a0a0.7a0.05a0a0a0a360a1a0a1a0a1a0a0.5a0.15a1a0a0a0a0a0a0a0a0a0a0a2a1a0a0a0a186a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // squares
		auto bgParticleNode3 = CCParticleSystemQuad::create();
        if (bgParticleNode3) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("30a-1a1a0.3a30a0a180a29a0a11a0a0a0a0a0a0a0a7a3a0a89a1a0a1a0a1a0a0.5a0.15a0a0a0a84a1a0a1a0a1a0a0.25a0.1a0a0a0a0a300a0a0a0a0a0a1a2a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}