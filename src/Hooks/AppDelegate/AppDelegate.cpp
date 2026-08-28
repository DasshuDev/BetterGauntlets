#include <Geode/Geode.hpp>
#include <Geode/modify/AppDelegate.hpp>
#include "../../Managers/StatsSyncManager.hpp"
#include "../../Data/CustomGauntletManager.hpp"

using namespace geode::prelude;

class $modify(GRAppDelegate, AppDelegate) {
    void applicationDidEnterBackground() {
        AppDelegate::applicationDidEnterBackground();

        auto* mgr = CustomGauntletManager::get();
        StatsSyncManager::get()->sync(mgr->getCrystalTotal(), mgr->getCoinTotal());
    }
};
