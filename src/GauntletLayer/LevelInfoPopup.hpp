#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class LevelInfoPopup : public geode::Popup<GJGameLevel*> {
protected:
    GJGameLevel* m_level;
    
    bool setup(GJGameLevel* level) override;

public:
    static LevelInfoPopup* create(GJGameLevel* level);
    void onPlay(CCObject* sender);
};