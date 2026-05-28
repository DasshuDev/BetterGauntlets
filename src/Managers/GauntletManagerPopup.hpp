#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/web.hpp>
// #include "GauntletManagerAPI.hpp"
#include "../Data/CustomGauntletData.hpp"

using namespace geode::prelude;

class GauntletManagerPopup : public Popup {
protected:
    bool init(float width, float height, char const* bg);

    std::vector<CustomGauntletData> m_gauntlets;
    CCLayer*       m_listLayer     = nullptr;
    CCLayer*       m_panelLayer    = nullptr;
    LoadingCircle* m_loadingCircle = nullptr;

    async::TaskHolder<geode::Result<std::string>> m_argonHolder;
    async::TaskHolder<web::WebResponse> m_fetchHolder;
    async::TaskHolder<web::WebResponse> m_deleteHolder;

    void buildPanelView();
    void buildGauntletList();
    void buildGauntletRow(CustomGauntletData const& g, float yPos);
    void startArgonAuth();
    void fetchGauntlets();
    void onCreateNew(CCObject* sender);
    void onEdit(int gauntletId);
    void onDelete(int gauntletId);

public:
    static GauntletManagerPopup* create();
};