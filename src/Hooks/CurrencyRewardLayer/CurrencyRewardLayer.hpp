#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/CurrencyRewardLayer.hpp>

using namespace geode::prelude;

// Builds a standalone reward popup for the crystal currency by repurposing
// CurrencyRewardLayer's diamond counter (icon, label, flying pickup
// animation), reskinned to show crystals instead of diamonds. See
// GREndLevelLayer for when this actually gets triggered.
class $modify(GRCurrencyRewardLayer, CurrencyRewardLayer) {
    // Set right before triggering the level-complete flow so GREndLevelLayer
    // knows how many crystals to show once the results screen is up.
    inline static int s_pendingCrystals = 0;
    static void queueCrystalReward(int amount);
    static int consumePendingCrystalReward();

    // Creates the reskinned reward popup and adds it to parent (e.g. the
    // EndLevelLayer instance). No-op if rewardAmount <= 0.
    static void showCrystalReward(CCNode* parent, int rewardAmount);

    // Keeps the popup's layout correct while its counter ticks up (the
    // label's width changes every frame during the count animation).
    void update(float dt);
};
