#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/CurrencyRewardLayer.hpp>

using namespace geode::prelude;

class $modify(GRCurrencyRewardLayer, CurrencyRewardLayer) {
    inline static int m_pendingCrystals = 0;

    static void queueCrystalReward(int amount);
    static int consumePendingCrystalReward();
    static void showCrystalReward(CCNode* parent, int rewardAmount);

    inline static bool m_gauntletRewardStylingActive = false;

    static void setGauntletRewardStylingActive(bool active);
    static void restyleForGauntletReward(CurrencyRewardLayer* layer);
    static void restyleRewardUnlockLayerIcons(CurrencyRewardLayer* layer);

    inline static int m_gauntletRewardCoins = 0;
    
    static void setGauntletRewardCoins(int coins);
    static void applyGauntletCurrencyLabels(CurrencyRewardLayer* layer);

    bool init(
        int orbs, int stars, int moons, int diamonds,
        CurrencySpriteType demonKey, int keyCount,
        CurrencySpriteType shardType, int shardsCount,
        cocos2d::CCPoint position, CurrencyRewardType rewardType,
        float yOffset, float time
    );

    void update(float dt);
};
