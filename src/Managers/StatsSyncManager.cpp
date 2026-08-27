#include "StatsSyncManager.hpp"
#include "../APIs/StatsAPI.hpp"
#include "../Data/CustomGauntletManager.hpp"
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

    auto gm = GameManager::sharedState();
    IconType iconType = gm->m_playerIconType;
    int iconId = gm->activeIconForType(iconType);
    int color1 = gm->getPlayerColor();
    int color2 = gm->getPlayerColor2();
    int color3 = gm->getPlayerGlowColor();
    bool glow  = gm->getPlayerGlow();

    m_argonHolder.spawn(
        argon::startAuth(account),
        [this, accountId, username, crystals, coins, iconId, iconType, color1, color2, color3, glow, callback](Result<std::string> result) {
            if (!result.isOk()) {
                auto err = result.unwrapErr();
                log::warn("StatsSyncManager: auth failed - {}", err);
                if (callback) callback(false, err);
                return;
            }
            auto token = std::move(result).unwrap();

            m_pushHolder.spawn(
                StatsAPI::get()->push(
                    accountId, username, token, crystals, coins,
                    iconId, static_cast<int>(iconType), color1, color2, color3, glow
                ),
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

                    auto json = res.json().unwrapOr(matjson::Value());
                    auto manager = CustomGauntletManager::get();
                    manager->setCrystalTotal(json["crystals"].asInt().unwrapOr(manager->getCrystalTotal()));
                    manager->setCoinTotal(json["coins"].asInt().unwrapOr(manager->getCoinTotal()));

                    if (callback) callback(true, "");
                }
            );
        }
    );
}

void StatsSyncManager::completeGauntlet(int gauntletId, CompleteCallback callback) {
    if (!argon::signedIn()) {
        if (callback) callback(false, 0, "Not signed into a GD account");
        return;
    }

    auto account = argon::getGameAccountData();
    int accountId = account.accountId;

    m_completeArgonHolder.spawn(
        argon::startAuth(account),
        [this, accountId, gauntletId, callback](Result<std::string> result) {
            if (!result.isOk()) {
                auto err = result.unwrapErr();
                log::warn("StatsSyncManager: complete-gauntlet auth failed - {}", err);
                if (callback) callback(false, 0, err);
                return;
            }
            auto token = std::move(result).unwrap();

            m_completeHolder.spawn(
                StatsAPI::get()->completeGauntlet(accountId, token, gauntletId),
                [callback](web::WebResponse res) {
                    if (!res.ok()) {
                        auto err = fmt::format("HTTP {}", res.code());
                        log::warn(
                            "StatsSyncManager: complete-gauntlet failed - {} - {}",
                            err, res.string().unwrapOr("")
                        );
                        if (callback) callback(false, 0, err);
                        return;
                    }
                    auto json = res.json().unwrapOr(matjson::Value());
                    int reward = json["reward_coins"].asInt().unwrapOr(0);
                    if (callback) callback(true, reward, "");
                }
            );
        }
    );
}

void StatsSyncManager::completeLevel(int levelId, SyncCallback callback) {
    if (!argon::signedIn()) {
        if (callback) callback(false, "Not signed into a GD account");
        return;
    }

    auto account = argon::getGameAccountData();
    int accountId = account.accountId;

    m_completeLevelArgonHolder.spawn(
        argon::startAuth(account),
        [this, accountId, levelId, callback](Result<std::string> result) {
            if (!result.isOk()) {
                auto err = result.unwrapErr();
                log::warn("StatsSyncManager: complete-level auth failed - {}", err);
                if (callback) callback(false, err);
                return;
            }
            auto token = std::move(result).unwrap();

            m_completeLevelHolder.spawn(
                StatsAPI::get()->completeLevel(accountId, token, levelId),
                [callback](web::WebResponse res) {
                    if (!res.ok()) {
                        auto err = fmt::format("HTTP {}", res.code());
                        log::warn(
                            "StatsSyncManager: complete-level failed - {} - {}",
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

void StatsSyncManager::resetSelf(SyncCallback callback) {
    if (!argon::signedIn()) {
        if (callback) callback(false, "Not signed into a GD account");
        return;
    }

    auto account = argon::getGameAccountData();
    int accountId = account.accountId;

    m_resetArgonHolder.spawn(
        argon::startAuth(account),
        [this, accountId, callback](Result<std::string> result) {
            if (!result.isOk()) {
                auto err = result.unwrapErr();
                log::warn("StatsSyncManager: reset auth failed - {}", err);
                if (callback) callback(false, err);
                return;
            }
            auto token = std::move(result).unwrap();

            m_resetHolder.spawn(
                StatsAPI::get()->resetSelf(accountId, token),
                [callback](web::WebResponse res) {
                    if (!res.ok()) {
                        auto err = fmt::format("HTTP {}", res.code());
                        log::warn(
                            "StatsSyncManager: reset failed - {} - {}",
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
