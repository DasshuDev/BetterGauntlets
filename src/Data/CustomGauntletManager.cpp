#include "CustomGauntletManager.hpp"
#include <sstream>
#include <map>

using namespace geode::prelude;

CustomGauntletManager* CustomGauntletManager::get() {
    static CustomGauntletManager instance;
    return &instance;
}

std::string CustomGauntletManager::baseURL() {
    return "https://bettergauntlets.dev";
}

web::WebFuture CustomGauntletManager::fetchAll() {
    return web::WebRequest()
        .get(baseURL() + "/gauntlets");
}

// Helpers for the colon-delimited response format 

static std::vector<std::string> splitString(std::string const& str, char delim) {
    std::vector<std::string> parts;
    std::stringstream ss(str);
    std::string part;
    while (std::getline(ss, part, delim))
        parts.push_back(part);
    return parts;
}

static int toInt(std::string const& str, int def = 0) {
    return geode::utils::numFromString<int>(str).unwrapOr(def);
}

static ccColor3B parseColor(std::string const& str, ccColor3B def = {255, 255, 255}) {
    auto parts = splitString(str, ',');
    if (parts.size() < 3) return def;
    return {
        (GLubyte)toInt(parts[0], def.r),
        (GLubyte)toInt(parts[1], def.g),
        (GLubyte)toInt(parts[2], def.b)
    };
}

static CustomSlotLevel parseSlotLevel(std::string const& str) {
    CustomSlotLevel lvl;
    auto first = str.find(',');
    auto last  = str.rfind(',');
    if (first == std::string::npos || last == first) return lvl;

    auto mid = str.rfind(',', last - 1);
    if (mid == std::string::npos || mid == first) {
        auto parts = splitString(str, ',');
        if (parts.size() >= 4) {
            lvl.id      = toInt(parts[0]);
            lvl.name    = parts[1];
            lvl.creator = parts[2];
            lvl.stars   = toInt(parts[3]);
        }
        return lvl;
    }

    lvl.id      = toInt(str.substr(0, first));
    lvl.name    = str.substr(first + 1, mid - first - 1);
    lvl.creator = str.substr(mid + 1,   last - mid - 1);
    lvl.stars   = toInt(str.substr(last + 1));
    return lvl;
}

static std::map<int, std::string> parseGDFields(std::string const& entry) {
    std::map<int, std::string> fields;

    struct Pos { int key; size_t valueStart; size_t sepStart; };
    std::vector<Pos> positions;

    if (entry.size() >= 2 && entry[0] == '1' && entry[1] == ':')
        positions.push_back({1, 2, 0});

    for (int key = 2; key <= 20; key++) {
        auto marker = fmt::format(":{}:", key);
        size_t from = positions.empty() ? 0 : positions.back().valueStart;
        auto pos = entry.find(marker, from);
        if (pos != std::string::npos)
            positions.push_back({key, pos + marker.size(), pos});
    }

    for (size_t i = 0; i < positions.size(); i++) {
        size_t end = (i + 1 < positions.size())
            ? positions[i + 1].sepStart
            : entry.size();
        fields[positions[i].key] =
            entry.substr(positions[i].valueStart, end - positions[i].valueStart);
    }
    return fields;
}

static std::string stripPrefix(std::string const& str, std::string const& prefix) {
    if (str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0)
        return str.substr(prefix.size());
    return str;
}

// parse 

std::vector<CustomGauntletData> CustomGauntletManager::parse(std::string const& body) {
    m_cache.clear();

    std::stringstream ss(body);
    std::string entry;

    while (std::getline(ss, entry, '|')) {
        if (entry.empty()) continue;

        auto f = parseGDFields(entry);
        if (f.empty()) continue;

        CustomGauntletData d;

        if (f.count(1))  d.id      = toInt(f[1]);
        if (f.count(2))  d.name    = f[2];
        if (f.count(3))  d.iconURL = f[3];
        if (f.count(4))  d.bgColor = parseColor(f[4], {34, 34, 34});

        for (int i = 0; i < 5; i++)
            if (f.count(5 + i)) d.levels[i] = parseSlotLevel(f[5 + i]);

        if (f.count(10)) d.nameColor    = parseColor(f[10]);
        if (f.count(11)) d.nodeColor    = parseColor(f[11]);
        if (f.count(12)) d.accentColor1 = parseColor(f[12]);
        if (f.count(13)) d.accentColor2 = parseColor(f[13]);
        if (f.count(14)) d.bgIndex      = toInt(f[14], 1);
        if (f.count(15)) d.description  = f[15];

        if (f.count(16)) {
            auto info = splitString(f[16], ',');
            if (info.size() >= 1) d.infoDate      = stripPrefix(info[0], "Released on: ");
            if (info.size() >= 2) d.infoVersion    = stripPrefix(info[1], "Version: ");
            if (info.size() >= 3) d.infoSuggester  = stripPrefix(info[2], "Suggested by: ");
            if (info.size() >= 4) d.infoAccID      = toInt(info[3]);
        }

        if (f.count(17)) d.featured    = toInt(f[17]) != 0;
        if (f.count(18)) d.rewardCoins = toInt(f[18], 0);

        m_cache.push_back(d);
    }

    m_hasCached = true;
    return m_cache;
}

bool CustomGauntletManager::hasCached() const { return m_hasCached; }

std::vector<CustomGauntletData> const& CustomGauntletManager::getCached() const {
    return m_cache;
}

void CustomGauntletManager::clearCache() {
    m_cache.clear();
    m_hasCached = false;
}

void CustomGauntletManager::beginFetch() {
    m_isFetching = true;
    m_fetchHolder.spawn(fetchAll(), [this](web::WebResponse res) {
        m_isFetching = false;
        bool ok = res.ok();
        if (ok) parse(res.string().unwrapOr(""));

        auto waiting = std::move(m_waitingCallbacks);
        m_waitingCallbacks.clear();
        for (auto& cb : waiting) cb(ok, res.code());
    });
}

void CustomGauntletManager::warm() {
    if (m_hasCached || m_isFetching) return;
    beginFetch();
}

void CustomGauntletManager::whenReady(std::function<void(bool, int)> callback) {
    if (m_hasCached) {
        callback(true, 200);
        return;
    }
    m_waitingCallbacks.push_back(std::move(callback));
    if (!m_isFetching) beginFetch();
}

cocos2d::CCTexture2D* CustomGauntletManager::getCachedIcon(std::string const& url) const {
    auto it = m_iconCache.find(url);
    if (it == m_iconCache.end()) return nullptr;
    return it->second;
}

void CustomGauntletManager::cacheIcon(std::string const& url, cocos2d::CCTexture2D* texture) {
    if (!texture) return;
    m_iconCache[url] = texture;
}

bool CustomGauntletManager::isCustomGauntletLevel(int levelID) const {
    for (auto const& gauntlet : m_cache) {
        for (auto const& slot : gauntlet.levels) {
            if (slot.id == levelID) return true;
        }
    }
    return false;
}

CustomGauntletData const* CustomGauntletManager::findGauntletForLevel(int levelID) const {
    for (auto const& gauntlet : m_cache) {
        for (auto const& slot : gauntlet.levels) {
            if (slot.id == levelID) return &gauntlet;
        }
    }
    return nullptr;
}

bool CustomGauntletManager::isGauntletFullyCompleted(CustomGauntletData const& gauntlet) const {
    bool hasAnyLevel = false;
    for (auto const& slot : gauntlet.levels) {
        if (slot.id == 0) continue; // unused slot - gauntlets aren't required to fill all 5
        hasAnyLevel = true;
        if (!isLevelRewardClaimed(slot.id)) return false;
    }
    return hasAnyLevel;
}

int CustomGauntletManager::crystalsForLevel(GJGameLevel* level) {
    if (!level) return 0;

    if (level->m_demon.value()) {
        switch (static_cast<DemonDifficultyType>(level->m_demonDifficulty)) {
            case DemonDifficultyType::EasyDemon:    return 30;
            case DemonDifficultyType::MediumDemon:  return 40;
            case DemonDifficultyType::HardDemon:    return 50;
            case DemonDifficultyType::InsaneDemon:  return 75;
            case DemonDifficultyType::ExtremeDemon: return 100;
            default:                                return 50; // unrated demon defaults to hard
        }
    }

    switch (static_cast<GJDifficulty>(level->getAverageDifficulty())) {
        case GJDifficulty::Auto:   return 0;
        case GJDifficulty::Easy:   return 5;
        case GJDifficulty::Normal: return 10;
        case GJDifficulty::Hard:   return 15;
        case GJDifficulty::Harder: return 20;
        case GJDifficulty::Insane: return 25;
        default:                   return 0;  // NA / unrated
    }
}

// Player progress

void CustomGauntletManager::markPendingGauntletAttempt(int levelID) {
    m_pendingGauntletLevelID = levelID;
}

bool CustomGauntletManager::consumePendingGauntletAttempt(int levelID) {
    bool matches = m_pendingGauntletLevelID != 0 && m_pendingGauntletLevelID == levelID;
    m_pendingGauntletLevelID = 0;
    return matches;
}

void CustomGauntletManager::markPendingGauntletReward(int gauntletId, int coins) {
    m_pendingRewardGauntletID = gauntletId;
    m_pendingRewardCoins = coins;
}

bool CustomGauntletManager::consumePendingGauntletReward(int gauntletId, int& outCoins) {
    bool matches = m_pendingRewardGauntletID != 0 && m_pendingRewardGauntletID == gauntletId;
    if (matches) outCoins = m_pendingRewardCoins;
    m_pendingRewardGauntletID = 0;
    m_pendingRewardCoins = 0;
    return matches;
}

bool CustomGauntletManager::isLevelRewardClaimed(int levelID) const {
    auto claimed = Mod::get()->getSavedValue<matjson::Value>(
        "claimed-level-rewards", matjson::Value::array()
    );
    if (!claimed.isArray()) return false;
    for (auto const& id : claimed) {
        if (id.asInt().unwrapOr(0) == levelID) return true;
    }
    return false;
}

void CustomGauntletManager::markLevelRewardClaimed(int levelID) {
    auto claimed = Mod::get()->getSavedValue<matjson::Value>(
        "claimed-level-rewards", matjson::Value::array()
    );
    if (!claimed.isArray()) claimed = matjson::Value::array();
    claimed.push(levelID);
    Mod::get()->setSavedValue("claimed-level-rewards", claimed);
}

int CustomGauntletManager::getCrystalTotal() const {
    return Mod::get()->getSavedValue<int>("crystal-total", 0);
}

void CustomGauntletManager::addCrystals(int amount) {
    if (amount <= 0) return;
    Mod::get()->setSavedValue("crystal-total", getCrystalTotal() + amount);
}

void CustomGauntletManager::resetCrystals() {
    Mod::get()->setSavedValue("crystal-total", 0);
}

int CustomGauntletManager::getCoinTotal() const {
    return Mod::get()->getSavedValue<int>("coin-total", 0);
}

void CustomGauntletManager::addCoins(int amount) {
    if (amount <= 0) return;
    Mod::get()->setSavedValue("coin-total", getCoinTotal() + amount);
}

void CustomGauntletManager::resetCoins() {
    Mod::get()->setSavedValue("coin-total", 0);
}

void CustomGauntletManager::setCrystalTotal(int value) {
    Mod::get()->setSavedValue("crystal-total", value);
}

void CustomGauntletManager::setCoinTotal(int value) {
    Mod::get()->setSavedValue("coin-total", value);
}

void CustomGauntletManager::resetClaimedRewards() {
    Mod::get()->setSavedValue("claimed-level-rewards", matjson::Value::array());
}
