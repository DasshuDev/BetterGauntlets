#include "GauntletManagerCache.hpp"
#include "GauntletManagerAPI.hpp"

GauntletManagerCache* GauntletManagerCache::get() {
    static GauntletManagerCache instance;
    return &instance;
}

void GauntletManagerCache::fetch() {
    if (m_request.isPending()) return;

    m_request.spawn(
        GauntletManagerAPI::get()->fetchManagers(),
        [this](web::WebResponse res) {
            if (res.ok()) {
                std::unordered_set<int> ids;
                auto json = res.json().unwrapOr(matjson::Value::object());
                if (json.contains("managers") && json["managers"].isArray()) {
                    for (auto const& row : json["managers"]) {
                        if (auto id = row["account_id"].asInt(); id.isOk()) {
                            ids.insert(id.unwrap());
                        }
                    }
                }
                m_managerIDs = std::move(ids);
                m_hasFetched = true;
            } else {
                log::warn(
                    "[GauntletManagerCache] Failed to fetch manager list (code {}): {}",
                    res.code(), res.string().unwrapOr("<no body>")
                );
            }

            auto waiting = std::move(m_waiting);
            m_waiting.clear();
            for (auto& [accountID, cb] : waiting) {
                cb(m_managerIDs.contains(accountID));
            }
        }
    );
}

void GauntletManagerCache::warm() {
    if (m_hasFetched) return;
    fetch();
}

void GauntletManagerCache::refresh() {
    fetch();
}

void GauntletManagerCache::isManager(int accountID, std::function<void(bool)> callback) {
    if (m_hasFetched) {
        callback(m_managerIDs.contains(accountID));
        return;
    }

    m_waiting.push_back({accountID, std::move(callback)});
    fetch();
}
