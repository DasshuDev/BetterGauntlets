#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editBonusGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Bonus", ccc3(30, 54, 85), ccc3(255, 241, 20), ccc3(226, 102, 0));
    GameManager::get()->loadBackground(19);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(19)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("83a-1a2a0.45a33a90a90a0a0a300a0a0a0a11a0a0a0a100a10a0a0a1a0a0.996078a0a0a0a0.3a0.05a250a20a0a0a0.482353a0a0.447059a0a0a0a0.1a0.15a1a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a32a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}