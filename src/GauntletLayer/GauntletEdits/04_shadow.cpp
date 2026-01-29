#include "../GauntletLayer.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

void RedesignedGauntletLayer::editShadowGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 27;

    editGauntletLayer("Shadow", ccc3(46, 19, 45), ccc3(177, 1, 255), ccc3(59, 0, 141));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
		auto bgParticleNode = CCParticleSystemQuad::create();
        if (bgParticleNode) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "150a-1a3a0.45a43a90a90a0a0a300a200a15a0a0a0a0a0a175a0a0a43a0a0a0a0a0a0a0.25a0.05a350a0a0a38a0.286275a0a0a0a0.439216a0a0.1a0.05a0.35a0a1a0a0a0a0a0a0a0a0a1a0a0a0a0a182a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}