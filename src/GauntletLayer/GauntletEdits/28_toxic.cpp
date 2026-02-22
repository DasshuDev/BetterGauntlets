#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editToxicGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 31;

    editGauntletLayer("Toxic", ccc3(41, 4, 43), ccc3(0, 248, 0), ccc3(9, 87, 8));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
		auto bgParticleNode = CCParticleSystemQuad::create();
        if (bgParticleNode) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "50a-1a2a0.45a8a90a90a0a0a300a0a0a52a0a0a0a0a350a5a0a0a0.211765a0a0.658824a0a0.247059a0a0.15a0.05a350a0a0a0a0a0a0.254902a0a0.0196078a0a0.15a0.05a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 200));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "65a-1a2a0.45a26a90a90a0a0a300a0a0a69a0a0a0a0a20a5a0a0a0.309804a0a0.905882a0a0.360784a0a0.25a0.05a5a2a0a0a0.00392157a0a0.486275a0a0.0392157a0a0.15a0.15a0.39a0a0.5a0a0a0a0a0a0a0a0a2a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}