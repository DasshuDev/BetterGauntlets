#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editChaosGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Chaos", ccc3(33, 12, 6), ccc3(255, 145, 0), ccc3(255, 18, 0));
    GameManager::get()->loadBackground(14);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(14)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
		auto bgParticleNode = CCParticleSystemQuad::create();
    if (bgParticleNode) {
        CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("50a-1a2a0.45a3a90a90a0a0a300a0a0a-403a0a0a0a0a30a5a-45a0a1a0a0.513726a0a0.113725a0a1a0.05a10a0a-45a0a1a0a0.227451a0a0a0a0.5a0.15a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a98a0a4.2a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
        bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 15));
        bgParticles->setZOrder(-98);
        this->addChild(bgParticles);
        }
    }
}