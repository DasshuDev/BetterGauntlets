#include "../GauntletLayer.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

void RedesignedGauntletLayer::editGauntletFallback() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 1;

    editGauntletLayer("Unknown", ccc3(70, 70, 70), ccc3(233, 233, 233),{ 168, 168, 168 });
    GameManager::get()->loadBackground(backgroundValue);

    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));
}