#ifndef GAUNTLETLAYER_HPP
#define GAUNTLETLAYER_HPP

#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/GauntletLayer.hpp>

class $modify(RedesignedGauntletLayer, GauntletLayer) {
    struct Fields {
        bool m_loaded = false;
    };

    void gauntletLevel(int);
    cocos2d::CCNode* getChildBySpriteFrameNameRecursive(cocos2d::CCNode*, char const*);

    #ifdef GEODE_IS_WINDOWS
    void defineKeybind(const char*, std::function<void()>);
    #endif

    void loadLevelsFinished(cocos2d::CCArray*, char const*, int);
    void editGauntlets();
    void editGauntletSingle(std::string, cocos2d::ccColor3B, cocos2d::ccColor3B, cocos2d::ccColor3B);

    void editFireGauntlet();
    void editIceGauntlet();

    bool init(GauntletType);
};
#endif