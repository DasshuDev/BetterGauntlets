#include "GauntletManagerCache.hpp"
#include "GauntletManagerAPI.hpp"

GauntletManagerCache* GauntletManagerCache::get() {
    static GauntletManagerCache instance;
    return &instance;
}

void GauntletManagerCache::isManager(int accountID, std::function<void(bool)> callback) {
    if (auto it = m_cache.find(accountID); it != m_cache.end()) {
        callback(it->second);
        return;
    }

    m_waiting[accountID].push_back(std::move(callback));
    if (m_requests.contains(accountID)) return;

    m_requests[accountID].spawn(
        GauntletManagerAPI::get()->checkIsManager(accountID),
        [this, accountID](web::WebResponse res) {
            bool isManager = false;
            if (res.ok()) {
                auto json = res.json().unwrapOr(matjson::Value());
                isManager = json["isManager"].asBool().unwrapOr(false);
            }

            m_cache[accountID] = isManager;
            m_requests.erase(accountID);

            auto callbacks = std::move(m_waiting[accountID]);
            m_waiting.erase(accountID);
            for (auto& cb : callbacks) {
                cb(isManager);
            }
        }
    );
}
