#include "../GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editIceGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Ice", ccc3(8, 68, 83), ccc3(192, 236, 255), ccc3(0, 174, 255));
    GameManager::get()->loadBackground(36);
    
    auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(36));
    m_backgroundSprite->setTexture(bgTexture);

    auto bgParticleNode = CCParticleSystemQuad::create();
    if (bgParticleNode) {
        CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("50a-1a2a0.45a3a90a90a0a0a300a0a0a-403a0a0a0a0a10a5a-45a0a0.619608a0a1a0a0.996078a0a1a0.05a0a0a-45a0a0a0a0.545098a0a1a0a0.5a0.15a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a96a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
        bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 10));
        bgParticles->setZOrder(-2);
        this->addChild(bgParticles);
    }

    for (int i = 0; i < 5; i++) {
        auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
        if (level) {
            auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
            if (name) {
                name->setColor(ccc3(197, 237, 255));
            }
        }
    }
}