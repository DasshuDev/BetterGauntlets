#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../Data/CustomGauntletData.hpp"

using namespace geode::prelude;

class BetterGauntletLayer : public CCLayer, public LevelManagerDelegate {
public:
    static BetterGauntletLayer* create(CustomGauntletData const& data);
    static CCScene* scene(CustomGauntletData const& data);

protected:
    bool init(CustomGauntletData const& data);

    CustomGauntletData              m_data;
    CCArray*                        m_loadedLevels = nullptr;
    CCMenu*                         m_levelsMenu   = nullptr;
    CCSprite*                       m_islandSpr    = nullptr;
    CCLabelBMFont*                  m_titleLabel   = nullptr;
    CCLabelBMFont*                  m_titleShadow  = nullptr;

    async::TaskHolder<web::WebResponse> m_iconHolder;
    async::TaskHolder<web::WebResponse> m_completionHolder;

    // Level loading
    void loadLevels();
    void loadLevelsFinished(CCArray* levels, char const* key) override;
    void loadLevelsFailed(char const* key, int p1) override;

    // UI builders
    void buildBackground();
    void buildCorners();
    void buildTitle();
    void buildLevelButtons(CCArray* levels);
    void loadIslandIcon();

    // Callbacks
    void onBack(CCObject* sender);
    void onLevel(CCObject* sender);
    void onLocked(CCObject* sender);
    void onInfo(CCObject* sender);
    void gauntletVault(CCObject* obj);
    void reportCompletion(int levelID, int stars);
};
