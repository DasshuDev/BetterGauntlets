#include "GauntletCompletionPopup.hpp"
#include "Geode/cocos/CCDirector.h"
#include "Geode/cocos/actions/CCActionEase.h"
#include "Geode/cocos/actions/CCActionInterval.h"
#include <Geode/binding/RewardUnlockLayer.hpp>
#include <Geode/binding/RewardsPage.hpp>
#include <Geode/modify/RewardUnlockLayer.hpp>

class $modify(GCPRewardUnlockLayer, RewardUnlockLayer) {
    void onClose(CCObject* sender) {
        log::debug("GCPRewardUnlockLayer::onClose - entered");
        auto parentPopup = this->getParent()
            ? typeinfo_cast<ForceClosablePopup*>(this->getParent())
            : nullptr;
        log::debug("GCPRewardUnlockLayer::onClose - parentPopup found={}", parentPopup != nullptr);

        RewardUnlockLayer::onClose(sender); // run original close/cleanup first

        if (parentPopup) {
            log::debug("GCPRewardUnlockLayer::onClose - forwarding close to parent popup");
            parentPopup->forceCloseFromReward(sender);
        }
    }
};

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
        return CCEaseExponentialOut::create(
            CCScaleBy::create(1.5, 1/1.4)
        );
    }

    CCFiniteTimeAction* popInAction() {
        return CCSequence::create(
            popInLeadAction(),
            popInSettleAction(),
            nullptr
        );
    }

    CCFiniteTimeAction* tintFlashAction(ccColor3B color) {
        return CCSequence::create(
            CCDelayTime::create(2),
            CCTintTo::create(0, 255, 255, 255),
            CCTintTo::create(1.5, color.r, color.g, color.b),
            nullptr
        );
    }

    CCFiniteTimeAction* scaleDown() {
        return CCSequence::create(
            CCDelayTime::create(2),
            CCEaseExponentialIn::create(CCScaleTo::create(1.25, 0.25)),
            nullptr
        );
    }

    CCFiniteTimeAction* fadeOut() {
        return CCSequence::create(
            CCDelayTime::create(2),
            CCFadeTo::create(1, 0),
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
    if (ret && ret->init(type, titleColor, highlightColor, false)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

GauntletCompletionPopup* GauntletCompletionPopup::createDebug(GauntletType type, ccColor3B titleColor, ccColor3B highlightColor) {
    auto ret = new GauntletCompletionPopup();
    if (ret && ret->init(type, titleColor, highlightColor, true)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GauntletCompletionPopup::init(GauntletType type, ccColor3B titleColor, ccColor3B highlightColor, bool debugReward) {
    if (!Popup::init(m_size.width - 30, m_size.height - 30, "GJ_square05.png")) return false;
    
    m_bgSprite->setVisible(false);
    m_closeBtn->setOpacity(0);
    m_noElasticity = true;
    
    this->setOpacity(215);

    auto spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    spriteFrameCache->addSpriteFramesWithFile("GJ_ShopSheet.plist");

    std::string name = GauntletNode::nameForType(type);
    auto frame = GauntletNode::frameForType(type);

    auto gsm = GameStatsManager::sharedState();
    int gauntletID = static_cast<int>(type);

    Ref<GJRewardItem> reward;
    if (debugReward) {
        reward = GJRewardItem::createSpecial(
            static_cast<GJRewardType>(0), 100, 5,
            static_cast<SpecialRewardItem>(0), 0,
            static_cast<SpecialRewardItem>(0), 0,
            0, 0
        );
        log::info("{} Gauntlet completion popup opened in debug mode - no real reward granted", name);
    } else if (!gsm->isGauntletChestUnlocked(gauntletID)) {
        if (auto r = gsm->unlockGauntletChest(gauntletID)) {
            reward = r;
            int itemCount = reward->m_rewardObjects ? reward->m_rewardObjects->count() : 0;
            log::info("{} Gauntlet chest unlocked (id {}) - {} reward item(s), chestID {}", name, gauntletID, itemCount, reward->m_chestID);
        }
    } else {
        log::info("{} Gauntlet chest already unlocked (id {})", name, gauntletID);
    }

    auto node = CCNode::create();
    node->setID("plaque-node");
    node->setPosition(CCDirector::get()->getWinSize() / 2);
    this->addChild(node);
 
    auto plaqueBase = CCSprite::createWithSpriteFrameName("GR_plaqueBase_001.png"_spr);
    if (plaqueBase) {
        plaqueBase->setID("plaque-base");
        plaqueBase->setColor({80, 80, 80});
        node->addChild(plaqueBase, 0);
    }

    auto plaqueAccent = CCSprite::createWithSpriteFrameName("GR_plaqueAccent_001.png"_spr);
    if (plaqueAccent) {
        plaqueAccent->setID("plaque-accent");
        plaqueAccent->setColor({0, 0, 0});
        plaqueAccent->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
        plaqueAccent->setOpacity(128);
        node->addChild(plaqueAccent, 1);
    }

    auto titleLabel = CCLabelBMFont::create(fmt::format("The {} Gauntlet", name).c_str(), "bigFont.fnt");
    titleLabel->setID("completion-title");
    titleLabel->setScale(0.8);
    titleLabel->setPositionY(-55);
    prepareFloatIn(titleLabel);
    node->addChild(titleLabel, 3);

    auto completeLabel = CCLabelBMFont::create("Completed!", "goldFont.fnt");
    completeLabel->setID("completion-label");
    completeLabel->setScale(0.65);
    completeLabel->setPositionY(-75);
    prepareFloatIn(completeLabel);
    node->addChild(completeLabel, 3);

    auto gauntletFrame = CCSpriteGrayscale::createWithSpriteFrameName(frame.c_str());
    if (gauntletFrame) {
        gauntletFrame->setID("grey-frame");
        gauntletFrame->setColor({128, 128, 128});
        gauntletFrame->setScale(1.25);
        node->addChild(gauntletFrame, 2);
    }

    auto gauntletFrameShadow = CCSprite::createWithSpriteFrameName(frame.c_str());
    if (gauntletFrameShadow) {
        gauntletFrameShadow->setColor({0, 0, 0});
        gauntletFrameShadow->setOpacity(75);
        gauntletFrameShadow->setID("gauntlet-frame");
        gauntletFrameShadow->setPositionY(-10);
        gauntletFrameShadow->setScaleX(1.25);
        gauntletFrameShadow->setScaleY(gauntletFrame->getScaleY() * 1.2);
        node->addChild(gauntletFrameShadow, 1);
    }

    if (plaqueBase && plaqueAccent && gauntletFrame && gauntletFrameShadow && titleLabel && completeLabel) {
        plaqueBase->runAction(CCSpawn::create(
            popInAction(), 
            tintFlashAction(highlightColor),
            CCSequence::create(
                CCDelayTime::create(3),
                CCSpawn::create(
                    fadeOut(),
                    nullptr
                ),
                nullptr
            ),
            nullptr
        ));
        plaqueAccent->runAction(CCSpawn::create(
            popInAction(), 
            tintFlashAction(titleColor),
            CCSequence::create(
                CCDelayTime::create(3),
                CCSpawn::create(
                    fadeOut(),
                    nullptr
                ),
                nullptr
            ),
            nullptr
        ));
        gauntletFrame->runAction(CCSpawn::create(
            popInAction(),
            CCSequence::create(
                CCDelayTime::create(3),
                CCSpawn::create(
                    fadeOut(),
                    nullptr
                ),
                nullptr
            ),
            nullptr
        ));
        gauntletFrameShadow->runAction(CCSpawn::create(
            popInAction(),
            CCSequence::create(
                CCDelayTime::create(3),
                CCSpawn::create(
                    fadeOut(),
                    nullptr
                ),
                nullptr
            ),
            nullptr
        ));
        node->runAction(CCSpawn::create(
            CCSequence::create(
                CCDelayTime::create(2.75),
                CCSpawn::create(
                    scaleDown(),
                    nullptr
                ),
                nullptr
            ),
            nullptr
        ));

        node->runAction(CCSequence::create(
            CCDelayTime::create(2),
            CallFuncExt::create([this, node, titleColor, highlightColor, gauntletFrame, frame, titleLabel, completeLabel, reward] {
                auto burstA = burstParticles(titleColor);
                burstA->setOpacity(128);
                burstA->setID("tint-burst");
                node->addChild(burstA, -1);

                auto burstB = burstParticles(highlightColor);
                burstB->setScale(0.85);
                burstB->setID("tint-burst");
                node->addChild(burstB, -1);

                titleLabel->runAction(CCSequence::create(
                    CCDelayTime::create(1.15),
                    floatInAction(),
                    nullptr
                ));
                titleLabel->runAction(CCSpawn::create(
                    // scaleDown(), 
                    fadeOut(), 
                    nullptr
                ));
                completeLabel->runAction(CCSequence::create(
                    CCDelayTime::create(1.35),
                    floatInAction(),
                    nullptr
                ));
                completeLabel->runAction(CCSpawn::create(
                    // scaleDown(), 
                    fadeOut(), 
                    nullptr
                ));

                auto coloredFrame = CCSprite::createWithSpriteFrameName(frame.c_str());
                if (coloredFrame) {
                    coloredFrame->setID("colored-frame");
                    coloredFrame->setPosition(gauntletFrame->getPosition());
                    coloredFrame->setScale(gauntletFrame->getScale());
                    coloredFrame->runAction(popInSettleAction());
                    coloredFrame->runAction(fadeOut());
                    node->addChild(coloredFrame, 2);
                }
                gauntletFrame->removeFromParent();
            }),
            nullptr
        ));

        if (reward) {
            this->runAction(CCSequence::create(
                CCDelayTime::create(6.5),
                CallFuncExt::create([this, reward] {
                    int GauntletChestStyle = 2;
                    auto rewardsPage = RewardsPage::create();
                    if (auto unlockLayer = RewardUnlockLayer::create(GauntletChestStyle, rewardsPage)) {
                        if (unlockLayer->showCollectReward(reward)) {
                            this->addChild(unlockLayer, 200);
                        }
                    }
                }),
                nullptr
            ));
        }
    }

    {
        CCParticleSystemQuad* inner = GameToolbox::particleFromString(
            "200a1.5a2a0a100a-180a180a0a0a200a200a0a0a-2000a0a0a0a5a0a0a62a1a0a1a0a1a0a0.35a0.15a0a0a0a87a1a0a1a0a1a0a0.15a0.05a0.2a0a0.5a0.15a75a25a0a0a0a0a0a2a1a0a0a0a0a0a3.5a0a0a0a0a0a0a0a0a0a0a0a0",
            NULL,
            false
        );
        // inner->setPosition(CCDirector::get()->getWinSize() / 2);
        inner->setID("inner-particles");
        node->addChild(inner, -1);
    }

    return true;
}
