#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../GauntletInfo/GauntletInfo.hpp"

using namespace geode::prelude;

class BetterGauntletLayer : public CCLayer, public LevelManagerDelegate, public DialogDelegate {
public:
    static BetterGauntletLayer* create(GauntletType type);
    static CCScene* scene(GauntletType type);
    ~BetterGauntletLayer() override;

    // Core data - named to match what GauntletEdits expect
    GauntletType m_gauntletType = GauntletType::Fire;
    CCSprite* m_backgroundSprite = nullptr;
    CCArray* m_levels = nullptr;

    // UI
    CCMenu* m_levelsMenu = nullptr;
    CCSprite* m_lockSprite = nullptr;
    bool m_loaded = false;
    bool m_exiting = false;
    std::vector<bool> m_lockedStates;

protected:
    bool init(GauntletType type);
    void onEnter() override;
    void keyBackClicked() override;

    // LevelManagerDelegate (3-param overload used by GD gauntlet fetching)
    void loadLevelsFinished(cocos2d::CCArray* levels, char const* key, int type) override;
    void loadLevelsFailed(char const* key, int type) override;

    // Setup
    void setupGauntlet(cocos2d::CCArray* levels);
    void editGauntlets();
    void checkForUnlocks();
    void checkGauntletCompletion();
    bool isGauntletFullyComplete();
    void doExit();
    void dialogClosed(DialogLayer* layer) override;
    void playUnlockAnimation(cocos2d::CCNode* levelSpr, int index);
    void playUnlockParticlesIn(cocos2d::CCNode* levelSpr, int index);
    void playUnlockParticlesOut(cocos2d::CCNode* levelSpr, int index);
    void islandShake(cocos2d::CCSprite* islandSpr, float duration);
    cocos2d::CCFiniteTimeAction* generateShakeAction(cocos2d::CCPoint originalPos, float xyOffset, float duration);
    void onUnlockPeak(cocos2d::CCNode* sender);
    void editGauntletLayer(std::string title, cocos2d::ccColor3B bgColor, cocos2d::ccColor3B titleColor, cocos2d::ccColor3B highlightColor);
    void setupInfo();

    // Callbacks
    void onBack(cocos2d::CCObject* sender);
    void onLevel(cocos2d::CCObject* sender);
    void onLocked(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void gauntletVault(cocos2d::CCObject* sender);
    void onDebugReward(cocos2d::CCObject* sender);

    // Per-gauntlet theme functions 
    void editGauntletFallback();
    void editFireGauntlet();
    void editIceGauntlet();
    void editPoisonGauntlet();
    void editShadowGauntlet();
    void editLavaGauntlet();
    void editBonusGauntlet();
    void editChaosGauntlet();
    void editDemonGauntlet();
    void editTimeGauntlet();
    void editCrystalGauntlet();
    void editMagicGauntlet();
    void editSpikeGauntlet();
    void editMonsterGauntlet();
    void editDoomGauntlet();
    void editDeathGauntlet();
    void editForestGauntlet();
    void editRuneGauntlet();
    void editForceGauntlet();
    void editSpookyGauntlet();
    void editDragonGauntlet();
    void editWaterGauntlet();
    void editHauntedGauntlet();
    void editAcidGauntlet();
    void editWitchGauntlet();
    void editPowerGauntlet();
    void editPotionGauntlet();
    void editSnakeGauntlet();
    void editToxicGauntlet();
    void editHalloweenGauntlet();
    void editTreasureGauntlet();
    void editGhostGauntlet();
    void editSpiderGauntlet();
    void editGemGauntlet();
    void editInfernoGauntlet();
    void editPortalGauntlet();
    void editStrangeGauntlet();
    void editFantasyGauntlet();
    void editChristmasGauntlet();
    void editSurpriseGauntlet();
    void editMysteryGauntlet();
    void editCursedGauntlet();
    void editCyborgGauntlet();
    void editCastleGauntlet();
    void editGraveGauntlet();
    void editTempleGauntlet();
    void editWorldGauntlet();
    void editGalaxyGauntlet();
    void editUniverseGauntlet();
    void editDiscordGauntlet();
    void editSplitGauntlet();
    void editNCSGauntlet1();
    void editNCSGauntlet2();
    void editSpaceGauntlet();
    void editCosmosGauntlet();
    void editRandomGauntlet();
    void editChanceGauntlet();
    void editCinemaGauntlet();
    void editFutureGauntlet();
    void editUtopiaGauntlet();
    void editLoveGauntlet();
    // void editDualityGauntlet();
    // void editParadoxGauntlet();
};
