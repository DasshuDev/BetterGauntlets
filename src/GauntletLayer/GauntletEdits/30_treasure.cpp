#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editTreasureGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 42;

    editGauntletLayer("Treasure", ccc3(29, 39, 33), ccc3(255, 255, 155), ccc3(255, 140, 73));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // shine
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "8a-1a2a0a4a90a90a0a0a0a0a0a0a11a0a0a0a200a100a0a0a1a0a0.996078a0a0.596078a0a0.15a0.05a900a200a0a75a0.482353a0a0.447059a0a0a0a0.05a0a1a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a42a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 20));
            bgParticles->setScaleX(2.0f);
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // aurora
        auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "85a-1a2a0.45a35a90a90a0a0a300a0a0a0a11a0a0a0a200a100a45a0a1a0a0.996078a0a0.596078a0a0.15a0.05a900a200a45a0a0.482353a0a0.447059a0a0a0a0.05a0a1a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a169a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 20));
            bgParticles->setContentSize({2.0, 1.0});
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}