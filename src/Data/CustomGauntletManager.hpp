#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "CustomGauntletData.hpp"

using namespace geode::prelude;

class CustomGauntletManager {
public:
    static CustomGauntletManager* get();

    // Fetch all published gauntlets from server
    web::WebFuture fetchAll();

    // Parse the server's colon-delimited response into a list
    std::vector<CustomGauntletData> parse(std::string const& body);

    // Cache
    bool hasCached() const;
    std::vector<CustomGauntletData> const& getCached() const;
    void clearCache();

private:
    CustomGauntletManager() = default;
    std::vector<CustomGauntletData> m_cache;
    bool m_hasCached = false;

    static std::string baseURL();
};