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

    auto title = CCLabelBMFont::create("Leaderboard", "goldFont.fnt");
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

    float listHeight = m_size.height - 60;
    float listCenterY = 20 + listHeight / 2;

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

    fetchLeaderboard();

    return true;
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
        LeaderboardAPI::get()->fetch(100),
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
    row->setContentSize({listWidth, 45});

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
    nameLabel->limitLabelWidth(listWidth - 165, 0.7, 0.1);

    auto nameMenu = CCMenu::create();
    nameMenu->setID("name-menu"_spr);
    nameMenu->setAnchorPoint({0, 0.5});
    nameMenu->setPosition({80, (row->getContentHeight() / 2) + 1});
    row->addChild(nameMenu); 

    int accountId = entry.accountId;
    auto nameBtn = CCMenuItemExt::createSpriteExtra(
        nameLabel,
        [accountId](CCMenuItemSpriteExtra*) {
        ProfilePage::create(accountId, false)->show();
    });
    // nameBtn->setScale(0.7);
    nameBtn->setAnchorPoint({0, 0.5});
    nameMenu->addChild(nameBtn);

    auto valueLabel = CCLabelBMFont::create(std::to_string(entry.crystals).c_str(), "bigFont.fnt");
    valueLabel->setScale(0.45);
    valueLabel->setAnchorPoint({1, 0.5});
    valueLabel->setPosition({listWidth - 12, row->getContentHeight() / 2});
    row->addChild(valueLabel);

    auto icon = CCSprite::create("GR_crystal_001.png"_spr);
    if (icon) {
        icon->setScale(0.35);
        icon->setAnchorPoint({1, 0.5});
        icon->setPosition({
            valueLabel->getPositionX() - valueLabel->getScaledContentWidth() - 4,
            row->getContentHeight() / 2
        });
        row->addChild(icon);
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
