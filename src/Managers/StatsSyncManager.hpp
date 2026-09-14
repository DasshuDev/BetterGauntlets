#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <functional>
#include <chrono>
#include <vector>

using namespace geode::prelude;

class StatsSyncManager {
public:
    static StatsSyncManager* get();

    using SyncCallback = std::function<void(bool success, std::string const& error)>;
    using CompleteCallback = std::function<void(bool success, int rewardCoins, std::string const& error)>;

    void sync(int crystals, int coins, SyncCallback callback = nullptr);
    void resetSelf(SyncCallback callback = nullptr);
    void completeGauntlet(int gauntletId, CompleteCallback callback = nullptr);
    void completeLevel(int levelId, SyncCallback callback = nullptr);

    // Re-attempts any level/gauntlet completion pushes that were marked locally but never
    // confirmed synced (see CustomGauntletManager's pending-sync tracking). Safe to call
    // repeatedly - each entry only clears once its push actually succeeds. Should only be
    // called once the gauntlet cache is ready, since resolving a level's gauntlet requires it.
    void retryPendingCompletions();

private:
    StatsSyncManager() = default;

    // Sequential (one at a time - completeLevel/completeGauntlet each reuse a single shared
    // TaskHolder, so firing them concurrently would cancel all but the last in-flight request)
    // walk over a pending-sync list, retrying each entry before moving to the next.
    void retryPendingLevelSync(std::vector<int> pending, size_t index);
    void retryPendingGauntletSync(std::vector<int> pending, size_t index);

    async::TaskHolder<geode::Result<std::string>> m_argonHolder;
    async::TaskHolder<web::WebResponse> m_pushHolder;

    async::TaskHolder<geode::Result<std::string>> m_resetArgonHolder;
    async::TaskHolder<web::WebResponse> m_resetHolder;

    async::TaskHolder<geode::Result<std::string>> m_completeArgonHolder;
    async::TaskHolder<web::WebResponse> m_completeHolder;

    async::TaskHolder<geode::Result<std::string>> m_completeLevelArgonHolder;
    async::TaskHolder<web::WebResponse> m_completeLevelHolder;

    std::chrono::steady_clock::time_point m_lastSync = std::chrono::steady_clock::time_point::min();
};
