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

    auto progress = static_cast<CCLabelBMFont*>(this->getChildByIDRecursive("gauntlet-progress-label"));
    auto bg = static_cast<CCSprite*>(progress->getParent()->getParent()->getChildByIDRecursive("background"));
    auto name = static_cast<CCLabelBMFont*>(progress->getParent()->getParent()->getChildByIDRecursive("gauntlet-label"));

    auto nameColor = name->getColor();
    m_fields->m_gradient = CCLayerGradient::create(
        {nameColor.r, nameColor.g, nameColor.b, 0},
        {nameColor.r, nameColor.g, nameColor.b, 128}
    );
    m_fields->m_gradient->setVector({1, 0});
    m_fields->m_gradient->setContentSize(bg->getContentSize());
    m_fields->m_gradient->setAnchorPoint({0.5, 0.5});

    auto stencil = NineSlice::create("square04_001.png");
    stencil->setContentSize({155, 333});
    stencil->setScale(0.625);

    auto gradientClip = CCClippingNode::create(stencil);
    gradientClip->setAlphaThreshold(0);
    gradientClip->setContentSize(stencil->getContentSize());
    gradientClip->setID("gradient-clip");
    bg->getParent()->addChild(gradientClip);

    m_fields->m_gradient->setPositionY(-110);
    m_fields->m_gradient->setScaleX(2);
    m_fields->m_gradient->setScaleY(1.2);
    m_fields->m_gradient->setRotation(67);
    m_fields->m_gradient->setID("gradient-sprite");
    gradientClip->addChild(m_fields->m_gradient);

    // I hate robtop sometimes WHY GRANT THE REWARD ON GAUNTLET ENTRY IN GAUNTLETSELECTLAYER??
    if (m_rewardNode) m_rewardNode->setVisible(false);
    bg->setVisible(true);
    progress->setVisible(true);

    if (progress->getString() == std::string("5/5")) {
        progress->setColor({100, 255, 100});

        m_fields->m_gradientParticles = GameToolbox::particleFromString(
            "15a-1a1.75a0a8a90a0a30a15a55a0a0a0a0a0a0a0a5a3a0a60a0.329412a0a0.968627a0a0.337255a0a1a0a2a0a0a67a0a0a0.321569a0a0.00392157a0a1a0a0a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a0a0a0.75a0.5a0a0a0a0a0a0a0a0a0a0a0",
            NULL,
            false
        );
        m_fields->m_gradientParticles->setPosition({0, -110});
        gradientClip->addChild(m_fields->m_gradientParticles);
    }
}