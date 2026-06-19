#pragma once

#include <Geode/utils/web.hpp>
#include <Geode/loader/Event.hpp>
#include "../CustomGauntlets/CustomGauntletLayer.hpp"
#include "../CustomGauntlets/CustomGauntletNode.hpp"
#include "../Data/CustomGauntletManager.hpp"
#include <Geode/modify/GauntletSelectLayer.hpp>
#include <alphalaneous.alphas-ui-pack/include/API.hpp>

using namespace geode::prelude;

class $modify(RedesignedGauntletSelectLayer, GauntletSelectLayer) {
    inline static float m_scrollLocation = 0.f;

    struct Fields {
        std::vector<CCMenuItemSpriteExtra*> m_dots = {};
        CCMenu* m_dotsMenu = nullptr;
        int currentGauntletPage = 0;
        bool showingCustomList = false;
        int m_dialogIndex = 0;
        int m_dialogSprite = 0;
        alpha::ui::AdvancedScrollLayer* m_customScrollLayer = nullptr;
        alpha::ui::AdvancedScrollBar* m_customScrollBar = nullptr;
        CCSprite* m_vanillaTitle = nullptr;
        CCSprite* m_betterTitle = nullptr;
        CCMenu* m_gauntletBtnContainer = nullptr;
        CCLabelBMFont* m_sliderLabel = nullptr;
        LoadingCircle* m_loadingCircle = nullptr;
        async::TaskHolder<Result<std::string>> m_listener;
        async::TaskHolder<web::WebResponse> m_managerCheckHolder;
        async::TaskHolder<web::WebResponse> m_fetchHolder;
    };

    // Pre-setup
    cocos2d::CCNode* getChildBySpriteFrameNameRecursive(cocos2d::CCNode* parent, char const* name);
    bool init(int gauntletType);
    void onLock(CCObject* sender);
    void onBack(CCObject* sender);
    void onNewInfo(CCObject* sender);
    void onDiscord(CCObject* sender);
    void toggleList(CCObject* sender);
    void startAuth();
    void buildCustomList();
    void populateCustomList(std::vector<CustomGauntletData> const& gauntlets);

    // Level loading
    void updateDots();
    void loadLevelsFinished(CCArray* gauntlets, char const* key, int type);
    void loadLevelsFailed(char const* key, int type);
    void onRefresh(CCObject* sender);

    // Layer navigation
    void setupNavigation();
    void setupScrollMode();
    void setupDotMode();
    void saveScrollPos();
    void loadScrollPos();
    void styleGauntletButtons();
    void onDot(CCObject* sender);
    void findCurrentGauntletPageUsing(CCArray* pageButtons);

    // Gauntlet opening
    void pressGauntlet(int desiredGauntlet);
    void onPlay(CCObject* sender);

    // Android? I think?
    #ifndef GEODE_IS_ANDROID
    void scrollLayerWillScrollToPage(BoomScrollLayer* p0, int p1);
    void scrollLayerScrolledToPage(BoomScrollLayer* p0, int p1);
    #else
    void updateArrows();
    #endif
    
};