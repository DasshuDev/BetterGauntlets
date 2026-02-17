#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editLoveGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 2;

    editGauntletLayer("Love", ccc3(61, 0, 38), ccc3(255, 163, 227), ccc3(254, 0, 140));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    // auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	// if (enableParticles) {
    //     // hearts
	// 	auto bgParticleNode1 = CCParticleSystemQuad::create();
    //     if (bgParticleNode1) {
    //         CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
    //             "70a-1a2a0.45a28a90a20a0a0a300a200a0a0a0a0a0a0a5a2a0a43a1a0a1a0a1a0a1a0a10a3a0a38a1a0a0.501961a0a0a0a0.5a0.15a0a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a0a169a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
    //             NULL,
    //             false
    //         );
    //         bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    //         bgParticles->setZOrder(-98);
    //         this->addChild(bgParticles);
    //     }
    // }
}