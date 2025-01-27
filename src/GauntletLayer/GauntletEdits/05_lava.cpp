#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editLavaGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Lava", ccc3(63, 9, 9), ccc3(255, 209, 56), ccc3(255, 51, 0));
    GameManager::get()->loadBackground(57);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(57)));

    auto bgParticleNode = CCParticleSystemQuad::create();
    if (bgParticleNode) {
        CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("65a-1a2a0.45a26a90a90a0a0a300a0a0a0a43a0a0a0a300a10a90a0a1a0a0.313726a0a0a0a0.3a0.05a1200a20a90a0a1a0a0.501961a0a0a0a0.1a0.15a0.35a0a0.7a0a0a0a0a0a0a0a0a1a1a0a0a0a32a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
        bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
        bgParticles->setZOrder(-2);
        this->addChild(bgParticles);
    }
}