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

void GauntletManagerCache::fetchHelpers() {
    if (m_helperRequest.isPending()) return;

    m_helperRequest.spawn(
        GauntletManagerAPI::get()->fetchHelpers(),
        [this](web::WebResponse res) {
            if (res.ok()) {
                std::unordered_set<int> ids;
                auto json = res.json().unwrapOr(matjson::Value::object());
                if (json.contains("helpers") && json["helpers"].isArray()) {
                    for (auto const& row : json["helpers"]) {
                        if (auto id = row["account_id"].asInt(); id.isOk()) {
                            ids.insert(id.unwrap());
                        }
                    }
                }
                m_helperIDs = std::move(ids);
                m_hasFetchedHelpers = true;
            } else {
                log::warn(
                    "[GauntletManagerCache] Failed to fetch helper list (code {}): {}",
                    res.code(), res.string().unwrapOr("<no body>")
                );
            }

            auto waiting = std::move(m_waitingHelpers);
            m_waitingHelpers.clear();
            for (auto& [accountID, cb] : waiting) {
                cb(m_helperIDs.contains(accountID));
            }
        }
    );
}

void GauntletManagerCache::fetchSupporters() {
    if (m_supporterRequest.isPending()) return;

    m_supporterRequest.spawn(
        GauntletManagerAPI::get()->fetchSupporters(),
        [this](web::WebResponse res) {
            if (res.ok()) {
                std::unordered_set<int> ids;
                auto json = res.json().unwrapOr(matjson::Value::object());
                if (json.contains("supporters") && json["supporters"].isArray()) {
                    for (auto const& row : json["supporters"]) {
                        if (auto id = row["account_id"].asInt(); id.isOk()) {
                            ids.insert(id.unwrap());
                        }
                    }
                }
                m_supporterIDs = std::move(ids);
                m_hasFetchedSupporters = true;
            } else {
                log::warn(
                    "[GauntletManagerCache] Failed to fetch supporter list (code {}): {}",
                    res.code(), res.string().unwrapOr("<no body>")
                );
            }

            auto waiting = std::move(m_waitingSupporters);
            m_waitingSupporters.clear();
            for (auto& [accountID, cb] : waiting) {
                cb(m_supporterIDs.contains(accountID));
            }
        }
    );
}

void GauntletManagerCache::warm() {
    if (!m_hasFetched) fetch();
    if (!m_hasFetchedHelpers) fetchHelpers();
    if (!m_hasFetchedSupporters) fetchSupporters();
}

void GauntletManagerCache::refresh() {
    fetch();
    fetchHelpers();
    fetchSupporters();
}

void GauntletManagerCache::isManager(int accountID, std::function<void(bool)> callback) {
    if (m_hasFetched) {
        callback(m_managerIDs.contains(accountID));
        return;
    }

    m_waiting.push_back({accountID, std::move(callback)});
    fetch();
}

void GauntletManagerCache::isHelper(int accountID, std::function<void(bool)> callback) {
    if (m_hasFetchedHelpers) {
        callback(m_helperIDs.contains(accountID));
        return;
    }

    m_waitingHelpers.push_back({accountID, std::move(callback)});
    fetchHelpers();
}

void GauntletManagerCache::isSupporter(int accountID, std::function<void(bool)> callback) {
    if (m_hasFetchedSupporters) {
        callback(m_supporterIDs.contains(accountID));
        return;
    }

    m_waitingSupporters.push_back({accountID, std::move(callback)});
    fetchSupporters();
}
