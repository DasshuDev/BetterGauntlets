#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void BetterGauntletLayer::editSplitGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 13;

    editGauntletLayer("Split", ccc3(31, 32, 41), ccc3(255, 110, 237), ccc3(72, 72, 112));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // grey aurora
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "15a-1a2a0a7a90a90a0a0a0a200a0a0a0a0a0a0a500a0a45a0a0.466667a0a0.376471a0a0.521569a0a1a0a500a0a45a0a0.0392157a0a0.027451a0a0.0431373a0a1a0a1a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a169a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(director->getScreenLeft(), winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // grey tear
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "300a-1a1.2a0.3a100a0a0a0a0a0a200a2000a0a0a0a0a0a100a0a-135a0a0.466667a0a0.376471a0a0.521569a0a0.2a0a50a0a-135a0a0.0392157a0a0.027451a0a0.0431373a0a0.1a0a0a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a211a0a3.66a1.85a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(director->getScreenLeft() - 25, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // pink aurora
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "15a-1a2a0a7a90a90a0a0a0a200a0a0a0a0a0a0a500a0a45a0a0.439216a0a0a0a0.376471a0a1a0a500a0a45a0a0.129412a0a0.0431373a0a0.109804a0a1a0a1a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a169a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(director->getScreenRight(), winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // pink tear
        {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "300a-1a1.2a0.3a100a0a0a0a0a0a200a-2000a0a0a0a0a0a100a0a45a0a0.439216a0a0a0a0.376471a0a0.2a0a50a0a45a0a0.129412a0a0.0431373a0a0.109804a0a0.1a0a0a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a211a0a3.66a1.85a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(director->getScreenRight() + 25, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}
