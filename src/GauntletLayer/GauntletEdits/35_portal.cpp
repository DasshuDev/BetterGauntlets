#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editPortalGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Portal", ccc3(31, 49, 77), ccc3(255, 50, 140), ccc3(109, 34, 86));
    GameManager::get()->loadBackground(57);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(57)));

    auto titlePos = getChildByIDRecursive("title-shadow");

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // swirl
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("20a-1a1a0.3a15a0a0a0a0a0a0a0a0a0a0a0a0a500a250a0a180a0.901961a0a0.556863a0a1a0a1a0a50a30a360a0a1a0a0a0a0a0a1a0a0.5a0a0.75a0a0a0a0a0a0a0a0a2a1a0a0a0a190a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(titlePos->getPosition());
            bgParticles->setScaleY(0.35);
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // swirl 2
		auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("20a-1a1a0.3a15a0a0a0a0a0a0a0a0a0a0a0a0a5000a250a0a180a0.9a0a0.556863a0a1a0a0.2a0a500a30a90a0a1a0a0a0a0a0a0.15a0a0.5a0a0.75a0a0a0a0a0a0a0a0a2a1a0a0a0a190a0a0a0a0a0a0a0a0a0a0a0a0a0a0;", NULL, false);
            bgParticles->setPosition(titlePos->getPosition());
            bgParticles->setScaleY(0.35);
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // glow
        auto bgParticleNode3 = CCParticleSystemQuad::create();
        if (bgParticleNode3) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("20a-1a1a0.3a15a0a0a0a0a0a0a0a0a0a0a0a0a500a250a0a0a0.972549a0a0.556863a0a1a0a0.25a0a300a200a360a0a1a0a0.501961a0a0.501961a0a0.25a0a0.5a0a0.75a0a0a0a0a0a0a0a0a2a1a0a0a0a180a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(titlePos->getPosition());
            bgParticles->setScaleY(0.35);
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}