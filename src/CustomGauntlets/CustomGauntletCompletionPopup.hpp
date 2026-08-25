#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include "../Data/CustomGauntletData.hpp"
#include "../Hooks/Custom Hooks/ForceClosablePopup.hpp"

using namespace geode::prelude;

class CustomGauntletCompletionPopup : public Popup, public ForceClosablePopup, public CoinCrystalStylablePopup {
protected:
    bool init(CustomGauntletData const& data, int rewardCoins, CCTexture2D* iconTexture);
    void onClose(CCObject* sender) override;

public:
    static CustomGauntletCompletionPopup* create(CustomGauntletData const& data, int rewardCoins, CCTexture2D* iconTexture);
    void forceCloseFromReward(CCObject* sender) override { this->onClose(sender); }
};
