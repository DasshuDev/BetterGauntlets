#include "Popup.hpp"

using namespace geode::prelude;

bool GauntletCustomizer::setup() {
    this->setTitle("Gauntlet Customizer");

    auto win = m_mainLayer->getContentSize();

    // Container layout using UIBuilder
    Build<CCNode>::create()
        .parent(m_mainLayer)
        .anchorPoint(0.5f, 0.5f)
        .pos(win / 2)
        .layout(AxisLayout::create()->setAxis(Axis::Column)->setGap(12.f))
        .child(
            Build<CCLabelBMFont>::create("Customize your Gauntlet options", "bigFont.fnt")
                .scale(0.6f)
        .collect()
        );

    return true;
}

GauntletCustomizer* GauntletCustomizer::create() {
    auto ret = new GauntletCustomizer();
    if (ret && ret->initAnchored(450.f, 250.f, "GJ_square05.png")) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}