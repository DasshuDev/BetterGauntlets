#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void BetterGauntletLayer::editRandomGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int randomNum = rand() % 60; // <-- Number of backgrounds available + 1
    int backgroundValue = randomNum;

    editGauntletLayer("Random", ccc3(129, 100, 18), ccc3(255, 254, 4), ccc3(253, 159, 1));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
            // left
            auto bgParticleNode1 = CCParticleSystemQuad::create();
            if (bgParticleNode1) {
                CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                    "25a-1a5a0a5a90a90a0a15a0a300a0a0a0a0a0a0a500a10a0a0a1a0a0.870588a0a0a0a0.5a0.15a500a0a0a0a1a0a0.580392a0a0a0a0.2a0.15a0.45a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a32a0a0.6a0a0a0a0a0a0a0a0a0a0a0a0",
                    NULL,
                    false
                );
                bgParticles->setPosition(ccp(director->getScreenLeft() - 20, winSize.height / 2));
                bgParticles->setZOrder(-98);
                this->addChild(bgParticles);
            }
            // right
            auto bgParticleNode2 = CCParticleSystemQuad::create();
            if (bgParticleNode2) {
                CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                    "25a-1a5a0a5a90a90a0a15a0a300a0a0a0a0a0a0a500a10a0a0a1a0a0.870588a0a0a0a0.5a0.15a500a0a0a0a1a0a0.580392a0a0a0a0.2a0.15a0.45a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a32a0a0.6a0a0a0a0a0a0a0a0a0a0a0a0",
                    NULL,
                    false
                );
                bgParticles->setPosition(ccp(director->getScreenRight() + 20, winSize.height / 2));
                bgParticles->setZOrder(-98);
                this->addChild(bgParticles);
            }
            // up
            auto bgParticleNode3 = CCParticleSystemQuad::create();
            if (bgParticleNode3) {
                CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                    "25a-1a5a0a5a90a90a0a15a300a0a0a0a0a0a0a0a500a10a90a0a1a0a0.870588a0a0a0a0.5a0.15a500a0a90a0a1a0a0.580392a0a0a0a0.2a0.15a0.45a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a32a0a0.6a0a0a0a0a0a0a0a0a0a0a0a0",
                    NULL,
                    false
                );
                bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 20));
                bgParticles->setZOrder(-98);
                this->addChild(bgParticles);
            }
            // down
            auto bgParticleNode4 = CCParticleSystemQuad::create();
            if (bgParticleNode4) {
                CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                    "25a-1a5a0a5a90a90a0a15a300a0a0a0a0a0a0a0a500a10a90a0a1a0a0.870588a0a0a0a0.5a0.15a500a0a90a0a1a0a0.580392a0a0a0a0.2a0.15a0.45a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a32a0a0.6a0a0a0a0a0a0a0a0a0a0a0a0",
                    NULL,
                    false
                );
                bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 20));
                bgParticles->setZOrder(-98);
                this->addChild(bgParticles);
            }
    }
}