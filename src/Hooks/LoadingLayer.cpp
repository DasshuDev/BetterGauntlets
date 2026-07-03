#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include "../Managers/GauntletManagerCache.hpp"

using namespace geode::prelude;

class $modify(GRLoadingLayer, LoadingLayer) {
    bool init(bool refresh) {
        if (!LoadingLayer::init(refresh)) return false;

        // Warm the manager cache as early as possible with the full manager
        // list, so every profile/comment badge check anywhere resolves
        // instantly instead of needing its own network round-trip.
        GauntletManagerCache::get()->warm();

        auto accountID = GJAccountManager::get()->m_accountID;
        GauntletManagerCache::get()->isManager(accountID, [accountID](bool isManager) {
            log::info("[GauntletRedesign] Account {} is {}a Gauntlet Manager", accountID, isManager ? "" : "not ");
        });

        return true;
    }
};
