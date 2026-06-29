#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../Data/CustomGauntletData.hpp"

using namespace geode::prelude;

using GauntletTapCallback = std::function<void(CustomGauntletData const&)>;

class CustomGauntletNode : public CCMenuItemSpriteExtra {
struct Fields {

};
public:
    static CustomGauntletNode* create(
        CustomGauntletData const& data,
        GauntletTapCallback       callback
    );

private:
    CustomGauntletData  m_data;
    GauntletTapCallback m_callback;

    CCNode* m_rewardNode = nullptr;
    CCSprite* m_chestSpr = nullptr;
    CCSprite* m_chestSprShadow = nullptr;
    CCLabelBMFont* m_rewardLabel = nullptr;
    CCLabelBMFont* m_rewardLabelShadow = nullptr;

    async::TaskHolder<web::WebResponse> m_iconHolder;

    bool init(CustomGauntletData const& data, GauntletTapCallback callback);
    void onTap(CCObject* sender);
    void loadIcon();
};
