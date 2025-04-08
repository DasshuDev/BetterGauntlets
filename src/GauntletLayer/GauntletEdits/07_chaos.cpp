#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editChaosGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Chaos", ccc3(33, 12, 6), ccc3(255, 145, 0), ccc3(255, 18, 0));
    GameManager::get()->loadBackground(14);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(14)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
		auto bgParticleNode = CCParticleSystemQuad::create();
    if (bgParticleNode) {
        CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("60a-1a2a0.45a5a-90a0a107a0a350a0a0a-500a0a0a0a0a750a5a0a47a0.811765a0a0.278431a0a0.211765a0a0.5a0a750a0a0a47a0.337255a0a0a0a0a0a0.4a0a0.25a0a0.4a0a0a0a0a0a0a0a0a2a1a0a0a0a192a0a4.5a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
        bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 200));
        bgParticles->setZOrder(-98);
        this->addChild(bgParticles);
        }
    }
}