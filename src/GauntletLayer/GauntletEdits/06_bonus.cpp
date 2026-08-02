#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void BetterGauntletLayer::editBonusGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 19;

    editGauntletLayer("Bonus", ccc3(30, 54, 85), ccc3(255, 241, 20), ccc3(226, 102, 0));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // stars
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "8a-1a2a0.45a3a90a0a47a0a300a0a0a0a0a0a0a50a20a5a0a24a1a0a0.976471a0a0.698039a0a0.5a0.1a15a0a0a24a1a0a0.898039a0a0.286275a0a0.15a0.1a0.45a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a41a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // rays
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "85a-1a2a0.45a35a90a90a0a0a300a0a0a0a11a0a0a0a200a10a45a0a1a0a0.996078a0a0.596078a0a0.3a0.05a500a20a45a0a0.482353a0a0.447059a0a0a0a0.1a0.15a1a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a169a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom()));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}
