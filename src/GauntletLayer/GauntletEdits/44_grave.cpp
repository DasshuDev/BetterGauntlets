#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editGraveGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Grave", ccc3(23, 32, 31), ccc3(176, 195, 207), ccc3(80, 66, 87));
    GameManager::get()->loadBackground(16);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(16)));

    // auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	// if (enableParticles) {
	// 	auto bgParticleNode = CCParticleSystemQuad::create();
    //     if (bgParticleNode) {
    //         CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("50a-1a2a0.45a3a90a90a0a0a300a0a0a-403a0a0a0a0a10a5a-45a0a0.619608a0a1a0a0.996078a0a1a0.05a0a0a-45a0a0a0a0.545098a0a1a0a0.5a0.15a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a96a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
    //         bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 10));
    //         bgParticles->setZOrder(-98);
    //         this->addChild(bgParticles);
    //     }
    // }
}