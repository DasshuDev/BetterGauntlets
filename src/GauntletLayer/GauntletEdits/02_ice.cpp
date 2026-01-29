#include "../GauntletLayer.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

void RedesignedGauntletLayer::editIceGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 36;

    editGauntletLayer("Ice", ccc3(8, 68, 83), ccc3(192, 236, 255), ccc3(0, 174, 255));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // Icicles
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "50a-1a2a0.45a3a90a90a0a0a300a0a0a-403a0a0a0a0a15a5a-45a0a0.619608a0a1a0a0.996078a0a1a0.05a5a0a-45a0a0a0a0.545098a0a1a0a0.5a0.15a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a210a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // Snowflakes
        auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "65a-1a5a0a13a90a90a0a0a300a0a0a-1000a0a0a152a298a10a4a0a43a1a0a1a0a1a0a1a0.05a5a0a0a38a0a0a0.482353a0a0.486275a0a0.15a0.15a0a0a0.5a0a0a0a0a0a0a0a0a2a1a0a0a0a43a0a10a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // Powder Snow
        auto bgParticleNode3 = CCParticleSystemQuad::create();
        if (bgParticleNode3) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "60a-1a2a0.45a5a-90a0a107a0a350a0a0a-500a0a0a0a0a750a5a0a47a0.811765a0a0.929412a0a0.945098a0a0.25a0a750a0a0a47a0.305882a0a0.301961a0a0.560784a0a0.05a0a0.25a0a0.4a0a0a0a0a0a0a0a0a2a1a0a0a0a46a0a4.5a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 200));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}