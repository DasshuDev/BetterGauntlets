#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editFireGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Fire", ccc3(63, 29, 9), ccc3(255, 255, 0), ccc3(231, 79, 0));
    GameManager::get()->loadBackground(14);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(14)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // ashes
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("130a-1a2a0.45a53a90a90a0a0a300a0a-44a126a0a0a0a120a35a10a0a40a1a0a0.87451a0a0.501961a0a1a0.05a0a0a0a25a1a0a0.25a0a0a0a0.5a0.15a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a0.6a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // smoke
        auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("65a-1a2a0.45a26a107a90a45a0a300a0a0a40a0a0a0a0a100a5a0a43a0a0a0a0a0a0a0.1a0.05a200a10a0a38a0a0a0a0a0a0a0a0.15a0.39a0a0.5a0a0a0a0a0a0a0a0a2a0a0a0a0a46a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }

    auto gauntletInfo = ("This is info for the Fire Gauntlet!");
}