#include "../GauntletLayer.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

void RedesignedGauntletLayer::editMonsterGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 38;

    editGauntletLayer("Monster", ccc3(23, 58, 28), ccc3(65, 214, 35), ccc3(8, 110, 13));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
		CCParticleSystemQuad* particle1 = CCParticleSystemQuad::create();
        if (particle1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
                "50a-1a2a0.45a10a90a20a0a0a300a200a0a0a0a0a0a0a10a5a0a43a0a0a0.537255a1a1a1a1a1a20a5a0a38a0a0a0.027451a0a1a0a0.5a0.15a0a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a0a169a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
                NULL,
                false
            );
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}