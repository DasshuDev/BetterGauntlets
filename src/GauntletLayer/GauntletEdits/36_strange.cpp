#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void BetterGauntletLayer::editStrangeGauntlet() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
    int backgroundValue = 17;

    editGauntletLayer("Strange", ccc3(42, 52, 3), ccc3(203, 134, 255), ccc3(112, 59, 117));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    // auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	// if (enableParticles) {
    //    {
    //        CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString(
    //            "50a-1a2a0.45a3a90a90a0a0a300a0a0a-403a0a0a0a0a10a5a-45a0a0.619608a0a1a0a0.996078a0a1a0.05a0a0a-45a0a0a0a0.545098a0a1a0a0.5a0.15a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a96a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
    //            NULL,
    //            false
    //        );
    //        bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    //        bgParticles->setZOrder(-98);
    //        this->addChild(bgParticles);
    //    }
    // }
}