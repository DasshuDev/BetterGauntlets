// GauntletNode.hpp
#pragma once
#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/GauntletNode.hpp>

class $modify(RedesignedGauntletNode, GauntletNode) {
struct Fields {
    cocos2d::CCSprite* m_gradient = nullptr;
    cocos2d::CCParticleSystemQuad* m_gradientParticles = nullptr;
};
public:
    static gd::string frameForType(GauntletType type);
    void generateNode();
    void onClaimReward();

};