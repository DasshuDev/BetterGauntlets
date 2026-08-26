#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/web.hpp>
#include <unordered_map>
#include "../APIs/LeaderboardAPI.hpp"

using namespace geode::prelude;

class LeaderboardPopup : public Popup, public UserInfoDelegate {
protected:
    bool init(float width, float height, char const* bg);
    void onClose(CCObject* sender) override;

    std::vector<LeaderboardEntry> m_entries;
    std::string m_sortBy = "crystals";

    CCLayer*       m_listLayer     = nullptr;
    CCMenu*        m_entryList     = nullptr;
    CCMenu*        m_tabMenu       = nullptr;
    CCMenu*        m_infoMenu      = nullptr;
    LoadingCircle* m_loadingCircle = nullptr;
    CCLabelBMFont* m_errorLabel    = nullptr;
    float m_tabBarY = 0;

    async::TaskHolder<web::WebResponse> m_fetchHolder;

    std::vector<int> m_iconFetchQueue;
    std::unordered_map<int, SimplePlayer*> m_pendingIconPlayers;
    bool m_fetchingIcon = false;

    void onSync(cocos2d::CCObject* sender);
    void fetchLeaderboard();
    void buildTabMenu();
    void onSwitchSort(std::string const& sortBy);
    void buildList();
    void buildRow(int rank, LeaderboardEntry const& entry, float listWidth);

    void queueIconFetch(int accountId, SimplePlayer* player);
    void fetchNextIcon();
    void applyIcon(SimplePlayer* player, GJUserScore* score);
    void getUserInfoFinished(GJUserScore* score) override;
    void getUserInfoFailed(int accountId) override;
    void onInfo(cocos2d::CCObject*);

public:
    static LeaderboardPopup* create();
};
