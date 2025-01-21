#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editFireGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Fire", ccc3(63, 29, 9), ccc3(255, 255, 0), ccc3(231, 79, 0));
    GameManager::get()->loadBackground(14);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(14)));

    auto bgParticleNode = CCParticleSystemQuad::create();
    if (bgParticleNode) {
        CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("30a-1a2a0.45a15a90a90a0a0a300a0a-44a140a0a0a0a196a10a3a0a40a1a0a0.75a0a0a0a1a0.05a0a0a0a25a1a0a0.25a0a0a0a0.5a0.15a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a0a0a0.6a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
        bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
        bgParticles->setZOrder(-2);
        this->addChild(bgParticles);
    }
}