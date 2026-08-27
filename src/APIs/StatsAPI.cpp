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
    int crystals, int coins,
    int iconId, int iconType, int color1, int color2, int color3, bool glow
) {
    auto body = matjson::Value();
    body["username"]  = username;
    body["crystals"]  = crystals;
    body["coins"]     = coins;
    body["icon_id"]   = iconId;
    body["icon_type"] = iconType;
    body["color1"]    = color1;
    body["color2"]    = color2;
    body["color3"]    = color3;
    body["glow"]      = glow;

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

web::WebFuture StatsAPI::completeLevel(int accountId, std::string const& token, int levelId) {
    auto body = matjson::Value();
    body["level_id"] = levelId;

    return web::WebRequest()
        .header("Content-Type", "application/json")
        .header("Authorization", "Bearer " + token)
        .header("X-Account-Id", std::to_string(accountId))
        .bodyString(body.dump())
        .post(baseURL() + "/level-complete");
}
