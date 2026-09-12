#include "GauntletManagerCache.hpp"
#include "GauntletManagerAPI.hpp"

GauntletManagerCache* GauntletManagerCache::get() {
    static GauntletManagerCache instance;
    return &instance;
}

GauntletManagerCache::GauntletManagerCache() {
    m_managers.fetch     = [] { return GauntletManagerAPI::get()->fetchManagers(); };
    m_managers.jsonKey   = "managers";
    m_managers.label     = "manager";

    m_helpers.fetch      = [] { return GauntletManagerAPI::get()->fetchHelpers(); };
    m_helpers.jsonKey    = "helpers";
    m_helpers.label      = "helper";

    m_supporters.fetch   = [] { return GauntletManagerAPI::get()->fetchSupporters(); };
    m_supporters.jsonKey = "supporters";
    m_supporters.label   = "supporter";
}

void GauntletManagerCache::fetchRole(RoleCache& cache) {
    if (cache.request.isPending()) return;

    cache.request.spawn(
        cache.fetch(),
        [&cache](web::WebResponse res) {
            if (res.ok()) {
                std::unordered_set<int> ids;
                auto json = res.json().unwrapOr(matjson::Value::object());
                if (json.contains(cache.jsonKey) && json[cache.jsonKey].isArray()) {
                    for (auto const& row : json[cache.jsonKey]) {
                        if (auto id = row["account_id"].asInt(); id.isOk()) {
                            ids.insert(id.unwrap());
                        }
                    }
                }
                cache.ids = std::move(ids);
                cache.hasFetched = true;
            } else {
                log::error("Failed to fetch {} list ({})", cache.label, res.code());
            }

            auto waiting = std::move(cache.waiting);
            cache.waiting.clear();
            for (auto& [accountID, cb] : waiting) {
                cb(cache.ids.contains(accountID));
            }
        }
    );
}

void GauntletManagerCache::checkRole(RoleCache& cache, int accountID, std::function<void(bool)> callback) {
    if (cache.hasFetched) {
        callback(cache.ids.contains(accountID));
        return;
    }

    cache.waiting.push_back({accountID, std::move(callback)});
    fetchRole(cache);
}

void GauntletManagerCache::warm() {
    if (!m_managers.hasFetched) fetchRole(m_managers);
    if (!m_helpers.hasFetched) fetchRole(m_helpers);
    if (!m_supporters.hasFetched) fetchRole(m_supporters);
}

void GauntletManagerCache::refresh() {
    fetchRole(m_managers);
    fetchRole(m_helpers);
    fetchRole(m_supporters);
}

void GauntletManagerCache::isManager(int accountID, std::function<void(bool)> callback) {
    checkRole(m_managers, accountID, std::move(callback));
}

void GauntletManagerCache::isHelper(int accountID, std::function<void(bool)> callback) {
    checkRole(m_helpers, accountID, std::move(callback));
}

void GauntletManagerCache::isSupporter(int accountID, std::function<void(bool)> callback) {
    checkRole(m_supporters, accountID, std::move(callback));
}
