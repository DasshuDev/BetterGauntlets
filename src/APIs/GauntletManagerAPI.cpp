#include "GauntletManagerAPI.hpp"

GauntletManagerAPI* GauntletManagerAPI::get() {
    static GauntletManagerAPI instance;
    return &instance;
}

std::string GauntletManagerAPI::baseURL() {
    return std::string("https://bettergauntlets.dev");
}

bool GauntletManagerAPI::isLoggedIn() const { return !m_token.empty(); }
void GauntletManagerAPI::setToken(std::string const& token) { m_token = token; }
std::string const& GauntletManagerAPI::getToken() const { return m_token; }

web::WebFuture GauntletManagerAPI::fetchAll() {
    return web::WebRequest()
        .header("Authorization", "Bearer " + m_token)
        .header("X-Account-Id", std::to_string(GJAccountManager::get()->m_accountID))
        .get(baseURL() + "/manage");
}

static matjson::Value buildGauntletBody(GauntletEditData const& data) {
    auto levels = matjson::Value::array();
    for (auto const& slot : data.levels) {
        auto lvl = matjson::Value();
        lvl["level_id"]   = slot.id;
        lvl["level_name"] = slot.name;
        lvl["creator"]    = slot.creator;
        lvl["stars"]      = slot.stars;
        levels.push(lvl);
    }
    auto body = matjson::Value();

    // general
    body["name"]            = data.name;
    body["icon_url"]        = data.iconURL;

    // shown in GauntletSelectLayer
    body["name_color_r"]    = (int)data.nameColor.r;
    body["name_color_g"]    = (int)data.nameColor.g;
    body["name_color_b"]    = (int)data.nameColor.b;
    body["node_color_r"]    = (int)data.nodeColor.r;
    body["node_color_g"]    = (int)data.nodeColor.g;
    body["node_color_b"]    = (int)data.nodeColor.b;

    // shown in GauntletLayer
    body["description"]     = data.description;
    body["background"]      = (int)data.bgIndex;
    body["color_r"]         = (int)data.bgColor.r;
    body["color_g"]         = (int)data.bgColor.g;
    body["color_b"]         = (int)data.bgColor.b;
    body["levels"]          = levels;
    body["accent_color1_r"] = (int)data.accentColor1.r;
    body["accent_color1_g"] = (int)data.accentColor1.g;
    body["accent_color1_b"] = (int)data.accentColor1.b;
    body["accent_color2_r"] = (int)data.accentColor2.r;
    body["accent_color2_g"] = (int)data.accentColor2.g;
    body["accent_color2_b"] = (int)data.accentColor2.b;
    body["reward_coins"]    = data.rewardCoins;

    // info card
    body["info_date"]       = data.infoDate;
    body["info_version"]    = data.infoVersion;
    body["info_suggester"]  = data.infoSuggester;
    body["info_acc_id"]     = data.infoAccID;

    return body;
}

web::WebFuture GauntletManagerAPI::create(GauntletEditData const& data) {
    log::debug(
        "create: token empty={}, accountId={}, body={}",
        m_token.empty(), GJAccountManager::get()->m_accountID, buildGauntletBody(data).dump()
    );
    return web::WebRequest()
        .header("Content-Type", "application/json")
        .header("Authorization", "Bearer " + m_token)
        .header("X-Account-Id", std::to_string(GJAccountManager::get()->m_accountID))
        .bodyString(buildGauntletBody(data).dump())
        .post(baseURL() + "/manage");
}

web::WebFuture GauntletManagerAPI::update(GauntletEditData const& data) {
    return web::WebRequest()
        .header("Content-Type", "application/json")
        .header("Authorization", "Bearer " + m_token)
        .header("X-Account-Id", std::to_string(GJAccountManager::get()->m_accountID))
        .bodyString(buildGauntletBody(data).dump())
        .put(baseURL() + fmt::format("/manage?id={}", data.id));
}

web::WebFuture GauntletManagerAPI::remove(int id) {
    log::debug(
        "remove: token empty={}, accountId={}, id={}",
        m_token.empty(), GJAccountManager::get()->m_accountID, id
    );
    return web::WebRequest()
        .header("Authorization", "Bearer " + m_token)
        .header("X-Account-Id", std::to_string(GJAccountManager::get()->m_accountID))
        .send("DELETE", baseURL() + fmt::format("/manage?id={}", id));
}

web::WebFuture GauntletManagerAPI::setFeatured(int id) {
    return web::WebRequest()
        .header("Authorization", "Bearer " + m_token)
        .header("X-Account-Id", std::to_string(GJAccountManager::get()->m_accountID))
        .send("PATCH", baseURL() + fmt::format("/manage?id={}", id));
}

web::WebFuture GauntletManagerAPI::uploadIcon(std::filesystem::path const& path) {
    auto result = file::readBinary(path);
    if (!result) {
        log::error("uploadIcon: failed to read file: {}", result.unwrapErr());
        return web::WebRequest().get("");
    }

    auto bytes = std::move(result).unwrap();

    return web::WebRequest()
        .header("Authorization",  "Bearer " + m_token)
        .header("Content-Type",   "image/png")
        .header("x-filename",     path.filename().string())
        .header("x-account-id",   std::to_string(GJAccountManager::get()->m_accountID))
        .body(std::move(bytes))
        .post(baseURL() + "/upload");
}

web::WebFuture GauntletManagerAPI::fetchManagers() {
    return web::WebRequest().get(baseURL() + "/managers");
}

web::WebFuture GauntletManagerAPI::fetchHelpers() {
    return web::WebRequest().get(baseURL() + "/helpers");
}

web::WebFuture GauntletManagerAPI::fetchSupporters() {
    return web::WebRequest().get(baseURL() + "/supporters");
}