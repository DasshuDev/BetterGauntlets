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
}
