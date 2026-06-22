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

    // Members
    alpha::ui::AdvancedScrollLayer* m_customScrollLayer = nullptr;
    alpha::ui::AdvancedScrollBar* m_customScrollBar = nullptr;
    CCMenu* m_gauntletBtnContainer = nullptr;
    CCLabelBMFont* m_sliderLabel = nullptr;
    CCSprite* m_vanillaTitle = nullptr;
    CCSprite* m_betterTitle = nullptr;
    CCNode* m_loadingCircle = nullptr;
    CCMenuItemSpriteExtra* m_refreshButton = nullptr;

    bool m_showingCustomList = false;
    bool m_exiting = false;
    cocos2d::CCArray* m_gauntletPacks = nullptr;

    async::TaskHolder<Result<std::string>> m_listener;
    async::TaskHolder<web::WebResponse> m_managerCheckHolder;
    async::TaskHolder<web::WebResponse> m_fetchHolder;
};
