#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void BetterGauntletLayer::editCinemaGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 52;

    editGauntletLayer("Cinema", ccc3(29, 29, 29), ccc3(138, 138, 138), ccc3(49, 49, 49));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // left streak
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "15a-1a2a0.9a5a90a20a0a0a0a0a0a0a0a0a0a0a0a0a0a10a0.972549a0a0.92549a0a0.784314a0a0.25a0.1a2500a1000a0a10a1a0a0.811765a0a0a0a0.1a0.03a0.3a0a0.2a0a0a0a0a0a0a0a0a2a1a0a0a0a212a0a0a0a0a0a0a0a0a0a0a2.5a0a0a0;",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(director->getScreenLeft() + 45, director->getScreenBottom() + 45));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // right streak
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "15a-1a2a0.9a5a90a20a0a0a0a0a0a0a0a0a0a0a0a0a-90a10a0.972549a0a0.92549a0a0.784314a0a0.25a0.1a2500a1000a-90a10a1a0a0.811765a0a0a0a0.1a0.03a0.3a0a0.2a0a0a0a0a0a0a0a0a2a1a0a0a0a212a0a0a0a0a0a0a0a0a0a0a2.5a0a0a0;",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(director->getScreenRight() - 45, director->getScreenBottom() + 45));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}
