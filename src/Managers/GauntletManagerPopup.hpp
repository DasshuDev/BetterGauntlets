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
    CCMenu*        m_tabMenu       = nullptr;
    LoadingCircle* m_loadingCircle = nullptr;

    int   m_activeTab = 0; // 0 = Published, 1 = Staged
    float m_tabBarY   = 0.f;

    async::TaskHolder<geode::Result<std::string>> m_argonHolder;
    async::TaskHolder<web::WebResponse> m_fetchHolder;
    async::TaskHolder<web::WebResponse> m_deleteHolder;
    async::TaskHolder<web::WebResponse> m_pushHolder;
    async::TaskHolder<web::WebResponse> m_featureHolder;
    std::vector<async::TaskHolder<web::WebResponse>> m_rowIconHolders;

    void buildPanelView();
    void buildTabMenu();
    void onSwitchTab(int tab);
    void buildGauntletList();
    void buildGauntletRow(CustomGauntletData const& g);
    void buildStagedRow(GauntletEditData const& g, int index);
    void loadRowIcon(CCNode* iconNode, std::string const& iconURL);
    CCMenuItemToggler* createFeatureToggle(bool featured, int gauntletId);
    void startArgonAuth();
    void fetchGauntlets();
    void saveStaged();
    void loadStaged();
    void onCreateNew(CCObject* sender);
    void onEdit(int gauntletId);
    void onEditStaged(int index);
    void onPushStaged(int index);
    void onDelete(int gauntletId);
    void onDeleteStaged(int index);
    void onToggleFeatured(int gauntletId);

public:
    static GauntletManagerPopup* create();
};