#include "GauntletCompletionPopup.hpp"
#include <Geode/binding/RewardUnlockLayer.hpp>
#include <Geode/binding/RewardsPage.hpp>

namespace {

    CCFiniteTimeAction* popInLeadAction() {
        return CCSequence::create(
            CCScaleBy::create(0, 0.5),
            CCEaseExponentialOut::create(CCScaleBy::create(1.5, 2)),
            CCEaseExponentialIn::create(CCScaleBy::create(0.5, 1.4)),
            nullptr
        );
    }

    CCFiniteTimeAction* popInSettleAction() {
        return CCEaseExponentialOut::create(CCScaleBy::create(1.5, 1/1.4));
    }

    CCFiniteTimeAction* popInAction() {
        return CCSequence::create(popInLeadAction(), popInSettleAction(), nullptr);
    }

    CCFiniteTimeAction* tintFlashAction(ccColor3B color) {
        return CCSequence::create(
            CCDelayTime::create(2),
            CCTintTo::create(0, 255, 255, 255),
            CCTintTo::create(1.5, color.r, color.g, color.b),
            nullptr
        );
    }

    ccColor4F toColor4F(ccColor3B color, float alpha = 1.f) {
        return { color.r / 255.f, color.g / 255.f, color.b / 255.f, alpha };
    }

    CCFiniteTimeAction* floatInAction(float riseDistance = 25.f, float duration = 1) {
        return CCSpawn::create(
            CCFadeIn::create(duration),
            CCEaseExponentialOut::create(CCMoveBy::create(duration, {0, riseDistance})),
            nullptr
        );
    }

    template <typename T>
    void prepareFloatIn(T* node, float riseDistance = 15.f) {
        node->setOpacity(0);
        node->setPositionY(node->getPositionY() - riseDistance);
    }

    CCParticleSystemQuad* burstParticles(ccColor3B color) {
        auto burst = GameToolbox::particleFromString(
            "100a0a1.5a0.67a-1a-180a180a0a0a200a200a0a0a-2000a0a0a0a5a0a0a62a1a0a1a0a1a0a1a0.15a0a0a0a87a1a0a1a0a1a0a0.15a0.05a0a0a0.5a0.35a70a0a250a125a0a0a1a2a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a2a0a0",
            NULL, false);
        burst->setStartColor(toColor4F(color));
        burst->setEndColor(toColor4F(color, 0.f));

        return burst;
    }
}

GauntletCompletionPopup* GauntletCompletionPopup::create(GauntletType type, ccColor3B titleColor, ccColor3B highlightColor) {
    auto ret = new GauntletCompletionPopup();
    if (ret && ret->init(type, titleColor, highlightColor)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GauntletCompletionPopup::init(GauntletType type, ccColor3B titleColor, ccColor3B highlightColor) {
    if (!Popup::init(520, 280, "GJ_square05.png")) return false;
    
    m_bgSprite->setVisible(false);
    m_closeBtn->setOpacity(0);
    m_noElasticity = true;
    
    this->setOpacity(215);

    auto textureCache = CCTextureCache::sharedTextureCache();
    auto spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    textureCache->addImage("GJ_ShopSheet.png", false);
    spriteFrameCache->addSpriteFramesWithFile("GJ_ShopSheet.plist");

    std::string name = GauntletNode::nameForType(type);
    auto frame = GauntletNode::frameForType(type);

    auto gsm = GameStatsManager::sharedState();
    int gauntletID = static_cast<int>(type);

    if (!gsm->isGauntletChestUnlocked(gauntletID)) {
        if (auto reward = gsm->unlockGauntletChest(gauntletID)) {
            int itemCount = reward->m_rewardObjects ? reward->m_rewardObjects->count() : 0;
            log::info("{} Gauntlet chest unlocked (id {}) - {} reward item(s), chestID {}", name, gauntletID, itemCount, reward->m_chestID);
        } else {
            log::warn("{} Gauntlet chest unlock returned no reward (id {})", name, gauntletID);
        }
    } else {
        log::info("{} Gauntlet chest already unlocked (id {})", name, gauntletID);
    }

    Ref<GJRewardItem> debugReward = GJRewardItem::createSpecial(
        static_cast<GJRewardType>(0), 100, 5,
        static_cast<SpecialRewardItem>(0), 0,
        static_cast<SpecialRewardItem>(0), 0,
        0, 0
    );
    m_mainLayer->runAction(CCSequence::create(
        CCDelayTime::create(5),
        CallFuncExt::create([this, debugReward] {
            auto rewardsPage = RewardsPage::create();
            if (auto unlockLayer = RewardUnlockLayer::create(1, rewardsPage)) {
                if (unlockLayer->showCollectReward(debugReward)) m_mainLayer->addChild(unlockLayer, 100);
            }
        }),
        nullptr
    ));
 
    auto plaqueBase = CCSprite::createWithSpriteFrameName("GR_plaqueBase_001.png"_spr);
    if (plaqueBase) {
        plaqueBase->setID("plaque-base");
        plaqueBase->setColor({80, 80, 80});
        plaqueBase->setPosition(m_mainLayer->getContentSize() / 2);
        m_mainLayer->addChild(plaqueBase, 0);
    }

    auto plaqueAccent = CCSprite::createWithSpriteFrameName("GR_plaqueAccent_001.png"_spr);
    if (plaqueAccent) {
        plaqueAccent->setID("plaque-accent");
        plaqueAccent->setColor({0, 0, 0});
        plaqueAccent->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
        plaqueAccent->setOpacity(128);
        plaqueAccent->setPosition(m_mainLayer->getContentSize() / 2);
        m_mainLayer->addChild(plaqueAccent, 1);
    }

    auto titleLabel = CCLabelBMFont::create(fmt::format("The {} Gauntlet", name).c_str(), "bigFont.fnt");
    titleLabel->setID("completion-title");
    titleLabel->setScale(0.8);
    titleLabel->setPosition(m_size.width / 2, m_size.height / 2 - 55);
    prepareFloatIn(titleLabel);
    m_mainLayer->addChild(titleLabel, 3);

    auto completeLabel = CCLabelBMFont::create("Completed!", "goldFont.fnt");
    completeLabel->setID("completion-label");
    completeLabel->setScale(0.65);
    completeLabel->setPosition(m_size.width / 2, m_size.height / 2 - 75);
    prepareFloatIn(completeLabel);
    m_mainLayer->addChild(completeLabel, 3);

    auto gauntletFrame = CCSpriteGrayscale::createWithSpriteFrameName(frame.c_str());
    if (gauntletFrame) {
        gauntletFrame->setID("gauntlet-frame");
        gauntletFrame->setColor({128, 128, 128});
        gauntletFrame->setPosition(m_mainLayer->getContentSize() / 2);
        gauntletFrame->setScale(1.25);
        m_mainLayer->addChild(gauntletFrame, 2);
    }

    auto gauntletFrameShadow = CCSprite::createWithSpriteFrameName(frame.c_str());
    if (gauntletFrameShadow) {
        gauntletFrameShadow->setColor({0, 0, 0});
        gauntletFrameShadow->setOpacity(75);
        gauntletFrameShadow->setID("gauntlet-frame");
        gauntletFrameShadow->setPosition({m_mainLayer->getContentWidth() / 2, (m_mainLayer->getContentHeight() / 2) - 10});
        gauntletFrameShadow->setScaleX(1.25);
        gauntletFrameShadow->setScaleY(gauntletFrame->getScaleY() * 1.2);
        m_mainLayer->addChild(gauntletFrameShadow, 1);
    }

    if (plaqueBase && plaqueAccent && gauntletFrame && gauntletFrameShadow && titleLabel && completeLabel) {
        plaqueBase->runAction(CCSpawn::create(popInAction(), tintFlashAction(highlightColor), nullptr));
        plaqueAccent->runAction(CCSpawn::create(popInAction(), tintFlashAction(titleColor), nullptr));
        gauntletFrame->runAction(popInAction());
        gauntletFrameShadow->runAction(popInAction());

        CCPoint burstPos = m_mainLayer->getContentSize() / 2;
        m_mainLayer->runAction(CCSequence::create(
            CCDelayTime::create(2),
            CallFuncExt::create([this, burstPos, titleColor, highlightColor, gauntletFrame, frame, titleLabel, completeLabel] {
                auto burstA = burstParticles(titleColor);
                burstA->setOpacity(128);
                burstA->setPosition(burstPos);
                burstA->setID("tint-burst");
                m_mainLayer->addChild(burstA, -1);

                auto burstB = burstParticles(highlightColor);
                burstB->setScale(0.85);
                burstB->setPosition(burstPos);
                burstB->setID("tint-burst");
                m_mainLayer->addChild(burstB, -1);

                titleLabel->runAction(CCSequence::create(
                    CCDelayTime::create(1.15),
                    floatInAction(),
                    nullptr
                ));
                completeLabel->runAction(CCSequence::create(
                    CCDelayTime::create(1.35),
                    floatInAction(),
                    nullptr
                ));

                auto coloredFrame = CCSprite::createWithSpriteFrameName(frame.c_str());
                if (coloredFrame) {
                    coloredFrame->setID("gauntlet-frame");
                    coloredFrame->setPosition(gauntletFrame->getPosition());
                    coloredFrame->setScale(gauntletFrame->getScale());
                    coloredFrame->runAction(popInSettleAction());
                    m_mainLayer->addChild(coloredFrame, 2);
                }
                gauntletFrame->removeFromParent();
            }),
            nullptr
        ));
    }

    {
        CCParticleSystemQuad* particles = GameToolbox::particleFromString(
            "15a-1a2a1a5a180a180a0a0a67a67a0a117a28a0a0a0a25a0a0a62a1a0a1a0a1a0a0.35a0.15a0a0a0a87a1a0a1a0a1a0a0.15a0.05a0.5a0a0.5a0.35a0a0a20a15a0a0a0a2a1a0a0a0a28a0a10a0a0a0a0a0a0a0a0a0a0a0a0",
            NULL,
            false
        );
        particles->setPosition(m_mainLayer->getContentSize() / 2);
        particles->setID("particles");
        m_mainLayer->addChild(particles);
    }

    {
        CCParticleSystemQuad* inner = GameToolbox::particleFromString(
            "200a1.5a2a0a100a-180a180a0a0a200a200a0a0a-2000a0a0a0a5a0a0a62a1a0a1a0a1a0a0.35a0.15a0a0a0a87a1a0a1a0a1a0a0.15a0.05a0.2a0a0.5a0.15a75a25a0a0a0a0a0a2a1a0a0a0a0a0a3.5a0a0a0a0a0a0a0a0a0a0a0a0",
            NULL,
            false
        );
        inner->setPosition(m_mainLayer->getContentSize() / 2);
        inner->setID("inner-particles");
        m_mainLayer->addChild(inner, -1);
    }

    return true;
}
