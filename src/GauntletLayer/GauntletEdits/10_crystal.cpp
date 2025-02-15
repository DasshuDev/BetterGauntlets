#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editCrystalGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Crystal", ccc3(51, 2, 31), ccc3(248, 223, 255), ccc3(246, 58, 218));
    GameManager::get()->loadBackground(29);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(29)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // left crystals
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("10a-1a2a0.65a3a-135a35a0a0a300a0a0a0a0a0a0a0a5a1a-45a0a1a0a1a0a1a0a1a0a175a1a-45a0a1a0a0.431373a0a0.898039a0a1a0a0a0a0.35a0a0a0a25a0a0a0a1a2a1a0a0a1a210a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(director->getScreenLeft() - 10, director->getScreenBottom() - 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // right crystals
		auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("10a-1a2a0.65a3a-45a35a0a0a300a0a0a0a0a0a0a0a5a1a-45a0a1a0a1a0a1a0a1a0a175a1a-45a0a1a0a0.431373a0a0.898039a0a1a0a0a0a0.35a0a0a0a25a0a0a0a1a2a1a0a0a1a210a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(director->getScreenRight() + 10, director->getScreenBottom() - 10));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
    }
}