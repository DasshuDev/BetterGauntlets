#include "../GauntletLayer.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

void RedesignedGauntletLayer::editChristmasGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 29;

    editGauntletSingle("Christmas", ccc3(19, 43, 68), ccc3(231, 255, 254), ccc3(86, 202, 255));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
		auto bgParticleNode = CCParticleSystemQuad::create();
        if (bgParticleNode) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "200a-1a7.5a0a16a-115a0a40a30a300a0a0a-161a0a0a0a40a10a5a0a173a1a0a1a0a1a0a0.5a0.25a10a5a0a192a1a0a1a0a1a0a0.25a0a0a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a43a0a2.8a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}