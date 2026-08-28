#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class StatsAPI {
public:
    static StatsAPI* get();

    web::WebFuture push(
        int accountId, std::string const& username, std::string const& token,
        int crystals, int coins,
        int iconId, int iconType, int color1, int color2, int color3, bool glow
    );

    web::WebFuture resetSelf(int accountId, std::string const& token);

    web::WebFuture completeGauntlet(int accountId, std::string const& token, int gauntletId);
    web::WebFuture completeLevel(int accountId, std::string const& token, int levelId);

private:
    StatsAPI() = default;
    std::string baseURL();
};
