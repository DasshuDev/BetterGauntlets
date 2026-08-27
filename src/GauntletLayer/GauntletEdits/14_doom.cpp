#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void BetterGauntletLayer::editDoomGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 35;

    editGauntletLayer("Doom", ccc3(48, 36, 33), ccc3(141, 133, 128), ccc3(75, 69, 60));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "13a-1a2a0.45a5a90a20a250a10a300a0a0a-225a0a0a0a0a10a15a0a43a1a0a1a0a1a0a1a0.05a3a0a0a38a1a0a1a0a1a0a0a0a0a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a0a145a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "8a-1a2a0.45a3a90a20a303a40a300a0a0a-260a0a0a0a39a20a5a0a0a1a0a1a0a1a0a0.3a0.15a15a0a0a92a1a0a1a0a1a0a0a0a0a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a0a34a0a1a0.3a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}
