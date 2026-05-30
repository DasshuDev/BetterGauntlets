#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/web.hpp>
#include "../Data/CustomGauntletData.hpp"
#include "GauntletManagerAPI.hpp"

using namespace geode::prelude;

class GauntletManagerPopup : public Popup {
protected:
    bool init(float width, float height, char const* bg);

    std::vector<CustomGauntletData> m_gauntlets; // server-side
    std::vector<GauntletEditData>   m_staged;    // local staging area

    CCLayer*       m_listLayer     = nullptr;
    CCLayer*       m_panelLayer    = nullptr;
    NineSlice*     m_listBG        = nullptr;
    CCMenu*        m_gauntletList  = nullptr;
    LoadingCircle* m_loadingCircle = nullptr;
    CCMenu*        m_actionMenu    = nullptr;

    async::TaskHolder<geode::Result<std::string>> m_argonHolder;
    async::TaskHolder<web::WebResponse> m_fetchHolder;
    async::TaskHolder<web::WebResponse> m_deleteHolder;
    async::TaskHolder<web::WebResponse> m_pushHolder;

    void buildPanelView();
    void buildGauntletList();
    void buildGauntletRow(CustomGauntletData const& g);
    void buildStagedRow(GauntletEditData const& g, int index);
    void startArgonAuth();
    void fetchGauntlets();
    void onCreateNew(CCObject* sender);
    void onEdit(int gauntletId);
    void onEditStaged(int index);
    void onPushStaged(int index);
    void onDelete(int gauntletId);

public:
    static GauntletManagerPopup* create();
};