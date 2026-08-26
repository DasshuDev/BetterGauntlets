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
    GauntletManagerCache() = default;
    void fetch();
    void fetchHelpers();
    void fetchSupporters();

    std::unordered_set<int> m_managerIDs;
    bool m_hasFetched = false;
    std::vector<std::pair<int, std::function<void(bool)>>> m_waiting;
    async::TaskHolder<web::WebResponse> m_request;

    std::unordered_set<int> m_helperIDs;
    bool m_hasFetchedHelpers = false;
    std::vector<std::pair<int, std::function<void(bool)>>> m_waitingHelpers;
    async::TaskHolder<web::WebResponse> m_helperRequest;

    std::unordered_set<int> m_supporterIDs;
    bool m_hasFetchedSupporters = false;
    std::vector<std::pair<int, std::function<void(bool)>>> m_waitingSupporters;
    async::TaskHolder<web::WebResponse> m_supporterRequest;
};