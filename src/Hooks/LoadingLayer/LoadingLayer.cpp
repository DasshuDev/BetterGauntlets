#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include "../../APIs/GauntletManagerCache.hpp"

using namespace geode::prelude;

class $modify(GRLoadingLayer, LoadingLayer) {
    bool init(bool refresh) {
        if (!LoadingLayer::init(refresh)) return false;

        GauntletManagerCache::get()->warm();

        auto accountID = GJAccountManager::get()->m_accountID;
        GauntletManagerCache::get()->isManager(accountID, [accountID](bool isManager) {
            log::info("Account {} is {}a Gauntlet Manager", accountID, isManager ? "" : "not ");
        });

        return true;
    }
};
