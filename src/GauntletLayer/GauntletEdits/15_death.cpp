#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editDeathGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Death", ccc3(22, 22, 22), ccc3(100, 100, 100), ccc3(48, 48, 48));
    GameManager::get()->loadBackground(16);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(16)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
		auto bgParticleNode = CCParticleSystemQuad::create();
        if (bgParticleNode) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("8a-1a2a0.45a3a90a20a250a10a300a0a0a0a0a0a0a188a30a15a0a0a1a0a1a0a1a0a1a0.05a3a0a0a0a1a0a1a0a1a0a0a0a0a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a1a34a0a1a0.3a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}