#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void BetterGauntletLayer::editWaterGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 2;

    editGauntletLayer("Water", ccc3(13, 11, 48), ccc3(78, 212, 230), ccc3(18, 80, 151));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // lightrays
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "50a-1a2a0.45a20a0a0a0a50a500a0a0a0a11a0a0a0a750a150a0a0a0.411765a0a0.513726a0a1a0.4a0.3a0.05a500a100a0a0a0a0a0a0a0.34902a0a0.1a0.15a1a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a188a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 30));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // refractions
		auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "30a-1a1a0.3a30a0a0a0a0a300a200a0a0a0a0a0a0a500a1a0a45a0a0a0.470588a0a0.52549a0a0.1a0a500a1a0a45a0a0a0.0980392a0a0.352941a0a0.1a0a0.4a0a0.4a0a0a0a0a0a0a0a0a2a1a0a0a0a40a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}