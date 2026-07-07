#include "EndLevelLayer.hpp"
#include "../CurrencyRewardLayer/CurrencyRewardLayer.hpp"

using namespace geode::prelude;

void GREndLevelLayer::customSetup() {
    EndLevelLayer::customSetup();

    int reward = GRCurrencyRewardLayer::consumePendingCrystalReward();
    if (reward <= 0) return;

    auto self = Ref<EndLevelLayer>(this);
    queueInMainThread([self, reward]() {
        GRCurrencyRewardLayer::showCrystalReward(self.data(), reward);
    });
}
