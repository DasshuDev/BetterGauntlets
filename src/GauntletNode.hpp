#ifndef GAUNTLETNODE_HPP
#define GAUNTLETNODE_HPP

#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/GauntletNode.hpp>

class $modify(RedesignedGauntletNode, GauntletNode) {
    static gd::string frameForType(GauntletType);
};
#endif