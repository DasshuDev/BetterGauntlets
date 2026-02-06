// CustomGauntletPage.cpp
#include "CustomGauntletPage.hpp"

using namespace geode::prelude;

namespace CustomGauntletPage {

    CCMenuItemSpriteExtra* createCustomGauntletButton(int gauntletType, int buttonIndex, cocos2d::SEL_MenuHandler callback, CCObject* target) {

        CCSize winSize = CCDirector::sharedDirector()->getWinSize();

        CCSprite* spriteFrame = CCSprite::createWithSpriteFrameName(CustomGauntlets::getSprite(gauntletType).c_str());
        CCSprite* spriteFrameShadow = CCSprite::createWithSpriteFrameName(CustomGauntlets::getSprite(gauntletType).c_str());
        
        auto container = CCSprite::create();
        container->setContentSize({110, 220});
        container->setAnchorPoint({0.5, 0.5});
        container->setID(fmt::format("gauntlet-button-{}", buttonIndex));
        
        // Background
        auto bg = CCScale9Sprite::create("GJ_squareB_01.png");
        bg->setColor(CustomGauntlets::getBGColor(gauntletType));
        bg->setContentSize({110, 220});
        bg->setPosition(container->getScaledContentSize() / 2);
        bg->setID("background");
        container->addChild(bg, -1);
        
        // Gauntlet Sprite
        spriteFrame->setPosition({55, 130});
        spriteFrame->setID("gauntlet-sprite");  
        container->addChild(spriteFrame);

        // Gauntlet Sprite Shadow
        spriteFrameShadow->setPosition({spriteFrame->getPositionX(), spriteFrame->getPositionY() - 10});
        spriteFrameShadow->setID("gauntlet-sprite-shadow");
        spriteFrameShadow->setColor(ccc3(0, 0, 0));
        spriteFrameShadow->setScaleY(1.2);
        spriteFrameShadow->setOpacity(50);
        spriteFrameShadow->setZOrder(spriteFrame->getZOrder() - 1);
        container->addChild(spriteFrameShadow);
        
        // Name Label
        auto nameLabel = CCLabelBMFont::create(CustomGauntlets::getName(gauntletType).c_str(), "bigFont.fnt");
        nameLabel->setColor(CustomGauntlets::getNameColor(gauntletType));
        nameLabel->setPosition({55, 194});
        nameLabel->setScale(0.7);
        nameLabel->setID("gauntlet-name-label");
        container->addChild(nameLabel, 2);
        
        // Name Shadow
        auto nameShadow = CCLabelBMFont::create(CustomGauntlets::getName(gauntletType).c_str(), "bigFont.fnt");
        nameShadow->setPosition({nameLabel->getPositionX() + 2, nameLabel->getPositionY() - 2});
        nameShadow->setScale(0.7);
        nameShadow->setColor(ccc3(0, 0, 0));
        nameShadow->setOpacity(100);
        nameShadow->setID("gauntlet-name-shadow-label");
        container->addChild(nameShadow, 1);
        
        // "Gauntlet" Label
        auto gauntletLabel = CCLabelBMFont::create("Gauntlet", "bigFont.fnt");
        gauntletLabel->setColor(CustomGauntlets::getNameColor(gauntletType));
        gauntletLabel->setPosition({55, nameLabel->getPositionY() - 15});
        gauntletLabel->setScale(0.45);
        gauntletLabel->setID("gauntlet-label");
        container->addChild(gauntletLabel, 2);
        
        // "Gauntlet" Shadow
        auto gauntletShadow = CCLabelBMFont::create("Gauntlet", "bigFont.fnt");
        gauntletShadow->setPosition({gauntletLabel->getPositionX() + 2, gauntletLabel->getPositionY() - 2});
        gauntletShadow->setScale(0.45);
        gauntletShadow->setColor(ccc3(0, 0, 0));
        gauntletShadow->setOpacity(100);
        gauntletShadow->setID("gauntlet-shadow-label");
        container->addChild(gauntletShadow, 1);
        
        // Level Counter (completed levels / 5)
        // TODO: Replace 0 with actual completed level count from GameStatsManager or saved data
        int completedLevels = 0; // You'll need to track this based on user progress
        std::string levelCounterText = fmt::format("{}/5", completedLevels);
        
        auto levelCounter = CCLabelBMFont::create(levelCounterText.c_str(), "bigFont.fnt");
        levelCounter->setPosition({55, 84});
        levelCounter->setScale(0.4);
        levelCounter->setID("level-counter-label");
        container->addChild(levelCounter, 2);
        
        // Level Counter Shadow
        auto levelCounterShadow = CCLabelBMFont::create(levelCounterText.c_str(), "bigFont.fnt");
        levelCounterShadow->setPosition({levelCounter->getPositionX() + 2, levelCounter->getPositionY() - 2});
        levelCounterShadow->setScale(0.4);
        levelCounterShadow->setColor(ccc3(0, 0, 0));
        levelCounterShadow->setOpacity(100);
        levelCounterShadow->setID("level-counter-shadow-label");
        container->addChild(levelCounterShadow, 1);
        
        // Reward Node (chest sprite and reward label)
        // You can customize this section as needed
        auto rewardNode = CCNode::create();
        rewardNode->setPosition({55, 20});
        rewardNode->setID("reward-node");
        container->addChild(rewardNode);
        
        // Chest Sprite
        auto chestSprite = CCSprite::createWithSpriteFrameName("chest_01_02_001.png");
        chestSprite->setScale(0.4);
        chestSprite->setPosition({0, 23});
        chestSprite->setID("chest-sprite");
        rewardNode->addChild(chestSprite, 2);
        
        // Chest Shadow
        auto chestShadow = CCSprite::createWithSpriteFrameName("chest_01_02_001.png");
        chestShadow->setScale(0.4);
        chestShadow->setPosition({chestSprite->getPositionX() + 2, chestSprite->getPositionY() - 2});
        chestShadow->setColor(ccc3(0, 0, 0));
        chestShadow->setOpacity(100);
        chestShadow->setID("chest-shadow-sprite");
        rewardNode->addChild(chestShadow, 1);
        
        // Reward Label (you can customize this text)
        auto rewardLabel = CCLabelBMFont::create("Custom", "goldFont.fnt");
        rewardLabel->setPosition({0, 9.5});
        rewardLabel->setScale(0.5);
        rewardLabel->setID("reward-label");
        rewardNode->addChild(rewardLabel, 2);
        
        // Reward Label Shadow
        auto rewardShadow = CCLabelBMFont::create("Custom", "goldFont.fnt");
        rewardShadow->setPosition({rewardLabel->getPositionX() + 2, rewardLabel->getPositionY() - 2});
        rewardShadow->setScale(0.5);
        rewardShadow->setColor(ccc3(0, 0, 0));
        rewardShadow->setOpacity(100);
        rewardShadow->setID("reward-shadow-label");
        rewardNode->addChild(rewardShadow, 1);
        
        // Create the clickable button
        auto btn = CCMenuItemSpriteExtra::create(
            container,
            target,
            callback
        );
        btn->setTag(gauntletType);
        btn->setID("custom-gauntlet-button");
        btn->setPosition(winSize / 2);
        btn->m_scaleMultiplier = 1.05;
        
        return btn;
    }

    CCMenu* addCustomGauntletsPage(BoomScrollLayer* scrollLayer, cocos2d::SEL_MenuHandler callback, CCObject* target, CCSize winSize) {

        // Create the page
        CCMenu* customGauntletsPage = CCMenu::create();
        customGauntletsPage->setPosition(winSize / 2);
        customGauntletsPage->setContentSize({winSize.width, 234});
        customGauntletsPage->setID(fmt::format("gauntlet-page-{}", scrollLayer->getTotalPages() + 1));
        
        // Create the gauntlet menu
        CCMenu* customGauntletMenu = CCMenu::create();
        customGauntletMenu->setPosition(0, -11); // Match regular gauntlet menu position
        customGauntletMenu->setID("gauntlet-menu");
        
        // Add Star Gauntlet button
        auto starBtn = createCustomGauntletButton(
            CustomGauntlets::Star, 
            1, 
            callback, 
            target
        );
        customGauntletMenu->addChild(starBtn);
        
        customGauntletsPage->addChild(customGauntletMenu);
        
        // Add the page to the scroll layer
        int insertPos = scrollLayer->getTotalPages();
        scrollLayer->addPage(customGauntletsPage, insertPos);
        scrollLayer->updatePages();
        
        return customGauntletsPage;
    }

    void positionCustomGauntletButtons(CCMenu* customPage, CCSize winSize) {
        auto customPageMenu = customPage->getChildByID("gauntlet-menu");
        if (!customPageMenu) return;
        
        std::vector<CCNode*> customBtns;
        for (int b = 0; b < 3; b++) {
            auto btnWrapper = static_cast<CCMenuItemSpriteExtra*>(customPageMenu->getChildByIDRecursive(fmt::format("gauntlet-button-{}", b + 1)));
            if (btnWrapper) {
                customBtns.push_back(btnWrapper);
            }
        }
    }
}