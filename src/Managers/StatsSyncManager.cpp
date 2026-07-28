#include "StatsSyncManager.hpp"
#include "StatsAPI.hpp"
#include <argon/argon.hpp>

StatsSyncManager* StatsSyncManager::get() {
    static StatsSyncManager instance;
    return &instance;
}

void StatsSyncManager::sync(int crystals, int coins, SyncCallback callback) {
    if (!argon::signedIn()) {
        if (callback) callback(false, "Not signed into a GD account");
        return;
    }

    auto account = argon::getGameAccountData();
    int accountId = account.accountId;
    std::string username = account.username;

    m_argonHolder.spawn(
        argon::startAuth(account),
        [this, accountId, username, crystals, coins, callback](Result<std::string> result) {
            if (!result.isOk()) {
                auto err = result.unwrapErr();
                log::warn("StatsSyncManager: auth failed - {}", err);
                if (callback) callback(false, err);
                return;
            }
            auto token = std::move(result).unwrap();

            m_pushHolder.spawn(
                StatsAPI::get()->push(accountId, username, token, crystals, coins),
                [callback](web::WebResponse res) {
                    if (!res.ok()) {
                        auto err = fmt::format("HTTP {}", res.code());
                        log::warn(
                            "StatsSyncManager: push failed - {} - {}",
                            err, res.string().unwrapOr("")
                        );
                        if (callback) callback(false, err);
                        return;
                    }
                    if (callback) callback(true, "");
                }
            );
        }
    );
}
