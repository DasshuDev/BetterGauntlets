#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
#include "CustomGauntletData.hpp"

using namespace geode::prelude;

class CustomGauntletManager {
public:
    static CustomGauntletManager* get();

    web::WebFuture fetchAll();

    std::vector<CustomGauntletData> parse(std::string const& body);

    bool hasCached() const;
    std::vector<CustomGauntletData> const& getCached() const;
    void clearCache();

    void warm();

    void whenReady(std::function<void(bool ok, int code)> callback);

    cocos2d::CCTexture2D* getCachedIcon(std::string const& url) const;
    void cacheIcon(std::string const& url, cocos2d::CCTexture2D* texture);

    bool isCustomGauntletLevel(int levelID) const;
    void markPendingGauntletAttempt(int levelID);

    void markPendingGauntletReward(int gauntletId, int coins);
    bool consumePendingGauntletReward(int gauntletId, int& outCoins);

    CustomGauntletData const* findGauntletForLevel(int levelID) const;

    bool isGauntletFullyCompleted(CustomGauntletData const& gauntlet) const;

    bool consumePendingGauntletAttempt(int levelID);

    bool isLevelRewardClaimed(int levelID) const;
    void markLevelRewardClaimed(int levelID);

    std::vector<int> getPendingLevelSyncs() const;
    void markLevelSyncPending(int levelID);
    void clearLevelSyncPending(int levelID);

    std::vector<int> getPendingGauntletSyncs() const;
    void markGauntletSyncPending(int gauntletId);
    void clearGauntletSyncPending(int gauntletId);
    bool isGauntletRewardSynced(int gauntletId) const;
    void markGauntletRewardSynced(int gauntletId);

    int getCrystalTotal() const;
    void addCrystals(int amount);
    void resetCrystals();

    int getCoinTotal() const;
    void addCoins(int amount);
    void resetCoins();

    void setCrystalTotal(int value);
    void setCoinTotal(int value);

    void resetClaimedRewards();

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