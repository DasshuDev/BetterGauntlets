#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
#include <chrono>

using namespace geode::prelude;

class GauntletManagerCache {
public:
    static GauntletManagerCache* get();

    void warm();

    void refresh();

    void isManager(int accountID, std::function<void(bool)> callback);
    void isHelper(int accountID, std::function<void(bool)> callback);
    void isSupporter(int accountID, std::function<void(bool)> callback);

private:
    GauntletManagerCache();
    struct RoleCache {
        std::string jsonKey;
        std::string label;

        std::unordered_set<int> ids;
        bool hasFetched = false;
        std::vector<std::pair<int, std::function<void(bool)>>> waiting;
    };

    void fetchRoles();
    void checkRole(RoleCache& cache, int accountID, std::function<void(bool)> callback);

    RoleCache m_managers;
    RoleCache m_helpers;
    RoleCache m_supporters;

    bool m_isFetching = false;
    async::TaskHolder<web::WebResponse> m_request;

    std::chrono::steady_clock::time_point m_lastRefresh = std::chrono::steady_clock::time_point::min();
};
