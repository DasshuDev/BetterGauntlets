#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editCrystalGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Crystal", ccc3(28, 0, 7), ccc3(248, 223, 255), ccc3(246, 58, 218));
    GameManager::get()->loadBackground(29);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(29)));

    auto enableParticles = Mod::get()->getSettingValue<bool>("enable-background-particles");
	if (enableParticles) {
        // aura
		auto bgParticleNode1 = CCParticleSystemQuad::create();
        if (bgParticleNode1) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("30a-1a1a0.3a30a0a0a0a0a300a200a0a0a0a0a0a0a500a1a0a45a0.596078a0a0.34902a0a0.658824a0a0.05a0a500a1a0a45a0.560784a0a0.223529a0a0.47451a0a0.05a0a0.4a0a0.4a0a0a0a0a0a0a0a0a2a1a0a0a0a40a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // small shine
		auto bgParticleNode2 = CCParticleSystemQuad::create();
        if (bgParticleNode2) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("200a-1a2a0.45a81a90a20a0a0a300a200a-33a-33a0a0a0a0a7a4a0a43a1a0a1a0a1a0a0.2a0a14a4a0a38a1a0a0.431373a0a0.894118a0a0.1a0.1a0.3a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a0a169a0a10a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // medium shine
		auto bgParticleNode3 = CCParticleSystemQuad::create();
        if (bgParticleNode3) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("60a-1a2a0.45a24a90a20a0a0a300a200a-88a-88a0a0a0a0a25a10a0a43a1a0a1a0a1a0a0.7a1a40a10a0a38a1a0a0.431373a0a0.894118a0a0.5a0.15a0.3a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a0a169a0a10a0a0a0a0a0a0a0a0a0a0a0a0;", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // large shine
		auto bgParticleNode4 = CCParticleSystemQuad::create();
        if (bgParticleNode4) {
            CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("20a-1a2a0.45a8a90a20a0a0a300a200a-262a-262a0a0a0a0a100a5a0a43a1a0a1a0a1a0a1a1a150a5a0a38a1a0a0.431373a0a0.894118a0a0.85a0.15a0.3a0a0.45a0a0a0a0a0a0a0a0a2a1a0a0a0a169a0a10a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
            bgParticles->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            bgParticles->setZOrder(-98);
            this->addChild(bgParticles);
        }
        // // left crystals
		// auto bgParticleNode1 = CCParticleSystemQuad::create();
        // if (bgParticleNode1) {
        //     CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("10a-1a2a0.65a3a-135a35a0a0a300a0a0a0a0a0a0a0a5a1a-45a0a1a0a1a0a1a0a1a0a175a1a-45a0a1a0a0.431373a0a0.898039a0a1a0a0a0a0.35a0a0a0a25a0a0a0a1a2a1a0a0a1a210a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
        //     bgParticles->setPosition(ccp(director->getScreenLeft() - 10, director->getScreenBottom() - 10));
        //     bgParticles->setZOrder(-98);
        //     this->addChild(bgParticles);
        // }
        // // right crystals
		// auto bgParticleNode2 = CCParticleSystemQuad::create();
        // if (bgParticleNode2) {
        //     CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("10a-1a2a0.65a3a-45a35a0a0a300a0a0a0a0a0a0a0a5a1a-45a0a1a0a1a0a1a0a1a0a175a1a-45a0a1a0a0.431373a0a0.898039a0a1a0a0a0a0.35a0a0a0a25a0a0a0a1a2a1a0a0a1a210a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
        //     bgParticles->setPosition(ccp(director->getScreenRight() + 10, director->getScreenBottom() - 10));
        //     bgParticles->setZOrder(-98);
        //     this->addChild(bgParticles);
        // }
    }
}