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

    // Player progress - a level only counts as "completed" for gauntlet
    // purposes (crystal reward + the X/5 badge) if it was finished while
    // played through the gauntlet UI, not just completed some other way.

    // Call when the player taps a level from the gauntlet screen, right
    // before pushing into LevelInfoLayer/PlayLayer - arms the *next*
    // PlayLayer as a gauntlet attempt for this level.
    void markPendingGauntletAttempt(int levelID);

    // Called once from PlayLayer::init(): returns whether levelID is the one
    // just armed via markPendingGauntletAttempt(), consuming the flag either
    // way so it can't leak onto some later, unrelated play session.
    bool consumePendingGauntletAttempt(int levelID);

    // Whether this level's crystal reward has already been granted (i.e. it
    // was completed at least once through the gauntlet).
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