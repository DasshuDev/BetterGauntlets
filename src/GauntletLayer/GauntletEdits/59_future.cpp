#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void BetterGauntletLayer::editFutureGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 39;

    editGauntletLayer("Future", ccc3(24, 28, 48), ccc3(213, 255, 148), ccc3(0, 255, 0));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // sparks
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "15a-1a2a0.9a5a90a20a0a0a300a200a0a0a0a0a0a0a0a0a0a43a1a0a1a0a1a0a0.25a0.1a2500a1000a0a38a0.203922a0a0.921569a0a0.0627451a0a0.05a0.03a0.3a0a0.2a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a0a0a0a0a0a0a0a0a0a2.35a0a0a0;",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // circle
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "15a-1a2a0.9a5a90a20a0a0a0a0a0a0a0a0a0a0a750a100a0a43a1a0a1a0a1a0a0.25a0.1a50a0a0a38a0.203922a0a0.921569a0a0.0627451a0a0.05a0.03a0.3a0a0.2a0a0a0a0a0a0a0a0a2a1a0a0a0a161a0a0a0a0a0a0a0a0a0a0a2.35a0a0a0;",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}
