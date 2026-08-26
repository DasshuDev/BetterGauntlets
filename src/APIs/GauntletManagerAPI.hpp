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

    // Coins credited to the completing player when all 5 levels are claimed
    int rewardCoins = 0;

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
    web::WebFuture fetchAll();
    web::WebFuture create(GauntletEditData const& data);
    web::WebFuture update(GauntletEditData const& data);
    web::WebFuture remove(int id);
    web::WebFuture setFeatured(int id);
    web::WebFuture uploadIcon(std::filesystem::path const& path);
    web::WebFuture fetchManagers();
    web::WebFuture fetchHelpers();
    web::WebFuture fetchSupporters();

    bool isLoggedIn() const;
    void setToken(std::string const& token);
    std::string const& getToken() const;

private:
    GauntletManagerAPI() = default;
    std::string m_token;
    std::string baseURL();
};