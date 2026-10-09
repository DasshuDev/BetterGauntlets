#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

namespace UnreleasedGauntlets {
    bool isEnabled();
    void appendMissingGauntlets(CCArray* packs);
    bool isUnreleased(GauntletType type);
}
