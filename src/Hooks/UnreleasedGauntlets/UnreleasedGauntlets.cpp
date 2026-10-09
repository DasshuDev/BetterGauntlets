#include "UnreleasedGauntlets.hpp"
#include <set>

using namespace geode::prelude;

static const std::vector<GauntletType> ALL_GAUNTLET_TYPES = {
    GauntletType::Fire,
    GauntletType::Ice,
    GauntletType::Poison,
    GauntletType::Shadow,
    GauntletType::Lava,
    GauntletType::Bonus,
    GauntletType::Chaos,
    GauntletType::Demon,
    GauntletType::Time,
    GauntletType::Crystal,
    GauntletType::Magic,
    GauntletType::Spike,
    GauntletType::Monster,
    GauntletType::Doom,
    GauntletType::Death,
    GauntletType::Forest,
    GauntletType::Rune,
    GauntletType::Force,
    GauntletType::Spooky,
    GauntletType::Dragon,
    GauntletType::Water,
    GauntletType::Haunted,
    GauntletType::Acid,
    GauntletType::Witch,
    GauntletType::Power,
    GauntletType::Potion,
    GauntletType::Snake,
    GauntletType::Toxic,
    GauntletType::Halloween,
    GauntletType::Treasure,
    GauntletType::Ghost,
    GauntletType::Spider,
    GauntletType::Gem,
    GauntletType::Inferno,
    GauntletType::Portal,
    GauntletType::Strange,
    GauntletType::Fantasy,
    GauntletType::Christmas,
    GauntletType::Surprise,
    GauntletType::Mystery,
    GauntletType::Cursed,
    GauntletType::Cyborg,
    GauntletType::Castle,
    GauntletType::Grave,
    GauntletType::Temple,
    GauntletType::World,
    GauntletType::Galaxy,
    GauntletType::Universe,
    GauntletType::Discord,
    GauntletType::Split,
    GauntletType::NCS,
    GauntletType::NCS2,
    GauntletType::Space,
    GauntletType::Cosmos,
    GauntletType::Random,
    GauntletType::Chance,
    GauntletType::Future,
    GauntletType::Utopia,
    GauntletType::Cinema,
    GauntletType::Love
};

static std::set<int> s_unreleasedIDs;

bool UnreleasedGauntlets::isEnabled() {
    return Mod::get()->getSettingValue<bool>("unreleased-gauntlets");
}

void UnreleasedGauntlets::appendMissingGauntlets(CCArray* packs) {
    s_unreleasedIDs.clear();
    if (!packs || !isEnabled()) return;

    std::set<int> existingIDs;
    std::vector<std::string> foundIDs;
    for (auto* pack : CCArrayExt<GJMapPack*>(packs)) {
        existingIDs.insert(pack->m_packID);
        foundIDs.push_back(std::to_string(pack->m_packID));
    }
    log::info("Server returned {} gauntlets: {}", foundIDs.size(), fmt::join(foundIDs, ", "));

    for (auto type : ALL_GAUNTLET_TYPES) {
        int id = static_cast<int>(type);
        if (existingIDs.count(id)) continue;

        std::string idStr = std::to_string(id);
        auto dict = CCDictionary::create();
        dict->setObject(CCString::create(idStr.c_str()), "1");
        dict->setObject(CCString::create("-1,-1,-1,-1,-1"), "3");

        auto pack = GJMapPack::create(dict);
        pack->m_isGauntlet = true;
        packs->addObject(pack);
        s_unreleasedIDs.insert(id);
    }

    std::vector<std::string> unreleasedIDs;
    for (int id : s_unreleasedIDs) unreleasedIDs.push_back(std::to_string(id));
    log::info("Added {} unreleased gauntlets: {}", unreleasedIDs.size(), fmt::join(unreleasedIDs, ", "));
}

bool UnreleasedGauntlets::isUnreleased(GauntletType type) {
    return s_unreleasedIDs.count(static_cast<int>(type)) > 0;
}
