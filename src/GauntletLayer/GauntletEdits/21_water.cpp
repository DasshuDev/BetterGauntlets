#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editWaterGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Water", ccc3(13, 11, 48), ccc3(78, 212, 230), ccc3(18, 80, 151));
    GameManager::get()->loadBackground(2);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(2)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // lightrays
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("85a-1a2a0.45a35a45a180a0a0a350a0a0a0a11a0a0a0a200a10a45a0a0.270588a0a0.662745a0a0.658824a0a0.3a0.05a1000a20a45a0a0a0a0.0666667a0a0.482353a0a0.1a0.15a1a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a31a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // bubbles
		auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("30a-1a5a0.3a3a90a0a124a0a300a0a0a0a0a14a0a0a10a5a0a0a0a0a0.901961a0a1a0a1a0a0a0a0a0a1a0a1a0a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a2a1a0a0a0a165a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}