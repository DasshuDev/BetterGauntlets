#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>

using namespace geode::prelude;

// Caches the full list of Gauntlet Manager account IDs from the Supernova
// backend (GET /managers) so callers (e.g. badges) don't need to hardcode
// account IDs and don't need a network round-trip per account checked.
class GauntletManagerCache {
public:
    static GauntletManagerCache* get();

    // Fetches the manager list if it hasn't been fetched (or isn't already
    // being fetched). Safe to call repeatedly - only ever fetches once.
    void warm();

    // Forces a fresh fetch of the manager list even if one already
    // succeeded, so newly added/removed managers are picked up without
    // restarting the game. No-op if a fetch is already in flight.
    void refresh();

    // Resolves whether accountID is a manager. Calls back synchronously if
    // the list is already cached, otherwise queues the callback until the
    // list finishes fetching (triggering that fetch if needed).
    void isManager(int accountID, std::function<void(bool)> callback);

private:
    GauntletManagerCache() = default;
    void fetch();

    std::unordered_set<int> m_managerIDs;
    bool m_hasFetched = false;
    std::vector<std::pair<int, std::function<void(bool)>>> m_waiting;
    async::TaskHolder<web::WebResponse> m_request;
};
