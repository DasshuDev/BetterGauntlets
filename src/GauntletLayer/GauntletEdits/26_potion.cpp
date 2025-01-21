#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editPotionGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Potion", ccc3(58, 30, 52), ccc3(255, 0, 144), ccc3(139, 0, 148));
    GameManager::get()->loadBackground(25);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(25)));

    // auto bgParticleNode = CCParticleSystemQuad::create();
    // if (bgParticleNode) {
    //     CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("50a-1a2a0.45a20a90a90a0a0a300a0a-44a140a0a0a0a196a25a15a0a40a1a0a0.75a0a0a0a1a0.05a0a0a0a25a1a0a0.25a0a0a0a0.5a0.15a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a0.6a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
    //     bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
    //     bgParticles->setZOrder(-2);
    //     this->addChild(bgParticles);
    // }
}