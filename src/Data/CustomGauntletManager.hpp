#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "CustomGauntletData.hpp"

using namespace geode::prelude;

class CustomGauntletManager {
public:
    static CustomGauntletManager* get();

    // Fetch all published gauntlets from server
    web::WebFuture fetchAll();

    // Parse the server's colon-delimited response into a list
    std::vector<CustomGauntletData> parse(std::string const& body);

    // Cache
    bool hasCached() const;
    std::vector<CustomGauntletData> const& getCached() const;
    void clearCache();
    void markPendingGauntletAttempt(int levelID);

    bool consumePendingGauntletAttempt(int levelID);

    // Has the reward been granted?
    bool isLevelRewardClaimed(int levelID) const;
    void markLevelRewardClaimed(int levelID);

    // Persisted running crystal total.
    int getCrystalTotal() const;
    void addCrystals(int amount);

private:
    CustomGauntletManager() = default;
    std::vector<CustomGauntletData> m_cache;
    bool m_hasCached = false;
    int m_pendingGauntletLevelID = 0;

    static std::string baseURL();
};