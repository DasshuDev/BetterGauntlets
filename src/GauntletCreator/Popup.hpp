#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <UIBuilder.hpp>

using namespace geode::prelude;

class GauntletCreator : public Popup<>, public ColorPickPopupDelegate {
protected:
    bool setup() override;
    
    cocos2d::CCMenu* createToggleSetting(const std::string& title, const std::string& key);
    cocos2d::CCMenu* createColorSetting(const std::string& title, const std::string& key);

public:
    static GauntletCreator* create();
};
