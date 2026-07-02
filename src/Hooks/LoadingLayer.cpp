#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include "../Managers/GauntletManagerCache.hpp"

using namespace geode::prelude;

class $modify(GRLoadingLayer, LoadingLayer) {
    bool init(bool refresh) {
        if (!LoadingLayer::init(refresh)) return false;

        // Warm the manager cache as early as possible so the gauntlet select
        // layer and profile page don't have to wait on /ismanager once the
        // local player actually gets there.
        auto accountID = GJAccountManager::get()->m_accountID;
        GauntletManagerCache::get()->isManager(accountID, [accountID](bool isManager) {
            log::info("[GauntletRedesign] Account {} is {}a Gauntlet Manager", accountID, isManager ? "" : "not ");
        });

        return true;
    }
};
