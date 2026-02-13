#ifndef GAUNTLETLAYER_HPP
#define GAUNTLETLAYER_HPP

#pragma once

#include <Geode/modify/GauntletLayer.hpp>
#include <Geode/ui/Popup.hpp>
#include <UIBuilder.hpp>

using namespace geode::prelude;

class $modify(RedesignedGauntletLayer, GauntletLayer) {
    struct Fields {
        bool m_loaded = false;
        CCMenu* m_levelsMenu = nullptr;
    };

    void gauntletLevel(int);
    cocos2d::CCNode* getChildBySpriteFrameNameRecursive(cocos2d::CCNode*, char const*);

    // #ifndef GEODE_IS_IOS
    //     void defineKeybind(const char*, std::function<void()>);
    // #endif

    void loadLevelsFinished(cocos2d::CCArray*, char const*, int) override;
    void editGauntlets();
    void editGauntletLayer(std::string, cocos2d::ccColor3B, cocos2d::ccColor3B, cocos2d::ccColor3B);
    void gauntletVault(cocos2d::CCObject* obj);
    void setupGauntlet(cocos2d::CCArray* levels);
    void setupInfo();
    void onInfo(cocos2d::CCObject* obj);
    void onLevelInfo(CCObject* sender);
    void onLocked(CCObject* sender);

    bool init(GauntletType);

    // Individual Gauntlet Callbacks
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
};
#endif