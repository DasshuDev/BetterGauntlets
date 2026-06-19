#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../Data/CustomGauntletData.hpp"

using namespace geode::prelude;

using GauntletTapCallback = std::function<void(CustomGauntletData const&)>;

class CustomGauntletNode : public CCMenuItemSpriteExtra {
public:
    static CustomGauntletNode* create(
        CustomGauntletData const& data,
        GauntletTapCallback       callback
    );

private:
    CustomGauntletData  m_data;
    GauntletTapCallback m_callback;

    async::TaskHolder<web::WebResponse> m_iconHolder;

    bool init(CustomGauntletData const& data, GauntletTapCallback callback);
    void onTap(CCObject* sender);
    void loadIcon();
};
