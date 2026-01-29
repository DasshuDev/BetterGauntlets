#include "../GauntletLayer.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

void RedesignedGauntletLayer::editGauntletFallback() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 1;

    editGauntletLayer("Unnamed", ccc3(40, 125, 225), ccc3(199, 252, 89), ccc3(125, 234, 49));
    GameManager::get()->loadBackground(backgroundValue);

    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));
}