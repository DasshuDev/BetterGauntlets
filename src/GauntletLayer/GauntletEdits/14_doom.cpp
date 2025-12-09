#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editDoomGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto backgroundValue = 35;

    editGauntletSingle("Doom", ccc3(48, 36, 33), ccc3(141, 133, 128), ccc3(75, 69, 60));
    GameManager::get()->loadBackground(backgroundValue);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(backgroundValue)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
		auto bgParticleNode = CCParticleSystemQuad::create();
        if (bgParticleNode) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("13a-1a2a0.45a5a90a20a250a10a300a0a0a-225a0a0a0a0a10a15a0a43a1a0a1a0a1a0a1a0.05a3a0a0a38a1a0a1a0a1a0a0a0a0a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a0a145a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}