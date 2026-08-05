#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <functional>

using namespace geode::prelude;

class StatsSyncManager {
public:
    static StatsSyncManager* get();

    using SyncCallback = std::function<void(bool success, std::string const& error)>;
    using CompleteCallback = std::function<void(bool success, int rewardCoins, std::string const& error)>;

    void sync(int crystals, int coins, SyncCallback callback = nullptr);
    void resetSelf(SyncCallback callback = nullptr);
    void completeGauntlet(int gauntletId, CompleteCallback callback = nullptr);

private:
    StatsSyncManager() = default;

    async::TaskHolder<geode::Result<std::string>> m_argonHolder;
    async::TaskHolder<web::WebResponse> m_pushHolder;

    async::TaskHolder<geode::Result<std::string>> m_resetArgonHolder;
    async::TaskHolder<web::WebResponse> m_resetHolder;

    async::TaskHolder<geode::Result<std::string>> m_completeArgonHolder;
    async::TaskHolder<web::WebResponse> m_completeHolder;
};
