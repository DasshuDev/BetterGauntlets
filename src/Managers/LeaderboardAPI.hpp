#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

struct LeaderboardEntry {
    int accountId = 0;
    std::string username;
    int crystals = 0;
};

class LeaderboardAPI {
public:
    static LeaderboardAPI* get();

    web::WebFuture fetch(int limit = 100);

    static std::vector<LeaderboardEntry> parse(matjson::Value const& json);

private:
    LeaderboardAPI() = default;
    std::string baseURL();
};
