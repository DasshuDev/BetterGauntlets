#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editDemonGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto backgroundValue = 38;

    editGauntletSingle("Demon", ccc3(29, 29, 29), ccc3(202, 81, 81), ccc3(99, 3, 3));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // easy
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("5a-1a2a0.45a2a90a90a0a0a300a200a0a0a0a0a0a0a15a5a0a24a1a0a1a0a1a0a1a0.05a10a0a0a24a1a0a1a0a1a0a0.5a0.15a0.45a0a0.25a0a0a0a0a0a0a0a0a2a0a0a0a0a58a0a0a0a0a0a0a0a0a0a0a0a0a0a0;", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // medium
		auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("5a-1a2a0.45a2a90a90a0a0a300a200a0a0a0a0a0a0a15a5a0a24a1a0a1a0a1a0a1a0.05a10a0a0a24a1a0a1a0a1a0a0.5a0.15a0.45a0a0.25a0a0a0a0a0a0a0a0a2a0a0a0a0a59a0a0a0a0a0a0a0a0a0a0a0a0a0a0;", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // hard
		auto bgParticleNode3 = CCParticleSystemQuad::create();
        if (bgParticleNode3) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("5a-1a2a0.45a2a90a90a0a0a300a200a0a0a0a0a0a0a15a5a0a24a1a0a1a0a1a0a1a0.05a10a0a0a24a1a0a1a0a1a0a0.5a0.15a0.45a0a0.25a0a0a0a0a0a0a0a0a2a0a0a0a0a60a0a0a0a0a0a0a0a0a0a0a0a0a0a0;", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // insane
		auto bgParticleNode4 = CCParticleSystemQuad::create();
        if (bgParticleNode4) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("5a-1a2a0.45a2a90a90a0a0a300a200a0a0a0a0a0a0a15a5a0a24a1a0a1a0a1a0a1a0.05a10a0a0a24a1a0a1a0a1a0a0.5a0.15a0.45a0a0.25a0a0a0a0a0a0a0a0a2a0a0a0a0a61a0a0a0a0a0a0a0a0a0a0a0a0a0a0;", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // extreme
		auto bgParticleNode5 = CCParticleSystemQuad::create();
        if (bgParticleNode5) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("5a-1a2a0.45a2a90a90a0a0a300a200a0a0a0a0a0a0a15a5a0a24a1a0a1a0a1a0a1a0.05a10a0a0a24a1a0a1a0a1a0a0.5a0.15a0.45a0a0.25a0a0a0a0a0a0a0a0a2a0a0a0a0a62a0a0a0a0a0a0a0a0a0a0a0a0a0a0;", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // ashes
		auto bgParticleNode6 = CCParticleSystemQuad::create();
        if (bgParticleNode6) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("95a-1a2a0.45a53a90a90a30a0a300a0a-44a126a0a0a0a120a15a10a0a40a1a0a1a0a0.501961a0a1a0.15a0a0a0a25a1a0a0.25a0a0a0a0.5a0.15a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a0.6a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
            bgParticles->setZOrder(-97);
            this->addChild(bgParticles);
        }
        // aura
        auto bgParticleNode7 = CCParticleSystemQuad::create();
        if (bgParticleNode7) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("60a-1a2a0.45a5a-90a0a107a0a350a0a0a500a0a0a0a0a750a5a0a47a0.811765a0a0.490196a0a0.211765a0a0.5a0a750a0a0a47a0.337255a0a0a0a0a0a0.4a0a0.25a0a0.4a0a0a0a0a0a0a0a0a2a1a0a0a0a26a0a4.5a0a0a0a0a0a0a0a0a0a0a0a0;", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 225));
            bgParticles->setZOrder(-97);
            this->addChild(bgParticles);
        }
    }
}