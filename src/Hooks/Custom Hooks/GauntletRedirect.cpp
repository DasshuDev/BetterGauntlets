#include <Geode/Geode.hpp>
#include <Geode/modify/GauntletLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "../../GauntletLayer/GauntletLayer.hpp"
#include "../../Data/CustomGauntletManager.hpp"

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
    bool init(GJGameLevel* level, bool challenge) {
        bool isCustomGauntletLevel = level && CustomGauntletManager::get()->isCustomGauntletLevel(
            level->m_levelID.value()
        );

        if (isCustomGauntletLevel) level->m_gauntletLevel2 = true;

        if (!LevelInfoLayer::init(level, challenge)) return false;

        if (isCustomGauntletLevel && m_starsIcon && m_starsLabel) {
            int crystalCount = CustomGauntletManager::crystalsForLevel(level);
            bool rated = level->m_stars.value() > 0;
            float yOffset = rated ? -15.f : 0.f;

            auto parent = m_starsIcon->getParent();
            if (parent) {
                auto crystalIcon = CCSprite::create("GR_crystal_001.png"_spr);
                crystalIcon->setID("crystal-icon"_spr);
                crystalIcon->setScale(0.25);
                crystalIcon->setAnchorPoint(m_starsIcon->getAnchorPoint());
                crystalIcon->setPosition(ccp(m_starsIcon->getPositionX(), m_starsIcon->getPositionY() + yOffset));
                parent->addChild(crystalIcon, m_starsIcon->getZOrder());

                auto crystalLabel = CCLabelBMFont::create(
                    fmt::format("{}", crystalCount).c_str(), "bigFont.fnt"
                );
                crystalLabel->setID("crystal-count"_spr);
                crystalLabel->setAnchorPoint(m_starsLabel->getAnchorPoint());
                crystalLabel->setScale(m_starsLabel->getScale());
                crystalLabel->setPosition(ccp(m_starsLabel->getPositionX(), m_starsLabel->getPositionY() + yOffset));
                parent->addChild(crystalLabel, m_starsLabel->getZOrder());

                repositionGauntletCoins();
            }
        }

        return true;
    }

    // LevelInfoLayer
    
    void levelDownloadFinished(GJGameLevel* level) {
        LevelInfoLayer::levelDownloadFinished(level);

        if (level && CustomGauntletManager::get()->isCustomGauntletLevel(level->m_levelID.value())) {
            repositionGauntletCoins();
        }
    }

    void repositionGauntletCoins() {
        if (!m_coins) return;
        for (auto* coin : CCArrayExt<CCNode*>(m_coins)) {
            if (!coin) continue;
            coin->setPositionY(coin->getPositionY() - 15.f);
        }
    }

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
