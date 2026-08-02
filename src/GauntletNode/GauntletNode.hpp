// GauntletNode.hpp
#pragma once
#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/GauntletNode.hpp>

class $modify(RedesignedGauntletNode, GauntletNode) {
struct Fields {
    cocos2d::CCSprite* m_gradient = nullptr;
    cocos2d::CCParticleSystemQuad* m_gradientParticles = nullptr;
    // cocos2d::CCSprite* m_claimSpr = nullptr; // unused - created but never attached to the scene
};
public:
    static gd::string frameForType(GauntletType type);
    void generateNode();
    void onClaimReward();

};