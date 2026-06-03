// GauntletNode.cpp
#include "GauntletNode.hpp"

using namespace geode::prelude;

gd::string RedesignedGauntletNode::frameForType(GauntletType type) {
    switch (type) {
        case GauntletType::Discord: return "island_discord_002.png"_spr;
        default: return GauntletNode::frameForType(type);
    }
}

void RedesignedGauntletNode::generateNode() {
    GauntletNode::generateNode();

    m_fields->m_gradient = CCSprite::createWithSpriteFrameName("GR_pureGradient_001.png"_spr);

    auto progress = static_cast<CCLabelBMFont*>(this->getChildByIDRecursive("gauntlet-progress-label"));
    auto bg = static_cast<CCSprite*>(progress->getParent()->getParent()->getChildByIDRecursive("background"));

    auto stencil = NineSlice::create("square04_001.png");
    stencil->setContentSize({155, 333});
    stencil->setScale(0.625);

    auto gradientClip = CCClippingNode::create(stencil);
    gradientClip->setAlphaThreshold(0);
    gradientClip->setContentSize(stencil->getContentSize());
    gradientClip->setID("gradient-clip");
    bg->getParent()->addChild(gradientClip);

    // Nodes are fully constructed after the base call
    if (m_gauntletInfoNode) m_gauntletInfoNode->setVisible(true);
    if (m_rewardNode) m_rewardNode->setVisible(false);

    if (progress->getString() == std::string("5/5")) {
        progress->setColor({0, 255, 0});
        m_fields->m_gradient->setColor({0, 255, 0});
        m_fields->m_gradient->setOpacity(100);
        m_fields->m_gradient->setPositionY(-35);
        m_fields->m_gradient->setScaleX(1.5);
        m_fields->m_gradient->setScaleY(1.65);
        m_fields->m_gradient->setRotation(67);
        gradientClip->addChild(m_fields->m_gradient);
    }
}

// void RedesignedGauntletNode::onClaimReward() {
//     GauntletNode::onClaimReward();

//     auto progress = static_cast<CCLabelBMFont*>(this->getChildByIDRecursive("gauntlet-progress-label"));
//     auto bg = static_cast<CCSprite*>(progress->getParent()->getParent()->getChildByIDRecursive("background"));
//     auto chestBase = CCSprite::createWithSpriteFrameName("chest_02_04_001.png");
//     auto chestTop = CCSprite::createWithSpriteFrameName("chest_02_04_back_001.png");

//     auto chestBaseShadow = CCSprite::createWithSpriteFrameName("chest_02_04_001.png");
//     chestBaseShadow->setColor({0, 0, 0});
//     chestBaseShadow->setOpacity(50);
//     chestBaseShadow->setPosition({chestBase->getPositionX() + 2, chestBase->getPositionY() - 3});
    
//     auto chestTopShadow = CCSprite::createWithSpriteFrameName("chest_02_04_back_001.png");
//     chestTopShadow->setColor({0, 0, 0});
//     chestTopShadow->setOpacity(50);
//     chestTopShadow->setPosition({chestTop->getPositionX() + 2, chestTop->getPositionY() - 3});

//     if (m_gauntletInfoNode) m_gauntletInfoNode->setVisible(true);
//     if (m_rewardNode)       m_rewardNode->setVisible(false);

//     bg->getParent()->addChild(chestTopShadow);
//     bg->getParent()->addChild(chestBaseShadow);
//     bg->getParent()->addChild(chestTop, 1);
//     bg->getParent()->addChild(chestBase, 1);
// }