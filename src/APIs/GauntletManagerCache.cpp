#include "GauntletManagerCache.hpp"
#include "GauntletManagerAPI.hpp"

GauntletManagerCache* GauntletManagerCache::get() {
    static GauntletManagerCache instance;
    return &instance;
}

GauntletManagerCache::GauntletManagerCache() {
    m_managers.jsonKey   = "managers";
    m_managers.label     = "manager";

    m_helpers.jsonKey    = "helpers";
    m_helpers.label      = "helper";

    m_supporters.jsonKey = "supporters";
    m_supporters.label   = "supporter";
}

void GauntletManagerCache::fetchRoles() {
    if (m_isFetching) return;
    m_isFetching = true;

    m_request.spawn(
        GauntletManagerAPI::get()->fetchRoles(),
        [this](web::WebResponse res) {
            m_isFetching = false;
            auto json = res.ok() ? res.json().unwrapOr(matjson::Value::object()) : matjson::Value::object();

            for (auto* cache : { &m_managers, &m_helpers, &m_supporters }) {
                if (res.ok()) {
                    std::unordered_set<int> ids;
                    if (json.contains(cache->jsonKey) && json[cache->jsonKey].isArray()) {
                        for (auto const& row : json[cache->jsonKey]) {
                            if (auto id = row["account_id"].asInt(); id.isOk()) {
                                ids.insert(id.unwrap());
                            }
                        }
                    }
                    cache->ids = std::move(ids);
                    cache->hasFetched = true;
                } else {
                    log::error("Failed to fetch {} list ({})", cache->label, res.code());
                }

                auto waiting = std::move(cache->waiting);
                cache->waiting.clear();
                for (auto& [accountID, cb] : waiting) {
                    cb(cache->ids.contains(accountID));
                }
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
    fetchRoles();
}

void GauntletManagerCache::warm() {
    if (!m_managers.hasFetched || !m_helpers.hasFetched || !m_supporters.hasFetched) fetchRoles();
    m_lastRefresh = std::chrono::steady_clock::now();
}

void GauntletManagerCache::refresh() {
    auto now = std::chrono::steady_clock::now();
    if (now - m_lastRefresh < std::chrono::minutes(5)) return;
    m_lastRefresh = now;

    fetchRoles();
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
