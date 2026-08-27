#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

struct LeaderboardEntry {
    int accountId = 0;
    std::string username;
    int crystals = 0;
    int coins = 0;
    int iconId = 0;
    IconType iconType = IconType::Cube;
    int color1 = 0;
    int color2 = 0;
    int color3 = 0;
    bool glow = false;
};

class LeaderboardAPI {
public:
    static LeaderboardAPI* get();

    web::WebFuture fetch(int limit = 100, std::string const& sortBy = "crystals");

    static std::vector<LeaderboardEntry> parse(matjson::Value const& json);

private:
    LeaderboardAPI() = default;
    std::string baseURL();
};
