#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editSpikeGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Spike", ccc3(54, 37, 4), ccc3(255, 222, 92), ccc3(177, 116, 43));
    GameManager::get()->loadBackground(15);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(15)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // sand
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("500a-1a2a0a200a-25a0a500a100a0a400a0a0a0a0a111a39a3a0a0a43a0.847059a0a0.729412a0a0.352941a0a1a0a6a0a0a38a0.811765a0a0.611765a0a0.254902a0a0.5a0.15a0a0a0a0a0a0a0a0a0a0a0a2a0a0a0a0a25a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(director->getScreenLeft() - 10, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // dust
		auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("200a-1a2a0a81a-25a0a500a100a0a400a0a0a0a0a111a39a200a0a0a43a0.282353a0a0.215686a0a0.00392157a0a0.11a0a200a0a0a38a0.152941a0a0.0980392a0a0a0a0.1a0.15a0a0a0a0a0a0a0a0a0a0a0a2a1a0a0a0a46a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(director->getScreenLeft() - 10, director->getScreenTop() + 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}