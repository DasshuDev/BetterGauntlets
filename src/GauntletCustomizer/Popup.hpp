#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <UIBuilder.hpp>

using namespace geode::prelude;

class GauntletCustomizer : public Popup<> {
protected:
    bool setup() override;

public:
    static GauntletCustomizer* create();
};