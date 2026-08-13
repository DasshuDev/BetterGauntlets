#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class GauntletCompletionPopup : public Popup {
protected:
    bool init(GauntletType type, ccColor3B titleColor, ccColor3B highlightColor);

public:
    static GauntletCompletionPopup* create(GauntletType type, ccColor3B titleColor, ccColor3B highlightColor);
};
