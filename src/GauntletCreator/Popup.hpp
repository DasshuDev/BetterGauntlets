#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <UIBuilder.hpp>

using namespace geode::prelude;

class GauntletCreator : public Popup<>, public ColorPickPopupDelegate {
protected:
    bool setup() override;

    void onCreatorInfoPressed(CCObject* sender);
    void onColorPicked(ccColor4B color);
    void onColorSetting(CCObject*);
    void colorPickPopupDidSelectColor(
        ColorPickPopup* popup,
        ccColor4B color
    );
    
    cocos2d::CCMenu* createToggleSetting(const std::string& title, const std::string& key);
    cocos2d::CCMenu* createColorSetting(const std::string& title, const std::string& key);
    

public:
    static GauntletCreator* create();

    ccColor4B m_gauntletColor1 = Mod::get()->getSavedValue<ccColor4B>("testColor1", ccColor4B{0, 255, 0, 255});
    ccColor4B m_gauntletColor2 = Mod::get()->getSavedValue<ccColor4B>("testColor2", ccColor4B{0, 255, 255, 255});
    ccColor4B m_currentColor;
    std::string m_colorKey;
    CCSprite* m_colorPreview = nullptr;
};
