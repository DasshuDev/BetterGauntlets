#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/loader/Event.hpp>
#include <alphalaneous.alphas-ui-pack/include/API.hpp>
#include "../CustomGauntlets/CustomGauntletLayer.hpp"
#include "../CustomGauntlets/CustomGauntletNode.hpp"
#include "../Data/CustomGauntletManager.hpp"
#include "../GauntletLayer/GauntletLayer.hpp"

using namespace geode::prelude;

class BetterGauntletSelectLayer : public CCLayer, public LevelManagerDelegate {
public:
    static BetterGauntletSelectLayer* create();
    static CCScene* scene();
    ~BetterGauntletSelectLayer() override;

    inline static float s_scrollLocation = 0.f;
    inline static bool s_showCustomList = false;

protected:
    bool init() override;
    void keyBackClicked() override;
    void onEnterTransitionDidFinish() override;

    // LevelManagerDelegate
    void loadLevelsFinished(cocos2d::CCArray* levels, char const* key, int type) override;
    void loadLevelsFailed(char const* key, int type) override;

    // UI building
    void buildBackground();
    void buildDecorations();
    void buildMenus();
    void buildGauntletNodes(cocos2d::CCArray* gauntlets);
    void updateCrystalLabel();

    // Navigation
    void setupScrollMode();
    void styleGauntletButtons();

    // Scroll position
    void saveScrollPos();
    void loadScrollPos();

    // Callbacks
    void onBack(cocos2d::CCObject* sender);
    void onRefresh(cocos2d::CCObject* sender);
    void onPlay(cocos2d::CCObject* sender);
    void onNewInfo(cocos2d::CCObject* sender);
    void onDiscord(cocos2d::CCObject* sender);
    void toggleList(cocos2d::CCObject* sender);

    // Custom gauntlets
    void buildCustomList();
    void populateCustomList(std::vector<CustomGauntletData> const& gauntlets);
    void checkManagerStatus();

    // Members
    alpha::ui::AdvancedScrollLayer* m_customScrollLayer = nullptr;
    alpha::ui::AdvancedScrollBar* m_customScrollBar = nullptr;
    CCMenu* m_gauntletBtnContainer = nullptr;
    CCLabelBMFont* m_sliderLabel = nullptr;
    CCSprite* m_vanillaTitle = nullptr;
    CCSprite* m_betterTitle = nullptr;
    CCNode* m_loadingCircle = nullptr;
    CCMenuItemSpriteExtra* m_refreshButton = nullptr;
    CCLabelBMFont* m_crystalLabel = nullptr;

    // The server-hosted custom gauntlet list is built once (kicked off from
    // init(), same as the vanilla list) and kept alive for the lifetime of
    // this layer - toggling only flips visibility so icons/nodes aren't
    // re-fetched and re-created every time the player switches tabs.
    alpha::ui::AdvancedScrollLayer* m_customGauntletScrollLayer = nullptr;
    alpha::ui::AdvancedScrollBar* m_customGauntletScrollBar = nullptr;
    CCNode* m_customListLoadingCircle = nullptr;

    // Resolved once via checkManagerStatus() and added to bottom-left-menu
    // right away (never toggled) - flipping visibility after the fact broke
    // the menu's ColumnLayout, which only sizes/positions around whichever
    // children were visible the last time updateLayout() ran.
    CCMenuItemSpriteExtra* m_managerButton = nullptr;

    bool m_showingCustomList = false;
    bool m_exiting = false;
    cocos2d::CCArray* m_gauntletPacks = nullptr;

    async::TaskHolder<web::WebResponse> m_fetchHolder;
};
