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

// Color Setting
CCMenu* GauntletCreator::createColorSetting(const std::string& title, const std::string& key) {

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
    
    CCSprite* colorPreview = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
        colorPreview->setScale(0.75);

    ccColor4B color = mod->getSettingValue<ccColor4B>(key);
        colorPreview->setColor(ccc3(color.r, color.g, color.b));
        colorPreview->setOpacity(255);
    
    CCMenuItemSpriteExtra* colorBtn = Build<CCMenuItemSpriteExtra>::create(
            colorPreview,
            this,
            menu_selector(GauntletCreator::onColorSetting) // placeholder
        )
        .id(fmt::format("{}-color-btn", key))
        .parent(settingLayout)
        .collect();
    
    Build<CCLabelBMFont>::create(title.c_str(), "bigFont.fnt")
        .id(fmt::format("{}-label"_spr, key))
        .scale(0.5)
        .alignment(kCCTextAlignmentLeft)
        .parent(settingLayout)
        .collect();
    
    m_colorKey = key;
    m_currentColor = mod->getSettingValue<ccColor4B>(key);
    m_colorPreview = colorPreview;
    
    settingLayout->setID(fmt::format("{}-setting"_spr, key));
    settingLayout->updateLayout();
    
    return settingLayout;
}

// Popup Setup
bool GauntletCreator::init() {
    // Initialize the popup with width=380, height=280
    if (!Popup::init(380.f, 280.f))
        return false;
    
    this->setTitle("Gauntlet Creator");

    auto winSize = m_mainLayer->getContentSize();

    auto background = Build<CCScale9Sprite>::create("square02b_001.png")
        .contentSize(winSize.width - 40, winSize.height - 60)
        .pos(winSize.width / 2, winSize.height / 2 - 10)
        .color(ccc3(0, 0, 0))
        .opacity(80)
        .parent(m_mainLayer)
        .collect();

    // Buttons Menu (Info, Confirm, etc.)
    CCMenu* buttonsMenu = Build<CCMenu>::create()
        .pos(CCPointZero)
        .scale(m_mainLayer->getScale())
        .parent(m_mainLayer)
        .collect();

    CCMenuItemSpriteExtra* infoBtn = Build<CCMenuItemSpriteExtra>::create(
            CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
            this,
            menu_selector(GauntletCreator::onCreatorInfoPressed)
        )
        .pos(winSize.width - 20, winSize.height - 20)
        .parent(buttonsMenu)
        .collect();

    // Settings Menu
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
    
    TextInput* gauntletName = Build<TextInput>::create(settingsMenu->getContentWidth(), "Gauntlet Type (ex. Fire)", "bigFont.fnt")
        .id("name-input"_spr)
        .parent(settingsMenu)
        .collect();
        gauntletName->setMaxCharCount(20);

    // Create Settings
    settingsMenu->addChild(createToggleSetting("Toggle Setting", "testBool"));
    settingsMenu->addChild(createColorSetting("Main Color", "testColor1"));
    settingsMenu->addChild(createColorSetting("Accent Color", "testColor2"));

    settingsMenu->updateLayout();

    return true;
}

// Popup Visuals
GauntletCreator* GauntletCreator::create() {
    auto popup = new GauntletCreator();
    if (popup->init()) {
        popup->autorelease();
        return popup;
    }
    delete popup;
    return nullptr;
}

void GauntletCreator::onCreatorInfoPressed(CCObject* sender) {
    utils::web::openLinkInBrowser("https://dasshu.dev/geode/better-gauntlets/GauntletCreator");
}

void GauntletCreator::onColorSetting(CCObject*) {
    ColorPickPopup* popup = ColorPickPopup::create(false);
    // popup->setDelegate(this);
    popup->show();
}

void GauntletCreator::colorPickPopupDidSelectColor(
    ColorPickPopup*,
    ccColor4B color
) {
    // save
    Mod::get()->setSettingValue<ccColor4B>(m_colorKey, color);

    // update preview immediately
    m_colorPreview->setColor({ color.r, color.g, color.b });
    m_colorPreview->setOpacity(color.a);
}