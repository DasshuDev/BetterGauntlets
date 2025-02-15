#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editForestGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Forest", ccc3(33, 68, 34), ccc3(148, 255, 98), ccc3(61, 165, 20));
    GameManager::get()->loadBackground(26);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(26)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
		auto bgParticleNode = CCParticleSystemQuad::create();
        if (bgParticleNode) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("50a-1a3a0.45a8a-118a0a143a0a300a0a0a0a0a0a0a56a10a2a0a50a0.0705882a0a1a0a0.137255a0a1a0.05a5a0a0a53a0.054902a0a0.45098a0a0a0a0.5a0a0a0a0.15a0a0a0a0a0a0a0a0a2a1a0a0a0a146a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}