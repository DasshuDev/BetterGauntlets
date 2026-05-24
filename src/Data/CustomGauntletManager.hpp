#pragma once
#include <Geode/Geode.hpp>
#include "CustomGauntletData.hpp"

using namespace geode::prelude;

class CustomGauntletManager {
public:
    static CustomGauntletManager* get();

    // Fetches from server URL setting and parses response.
    // Calls onSuccess with parsed gauntlets, or onFailure with error string.
    void fetch(
        std::function<void(std::vector<CustomGauntletData>)> onSuccess,
        std::function<void(std::string)> onFailure
    );

    // Returns cached result from last successful fetch, empty if never fetched.
    std::vector<CustomGauntletData> const& getCached() const;

    bool hasCached() const;

private:
    CustomGauntletManager() = default;
    std::vector<CustomGauntletData> m_cache;
    bool m_hasCached = false;

    // Parses raw GD gauntlet response body into a vector of CustomGauntletData.
    std::vector<CustomGauntletData> parse(std::string const& raw);
};