#include "GauntletManagerAPI.hpp"

GauntletManagerAPI* GauntletManagerAPI::get() {
    static GauntletManagerAPI instance;
    return &instance;
}

std::string GauntletManagerAPI::baseURL() {
    return Mod::get()->getSettingValue<std::string>("server-url-base");
}

bool GauntletManagerAPI::isLoggedIn() const { return !m_token.empty(); }
void GauntletManagerAPI::setToken(std::string const& token) { m_token = token; }
std::string const& GauntletManagerAPI::getToken() const { return m_token; }

web::WebFuture GauntletManagerAPI::login(
    std::string const& username,
    std::string const& password
) {
    auto body = matjson::Value();
    body["username"] = username;
    body["password"] = password;

    return web::WebRequest()
        .header("Content-Type", "application/json")
        .bodyString(body.dump())
        .post(baseURL() + "/login");
}

web::WebFuture GauntletManagerAPI::fetchAll() {
    return web::WebRequest()
        .header("Authorization", "Bearer " + m_token)
        .get(baseURL() + "/manage");
}

static matjson::Value buildGauntletBody(GauntletEditData const& data) {
    auto levels = matjson::Value::array();
    for (auto const& slot : data.slots) {
        auto lvl = matjson::Value();
        lvl["level_id"]   = slot.id;
        lvl["level_name"] = slot.name;
        lvl["creator"]    = slot.creator;
        lvl["stars"]      = slot.stars;
        levels.push(lvl);
    }
    auto body = matjson::Value();
    body["name"]         = data.name;
    body["description"]  = data.description;
    body["icon_url"]     = data.iconURL;
    body["color_r"]      = (int)data.bgColor.r;
    body["color_g"]      = (int)data.bgColor.g;
    body["color_b"]      = (int)data.bgColor.b;
    body["name_color_r"] = (int)data.nameColor.r;
    body["name_color_g"] = (int)data.nameColor.g;
    body["name_color_b"] = (int)data.nameColor.b;
    body["levels"]       = levels;
    return body;
}

web::WebFuture GauntletManagerAPI::create(GauntletEditData const& data) {
    return web::WebRequest()
        .header("Content-Type", "application/json")
        .header("Authorization", "Bearer " + m_token)
        .bodyString(buildGauntletBody(data).dump())
        .post(baseURL() + "/manage");
}

web::WebFuture GauntletManagerAPI::update(GauntletEditData const& data) {
    return web::WebRequest()
        .header("Content-Type", "application/json")
        .header("Authorization", "Bearer " + m_token)
        .bodyString(buildGauntletBody(data).dump())
        .put(baseURL() + fmt::format("/manage?id={}", data.id));
}

web::WebFuture GauntletManagerAPI::remove(int id) {
    return web::WebRequest()
        .header("Authorization", "Bearer " + m_token)
        .send("DELETE", baseURL() + fmt::format("/manage?id={}", id));
}

web::WebFuture GauntletManagerAPI::uploadIcon(std::filesystem::path const& path) {
    auto bytes = file::readBinary(path);
    if (!bytes) return web::WebRequest().get("");

    return web::WebRequest()
        .header("Authorization",  "Bearer " + m_token)
        .header("x-filename",     path.filename().string())
        .header("x-content-type", "image/png")
        .body(bytes.unwrap())
        .post(baseURL() + "/upload");
}

web::WebFuture GauntletManagerAPI::checkIsManager(int accountID) {
    return web::WebRequest()
        .get(baseURL() + fmt::format("/ismanager?accountId={}", accountID));
}