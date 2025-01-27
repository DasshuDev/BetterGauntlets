#include "../GauntletLayer.hpp"
#include <cstdlib>

using namespace geode::prelude;

void RedesignedGauntletLayer::editDiscordGauntlet() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    editGauntletSingle("Discord", ccc3(44, 47, 51), ccc3(88, 101, 212), ccc3(35, 41, 99));
    GameManager::get()->loadBackground(10);
    
    m_backgroundSprite->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(GameManager::get()->getBGTexture(10)));

    // auto bgParticleNode = CCParticleSystemQuad::create();
    // if (bgParticleNode) {
    //     CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("50a-1a2a0.45a20a90a90a0a0a300a0a-44a140a0a0a0a196a25a15a0a40a1a0a0.75a0a0a0a1a0.05a0a0a0a25a1a0a0.25a0a0a0a0.5a0.15a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a0.6a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
    //     bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
    //     bgParticles->setZOrder(-2);
    //     this->addChild(bgParticles);
    // }

    // for (int i = 0; i < 5; i++) {
    //     // Get the level by its recursive ID
    //     auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
    //     if (level) {
    //         for (int c = 0; c < 5; c++) {
    //             // Safely cast the child to CCSprite
    //             auto sprite = static_cast<CCSprite*>(level->getChildByTag(c + 1));
    //             // Get children of the sprite and ensure they exist
    //             CCArray* children = sprite->getChildren();
    //             // Safely cast the first child to CCSprite
    //             auto swapSprite = static_cast<CCSprite*>(children->objectAtIndex(0));
    //             // Check and apply settings
    //             bool updatedSprite = Mod::get()->getSettingValue<bool>("update-discord-sprite");
    //             if (!updatedSprite) {
    //                 log::error("yeah no it died");
    //             } else {
    //                 // Update the sprite's display frame
    //                 static auto discordSprite = CCSprite::create("island_discord_002.png"_spr);
    //                 if (discordSprite) {
    //                     swapSprite->setDisplayFrame(discordSprite->displayFrame());
    //                     log::info("yeaah it worked");
    //                 }
    //             }
    //         }
    //     } else {
    //         log::error("die die die die die die die");
    //     }
    // }
}