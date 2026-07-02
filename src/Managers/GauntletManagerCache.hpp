#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>

using namespace geode::prelude;

// Caches "is this account a Gauntlet Manager" lookups against the Supernova
// backend so callers (e.g. badges) don't need to hardcode account IDs and
// don't spam /ismanager for the same account more than once.
class GauntletManagerCache {
public:
    static GauntletManagerCache* get();

    // Resolves whether accountID is a manager. Calls back synchronously if
    // already cached, otherwise queues the callback until the pending
    // request for that account (spawned automatically) completes.
    void isManager(int accountID, std::function<void(bool)> callback);

private:
    GauntletManagerCache() = default;

    std::unordered_map<int, bool> m_cache;
    std::unordered_map<int, std::vector<std::function<void(bool)>>> m_waiting;
    std::unordered_map<int, async::TaskHolder<web::WebResponse>> m_requests;
};
