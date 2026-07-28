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
