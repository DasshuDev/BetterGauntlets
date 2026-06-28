#include <Geode/Geode.hpp>
#include <Geode/modify/GauntletLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "../GauntletLayer/GauntletLayer.hpp"

using namespace geode::prelude;

static GauntletType findGauntletTypeForLevel(int levelID) {
    auto savedGauntlets = GameLevelManager::get()->m_savedGauntlets;
    if (!savedGauntlets) return GauntletType::Fire;
    auto keys = savedGauntlets->allKeys();
    if (!keys) return GauntletType::Fire;

    for (auto* obj : CCArrayExt<CCString*>(keys)) {
        auto pack = static_cast<GJMapPack*>(
            savedGauntlets->objectForKey(obj->getCString())
        );
        if (!pack || !pack->m_levels) continue;
        for (auto* lvlObj : CCArrayExt<CCString*>(pack->m_levels)) {
            if (lvlObj->intValue() == levelID)
                return static_cast<GauntletType>(pack->m_packID);
        }
    }
    return GauntletType::Fire;
}

class $modify(GauntletLayerRedirect, GauntletLayer) {
    static CCScene* scene(GauntletType type) {
        log::info("[GauntletRedirect] GauntletLayer::scene({})", (int)type);
        auto s = BetterGauntletLayer::scene(type);
        if (s) return s;
        return GauntletLayer::scene(type);
    }

    bool init(GauntletType type) {
        log::info("[GauntletRedirect] GauntletLayer::init({})", (int)type);
        if (!GauntletLayer::init(type)) return false;
        auto t = type;
        Loader::get()->queueInMainThread([t]() {
            auto s = BetterGauntletLayer::scene(t);
            if (s) CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f, s));
        });
        return true;
    }
};

class $modify(GauntletLevelInfoLayer, LevelInfoLayer) {
    void onBack(CCObject* sender) {
        log::info("[GauntletRedirect] LevelInfoLayer::onBack gauntlet={}",
            m_level ? m_level->m_gauntletLevel : false);

        if (m_level && m_level->m_gauntletLevel) {
            auto type = findGauntletTypeForLevel(m_level->m_levelID.value());
            log::info("[GauntletRedirect] Redirecting to type {}", (int)type);
            auto scene = BetterGauntletLayer::scene(type);
            if (scene) {
                CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f, scene));
                return;
            }
        }
        LevelInfoLayer::onBack(sender);
    }
};
