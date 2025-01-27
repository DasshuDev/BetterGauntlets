#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editGauntletFallback() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Unnamed", ccc3(40, 125, 225), ccc3(199, 252, 89), ccc3(125, 234, 49));
    GameManager::get()->loadBackground(1);

    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(1)));
}