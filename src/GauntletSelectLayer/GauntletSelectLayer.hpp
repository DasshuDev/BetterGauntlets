#ifndef GAUNTLETSELECTLAYER_HPP
#define GAUNTLETSELECTLAYER_HPP

#ifndef DEBUG_BUILD

#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/GauntletSelectLayer.hpp>

class $modify(GauntletSelectLayerHook, GauntletSelectLayer) {
    struct Fields {
        bool m_loaded = false;
    };

    // add stuff here in a future update/rewrite
}

#endif