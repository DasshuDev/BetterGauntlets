// GauntletNode.hpp
#pragma once
#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/GauntletNode.hpp>

class $modify(RedesignedGauntletNode, GauntletNode) {
struct Fields {
    cocos2d::CCSprite* m_gradient = nullptr;
    cocos2d::CCSprite* m_checkmarkSpr = nullptr;
    cocos2d::CCSprite* m_checkmarkShadowSpr = nullptr;
    cocos2d::CCSprite* m_claimNode = nullptr;   
};
public:
    static gd::string frameForType(GauntletType type);
    void generateNode();
    // void onClaimReward();

};