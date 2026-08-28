#include "LeaderboardAPI.hpp"

LeaderboardAPI* LeaderboardAPI::get() {
    static LeaderboardAPI instance;
    return &instance;
}

std::string LeaderboardAPI::baseURL() {
    return std::string("https://bettergauntlets.dev");
}

web::WebFuture LeaderboardAPI::fetch(int limit, std::string const& sortBy) {
    return web::WebRequest()
        .get(baseURL() + fmt::format("/leaderboard?sort={}&limit={}", sortBy, limit));
}

static int parseIntField(matjson::Value const& val) {
    if (auto asInt = val.asInt(); asInt.isOk()) return asInt.unwrap();
    return geode::utils::numFromString<int>(val.asString().unwrapOr("0")).unwrapOr(0);
}

std::vector<LeaderboardEntry> LeaderboardAPI::parse(matjson::Value const& json) {
    std::vector<LeaderboardEntry> result;
    if (!json.isArray()) return result;

    for (auto const& row : json) {
        LeaderboardEntry entry;
        entry.accountId = parseIntField(row["account_id"]);
        entry.username  = row["username"].asString().unwrapOr("Unknown");
        entry.crystals  = parseIntField(row["crystals"]);
        entry.coins     = parseIntField(row["coins"]);
        entry.iconId    = parseIntField(row["icon_id"]);
        entry.iconType  = static_cast<IconType>(parseIntField(row["icon_type"]));
        entry.color1    = parseIntField(row["color1"]);
        entry.color2    = parseIntField(row["color2"]);
        entry.color3    = parseIntField(row["color3"]);
        entry.glow      = row["glow"].asBool().unwrapOr(false);
        result.push_back(entry);
    }
    return result;
}
