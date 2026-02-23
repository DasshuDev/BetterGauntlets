#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editPowerGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 38;

    editGauntletLayer("Power", ccc3(72, 58, 42), ccc3(231, 203, 182), ccc3(150, 114, 76));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
		auto bgParticleNode = CCParticleSystemQuad::create();
        if (bgParticleNode) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "15a-1a2a0.9a5a90a20a0a0a300a200a0a0a0a0a0a0a0a0a0a43a1a0a0.403922a0a0.403922a0a0.25a0.1a2500a1000a0a38a1a0a0.00784314a0a0a0a0.1a0.03a0.3a0a0.2a0a0a0a0a0a0a0a0a2a1a0a0a0a158a0a0a0a0a0a0a0a0a0a0a2.35a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "15a-1a2a0.9a5a90a20a0a0a0a0a0a0a0a0a0a0a650a100a0a43a1a0a0a0a0a0a0.25a0.1a50a0a0a38a0.364706a0a0.00392157a0a0a0a0.05a0.03a0.3a0a0.2a0a0a0a0a0a0a0a0a2a1a0a0a0a185a0a0a0a0a0a0a0a0a0a0a2.35a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}