#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../Data/CustomGauntletData.hpp"

using namespace geode::prelude;

class CustomGauntletLayer : public CCLayer, public LevelManagerDelegate {
public:
    static CustomGauntletLayer* create(CustomGauntletData const& data);
    static CCScene* scene(CustomGauntletData const& data);

protected:
    bool init(CustomGauntletData const& data);
    void onEnter() override;

    CustomGauntletData              m_data;
    CCArray*                        m_loadedLevels = nullptr;
    CCMenu*                         m_levelsMenu   = nullptr;
    CCSprite*                       m_islandSpr    = nullptr;
    CCLabelBMFont*                  m_titleLabel   = nullptr;
    CCLabelBMFont*                  m_titleShadow  = nullptr;
    std::vector<bool>               m_lockedStates;

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
    void checkForUnlocks();
    void playUnlockAnimation(CCNode* levelSpr, int index);
    void playUnlockParticlesIn(CCNode* levelSpr, int index);
    void playUnlockParticlesOut(CCNode* levelSpr, int index);
    void islandShake(CCSprite* islandSpr, float duration);
    CCFiniteTimeAction* generateShakeAction(CCPoint originalPos, float xyOffset, float duration);
    void onUnlockPeak(CCNode* sender);

    // Callbacks
    void onBack(CCObject* sender);
    void onLevel(CCObject* sender);
    void onLocked(CCObject* sender);
    void onInfo(CCObject* sender);
    void gauntletVault(CCObject* obj);
    void reportCompletion(int levelID, int stars);
};
