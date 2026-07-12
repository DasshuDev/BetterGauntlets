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

private:
    GauntletManagerCache() = default;
    void fetch();

    std::unordered_set<int> m_managerIDs;
    bool m_hasFetched = false;
    std::vector<std::pair<int, std::function<void(bool)>>> m_waiting;
    async::TaskHolder<web::WebResponse> m_request;
};