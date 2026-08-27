#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void BetterGauntletLayer::editDiscordGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 10;

    editGauntletLayer("Discord", ccc3(31, 31, 31), ccc3(88, 101, 212), ccc3(35, 41, 99));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "3a-1a2a0.3a1a90a0a30a0a350a100a0a0a0a0a0a0a0a15a0a0a1a0a0a0a0a0a1a0a250a0a0a0a1a0a0a0a0a0a0a0a0a0a1a0a0a0a0a0a0a0a0a2a0a0a0a0a2a0a0a0a0a0a0a0a0a0a0a7.5a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}