#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editShadowGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Shadow", ccc3(46, 19, 45), ccc3(177, 1, 255), ccc3(59, 0, 141));
    GameManager::get()->loadBackground(27);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(27)));

    auto bgParticleNode = CCParticleSystemQuad::create();
    if (bgParticleNode) {
        CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("65a-1a2a0.45a26a90a90a0a0a300a0a15a0a0a0a0a0a75a10a0a43a0a0a0a0a0a0a0.1a0.05a125a20a0a38a0.129412a0a0a0a0.192157a0a0a0.15a0.35a0a0.7a0a0a0a0a0a0a0a0a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
        bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
        bgParticles->setZOrder(-2);
        this->addChild(bgParticles);
    }
}