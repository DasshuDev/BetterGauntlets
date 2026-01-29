#include "../GauntletLayer.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

void RedesignedGauntletLayer::editLoveGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 13;

    editGauntletLayer("Cosmos", ccc3(13, 9, 0), ccc3(244, 255, 78), ccc3(254, 84, 0));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // stars
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "70a-1a2a0.45a28a90a20a0a0a300a200a0a0a0a0a0a0a5a2a0a43a1a0a1a0a1a0a1a0a10a3a0a38a1a0a0.501961a0a0a0a0.5a0.15a0a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a0a169a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // big stars
        auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "10a-1a2a0.45a4a90a20a0a0a300a200a0a0a0a0a0a0a50a0a0a43a1a0a1a0a1a0a1a0a0a0a0a38a1a0a0.501961a0a0a0a0.5a0.15a0.1a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a0a169a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // aurora
        auto bgParticleNode3 = CCParticleSystemQuad::create();
        if (bgParticleNode3) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "30a-1a1a0.3a30a0a0a0a0a300a200a0a0a0a0a0a0a500a1a0a45a0.501961a0a0.501961a0a0a0a0.05a0a500a1a0a45a0.501961a0a0.25098a0a0a0a0.05a0a0.4a0a0.4a0a0a0a0a0a0a0a0a2a1a0a0a0a40a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // clouds
        auto bgParticleNode4 = CCParticleSystemQuad::create();
        if (bgParticleNode4) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "15a-1a2a0.45a6a90a80a0a0a300a200a0a0a0a0a0a0a500a0a0a43a1a0a1a0a0a0a0.05a0a500a0a0a38a1a0a0.501961a0a0a0a0.05a0a0.45a0a0.45a0a400a0a400a0a-4a0a1a2a1a0a0a0a46a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            // bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 10));
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            bgParticles->setScaleY(0.5);
            this->addChild(bgParticles);
        }
        // shooting stars
        auto bgParticleNode5 = CCParticleSystemQuad::create();
        if (bgParticleNode5) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "15a-1a2a0.45a1a180a0a180a0a100a600a0a0a0a0a0a0a10a5a-90a0a1a0a1a0a1a0a1a0a0a0a-90a0a0a0a0a0a1a0a0.5a0.15a0a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a0a31a0a2a1a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            bgParticles->setRotation(-30);
            bgParticles->setScaleX(0.2f);
            bgParticles->setScaleY(5.0f);
            this->addChild(bgParticles);
        }
    }
}