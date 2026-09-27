#include "PlayLayer.hpp"
#include "../CurrencyRewardLayer/CurrencyRewardLayer.hpp"
#include "../../Data/CustomGauntletManager.hpp"
#include "../../Managers/StatsSyncManager.hpp"

using namespace geode::prelude;

void GRPlayLayer::levelComplete() {
    PlayLayer::levelComplete();

    if (m_isPracticeMode || !m_level) return;

    int levelID = m_level->m_levelID.value();

    // eligibility comes from saved progress (same rule as the island locks), not from how the level was opened,
    // so replays after exiting, vault entries etc. all count
    auto* manager = CustomGauntletManager::get();
    if (!manager->isCustomGauntletLevel(levelID)) return;
    if (manager->isLevelRewardClaimed(levelID)) return;
    if (!manager->isLevelUnlocked(levelID)) return;

    int reward = CustomGauntletManager::crystalsForLevel(m_level);
    manager->markLevelRewardClaimed(levelID);
    manager->addCrystals(reward);

    // flush now so a crash before the game's normal save doesn't lose the claim
    if (auto res = Mod::get()->saveData(); res.isErr()) {
        log::warn("Failed to save claimed level reward: {}", res.unwrapErr());
    }
    GRCurrencyRewardLayer::queueCrystalReward(reward);

    StatsSyncManager::get()->sync(manager->getCrystalTotal(), manager->getCoinTotal());

    manager->markLevelSyncPending(levelID);
    StatsSyncManager::get()->completeLevel(levelID, [manager, levelID](bool success, std::string const& error) {
        if (!success) {
            log::warn("Level completion sync failed, will retry later: {}", error);
            return;
        }
        manager->clearLevelSyncPending(levelID);

        auto* gauntlet = manager->findGauntletForLevel(levelID);
        if (!gauntlet || !manager->isGauntletFullyCompleted(*gauntlet)) return;
        if (manager->isGauntletRewardSynced(gauntlet->id)) return;

        int gauntletId = gauntlet->id;
        manager->markGauntletSyncPending(gauntletId);
        StatsSyncManager::get()->completeGauntlet(
            gauntletId,
            [gauntletId](bool success, int rewardCoins, std::string const& error) {
                if (!success) {
                    log::warn("Gauntlet completion sync failed, will retry later: {}", error);
                    return;
                }
                auto* manager = CustomGauntletManager::get();
                manager->clearGauntletSyncPending(gauntletId);
                manager->markGauntletRewardSynced(gauntletId);
                manager->addCoins(rewardCoins);
                manager->markPendingGauntletReward(gauntletId, rewardCoins);
            }
        );
    });
}
