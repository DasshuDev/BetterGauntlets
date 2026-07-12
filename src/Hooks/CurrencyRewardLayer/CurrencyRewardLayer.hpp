#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/CurrencyRewardLayer.hpp>

using namespace geode::prelude;

class $modify(GRCurrencyRewardLayer, CurrencyRewardLayer) {
    inline static int s_pendingCrystals = 0;
    static void queueCrystalReward(int amount);
    static int consumePendingCrystalReward();

    static void showCrystalReward(CCNode* parent, int rewardAmount);

    void update(float dt);
};
