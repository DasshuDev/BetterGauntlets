#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../Data/CustomGauntletData.hpp"

using namespace geode::prelude;

// Represents a level picked from search to fill a slot
struct SlotLevel {
    int         id      = 0;
    std::string name;
    std::string creator;
    int         stars   = 0;
};

// Full data for creating/editing a gauntlet from the in-game panel
struct GauntletEditData {
    int         id          = 0;   // 0 = new gauntlet
    std::string name;
    std::string description;
    std::string iconURL;
    ccColor3B   nameColor   = {255, 255, 255};
    ccColor3B   bgColor     = {255, 255, 255};
    std::array<SlotLevel, 5> slots;
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