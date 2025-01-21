#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editDemonGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Demon", ccc3(29, 29, 29), ccc3(202, 81, 81), ccc3(99, 3, 3));
    GameManager::get()->loadBackground(38);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(38)));

    // auto bgParticleNode = CCParticleSystemQuad::create();
    // if (bgParticleNode) {
    //     CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("50a-1a2a0.45a3a90a90a0a0a300a0a0a-403a0a0a0a0a10a5a-45a0a0.619608a0a1a0a0.996078a0a1a0.05a0a0a-45a0a0a0a0.545098a0a1a0a0.5a0.15a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a96a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
    //     bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 10));
    //     bgParticles->setZOrder(-2);
    //     this->addChild(bgParticles);
    // }
}