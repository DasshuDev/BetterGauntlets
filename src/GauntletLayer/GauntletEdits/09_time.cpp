#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void BetterGauntletLayer::editTimeGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 13;

    editGauntletLayer("Time", ccc3(0, 17, 75), ccc3(61, 152, 255), ccc3(0, 20, 61));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // vortex
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "10a-1a2a0.45a4a0a0a0a0a0a0a0a0a0a0a0a0a1000a0a0a0a0a1a0.2a0a1a0a0.2a0.1a0a0a720a0a1a0a0.75a0.25a1a0a0.05a0a0.6a0.2a0a0a0a0a0a0a0a0a0a2a1a0a0a0a190a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // pulses
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "2a-1a2a0.45a1a0a0a0a0a0a0a0a0a0a0a0a0a1500a0a0a100a0a0a0.2a0a1a0a0.25a0.05a0a0a0a100a1a0a1a0a1a0a0.1a0.05a1a0a0a0a0a0a0a0a0a0a0a2a1a0a0a0a43a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // swirling lines
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "30a-1a1a0.3a30a0a180a29a0a11a0a0a0a0a0a0a0a50a0a0a0a1a0a1a0a1a0a0.5a0.15a0a0a0a0a1a0a1a0a1a0a0.25a0.1a0a0a0a0a400a0a0a0a-150a0a1a2a1a0a0a1a32a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // lights
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "30a-1a1a0.3a30a0a180a29a0a11a0a0a0a0a0a0a0a1000a0a0a0a0a0a1a0a0.937255a0a0.15a0.05a0a0a0a0a0a0a0a0a1a0a0a0a0.4a0a0.4a0a400a0a0a0a0a0a1a2a1a0a0a1a25a0a0a0a0a0a0a0a0a0a0a0a0a0a0;",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-99);
            this->addChild(bgParticles);
        }
    }
}
