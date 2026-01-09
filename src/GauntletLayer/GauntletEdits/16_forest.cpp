#include "../GauntletLayer.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

void RedesignedGauntletLayer::editForestGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 26;

    editGauntletSingle("Forest", ccc3(33, 68, 34), ccc3(148, 255, 98), ccc3(61, 165, 20));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // leaves 1
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "50a-1a3a0.45a8a-118a0a143a0a300a0a0a0a0a0a0a56a10a2a0a50a0.25a0.25a0.5a0.35a0a0a1a0.05a5a0a0a53a0.054902a0a0.45098a0a0a0a0.5a0a0a0a0.15a0a0a0a0a0a0a0a0a2a1a0a0a0a146a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // leaves 2
		auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "50a-1a3a0.45a8a-118a0a71a0a300a0a0a0a0a0a0a35a5a2a0a50a0.25a0.25a0.5a0.35a0a0a1a0.05a3a0a0a53a0.054902a0a0.45098a0a0a0a0.5a0a0a0a0.15a0a0a0a0a0a0a0a0a2a1a0a0a0a146a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2 + 45, director->getScreenTop() + 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}