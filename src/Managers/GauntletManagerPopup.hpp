#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "GauntletManagerAPI.hpp"

using namespace geode::prelude;

class GauntletManagerPopup : public FLAlertLayer, public TextInputDelegate {
public:
    static GauntletManagerPopup* create();
    bool init();

private:
    std::vector<CustomGauntletData> m_gauntlets;
    CCLayer*       m_listLayer     = nullptr;
    LoadingCircle* m_loadingCircle = nullptr;

    CCTextInputNode* m_usernameInput = nullptr;
    CCTextInputNode* m_passwordInput = nullptr;
    CCLayer*         m_loginLayer    = nullptr;
    CCLayer*         m_panelLayer    = nullptr;

    async::TaskHolder<web::WebResponse> m_loginHolder;
    async::TaskHolder<web::WebResponse> m_fetchHolder;
    async::TaskHolder<web::WebResponse> m_deleteHolder;

    void buildLoginView();
    void buildPanelView();
    void buildGauntletList();
    void buildGauntletRow(CustomGauntletData const& g, float yPos);

    void onLogin(CCObject* sender);
    void onClose(CCObject* sender);
    void onCreateNew(CCObject* sender);
    void onEdit(int gauntletId);
    void onDelete(int gauntletId);
    void fetchGauntlets();
};