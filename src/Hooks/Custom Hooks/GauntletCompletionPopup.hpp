#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include "ForceClosablePopup.hpp"

using namespace geode::prelude;

class GauntletCompletionPopup : public Popup, public ForceClosablePopup {
protected:
    bool init(GauntletType type, ccColor3B titleColor, ccColor3B highlightColor, bool debugReward);

public:
    static GauntletCompletionPopup* create(GauntletType type, ccColor3B titleColor, ccColor3B highlightColor);
    static GauntletCompletionPopup* createDebug(GauntletType type, ccColor3B titleColor, ccColor3B highlightColor);
    void forceCloseFromReward(CCObject* sender) override { this->onClose(sender); }
};