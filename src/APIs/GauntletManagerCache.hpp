#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>

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
        std::function<web::WebFuture()> fetch;
        std::string jsonKey;
        std::string label;

        std::unordered_set<int> ids;
        bool hasFetched = false;
        std::vector<std::pair<int, std::function<void(bool)>>> waiting;
        async::TaskHolder<web::WebResponse> request;
    };

    void fetchRole(RoleCache& cache);
    void checkRole(RoleCache& cache, int accountID, std::function<void(bool)> callback);

    RoleCache m_managers;
    RoleCache m_helpers;
    RoleCache m_supporters;
};
