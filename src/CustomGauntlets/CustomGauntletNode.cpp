#include "CustomGauntletNode.hpp"
#include "../Data/CustomGauntletManager.hpp"

using namespace geode::prelude;

CustomGauntletNode* CustomGauntletNode::create(
    CustomGauntletData const& data,
    GauntletTapCallback callback
) {
    auto ret = new CustomGauntletNode();
    if (ret && ret->init(data, callback)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CustomGauntletNode::init(
    CustomGauntletData const& data,
    GauntletTapCallback callback
) {
    m_data     = data;
    m_callback = callback;

    auto sprite = CCNode::create();
    sprite->setContentSize({110, 234});
    sprite->setAnchorPoint({0.5, 0.5});

    auto node = NineSlice::create("GJ_squareB_01.png");
    node->setContentSize({110, 220});
    node->setColor(data.nodeColor);
    node->setPosition({sprite->getContentWidth() / 2, sprite->getContentHeight() / 2});
    node->setAnchorPoint({0.5, 0.5});
    sprite->addChild(node);

    auto ph = CCSprite::create("GR_unknownGauntlet_001.png"_spr);
    ph->setID("icon-placeholder");
    ph->setPosition({sprite->getContentWidth() / 2, sprite->getContentHeight() / 2 + 10});
    sprite->addChild(ph, 10);

    auto phs = CCSprite::create("GR_unknownGauntlet_001.png"_spr);
    phs->setID("icon-placeholder-shadow");
    phs->setColor({0, 0, 0});
    phs->setOpacity(50);
    phs->setScaleY(1.2f);
    phs->setPosition({ph->getPositionX(), ph->getPositionY() - 10.f});
    sprite->addChild(phs);

    // Name
    auto nameLabel = CCLabelBMFont::create(data.name.c_str(), "bigFont.fnt");
    nameLabel->setID("gauntlet-name"_spr);
    nameLabel->limitLabelWidth(80, 0.7, 0.00001);
    nameLabel->setColor(data.nameColor);
    nameLabel->setPosition({node->getContentWidth() / 2, node->getContentHeight() - 19});
    sprite->addChild(nameLabel, 2);

    auto nameShadow = CCLabelBMFont::create(data.name.c_str(), "bigFont.fnt");
    nameShadow->setID("gauntlet-shadow-label"_spr);
    nameShadow->setScale(nameLabel->getScale());
    nameShadow->setColor({0, 0, 0});
    nameShadow->setOpacity(50);
    nameShadow->setPosition({nameLabel->getPositionX() + 2, nameLabel->getPositionY() - 2});
    sprite->addChild(nameShadow, 1);

    auto gauntletLabel = CCLabelBMFont::create("Gauntlet", "bigFont.fnt");
    gauntletLabel->setID("gauntlet-label"_spr);
    gauntletLabel->setScale(0.45);
    gauntletLabel->setColor(data.nameColor);
    gauntletLabel->setPosition({node->getContentWidth() / 2, nameLabel->getPositionY() - 15});
    sprite->addChild(gauntletLabel, 2);

    auto gauntletShadow = CCLabelBMFont::create("Gauntlet", "bigFont.fnt");
    gauntletShadow->setID("gauntlet-shadow"_spr);
    gauntletShadow->setScale(gauntletLabel->getScale());
    gauntletShadow->setColor({0, 0, 0});
    gauntletShadow->setOpacity(50);
    gauntletShadow->setPosition({gauntletLabel->getPositionX() + 2, gauntletLabel->getPositionY() - 2});
    sprite->addChild(gauntletShadow, 1);

    m_rewardNode = CCNode::create();
    m_rewardNode->setPosition({node->getContentWidth() / 2, node->getContentHeight() / 2 - 60});
    sprite->addChild(m_rewardNode, 1);

    m_chestSpr = CCSprite::createWithSpriteFrameName("GR_chest01_01_001.png"_spr);
    m_chestSprShadow = CCSprite::createWithSpriteFrameName("GR_chest01_01_001.png"_spr);
    if (m_chestSpr && m_chestSprShadow) {
        m_chestSpr->setScale(0.3);
        m_chestSprShadow->setPosition(ccp(m_chestSpr->getPositionX() + 2, m_chestSpr->getPositionY() - 2));
        m_chestSprShadow->setScale(0.3);
        m_chestSprShadow->setOpacity(60);
        m_chestSprShadow->setColor({0, 0, 0});
    }
    m_rewardNode->addChild(m_chestSpr, 1);
    m_rewardNode->addChild(m_chestSprShadow, 0);

    m_rewardLabel = CCLabelBMFont::create("Reward", "goldFont.fnt");
    m_rewardLabelShadow = CCLabelBMFont::create("Reward", "goldFont.fnt");
    if (m_rewardLabel && m_rewardLabelShadow && m_chestSpr && m_chestSprShadow) {
        m_rewardLabel->setPositionY(m_chestSpr->getPositionY() - 13.5);
        m_rewardLabel->setScale(0.5);
        m_rewardLabel->setZOrder(3);
        m_rewardLabelShadow->setPosition(ccp(m_rewardLabel->getPositionX() + 2, m_rewardLabel->getPositionY() - 2));
        m_rewardLabelShadow->setScale(0.5);
        m_rewardLabelShadow->setOpacity(60);
        m_rewardLabelShadow->setColor({0, 0, 0});
    }
    m_rewardNode->addChild(m_rewardLabel, 1);
    m_rewardNode->addChild(m_rewardLabelShadow, 0);

    // Completion Badge - counts levels completed *through the gauntlet*
    // (crystal reward claimed), not just completed some other way.
    int completed = 0;
    for (auto const& slot : data.levels) {
        if (slot.id == 0) continue;
        if (CustomGauntletManager::get()->isLevelRewardClaimed(slot.id))
            completed++;
    }

    auto countLabel = CCLabelBMFont::create(fmt::format("{}/5", completed).c_str(), "bigFont.fnt");
    countLabel->setScale(0.4);
    countLabel->setColor(completed == 5 ? ccColor3B{100, 255, 100} : ccColor3B{255, 255, 255});
    countLabel->setPosition({sprite->getContentWidth() / 2, (sprite->getContentHeight() / 2) - 26.5f});
    sprite->addChild(countLabel, 2);

    auto countLabelShadow = CCLabelBMFont::create(fmt::format("{}/5", completed).c_str(), "bigFont.fnt");
    countLabelShadow->setScale(0.4);
    countLabelShadow->setOpacity(50);
    countLabelShadow->setColor({0, 0, 0});
    countLabelShadow->setPosition({sprite->getContentWidth() / 2 + 2, (sprite->getContentHeight() / 2) - 29.5f});
    sprite->addChild(countLabelShadow, 1);

    auto stencil = NineSlice::create("square04_001.png");
    stencil->setContentSize({155, 333});
    stencil->setScale(0.625);

    auto gradientClip = CCClippingNode::create(stencil);
    gradientClip->setAlphaThreshold(0);
    gradientClip->setContentSize(stencil->getContentSize());
    gradientClip->setID("gradient-clip");
    gradientClip->setPosition({55, 117.5});
    sprite->addChild(gradientClip);

    auto gradient = CCSprite::createWithSpriteFrameName("GR_pureGradient_001.png"_spr);
    gradient->setColor(data.nameColor);
    gradient->setOpacity(128);
    gradient->setPositionY(-35);
    gradient->setScaleX(1.5);
    gradient->setScaleY(2);
    gradient->setRotation(67);
    gradient->setID("gradient");
    gradient->setBlendFunc({GL_ONE, GL_ONE});
    gradientClip->addChild(gradient);

    if (completed == 5) {
        countLabel->setColor({100, 255, 100});

        auto checkmarkSpr = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
        checkmarkSpr->setScale(1.1);
        checkmarkSpr->setPositionY(-5);
        m_rewardNode->addChild(checkmarkSpr, 1);

        auto checkmarkSprShadow = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
        checkmarkSprShadow->setScale(1.1);
        checkmarkSprShadow->setPosition({checkmarkSpr->getPositionX() + 2, checkmarkSpr->getPositionY() - 3});
        checkmarkSprShadow->setOpacity(60);
        checkmarkSprShadow->setColor({0, 0, 0});
        m_rewardNode->addChild(checkmarkSprShadow, 0);

        m_chestSpr->setVisible(false);
        m_chestSprShadow->setVisible(false);
        m_rewardLabel->setVisible(false);
        m_rewardLabelShadow->setVisible(false);

        auto gradientParticles = GameToolbox::particleFromString(
            "15a-1a1.75a0a8a90a0a30a15a55a0a0a0a0a0a0a0a5a3a0a60a0.329412a0a0.968627a0a0.337255a0a1a0a2a0a0a67a0a0a0.321569a0a0.00392157a0a1a0a0a0a1a0a0a0a0a0a0a0a0a2a1a0a0a0a0a0a0.75a0.5a0a0a0a0a0a0a0a0a0a0a0",
            NULL, false
        );
        if (gradientParticles) {
            gradientParticles->setPosition({0, -110});
            gradientClip->addChild(gradientParticles);
        }
    }

    if (!CCMenuItemSpriteExtra::init(sprite, nullptr, nullptr, nullptr))
        return false;

    setTarget(this, menu_selector(CustomGauntletNode::onTap));
    setID(fmt::format("{}", data.name).c_str());
    m_scaleMultiplier = 1.05;

    loadIcon();
    return true;
}

void CustomGauntletNode::loadIcon() {
    if (m_data.iconURL.empty()) return;

    m_iconHolder.spawn(
        web::WebRequest().get(m_data.iconURL),
        [this](web::WebResponse res) {
            if (!res.ok()) return;
            auto bytes = res.data();
            Ref<CustomGauntletNode> self(this);
            queueInMainThread([self, bytes]() {
                if (!self->getParent()) return;

                auto img = new CCImage();
                if (!img->initWithImageData(
                        const_cast<unsigned char*>(bytes.data()), bytes.size())) {
                    delete img; return;
                }
                auto tex = new CCTexture2D();
                tex->initWithImage(img);
                delete img;

                auto icon = CCSprite::createWithTexture(tex);
                auto shadow = CCSprite::createWithTexture(tex);
                tex->release();
                if (!icon || !shadow) return;

                auto container = static_cast<CCNode*>(self->getChildren()->objectAtIndex(0));
                if (!container) return;

                icon->setID("gauntlet-icon");
                icon->setPosition({container->getContentWidth() / 2, container->getContentHeight() / 2 + 15});

                shadow->setScaleX(icon->getScaleX());
                shadow->setScaleY(icon->getScaleY() * 1.2);
                shadow->setID("gauntlet-icon-shadow");
                shadow->setColor({0, 0, 0});
                shadow->setOpacity(50);
                shadow->setPosition({icon->getPositionX(), icon->getPositionY() - 10});

                if (auto ph = container->getChildByIDRecursive("icon-placeholder")) ph->removeFromParent();
                if (auto phs = container->getChildByIDRecursive("icon-placeholder-shadow")) phs->removeFromParent();
                container->addChild(shadow);
                container->addChild(icon, 1);
            });
        }
    );
}

void CustomGauntletNode::onTap(CCObject*) {
    if (m_callback) m_callback(m_data);
}
