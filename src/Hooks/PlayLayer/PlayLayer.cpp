#include "PlayLayer.hpp"
#include "../CurrencyRewardLayer/CurrencyRewardLayer.hpp"
#include "../../Data/CustomGauntletManager.hpp"

using namespace geode::prelude;

namespace {

bool findGauntletLevelStars(int levelID, int& outStars) {
    for (auto const& gauntlet : CustomGauntletManager::get()->getCached()) {
        for (auto const& slot : gauntlet.levels) {
            if (slot.id == levelID) {
                outStars = slot.stars;
                return true;
            }
        }
    }
    return false;
}

}

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

    int stars = 0;
    if (!findGauntletLevelStars(levelID, stars)) return;

    auto* manager = CustomGauntletManager::get();
    if (manager->isLevelRewardClaimed(levelID)) return;

    int reward = stars * 2;
    manager->markLevelRewardClaimed(levelID);
    manager->addCrystals(reward);
    GRCurrencyRewardLayer::queueCrystalReward(reward);
}
