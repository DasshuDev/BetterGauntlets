#pragma once
#include <Geode/Geode.hpp>
#include <string>
// #include <vector>

using namespace geode::prelude;

// Per-level entry inside a custom gauntlet
struct CustomSlotLevel {
    int         id      = 0;
    std::string name;
    std::string creator;
    int         stars   = 0;
};

// Full server-side data for a published custom gauntlet
struct CustomGauntletData {
    int         id           = 0;
    std::string name;
    std::string description;
    std::string iconURL;

    ccColor3B   nameColor    = {255, 255, 255};
    ccColor3B   nodeColor    = {255, 255, 255};
    ccColor3B   bgColor      = {34,  34,  34 };
    ccColor3B   accentColor1 = {255, 255, 255};
    ccColor3B   accentColor2 = {255, 255, 255};

    int         bgIndex      = 1;

    std::array<CustomSlotLevel, 5> levels;

    std::string infoDate;
    std::string infoVersion;
    std::string infoSuggester;
    int         infoAccID    = 0;

    bool        featured     = false;
    int         rewardCoins  = 0;

    // Unused - no call sites. CustomGauntletManager::parse() (colon-delimited format) and
    // GauntletManagerPopup's local parseGauntletJson() are the parsers actually in use.
    /*
    static CustomGauntletData fromJson(matjson::Value const& obj) {
        CustomGauntletData d;
        d.id          = obj["id"].asInt().unwrapOr(0);
        d.name        = obj["name"].asString().unwrapOr("");
        d.description = obj["description"].asString().unwrapOr("");
        d.iconURL     = obj["icon_url"].asString().unwrapOr("");

        d.nameColor    = { (GLubyte)obj["name_color_r"].asInt().unwrapOr(255),
                           (GLubyte)obj["name_color_g"].asInt().unwrapOr(255),
                           (GLubyte)obj["name_color_b"].asInt().unwrapOr(255) };
        d.nodeColor    = { (GLubyte)obj["node_color_r"].asInt().unwrapOr(255),
                           (GLubyte)obj["node_color_g"].asInt().unwrapOr(255),
                           (GLubyte)obj["node_color_b"].asInt().unwrapOr(255) };
        d.bgColor      = { (GLubyte)obj["color_r"].asInt().unwrapOr(34),
                           (GLubyte)obj["color_g"].asInt().unwrapOr(34),
                           (GLubyte)obj["color_b"].asInt().unwrapOr(34) };
        d.accentColor1 = { (GLubyte)obj["accent_color1_r"].asInt().unwrapOr(255),
                           (GLubyte)obj["accent_color1_g"].asInt().unwrapOr(255),
                           (GLubyte)obj["accent_color1_b"].asInt().unwrapOr(255) };
        d.accentColor2 = { (GLubyte)obj["accent_color2_r"].asInt().unwrapOr(255),
                           (GLubyte)obj["accent_color2_g"].asInt().unwrapOr(255),
                           (GLubyte)obj["accent_color2_b"].asInt().unwrapOr(255) };

        d.bgIndex       = obj["bg_index"].asInt().unwrapOr(1);
        d.infoDate      = obj["info_date"].asString().unwrapOr("");
        d.infoVersion   = obj["info_version"].asString().unwrapOr("");
        d.infoSuggester = obj["info_suggester"].asString().unwrapOr("");
        d.infoAccID     = obj["info_acc_id"].asInt().unwrapOr(0);
        d.featured      = obj["featured"].asBool().unwrapOr(false);

        if (obj.contains("levels") && obj["levels"].isArray()) {
            int i = 0;
            for (auto const& lvl : obj["levels"]) {
                if (i >= 5) break;
                d.levels[i].id      = lvl["level_id"].asInt().unwrapOr(0);
                d.levels[i].name    = lvl["level_name"].asString().unwrapOr("");
                d.levels[i].creator = lvl["creator"].asString().unwrapOr("");
                d.levels[i].stars   = lvl["stars"].asInt().unwrapOr(0);
                i++;
            }
        }
        return d;
    }
    */
};