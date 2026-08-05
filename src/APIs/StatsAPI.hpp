#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class StatsAPI {
public:
    static StatsAPI* get();

    web::WebFuture push(
        int accountId, std::string const& username, std::string const& token,
        int crystals, int coins
    );

    // Resets the account's crystal total to 0 server-side. Unlike push(), this
    // is not subject to the server's GREATEST() ratchet - it's an explicit reset.
    web::WebFuture resetSelf(int accountId, std::string const& token);

    // Records a gauntlet completion; the server credits its coin reward
    // (once per account per gauntlet) and returns the account's new total.
    web::WebFuture completeGauntlet(int accountId, std::string const& token, int gauntletId);

private:
    StatsAPI() = default;
    std::string baseURL();
};
