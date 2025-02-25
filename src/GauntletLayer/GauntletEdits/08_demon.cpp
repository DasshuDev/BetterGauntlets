#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editDemonGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Demon", ccc3(29, 29, 29), ccc3(202, 81, 81), ccc3(99, 3, 3));
    GameManager::get()->loadBackground(38);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(38)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // easy
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("5a-1a2a0.45a2a90a90a0a0a300a200a0a0a0a0a0a0a10a5a0a24a1a0a1a0a1a0a1a0.05a3a0a0a24a1a0a1a0a1a0a0.5a0.15a0.45a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a58a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // medium
		auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("5a-1a2a0.45a2a90a90a0a0a300a200a0a0a0a0a0a0a10a5a0a24a1a0a1a0a1a0a1a0.05a3a0a0a24a1a0a1a0a1a0a0.5a0.15a0.45a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a59a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // hard
		auto bgParticleNode3 = CCParticleSystemQuad::create();
        if (bgParticleNode3) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("5a-1a2a0.45a2a90a90a0a0a300a200a0a0a0a0a0a0a10a5a0a24a1a0a1a0a1a0a1a0.05a3a0a0a24a1a0a1a0a1a0a0.5a0.15a0.45a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a60a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // insane
		auto bgParticleNode4 = CCParticleSystemQuad::create();
        if (bgParticleNode4) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("5a-1a2a0.45a2a90a90a0a0a300a200a0a0a0a0a0a0a10a5a0a24a1a0a1a0a1a0a1a0.05a3a0a0a24a1a0a1a0a1a0a0.5a0.15a0.45a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a61a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // extreme
		auto bgParticleNode5 = CCParticleSystemQuad::create();
        if (bgParticleNode5) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("5a-1a2a0.45a2a90a90a0a0a300a200a0a0a0a0a0a0a10a5a0a24a1a0a1a0a1a0a1a0.05a3a0a0a24a1a0a1a0a1a0a0.5a0.15a0.45a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a62a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}