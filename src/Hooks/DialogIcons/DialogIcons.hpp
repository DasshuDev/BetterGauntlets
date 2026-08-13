#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace DialogIcon {
    constexpr int DialogIconTagBase = 10100;
    constexpr int DialogIconCount = 999;

    void setDialogObjectIcon(DialogLayer* dialog, int characterFrame);
    void setDialogObjectCustomIcon(DialogLayer* dialog, const std::string& frameName);
    void setDialogSequenceCustomIcons(const std::vector<DialogObject*>& objects, const std::vector<std::string>& frameNames);
}