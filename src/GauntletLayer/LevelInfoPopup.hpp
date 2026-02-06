#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class LevelInfoPopup : public Popup {
protected:
    GJGameLevel* m_level;
    
    bool init(GJGameLevel* level);
    void onPlay(CCObject* sender);

public:
    static LevelInfoPopup* create(GJGameLevel* level);
};