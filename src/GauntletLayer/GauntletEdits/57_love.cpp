#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editLoveGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 2;

    editGauntletLayer("Love", ccc3(61, 0, 38), ccc3(255, 163, 227), ccc3(254, 0, 140));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // hearts
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "25a-1a2a0.45a10a90a90a30a0a300a0a-44a126a0a0a0a120a15a10a0a40a1a0a0.501961a0a0.882353a0a1a0.15a0a0a0a25a1a0a0a0a0.34902a0a0.5a0.15a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a21a0a0.6a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 20));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // sparkles
        auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "40a-1a2a0.45a16a-90a0a107a0a350a0a0a500a0a0a0a0a450a5a0a47a0.811765a0a0.211765a0a0.443137a0a0.5a0a250a0a0a47a0.337255a0a0a0a0.0745098a0a0.4a0a0.25a0a0.4a0a0a0a0a0a0a0a0a2a1a0a0a0a25a0a4.5a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 100));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // aura 1
        auto bgParticleNode3 = CCParticleSystemQuad::create();
        if (bgParticleNode3) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "60a-1a2a0.45a24a90a20a0a0a300a200a-33a-33a0a0a0a600a7a4a0a43a1a0a0.647059a0a0.792157a0a0.25a0.1a14a4a0a38a0.921569a0a0.0627451a0a0.411765a0a0.1a0.02a0.3a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a10a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // aura 2
        auto bgParticleNode4 = CCParticleSystemQuad::create();
        if (bgParticleNode4) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "30a-1a2a0.45a12a90a20a0a0a300a200a-88a-88a0a0a0a295a25a10a0a43a1a0a0.647059a0a0.792157a0a0.25a0.1a40a10a0a38a0.921569a0a0.0627451a0a0.411765a0a0.1a0.02a0.3a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a10a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // aura 3
        auto bgParticleNode5 = CCParticleSystemQuad::create();
        if (bgParticleNode5) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "10a-1a3a0a3a90a20a0a0a300a200a-262a-262a0a0a0a440a100a5a0a43a1a0a0.647059a0a0.792157a0a0.25a0.1a150a5a0a38a0.921569a0a0.0627451a0a0.411765a0a0.1a0.02a0.75a0a0.75a0a0a0a0a0a0a0a0a2a1a0a0a0a158a0a10a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}