#include "CustomGauntletCompletionPopup.hpp"
#include "GUI/CCControlExtension/CCScale9Sprite.h"
#include "Geode/cocos/CCDirector.h"
#include "Geode/cocos/actions/CCActionEase.h"
#include "Geode/cocos/actions/CCActionInterval.h"
#include <Geode/binding/RewardUnlockLayer.hpp>
#include <Geode/binding/RewardsPage.hpp>
#include <Geode/modify/GJChestSprite.hpp>

namespace {
    Ref<GJChestSprite> activeChestSprite;
    void restyleCurrencyIcons(CCNode* root, int attemptsLeft = 10);
}

class $modify(CGCPChestSprite, GJChestSprite) {
    void switchToState(ChestSpriteState state, bool noGlow) {
        GJChestSprite::switchToState(state, noGlow);

        log::debug(
            "CGCPChestSprite::switchToState - state={} isActive={}",
            static_cast<int>(state), this == activeChestSprite
        );

        int childCount = static_cast<int>(this->getChildrenCount());

        if (this != activeChestSprite) return;
        if (static_cast<int>(state) != 4) return;

        static const char* KnownFrames[] = {
            "chest_02_01_001.png",
            "chest_02_02_001.png",
            "chest_02_03_001.png",
            "chest_02_03_back_001.png",
            "chest_02_03_glow_001.png",
            "chest_02_04_001.png",
            "chest_02_04_back_001.png",
            "chest_02_04_glow_001.png",
        };
        auto frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();

        for (int i = 0; i < std::min(childCount, 9); i++) {
            auto colors = static_cast<CCSprite*>(this->getChildByIndex(i));
            if (!colors) continue;

            switch (i) {
                case 3: colors->setColor({255, 200, 0}); break;
                case 4: colors->setColor({170, 120, 0}); break;
                case 5:
                case 6:
                case 7:
                case 8: colors->setColor({84, 84, 84}); break;
                default: break;
            }
        }

        CCNode* root = this;
        while (auto parent = root->getParent()) root = parent;
        restyleCurrencyIcons(root);
    }
};

namespace {

    struct ChestFrameOverride {
        std::string vanillaName;
        std::string customName;
    };

    // replace chest sprites with GR chest sprites
    const std::vector<ChestFrameOverride> ChestFrameOverrides = {
        {"chest_02_02_001.png", "GR_chest_01_001.png"_spr},
        {"chest_02_03_001.png", "GR_chest_02_001.png"_spr},
        {"chest_02_03_glow_001.png", "chest_09_03_glow_001.png"},
        {"chest_02_04_001.png", "GR_chest_03_001.png"_spr},
        {"chest_02_04_back_001.png", "GR_chest_04_001.png"_spr},
        {"chest_02_04_glow_001.png", "chest_09_04_glow_001.png"},
    };

    std::unordered_map<std::string, Ref<CCSpriteFrame>> s_originalChestFrames;

    void applyCustomChestFrames() {
        auto cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        for (auto const& override_ : ChestFrameOverrides) {
            auto customFrame = cache->spriteFrameByName(override_.customName.c_str());
            if (!customFrame) continue;

            if (!s_originalChestFrames.count(override_.vanillaName)) {
                if (auto original = cache->spriteFrameByName(override_.vanillaName.c_str())) {
                    s_originalChestFrames[override_.vanillaName] = original;
                }
            }

            cache->addSpriteFrame(customFrame, override_.vanillaName.c_str());
        }
    }

    void restoreVanillaChestFrames() {
        auto cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        for (auto& [frameName, frame] : s_originalChestFrames) {
            cache->addSpriteFrame(frame, frameName.c_str());
        }
        s_originalChestFrames.clear();
    }

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

    CCScale9Sprite* findScale9Sprite(CCNode* root) {
        if (!root) return nullptr;
        if (auto self = typeinfo_cast<extension::CCScale9Sprite*>(root)) return self;
        if (auto children = root->getChildren()) {
            for (auto obj : CCArrayExt<CCNode*>(children)) {
                if (auto found = findScale9Sprite(obj)) return found;
            }
        }
        return nullptr;
    }

    void restyleRewardUnlockLayer(RewardUnlockLayer* unlockLayer) {
        activeChestSprite = unlockLayer->m_chestSprite;

        if (auto bg = findScale9Sprite(unlockLayer)) {
            bg->setVisible(false);

            auto newBG = NineSlice::create("GJ_square05.png");
            newBG->setContentSize(bg->getContentSize());
            newBG->setPosition(bg->getPosition());
            if (auto parent = bg->getParent()) parent->addChild(newBG);
            bg->removeFromParent();
        }
    }

    CurrencyRewardLayer* findCurrencyRewardLayer(CCNode* root) {
        if (!root) return nullptr;
        if (auto self = typeinfo_cast<CurrencyRewardLayer*>(root)) return self;
        if (auto children = root->getChildren()) {
            for (auto obj : CCArrayExt<CCNode*>(children)) {
                if (auto found = findCurrencyRewardLayer(obj)) return found;
            }
        }
        return nullptr;
    }

    CCSpriteFrame* frameFromFile(char const* frameName) {
        auto spr = CCSprite::create(frameName);
        if (!spr) return nullptr;
        auto tex = spr->getTexture();
        if (!tex) return nullptr;
        return CCSpriteFrame::createWithTexture(tex, {{0, 0}, tex->getContentSize()});
    }

    void removeNonCurrencySprites(CCSpriteBatchNode* batchNode) {
        if (!batchNode) return;

        std::vector<CCNode*> toRemove;
        if (auto children = batchNode->getChildren()) {
            for (auto obj : CCArrayExt<CCNode*>(children)) {
                if (!typeinfo_cast<CurrencySprite*>(obj)) toRemove.push_back(obj);
            }
        }
        for (auto child : toRemove) batchNode->removeChild(child, true);
    }

    // restyle vanilla currency with GR variants
    void restyleCurrencyIcons(CCNode* root, int attemptsLeft) {
        log::debug("restyleCurrencyIcons - called, searching from {} ({} attempts left)",
            root ? typeid(*root).name() : "null root", attemptsLeft);

        auto rewardLayer = findCurrencyRewardLayer(root);
        if (!rewardLayer) {
            if (attemptsLeft <= 0) {
                log::warn("restyleCurrencyIcons - gave up waiting for CurrencyRewardLayer");
                return;
            }
            if (root) {
                root->runAction(CCSequence::create(
                    CCDelayTime::create(0.2f),
                    CallFuncExt::create([root, attemptsLeft] {
                        restyleCurrencyIcons(root, attemptsLeft - 1);
                    }),
                    nullptr
                ));
            }
            return;
        }
        log::debug("restyleCurrencyIcons - found CurrencyRewardLayer, m_objects count = {}",
            rewardLayer->m_objects ? rewardLayer->m_objects->count() : 0);

        auto coinFrame = frameFromFile("GR_gauntletCoin_001.png"_spr);
        auto crystalFrame = frameFromFile("GR_crystal_001.png"_spr);
        log::debug("restyleCurrencyIcons - coinFrame={} crystalFrame={}", coinFrame != nullptr, crystalFrame != nullptr);
        log::debug(
            "restyleCurrencyIcons - m_orbsSprite={} m_diamondsSprite={} m_orbBatchNode={} m_currencyBatchNode={}",
            rewardLayer->m_orbsSprite != nullptr, rewardLayer->m_diamondsSprite != nullptr,
            rewardLayer->m_orbBatchNode != nullptr, rewardLayer->m_currencyBatchNode != nullptr
        );

        if (rewardLayer->m_orbsSprite && coinFrame)
            rewardLayer->m_orbsSprite->setDisplayFrame(coinFrame);
        if (rewardLayer->m_diamondsSprite && crystalFrame)
            rewardLayer->m_diamondsSprite->setDisplayFrame(crystalFrame);

        if (rewardLayer->m_orbBatchNode && coinFrame) {
            rewardLayer->m_orbBatchNode->setTexture(coinFrame->getTexture());
            removeNonCurrencySprites(rewardLayer->m_orbBatchNode);
        }
        if (rewardLayer->m_currencyBatchNode && crystalFrame) {
            rewardLayer->m_currencyBatchNode->setTexture(crystalFrame->getTexture());
            removeNonCurrencySprites(rewardLayer->m_currencyBatchNode);
        }

        int spriteCount = 0;
        for (auto sprite : CCArrayExt<CurrencySprite*>(rewardLayer->m_objects)) {
            if (!sprite) continue;
            spriteCount++;

            if (sprite->m_spriteType == CurrencySpriteType::Diamond) {
                if (crystalFrame) sprite->setDisplayFrame(crystalFrame);
            } else {
                if (coinFrame) sprite->setDisplayFrame(coinFrame);
            }
        }
        log::debug("restyleCurrencyIcons - restyled {} CurrencySprite object(s)", spriteCount);

        if (rewardLayer->m_mainNode) rewardLayer->m_mainNode->updateLayout();
    }
}

void CustomGauntletCompletionPopup::onClose(CCObject* sender) {
    restoreVanillaChestFrames();
    activeChestSprite = nullptr;
    Popup::onClose(sender);
}

CustomGauntletCompletionPopup* CustomGauntletCompletionPopup::create(CustomGauntletData const& data, int rewardCoins, CCTexture2D* iconTexture) {
    auto ret = new CustomGauntletCompletionPopup();
    if (ret && ret->init(data, rewardCoins, iconTexture)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CustomGauntletCompletionPopup::init(CustomGauntletData const& data, int rewardCoins, CCTexture2D* iconTexture) {
    log::debug("CustomGauntletCompletionPopup::init - rewardCoins param = {}, data.rewardCoins = {}", rewardCoins, data.rewardCoins);

    CCSize winSize = CCDirector::get()->getWinSize();
    if (!Popup::init(winSize.width - 30, winSize.height - 30, "GJ_square05.png")) return false;

    m_bgSprite->setVisible(false);
    m_closeBtn->setOpacity(0);
    m_noElasticity = true;

    this->setOpacity(215);

    auto spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    spriteFrameCache->addSpriteFramesWithFile("GJ_ShopSheet.plist");
    applyCustomChestFrames();

    std::string name = data.name;
    ccColor3B titleColor = data.nameColor;
    ccColor3B highlightColor = data.accentColor2;
    
    Ref<CCSpriteFrame> iconFrame;
    if (iconTexture) {
        iconFrame = CCSpriteFrame::createWithTexture(
            iconTexture, {{0, 0}, iconTexture->getContentSize()}
        );
    }

    Ref<GJRewardItem> reward;
    int coins = rewardCoins;
    if (rewardCoins > 0) {
        reward = GJRewardItem::createSpecial(
            static_cast<GJRewardType>(0), coins * 1.5, coins,
            static_cast<SpecialRewardItem>(0), 0,
            static_cast<SpecialRewardItem>(0), 0,
            0, 0
        );
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

    auto gauntletFrame = CCSpriteGrayscale::createWithSpriteFrame(iconFrame);
    if (gauntletFrame) {
        gauntletFrame->setID("grey-frame");
        gauntletFrame->setColor({128, 128, 128});
        gauntletFrame->setScale(1.25);
        node->addChild(gauntletFrame, 2);
    }

    auto gauntletFrameShadow = CCSprite::createWithSpriteFrame(iconFrame);
    if (gauntletFrameShadow) {
        gauntletFrameShadow->setColor({0, 0, 0});
        gauntletFrameShadow->setOpacity(75);
        gauntletFrameShadow->setID("gauntlet-frame");
        gauntletFrameShadow->setPositionY(-10);
        gauntletFrameShadow->setScaleX(1.25);
        gauntletFrameShadow->setScaleY(gauntletFrame ? gauntletFrame->getScaleY() * 1.2 : 1.2);
        node->addChild(gauntletFrameShadow, 1);
    }

    log::debug(
        "CustomGauntletCompletionPopup gate - plaqueBase={} plaqueAccent={} gauntletFrame={} gauntletFrameShadow={} node={} (iconTexture={}, iconFrame={})",
        plaqueBase != nullptr, plaqueAccent != nullptr, gauntletFrame != nullptr,
        gauntletFrameShadow != nullptr, node != nullptr,
        iconTexture != nullptr, iconFrame != nullptr
    );

    if (plaqueBase && plaqueAccent && gauntletFrame && gauntletFrameShadow && node) {
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
            CallFuncExt::create([this, node, titleColor, highlightColor, gauntletFrame, iconFrame, titleLabel, completeLabel] {
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
                    fadeOut(),
                    nullptr
                ));
                completeLabel->runAction(CCSequence::create(
                    CCDelayTime::create(1.35),
                    floatInAction(),
                    nullptr
                ));
                completeLabel->runAction(CCSpawn::create(
                    fadeOut(),
                    nullptr
                ));

                auto coloredFrame = CCSprite::createWithSpriteFrame(iconFrame);
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
                CCDelayTime::create(5.25),
                CallFuncExt::create([this, reward] {
                    log::debug("CustomGauntletCompletionPopup - reward reveal CallFuncExt firing");

                    auto rewardsPage = RewardsPage::create();
                    auto unlockLayer = RewardUnlockLayer::create(2, rewardsPage);
                    if (!unlockLayer) {
                        log::warn("CustomGauntletCompletionPopup - RewardUnlockLayer::create returned null");
                        return;
                    }
                    if (!unlockLayer->showCollectReward(reward)) {
                        log::warn("CustomGauntletCompletionPopup - showCollectReward returned false");
                        return;
                    }

                    restyleRewardUnlockLayer(unlockLayer);
                    if (auto innerLayer = unlockLayer->getChildByIndex(0)) {
                        int cornerCount = static_cast<int>(innerLayer->getChildrenCount());
                        for (int i = 0; i < cornerCount; i++) {
                            auto corner = typeinfo_cast<CCSpriteWithHue*>(innerLayer->getChildByIndex(i));
                            if (!corner) continue;
                            auto newCorner = CCSprite::createWithSpriteFrameName("GR_rewardCorner_001.png"_spr);
                            newCorner->setID(fmt::format("corner-{}", i));
                            innerLayer->addChild(newCorner);

                            switch (i) {
                                case 0: {
                                    newCorner->setPosition({corner->getPositionX() - 1, corner->getPositionY() - 1.25f});
                                    break;
                                }
                                case 1: {
                                    newCorner->setPosition({corner->getPositionX() - 1, corner->getPositionY() + 1.25f});
                                    newCorner->setFlipY(true);
                                    break;
                                }
                                case 2: {
                                    newCorner->setPosition({corner->getPositionX() + 1, corner->getPositionY() + 1.25f});
                                    newCorner->setFlipY(true);
                                    newCorner->setFlipX(true);
                                    break;
                                }
                                case 3: {
                                    newCorner->setPosition({corner->getPositionX() + 1, corner->getPositionY() - 1.25f});
                                    newCorner->setFlipX(true);
                                    break;
                                }
                                default: break;
                            }
                            corner->setVisible(false);
                        }
                    }

                    this->addChild(unlockLayer, 200);
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
        inner->setID("inner-particles");
        node->addChild(inner, -1);
    }

    return true;
}
