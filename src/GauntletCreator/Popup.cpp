#include "Popup.hpp"
#include <Geode/ui/ColorPickPopup.hpp>

using namespace geode::prelude;

// Toggle Setting
CCMenu* GauntletCreator::createToggleSetting(const std::string& title, const std::string& key) {

    Mod* mod = Mod::get();
    auto winSize = m_mainLayer->getContentSize();

    CCMenu* settingLayout = Build<CCMenu>::create()
        .contentSize(0, 0)
        .pos(0, 0)
        .layout(RowLayout::create()
            ->setAutoScale(false)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAutoGrowAxis(0)
            ->setGap(7.5)
        )
        .anchorPoint(0, 0.5)
        .collect();
    
    CCMenuItemToggler* toggleBtn = Build<CCMenuItemToggler>::createToggle(CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
        [this, key, mod](CCMenuItemToggler* toggleBtn) {
            toggleBtn->toggle(mod->getSettingValue<bool>(key));
            mod->setSettingValue<bool>(key, !mod->getSettingValue<bool>(key));
        })
        .id(fmt::format("{}-toggle", key))
        .scale(0.75)
        .parent(settingLayout)
        .collect();
    
    Build<CCLabelBMFont>::create(title.c_str(), "bigFont.fnt")
		.id(fmt::format("{}-label"_spr, key))
		.scale(0.5)
        .alignment(kCCTextAlignmentLeft)
		.parent(settingLayout)
        .collect();
    
    settingLayout->setID(fmt::format("{}-setting"_spr, key));
    settingLayout->updateLayout();
    
    toggleBtn->toggle(mod->getSettingValue<bool>(key));
    return settingLayout;
}

// Popup Setup
bool GauntletCreator::setup() {
    this->setTitle("Popup of DOOM");

    auto winSize = m_mainLayer->getContentSize();

    auto background = Build<CCScale9Sprite>::create("square02b_001.png")
    .contentSize(winSize.width - 40, winSize.height - 60)
    .pos(winSize.width / 2, winSize.height / 2 - 10)
    .color(ccc3(0, 0, 0))
    .opacity(80)
    .parent(m_mainLayer)
    .collect();

    CCMenu* settingsContainer = Build<CCMenu>::create()
        .id("settings-container"_spr)
        .contentSize(background->getContentSize())
        .pos(background->getPosition())
        .layout(ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
            ->setGap(10.0f)
        )
        .parent(m_mainLayer)
        .collect();

    CCMenu* settingsMenu = Build<CCMenu>::create()
        .id("settings-menu"_spr)
        .contentSize(settingsContainer->getContentSize() - 20)
        .pos(settingsContainer->getPosition() - 20)
        .layout(ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
            ->setCrossAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisOverflow(false)
        )
        .parent(settingsContainer)
        .collect();

    // Setting Additions
    settingsMenu->addChild(createToggleSetting("Toggle Setting", "testBool"));

    settingsMenu->updateLayout();

    return true;

}

// Popup Visuals
GauntletCreator* GauntletCreator::create() {
    auto ret = new GauntletCreator();
    if (ret && ret->initAnchored(380.f, 280.f, "GJ_square05.png")) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
