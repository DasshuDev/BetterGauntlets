#include "LeaderboardPopup.hpp"
#include "StatsSyncManager.hpp"
#include "../Data/CustomGauntletManager.hpp"

LeaderboardPopup* LeaderboardPopup::create() {
    auto ret = new LeaderboardPopup();
    if (ret && ret->init(450, 270, "GJ_square05.png")) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool LeaderboardPopup::init(float width, float height, char const* bg) {
    if (!Popup::init(width, height, bg)) return false;

    auto title = CCLabelBMFont::create("Global Leaderboard", "goldFont.fnt");
    title->setPosition(m_size.width / 2, m_size.height - 20);
    title->setScale(0.75);
    m_mainLayer->addChild(title);

    m_loadingCircle = LoadingCircle::create();
    m_loadingCircle->setPosition(m_size / 2);
    m_loadingCircle->setVisible(false);
    m_loadingCircle->ignoreAnchorPointForPosition(false);
    m_mainLayer->addChild(m_loadingCircle, 10);

    auto syncMenu = CCMenu::create();
    syncMenu->setID("sync-menu");
    syncMenu->setPosition({m_size.width - 45, m_size.height - 20});
    m_mainLayer->addChild(syncMenu);

    auto syncSpr = ButtonSprite::create("Sync", "bigFont.fnt", "GJ_button_01.png");
    syncSpr->setScale(0.55);
    auto syncBtn = CCMenuItemExt::createSpriteExtra(
        syncSpr, [this](CCMenuItemSpriteExtra*) { onSync(nullptr); }
    );
    syncBtn->setID("sync-button");
    syncMenu->addChild(syncBtn);

    float tabAreaHeight = 22;
    float listHeight = m_size.height - 60 - tabAreaHeight;
    float listCenterY = 20 + listHeight / 2;
    m_tabBarY = 26 + listHeight + tabAreaHeight / 2;

    auto listBG = NineSlice::create("square02b_001.png");
    listBG->setContentSize({m_size.width - 40, listHeight});
    listBG->setPosition({m_size.width / 2, listCenterY});
    listBG->setColor({0, 0, 0});
    listBG->setOpacity(80);
    m_mainLayer->addChild(listBG);

    auto listOutline = NineSlice::create("GJ_square07.png");
    listOutline->setContentSize({m_size.width - 38, listHeight + 6});
    listOutline->setPosition({m_size.width / 2, listCenterY});
    listOutline->setColor({67, 67, 67});
    m_mainLayer->addChild(listOutline, 1);

    auto listClip = CCClippingNode::create(listBG);
    listClip->setAlphaThreshold(0);
    m_mainLayer->addChild(listClip);

    m_listLayer = CCLayer::create();
    m_listLayer->setAnchorPoint({0, 0});
    m_listLayer->setPosition({20, 20});
    m_listLayer->setID("list");
    m_listLayer->setContentSize(listBG->getContentSize());
    m_listLayer->ignoreAnchorPointForPosition(false);
    listClip->addChild(m_listLayer);

    m_infoMenu = CCMenu::create();
    m_infoMenu->setPosition({25, 25});
    m_mainLayer->addChild(m_infoMenu, 5);

    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSpr->setScale(0.75);
    auto infoBtn = CCMenuItemSpriteExtra::create(
        infoSpr,
        this,
        menu_selector(LeaderboardPopup::onInfo)
    );
    m_infoMenu->addChild(infoBtn);

    buildTabMenu();
    fetchLeaderboard();

    return true;
}

void LeaderboardPopup::buildTabMenu() {
    if (m_tabMenu) {
        m_tabMenu->removeFromParent();
        m_tabMenu = nullptr;
    }

    m_tabMenu = CCMenu::create();
    m_tabMenu->setID("tab-menu");
    m_tabMenu->setPosition({m_size.width / 2, m_tabBarY});
    m_tabMenu->setLayout(RowLayout::create()->setGap(6));
    m_mainLayer->addChild(m_tabMenu);

    auto makeTabBtn = [this](std::string const& text, std::string const& sortBy) {
        bool active = (m_sortBy == sortBy);
        auto spr = ButtonSprite::create(
            text.c_str(), "bigFont.fnt",
            active ? "GJ_button_02.png" : "GJ_button_05.png"
        );
        spr->setScale(0.5);
        if (!active) spr->setOpacity(180);
        return CCMenuItemExt::createSpriteExtra(spr, [this, sortBy](CCMenuItemSpriteExtra*) {
            onSwitchSort(sortBy);
        });
    };

    m_tabMenu->addChild(makeTabBtn("Crystals", "crystals"));
    m_tabMenu->addChild(makeTabBtn("Coins", "coins"));
    m_tabMenu->updateLayout();
}

void LeaderboardPopup::onSwitchSort(std::string const& sortBy) {
    if (m_sortBy == sortBy) return;
    m_sortBy = sortBy;
    buildTabMenu();
    fetchLeaderboard();
}

void LeaderboardPopup::onClose(CCObject* sender) {
    auto glm = GameLevelManager::get();
    if (glm->m_userInfoDelegate == this) glm->m_userInfoDelegate = nullptr;
    Popup::onClose(sender);
}

void LeaderboardPopup::onSync(CCObject*) {
    m_loadingCircle->setVisible(true);

    int crystals = CustomGauntletManager::get()->getCrystalTotal();

    Ref<LeaderboardPopup> self(this);
    StatsSyncManager::get()->sync(crystals, 0, [self](bool success, std::string const& error) {
        if (!self->getParent()) return; // popup already closed

        self->m_loadingCircle->setVisible(false);

        if (!success) {
            Notification::create(
                fmt::format("Sync failed: {}", error), NotificationIcon::Error
            )->show();
            return;
        }

        Notification::create("Stats synced!", NotificationIcon::Success)->show();
        self->fetchLeaderboard();
    });
}

void LeaderboardPopup::fetchLeaderboard() {
    if (m_errorLabel) {
        m_errorLabel->removeFromParent();
        m_errorLabel = nullptr;
    }
    m_loadingCircle->setVisible(true);

    m_fetchHolder.spawn(
        LeaderboardAPI::get()->fetch(100, m_sortBy),
        [this](web::WebResponse res) {
            m_loadingCircle->setVisible(false);

            if (!res.ok()) {
                m_entries.clear();
                buildList();
                m_errorLabel = CCLabelBMFont::create(
                    fmt::format("Failed to load. ({})", res.code()).c_str(), "chatFont.fnt"
                );
                m_errorLabel->setPosition(m_size / 2);
                m_errorLabel->setScale(0.6);
                m_mainLayer->addChild(m_errorLabel, 5);
                return;
            }

            auto json = res.json().unwrapOr(matjson::Value::array());
            m_entries = LeaderboardAPI::parse(json);
            buildList();

            if (m_entries.empty()) {
                m_errorLabel = CCLabelBMFont::create("No players yet!", "chatFont.fnt");
                m_errorLabel->setPosition(m_size / 2);
                m_errorLabel->setScale(0.6);
                m_mainLayer->addChild(m_errorLabel, 5);
            }
        }
    );
}

void LeaderboardPopup::buildList() {
    if (!m_listLayer) return;

    m_iconFetchQueue.clear();
    m_pendingIconPlayers.clear();

    m_listLayer->removeAllChildren();

    m_entryList = CCMenu::create();
    m_entryList->setID("leaderboard-list");
    m_entryList->setAnchorPoint({0.5, 1});
    m_entryList->setContentSize({m_listLayer->getContentWidth(), 0});
    m_entryList->setLayout(ColumnLayout::create()
        ->setGap(0)
        ->setAutoGrowAxis(true)
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::End)
        ->setAxisReverse(true)
    );

    auto scroll = ScrollLayer::create(m_listLayer->getContentSize(), true, true);
    scroll->m_contentLayer->addChild(m_entryList);
    m_listLayer->addChild(scroll);

    int rank = 1;
    for (auto const& entry : m_entries) {
        buildRow(rank, entry, m_entryList->getContentWidth());
        rank++;
    }

    m_entryList->updateLayout();

    scroll->m_contentLayer->setContentSize({m_listLayer->getContentWidth(), m_entryList->getContentHeight()});
    m_entryList->setPosition({m_listLayer->getContentWidth() / 2, scroll->m_contentLayer->getContentHeight()});
    scroll->moveToTop();
}

void LeaderboardPopup::buildRow(int rank, LeaderboardEntry const& entry, float listWidth) {
    auto row = CCNode::create();
    auto inset = 0;
    row->setContentSize({listWidth, 55});

    auto rowBG = CCScale9Sprite::create("square.png");
    rowBG->setContentSize(row->getContentSize());
    rowBG->setColor({0, 0, 0});
    rowBG->setOpacity(rank % 2 == 0 ? 40 : 80);
    rowBG->setAnchorPoint({0, 0});
    row->addChild(rowBG);

    ccColor4B rankColor = {255, 255, 255, 255};
    if (rank == 1) rankColor = {255, 215, 0, 80};
    else if (rank == 2) rankColor = {200, 200, 210, 80};
    else if (rank == 3) rankColor = {205, 127, 50, 80};
    else if (rank >= 4) rankColor = {0, 0, 0, 80};

    std::string trophy = "rankIcon_all_001.png";
    if (rank == 1)       trophy = "rankIcon_1_001.png";
    else if (rank == 2)  trophy = "rankIcon_top50_001.png";
    else if (rank == 3)  trophy = "rankIcon_top100_001.png";
    else if (rank <= 24) trophy = "rankIcon_top500_001.png";
    else if (rank <= 49) trophy = "rankIcon_top2500_001.png";

    auto gradient = NineSlice::createWithSpriteFrameName("GR_pureGradient_001.png"_spr);
    gradient->setInsetTop(inset);
    gradient->setInsetLeft(inset);
    gradient->setInsetRight(inset);
    gradient->setInsetBottom(inset);
    gradient->setPositionX(-75);
    gradient->setColor({rankColor.r, rankColor.g, rankColor.b});
    gradient->setOpacity(rankColor.a);
    gradient->setContentSize(row->getContentSize());
    gradient->setAnchorPoint({1, 1});
    gradient->setRotation(180);
    row->addChild(gradient);

    auto rankIcon = CCSprite::createWithSpriteFrameName(trophy.c_str());
    rankIcon->setScale(0.75);
    rankIcon->setPosition({22.5, row->getContentHeight() / 2});
    row->addChild(rankIcon);

    auto player = SimplePlayer::create(0);
    player->setPosition({55, row->getContentHeight() / 2});
    player->setScale(0.85);
    row->addChild(player);

    auto glm = GameLevelManager::get();
    if (auto score = glm->userInfoForAccountID(entry.accountId)) {
        applyIcon(player, score);
    } else {
        queueIconFetch(entry.accountId, player);
    }

    auto nameLabel = CCLabelBMFont::create(entry.username.c_str(), "goldFont.fnt");
    nameLabel->limitLabelWidth(listWidth - 210, 0.7, 0.1);

    auto nameMenu = CCMenu::create();
    nameMenu->setID("name-menu"_spr);
    nameMenu->setAnchorPoint({0, 0.5});
    nameMenu->setPosition({80, (row->getContentHeight() / 2 - 0.5f)});
    row->addChild(nameMenu);

    int accountId = entry.accountId;
    auto nameBtn = CCMenuItemExt::createSpriteExtra(
        nameLabel,
        [accountId](CCMenuItemSpriteExtra*) {
        ProfilePage::create(accountId, false)->show();
    });
    // nameBtn->setScale(0.7);
    nameBtn->setAnchorPoint({0, 0.4});
    nameMenu->addChild(nameBtn);

    bool showCoins = (m_sortBy == "coins");
    int primaryValue   = showCoins ? entry.coins : entry.crystals;
    int secondaryValue = showCoins ? entry.crystals : entry.coins;

    auto valueLabel = CCLabelBMFont::create(std::to_string(primaryValue).c_str(), "bigFont.fnt");
    valueLabel->setScale(0.45);
    valueLabel->setAnchorPoint({1, 0.5});
    valueLabel->setPosition({listWidth - 12, row->getContentHeight() / 2 + 10});
    row->addChild(valueLabel);

    float secondaryRightX = valueLabel->getPositionX() - valueLabel->getScaledContentWidth() - 4;

    auto icon = CCSprite::create(showCoins ? "GR_gauntletCoin_001.png"_spr : "GR_crystal_001.png"_spr);
    if (icon) {
        icon->setScale(0.35);
        icon->setAnchorPoint({1, 0.5});
        icon->setPosition({secondaryRightX, row->getContentHeight() / 2 + 10});
        row->addChild(icon);
        secondaryRightX = icon->getPositionX() - icon->getScaledContentWidth();
    }

    // The other stat, shown smaller and dimmer - vanity only, doesn't affect rank
    auto secondaryLabel = CCLabelBMFont::create(std::to_string(secondaryValue).c_str(), "bigFont.fnt");
    secondaryLabel->setScale(0.3);
    secondaryLabel->setOpacity(130);
    secondaryLabel->setAnchorPoint({1, 0.5});
    secondaryLabel->setPosition({listWidth - 12, row->getContentHeight() / 2 - 10});
    row->addChild(secondaryLabel);

    auto secondaryIcon = CCSprite::create(showCoins ? "GR_crystal_001.png"_spr : "GR_gauntletCoin_001.png"_spr);
    if (secondaryIcon) {
        secondaryIcon->setScale(0.27);
        secondaryIcon->setOpacity(130);
        secondaryIcon->setAnchorPoint({1, 0.5});
        secondaryIcon->setPosition({
            secondaryLabel->getPositionX() - secondaryLabel->getScaledContentWidth() - 3,
            row->getContentHeight() / 2 - 10
        });
        row->addChild(secondaryIcon);
    }

    m_entryList->addChild(row);
}

void LeaderboardPopup::queueIconFetch(int accountId, SimplePlayer* player) {
    m_pendingIconPlayers[accountId] = player;
    m_iconFetchQueue.push_back(accountId);
    if (!m_fetchingIcon) fetchNextIcon();
}

void LeaderboardPopup::fetchNextIcon() {
    if (m_iconFetchQueue.empty()) {
        m_fetchingIcon = false;
        return;
    }
    m_fetchingIcon = true;

    int accountId = m_iconFetchQueue.front();
    m_iconFetchQueue.erase(m_iconFetchQueue.begin());

    auto glm = GameLevelManager::get();
    glm->m_userInfoDelegate = this;
    glm->getGJUserInfo(accountId);
}

void LeaderboardPopup::applyIcon(SimplePlayer* player, GJUserScore* score) {
    if (!player || !score) return;
    int iconId = score->m_playerCube;
    player->updatePlayerFrame(iconId, score->m_iconType);

    auto gm = GameManager::sharedState();
    player->setColors(gm->colorForIdx(score->m_color1), gm->colorForIdx(score->m_color2));
    if (score->m_glowEnabled) player->setGlowOutline(gm->colorForIdx(score->m_color3));
    else player->disableGlowOutline();
}

void LeaderboardPopup::getUserInfoFinished(GJUserScore* score) {
    auto glm = GameLevelManager::get();
    if (glm->m_userInfoDelegate == this) glm->m_userInfoDelegate = nullptr;

    if (score) {
        log::debug(
            "LeaderboardPopup: fetched accountId={} iconType={} playerCube={} color1={} color2={} glow={}",
            score->m_accountID, (int)score->m_iconType, score->m_playerCube,
            score->m_color1, score->m_color2, score->m_glowEnabled
        );
        auto it = m_pendingIconPlayers.find(score->m_accountID);
        if (it != m_pendingIconPlayers.end()) {
            applyIcon(it->second, score);
            m_pendingIconPlayers.erase(it);
        } else {
            log::debug("LeaderboardPopup: no pending row waiting for accountId={}", score->m_accountID);
        }
    } else {
        log::debug("LeaderboardPopup: getUserInfoFinished called with a null score");
    }

    fetchNextIcon();
}

void LeaderboardPopup::getUserInfoFailed(int accountId) {
    auto glm = GameLevelManager::get();
    if (glm->m_userInfoDelegate == this) glm->m_userInfoDelegate = nullptr;

    log::warn("LeaderboardPopup: getUserInfoFailed for accountId={}", accountId);

    m_pendingIconPlayers.erase(accountId);
    fetchNextIcon();
}

void LeaderboardPopup::onInfo(CCObject*) {
    MDPopup::create(
        "Leaderboard Info",
        "This leaderboard tracks two stats: <cy>Crystals</c>, earned by completing "
        "Gauntlet levels, and <co>Coins</c>, earned by fully completing Custom Gauntlets. "
        "Use the tabs above to switch between rankings, and tap <cg>Sync</c> to push your "
        "latest stats to the server.",
        "OK"
    )->show();
}
