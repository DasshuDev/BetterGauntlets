#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editNCSGauntlet1() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("NCS I", ccc3(16, 131, 131), ccc3(32, 254, 254), ccc3(9, 71, 71));
    GameManager::get()->loadBackground(21);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(21)));
    m_backgroundSprite->setPositionY(-250);

    auto enableParticles1 = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles1) {
        // music notes
		auto bgParticleNode = CCParticleSystemQuad::create();
        if (bgParticleNode) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("13a-1a5a2a5a90a180a29a0a11a0a0a0a0a0a0a0a5a0a0a24a0a0a0.501961a0a1a0a0.5a0a25a0a0a24a0a0a1a0a1a0a0.5a0a0.5a0a0.7a0a150a10a300a50a0a0a1a2a1a0a0a0a202a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // aurora
        auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("30a-1a1a0.3a30a0a0a0a0a300a200a0a0a0a0a0a0a500a1a0a45a0.0784314a0a0.443137a0a0.52549a0a0.05a0a500a1a0a45a0.0745098a0a0.192157a0a0.219608a0a0.05a0a0.4a0a0.4a0a0a0a0a0a0a0a0a2a1a0a0a0a40a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}