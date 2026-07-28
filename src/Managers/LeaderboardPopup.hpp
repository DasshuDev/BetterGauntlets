#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/web.hpp>
#include <unordered_map>
#include "LeaderboardAPI.hpp"

using namespace geode::prelude;

class LeaderboardPopup : public Popup, public UserInfoDelegate {
protected:
    bool init(float width, float height, char const* bg);
    void onClose(CCObject* sender) override;

    std::vector<LeaderboardEntry> m_entries;

    CCLayer*       m_listLayer     = nullptr;
    CCMenu*        m_entryList     = nullptr;
    LoadingCircle* m_loadingCircle = nullptr;
    CCLabelBMFont* m_errorLabel    = nullptr;

    async::TaskHolder<web::WebResponse> m_fetchHolder;

    // Player icons are fetched one at a time - GameLevelManager only has a
    // single UserInfoDelegate slot - and queued behind whichever row asked first.
    std::vector<int> m_iconFetchQueue;
    std::unordered_map<int, SimplePlayer*> m_pendingIconPlayers;
    bool m_fetchingIcon = false;

    void onSync(cocos2d::CCObject* sender);
    void fetchLeaderboard();
    void buildList();
    void buildRow(int rank, LeaderboardEntry const& entry, float listWidth);

    void queueIconFetch(int accountId, SimplePlayer* player);
    void fetchNextIcon();
    void applyIcon(SimplePlayer* player, GJUserScore* score);
    void getUserInfoFinished(GJUserScore* score) override;
    void getUserInfoFailed(int accountId) override;

public:
    static LeaderboardPopup* create();
};
