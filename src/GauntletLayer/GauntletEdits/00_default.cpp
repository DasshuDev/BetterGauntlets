#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editGauntletFallback() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto backgroundValue = 1;

    editGauntletSingle("Unnamed", ccc3(40, 125, 225), ccc3(199, 252, 89), ccc3(125, 234, 49));
    GameManager::get()->loadBackground(backgroundValue);

    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));
}