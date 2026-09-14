#include "PlayLayer.hpp"
#include "../CurrencyRewardLayer/CurrencyRewardLayer.hpp"
#include "../../Data/CustomGauntletManager.hpp"
#include "../../Managers/StatsSyncManager.hpp"

using namespace geode::prelude;

bool GRPlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
    if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

    int levelID = level ? level->m_levelID.value() : 0;
    m_fields->m_isGauntletAttempt =
        CustomGauntletManager::get()->consumePendingGauntletAttempt(levelID);

    return true;
}

void GRPlayLayer::levelComplete() {
    PlayLayer::levelComplete();

    if (!m_fields->m_isGauntletAttempt) return;
    if (m_isPracticeMode || !m_level) return;

    int levelID = m_level->m_levelID.value();

    auto* manager = CustomGauntletManager::get();
    if (!manager->isCustomGauntletLevel(levelID)) return;
    if (manager->isLevelRewardClaimed(levelID)) return;

    int reward = CustomGauntletManager::crystalsForLevel(m_level);
    manager->markLevelRewardClaimed(levelID);
    manager->addCrystals(reward);
    GRCurrencyRewardLayer::queueCrystalReward(reward);

    StatsSyncManager::get()->sync(manager->getCrystalTotal(), 0);

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
