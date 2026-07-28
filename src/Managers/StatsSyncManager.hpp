#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <functional>

using namespace geode::prelude;

// Pushes the player's crystal/coin totals to the leaderboard backend.
// A singleton (rather than living on a transient layer like PlayLayer) so the
// in-flight request isn't cancelled if the player leaves the scene right away.
class StatsSyncManager {
public:
    static StatsSyncManager* get();

    using SyncCallback = std::function<void(bool success, std::string const& error)>;

    // Calls back with success=false (and a reason) if the player isn't signed
    // into a GD account, auth fails, or the push request fails.
    void sync(int crystals, int coins, SyncCallback callback = nullptr);

private:
    StatsSyncManager() = default;

    async::TaskHolder<geode::Result<std::string>> m_argonHolder;
    async::TaskHolder<web::WebResponse> m_pushHolder;
};
