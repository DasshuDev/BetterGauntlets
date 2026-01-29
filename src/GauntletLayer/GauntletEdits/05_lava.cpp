#include "../GauntletLayer.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

void RedesignedGauntletLayer::editLavaGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 57;

    editGauntletLayer("Lava", ccc3(63, 9, 9), ccc3(255, 209, 56), ccc3(255, 51, 0));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // lava
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "65a-1a2a0.45a26a90a90a0a0a300a0a0a0a43a0a0a0a300a10a90a0a1a0a0.313726a0a0a0a0.3a0.05a1200a20a90a0a1a0a0.501961a0a0a0a0.1a0.15a0.35a0a0.7a0a0a0a0a0a0a0a0a1a1a0a0a0a32a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // glow
        auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "60a-1a2a0.45a5a-90a0a107a0a350a0a0a500a0a0a0a0a750a5a90a47a0.811765a0a0.490196a0a0.211765a0a0.5a0a750a0a90a47a0.337255a0a0a0a0a0a0.4a0a0.25a0a0.4a0a0a0a0a0a0a0a0a2a1a0a0a0a26a0a4.5a0a0a0a0a0a0a0a0a0a0a0a0;",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 225));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // sparks
        auto bgParticleNode3 = CCParticleSystemQuad::create();
        if (bgParticleNode3) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "25a-1a2a0.45a6a90a20a250a10a300a0a0a-225a0a0a0a0a25a10a0a43a1a0a0.792157a0a0a0a1a0.05a0a0a0a38a1a0a0.152941a0a0a0a0.5a0.15a0a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // flow 1
        auto bgParticleNode4 = CCParticleSystemQuad::create();
        if (bgParticleNode4) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "65a-1a2a0.45a26a90a90a0a0a0a0a0a-535a43a0a0a0a250a50a180a0a1a0a0.313726a0a0a0a0.25a0.05a150a50a180a0a1a0a0.501961a0a0a0a0.05a0.15a0.35a0a0.7a0a0a0a0a0a0a0a0a1a1a0a0a0a32a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2 - 150, director->getScreenTop() + 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // flow 2
        auto bgParticleNode5 = CCParticleSystemQuad::create();
        if (bgParticleNode5) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "65a-1a2a0.45a26a90a90a0a0a0a0a0a-535a43a0a0a0a150a50a180a0a1a0a0.313726a0a0a0a0.1a0.05a90a50a180a0a1a0a0.501961a0a0a0a0.01a0.05a0.35a0a0.7a0a0a0a0a0a0a0a0a1a1a0a0a0a32a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2 - 195, director->getScreenTop() + 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // flow 3
        auto bgParticleNode6 = CCParticleSystemQuad::create();
        if (bgParticleNode6) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "65a-1a2a0.45a26a90a90a0a0a0a0a0a-535a43a0a0a0a150a50a180a0a1a0a0.313726a0a0a0a0.1a0.05a90a50a180a0a1a0a0.501961a0a0a0a0.01a0.05a0.35a0a0.7a0a0a0a0a0a0a0a0a1a1a0a0a0a32a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2 + 215, director->getScreenTop() + 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}