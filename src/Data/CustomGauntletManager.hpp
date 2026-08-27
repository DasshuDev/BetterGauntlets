#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
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

    // Kicks off (or joins) the initial gauntlet-list fetch as early as possible, so the list
    // is usually already cached by the time the player opens the custom gauntlet tab.
    void warm();

    // Registers a callback for when the cache is ready - joins an in-flight fetch (from warm()
    // or a previous call) instead of starting a duplicate one. Callers should check hasCached()
    // first to skip straight to getCached() on the already-warm fast path.
    void whenReady(std::function<void(bool ok, int code)> callback);

    // Icon textures decoded from each gauntlet's iconURL, shared across every
    // CustomGauntletNode instance instead of re-downloading/re-decoding per node.
    cocos2d::CCTexture2D* getCachedIcon(std::string const& url) const;
    void cacheIcon(std::string const& url, cocos2d::CCTexture2D* texture);

    // Is this level ID one of the slots across all cached custom gauntlets?
    bool isCustomGauntletLevel(int levelID) const;
    void markPendingGauntletAttempt(int levelID);

    // Bridges the async server reward (StatsSyncManager::completeGauntlet) from
    // wherever it resolves (PlayLayer/EndLevelLayer) to the CustomGauntletLayer
    // instance for that gauntlet, whenever it next enters.
    void markPendingGauntletReward(int gauntletId, int coins);
    bool consumePendingGauntletReward(int gauntletId, int& outCoins);

    // Looks up the cached gauntlet that has this level ID in one of its slots.
    CustomGauntletData const* findGauntletForLevel(int levelID) const;

    // True once every one of the gauntlet's 5 slots has a claimed reward.
    bool isGauntletFullyCompleted(CustomGauntletData const& gauntlet) const;

    bool consumePendingGauntletAttempt(int levelID);

    // Has the reward been granted?
    bool isLevelRewardClaimed(int levelID) const;
    void markLevelRewardClaimed(int levelID);

    // Persisted running crystal total.
    int getCrystalTotal() const;
    void addCrystals(int amount);
    void resetCrystals();

    int getCoinTotal() const;
    void addCoins(int amount);
    void resetCoins();

    // Overwrites the local totals outright, for applying the server's
    // GREATEST(local, server) reconciled values back after a sync push.
    void setCrystalTotal(int value);
    void setCoinTotal(int value);

    // Clears every claimed level reward, so previously-completed gauntlet
    // levels can be replayed for crystals and locked slots re-lock.
    void resetClaimedRewards();

    // Crystal reward for a level, based on its difficulty rating.
    static int crystalsForLevel(GJGameLevel* level);

private:
    CustomGauntletManager() = default;
    void beginFetch();

    std::vector<CustomGauntletData> m_cache;
    bool m_hasCached = false;
    int m_pendingGauntletLevelID = 0;
    int m_pendingRewardGauntletID = 0;
    int m_pendingRewardCoins = 0;

    bool m_isFetching = false;
    async::TaskHolder<web::WebResponse> m_fetchHolder;
    std::vector<std::function<void(bool, int)>> m_waitingCallbacks;

    std::unordered_map<std::string, Ref<cocos2d::CCTexture2D>> m_iconCache;

    static std::string baseURL();
};