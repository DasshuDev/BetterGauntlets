#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
// #include "../Data/CustomGauntletData.hpp"

using namespace geode::prelude;

// Represents a level picked from search to fill a slot
struct SlotLevel {
    int id = 0;
    std::string name;
    std::string creator;
    int stars = 0;
};

struct GauntletUserInfo {
    int accountId = 0;
    std::string username;

    static GauntletUserInfo fromJson(const matjson::Value& val) {
        GauntletUserInfo info;
        info.accountId = val["accountId"].asInt().unwrapOr(0);
        info.username = val["username"].asString().unwrapOr("Unknown");
        return info;
    }
};

// Full data for creating/editing a gauntlet from the in-game panel
struct GauntletEditData {
    int id = 0;
    std::string name;
    std::string description;
    std::string iconURL;
    int bgIndex = 1;
    ccColor3B nameColor = {255, 255, 255};
    ccColor3B nodeColor = {255, 255, 255};
    ccColor3B bgColor = {255, 255, 255};
    ccColor3B accentColor1 = {255, 255, 255};
    ccColor3B accentColor2 = {255, 255, 255};
    std::array<SlotLevel, 5> levels;

    // Info metadata
    std::string infoDate;
    std::string infoVersion;
    std::string infoSuggester;
    int infoAccID = 0;
};

class GauntletManagerAPI {
public:
    static GauntletManagerAPI* get();

    // Auth
    web::WebFuture login(std::string const& username, std::string const& password);

    // CRUD (more like CHUD)
    web::WebFuture fetchAll();
    web::WebFuture create(GauntletEditData const& data);
    web::WebFuture update(GauntletEditData const& data);
    web::WebFuture remove(int id);

    // Upload icon file, returns public URL
    web::WebFuture uploadIcon(std::filesystem::path const& path);

    // Check if current GD account is a manager
    web::WebFuture checkIsManager(int accountID);

    bool isLoggedIn() const;
    void setToken(std::string const& token);
    std::string const& getToken() const;

private:
    GauntletManagerAPI() = default;
    std::string m_token;
    std::string baseURL();
};