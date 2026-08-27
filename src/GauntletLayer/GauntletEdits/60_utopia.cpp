#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void BetterGauntletLayer::editUtopiaGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 39;

    editGauntletLayer("Utopia", ccc3(16, 18, 44), ccc3(67, 220, 246), ccc3(140, 0, 254));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // sparks
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "15a-1a2a0.9a5a90a20a0a0a300a200a0a0a0a0a0a0a0a0a0a43a0a0a0.886275a0a1a0a0.25a0.1a2500a1000a0a38a0.788235a0a0.0627451a0a0.921569a0a0.05a0.03a0.3a0a0.2a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a0a0a0a0a0a0a0a0a0a2.35a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // aurora
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "85a-1a2a0.45a35a90a90a0a0a300a0a0a0a11a0a0a0a200a10a0a0a0a0a0.486275a0a1a0a0.3a0.05a500a20a0a0a0.835294a0a0a0a1a0a0.1a0.15a1a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a27a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 5));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // aurora
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "1a-1a5a0a1a0a0a0a0a300a0a0a0a0a0a0a0a400a0a180a0a1a0a0.937255a0a0.619608a0a0.3a0.05a400a0a180a0a1a0a0.886275a0a0a0a0.1a0.15a1a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a31a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() + 100));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}
