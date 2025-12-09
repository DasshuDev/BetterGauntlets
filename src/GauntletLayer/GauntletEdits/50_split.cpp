#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editSplitGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto backgroundValue = 13;

    editGauntletSingle("Split", ccc3(49, 50, 65), ccc3(255, 110, 237), ccc3(113, 60, 107));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // grey aurora
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("30a-1a1a0.3a30a0a0a0a0a300a200a0a0a0a0a0a0a500a1a0a45a0.466667a0a0.376471a0a0.521569a0a0.1a0a500a1a0a45a0.301961a0a0.298039a0a0.447059a0a0.05a0a0.4a0a0.4a0a0a0a0a0a0a0a0a2a1a0a0a0a40a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(director->getScreenLeft() - 10, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // grey tear
		auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("300a-1a1.2a0.3a100a0a0a0a0a200a0a-75a222a0a0a0a0a200a0a90a0a0.466667a0a0.376471a0a0.521569a0a1a0a5a0a90a0a0.337255a0a0.184314a0a0.431373a0a1a0a0a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a32a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setRotation(-75);
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // pink aurora
		auto bgParticleNode3 = CCParticleSystemQuad::create();
        if (bgParticleNode3) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("30a-1a1a0.3a30a0a0a0a0a300a200a0a0a0a0a0a0a500a1a0a45a1a0a0a0a0.858824a0a0.05a0a500a1a0a45a0.678431a0a0a0a1a0a0.05a0a0.4a0a0.4a0a0a0a0a0a0a0a0a2a1a0a0a0a40a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(director->getScreenRight() + 10, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // pink tear
		auto bgParticleNode4 = CCParticleSystemQuad::create();
        if (bgParticleNode4) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("300a-1a1.2a0.3a100a0a0a0a0a200a0a75a-222a0a0a0a0a200a0a90a0a1a0a0a0a1a0a1a0a5a0a90a0a0.501961a0a0a0a1a0a1a0a0a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a32a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setRotation(-75);
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}