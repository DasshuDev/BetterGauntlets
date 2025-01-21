#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editAcidGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Acid", ccc3(45, 38, 17), ccc3(208, 255, 0), ccc3(162, 255, 24));
    GameManager::get()->loadBackground(24);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(24)));

    // auto bgParticleNode = CCParticleSystemQuad::create();
    // if (bgParticleNode) {
    //     CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("50a-1a2a0.45a20a90a90a0a0a300a0a-44a140a0a0a0a196a25a15a0a40a1a0a0.75a0a0a0a1a0.05a0a0a0a25a1a0a0.25a0a0a0a0.5a0.15a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a0.6a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
    //     bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
    //     bgParticles->setZOrder(-2);
    //     this->addChild(bgParticles);
    // }
}