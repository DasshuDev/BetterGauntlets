#ifndef GAUNTLETLAYER_HPP
#define GAUNTLETLAYER_HPP

#pragma once

#include <Geode/modify/GauntletLayer.hpp>
#include <Geode/ui/Popup.hpp>
#include <UIBuilder.hpp>
#include <chrono>

using namespace geode::prelude;

class $modify(RedesignedGauntletLayer, GauntletLayer) {
    struct Fields {
        bool m_loaded = false;
        CCMenu* m_levelsMenu = nullptr;
        // Drag states
        bool m_dragging = false;
        cocos2d::CCPoint m_touchStartLoc;
        cocos2d::CCPoint m_touchLastLoc;
        cocos2d::CCPoint m_menuStartPos;
        // Velocity tracking
        std::chrono::steady_clock::time_point m_touchLastTime;
        cocos2d::CCPoint m_velocity;
        bool m_flinging = false;
        float m_deceleration = 2000.0f;
        // Padding
        float m_padding = 50.f;
        float m_paddingExtra = 100.f;
        float m_minY = 0.f;
        float m_minX = 0.f;
        // Background parallax
        cocos2d::CCSprite* m_bgSprite = nullptr;
        cocos2d::CCSprite* m_bgSprite2 = nullptr;
        cocos2d::CCPoint m_bgStartPos = ccp(0, 0);
        cocos2d::CCPoint m_menuOriginPos = ccp(0, 0);
        float m_parallax = 0.25f;
    };

    void gauntletLevel(int);
    cocos2d::CCNode* getChildBySpriteFrameNameRecursive(cocos2d::CCNode*, char const*);

    #ifndef GEODE_IS_IOS
        void defineKeybind(const char*, std::function<void()>);
    #endif

    void loadLevelsFinished(cocos2d::CCArray*, char const*, int);
    void editGauntlets();
    void editGauntletLayer(std::string, cocos2d::ccColor3B, cocos2d::ccColor3B, cocos2d::ccColor3B);
    void gauntletVault(cocos2d::CCObject* obj);
    void setupGauntlet(cocos2d::CCArray* levels);
    void setupInfo();
    void onInfo(cocos2d::CCObject* obj);
    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void onLevelInfo(CCObject* sender);
    void onLocked(CCObject* sender);
    void updateParallax(CCPoint const& menuPos);

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