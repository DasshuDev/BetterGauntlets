#include "../GauntletLayer.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

void RedesignedGauntletLayer::editMagicGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 32;

    editGauntletSingle("Magic", ccc3(14, 16, 51), ccc3(77, 250, 255), ccc3(36, 52, 145));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // sparks
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "35a-1a2a0.45a14a90a20a0a0a300a200a0a0a0a0a0a0a10a5a0a43a0a0a0.537255a1a1a1a1a1a20a5a0a38a0a0a0.027451a0a1a0a0.5a0.15a0a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a0a169a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // bottom
        auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "83a-1a2a0.45a33a90a90a0a0a300a0a0a0a11a0a0a0a350a10a90a0a0.411765a0a0.517647a0a1a0.4a0.3a0.05a500a20a90a0a0a0a0a0a0.34902a0a0.1a0.15a1a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a32a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 20));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // top
        auto bgParticleNode3 = CCParticleSystemQuad::create();
        if (bgParticleNode3) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "83a-1a2a0.45a33a90a90a0a0a300a0a0a0a11a0a0a0a350a10a90a0a0.411765a0a0.517647a0a1a0.4a0.3a0.05a500a20a90a0a0a0a0a0a0.34902a0a0.1a0.15a1a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a32a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 20));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // left
        auto bgParticleNode4 = CCParticleSystemQuad::create();
        if (bgParticleNode4) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "83a-1a2a0.45a33a90a90a0a0a0a200a0a0a11a0a0a0a350a10a90a0a0.411765a0a0.517647a0a1a0.4a0.3a0.05a500a20a90a0a0a0a0a0a0.34902a0a0.1a0.15a1a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a32a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(director->getScreenLeft() - 10, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // right
        auto bgParticleNode5 = CCParticleSystemQuad::create();
        if (bgParticleNode5) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "83a-1a2a0.45a33a90a90a0a0a0a200a0a0a11a0a0a0a350a10a90a0a0.411765a0a0.517647a0a1a0.4a0.3a0.05a500a20a90a0a0a0a0a0a0.34902a0a0.1a0.15a1a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a32a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(director->getScreenRight() + 10, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}