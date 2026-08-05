#include "StatsAPI.hpp"

StatsAPI* StatsAPI::get() {
    static StatsAPI instance;
    return &instance;
}

std::string StatsAPI::baseURL() {
    return std::string("https://bettergauntlets.dev");
}

web::WebFuture StatsAPI::push(
    int accountId, std::string const& username, std::string const& token,
    int crystals, int coins
) {
    auto body = matjson::Value();
    body["username"] = username;
    body["crystals"] = crystals;
    body["coins"]    = coins;

    return web::WebRequest()
        .header("Content-Type", "application/json")
        .header("Authorization", "Bearer " + token)
        .header("X-Account-Id", std::to_string(accountId))
        .bodyString(body.dump())
        .post(baseURL() + "/stats");
}

web::WebFuture StatsAPI::resetSelf(int accountId, std::string const& token) {
    return web::WebRequest()
        .header("Authorization", "Bearer " + token)
        .header("X-Account-Id", std::to_string(accountId))
        .send("DELETE", baseURL() + "/stats");
}

web::WebFuture StatsAPI::completeGauntlet(int accountId, std::string const& token, int gauntletId) {
    auto body = matjson::Value();
    body["gauntlet_id"] = gauntletId;

    return web::WebRequest()
        .header("Content-Type", "application/json")
        .header("Authorization", "Bearer " + token)
        .header("X-Account-Id", std::to_string(accountId))
        .bodyString(body.dump())
        .post(baseURL() + "/gauntlet-complete");
}
