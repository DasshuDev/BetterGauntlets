#include "GauntletEditPopup.hpp"
#include "GauntletLevelSearchPopup.hpp"

using namespace geode::prelude;

GauntletEditPopup* GauntletEditPopup::create(
    GauntletEditData const& existing,
    EditSavedCallback onSaved
) {
    auto ret = new GauntletEditPopup();
    auto title = existing.id == 0 ? "Create Gauntlet" : "Edit Gauntlet";
    if (ret && ret->init(400, 270, "GJ_square05.png", existing, onSaved)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

static std::string getBgIconSpriteName(int index) {
    int bgIndex = std::clamp(index, 1, 59);
    return fmt::format("bgIcon_{:02d}_001.png", bgIndex);
}

bool GauntletEditPopup::init(
    float width, float height, char const* bg,
    GauntletEditData const& existing, EditSavedCallback onSaved
) {
    if (!Popup::init(width, height, bg)) return false;

    m_data = existing;
    m_onSaved = onSaved;

    auto titleStr = existing.id == 0 ? "Create Gauntlet" : "Edit Gauntlet";
    auto titleLabel = CCLabelBMFont::create(titleStr, "goldFont.fnt");
    titleLabel->setPosition(m_size.width / 2, m_size.height - 20);
    titleLabel->setScale(0.7f);
    titleLabel->setID("title-label");
    m_mainLayer->addChild(titleLabel);

    auto mainLayerBG = NineSlice::create("square02_001.png");
    mainLayerBG->setContentSize({m_mainLayer->getContentWidth() - 30, m_mainLayer->getContentHeight() - 55});
    mainLayerBG->setPosition({m_mainLayer->getContentWidth() / 2, m_mainLayer->getContentHeight() / 2 - 11.5f});
    mainLayerBG->setOpacity(80);
    m_mainLayer->addChild(mainLayerBG);

    //////////

    // >> Name + color
    auto nameContainer = CCMenu::create();
    nameContainer->setLayout(RowLayout::create()
        ->setGap(10)
        ->setAxisAlignment(AxisAlignment::Start)
    );
    nameContainer->setPosition({75, m_mainLayer->getContentHeight() - 72});
    nameContainer->setContentSize({100, 50});
    nameContainer->setID("name-container");

    auto nameLabel = CCLabelBMFont::create("Type", "goldFont.fnt");
    nameLabel->setPosition(nameContainer->getPositionX(), nameContainer->getPositionY() + 20);
    nameLabel->setScale(0.52);

    m_nameInput = TextInput::create(100, "Type");
    m_colorSpr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");

    auto colorBtn = CCMenuItemSpriteExtra::create(
        m_colorSpr,
        this,
        menu_selector(GauntletEditPopup::onPickNameColor)
    );

    nameContainer->addChild(m_nameInput);
    nameContainer->addChild(colorBtn);
    nameContainer->updateLayout();

    m_mainLayer->addChild(nameLabel);
    m_mainLayer->addChild(nameContainer);

    //////////

    // >> Desc
    auto descContainer = CCMenu::create();
    descContainer->setLayout(RowLayout::create()
        ->setGap(10)
        ->setAxisAlignment(AxisAlignment::Start)
    );
    descContainer->setPosition({255, m_mainLayer->getContentHeight() - 72});
    descContainer->setContentSize({240, 50});
    descContainer->setID("description-container");

    auto descLabel = CCLabelBMFont::create("Description", "goldFont.fnt");
    descLabel->setPosition(descContainer->getPositionX(), descContainer->getPositionY() + 20);
    descLabel->setScale(0.52);

    auto descInput = TextInput::create(350, "Description", "chatFont.fnt");
    descInput->setTextAlign(TextInputAlign::Left);
    descInput->setCommonFilter(CommonFilter::Any);

    descContainer->addChild(descInput);
    descContainer->updateLayout();

    m_mainLayer->addChild(descLabel);
    m_mainLayer->addChild(descContainer);

    //////////

    // >> Icon
    auto spriteContainer = CCMenu::create();
    spriteContainer->setPosition({75, nameContainer->getPositionY() - 45});
    spriteContainer->setContentSize({100, 50});
    spriteContainer->setID("sprite-container");

    auto spriteLabel = CCLabelBMFont::create("Icon", "goldFont.fnt");
    spriteLabel->setPosition(spriteContainer->getPositionX(), spriteContainer->getPositionY() + 20);
    spriteLabel->setScale(0.52);

    auto addSpriteBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Upload Icon", 100.f, 80.f, 1.f, true, "goldFont.fnt", "GJ_button_05.png"),
        this,
        menu_selector(GauntletEditPopup::onPickIcon)
    );
    addSpriteBtn->setScale(0.75);
    addSpriteBtn->m_baseScale = 0.75;
    addSpriteBtn->m_scaleMultiplier = 1.15;

    spriteContainer->addChild(addSpriteBtn);

    m_mainLayer->addChild(spriteLabel);
    m_mainLayer->addChild(spriteContainer);

    //////////

    // >> BG + color
    auto bgContainer = CCMenu::create();
    bgContainer->setLayout(RowLayout::create()
        ->setGap(10)
        ->setAxisAlignment(AxisAlignment::Center)
    );
    bgContainer->setPosition({75, spriteContainer->getPositionY() - 60});
    bgContainer->setContentSize({100, 50});
    bgContainer->setID("bg-container");

    auto bgLabel = CCLabelBMFont::create("Background", "goldFont.fnt");
    bgLabel->setPosition(bgContainer->getPositionX(), bgContainer->getPositionY() + 35);
    bgLabel->setScale(0.52);

    auto bgIcon = CCSprite::createWithSpriteFrameName(getBgIconSpriteName(m_bgIndex).c_str());
    
    auto bgBtn = CCMenuItemSpriteExtra::create(
        bgIcon,
        this,
        menu_selector(GauntletEditPopup::onPickBackground)
    );
    bgBtn->m_scaleMultiplier = 1.15;

    m_bgColorSpr = NineSlice::create("GJ_squareB_01.png");
    m_bgColorSpr->setContentHeight(140);
    m_bgColorSpr->setScale(0.3);

    auto bgColorBtn = CCMenuItemSpriteExtra::create(
        m_bgColorSpr,
        this,
        menu_selector(GauntletEditPopup::onPickBGColor)
    );
    bgColorBtn->m_scaleMultiplier = 1.15;

    m_bgIndexButton = bgBtn;
    m_bgIconSpr = bgIcon;

    bgContainer->addChild(bgBtn);
    bgContainer->addChild(bgColorBtn); 
    bgContainer->updateLayout();
    
    m_mainLayer->addChild(bgLabel);
    m_mainLayer->addChild(bgContainer);

    //////////

    // >> Lvls
    // ---> Start code here <---

    //////////

    // >> Preview
    auto previewContainer = CCMenu::create();
    previewContainer->setPosition({m_mainLayer->getContentWidth() - 58, 90});
    previewContainer->setID("preview-container");

    auto previewBG = NineSlice::create("GJ_squareB_01.png");
    previewBG->setID("preview-background");
    previewBG->setContentSize({110, 220});
    previewBG->setColor({m_selectedBGColor.r, m_selectedBGColor.g, m_selectedBGColor.b});
    previewBG->setScale(0.6);

    auto previewLabel = CCLabelBMFont::create("Preview", "goldFont.fnt");
    previewLabel->setPosition(previewBG->getPositionX(), previewBG->getScaledContentHeight() / 2 + 15);
    previewLabel->setScale(0.52);

    m_previewTitle = CCLabelBMFont::create("", "bigFont.fnt");
    m_previewTitle->setString(m_nameInput->getString().c_str());
    m_previewTitle->updateLabel();
    m_previewTitle->setPosition(previewBG->getPositionX(), previewBG->getScaledContentHeight() / 2);

    previewContainer->addChild(previewLabel);
    previewContainer->addChild(previewBG);
    previewContainer->addChild(m_previewTitle);

    m_mainLayer->addChild(previewContainer);

    //////////

    return true;
}

void GauntletEditPopup::onPickIcon(CCObject*) {
    m_pickHolder.spawn(
        file::pick(
            file::PickMode::OpenFile,
            file::FilePickOptions{
                dirs::getGameDir(),
                {{ "PNG Images", { "*.png" } }}
            }
        ),
        [this](geode::Result<std::optional<std::filesystem::path>> result) {
            if (!result || !result.unwrap().has_value()) return;
            auto path = result.unwrap().value();

            m_pendingIconPath = path;

            queueInMainThread([this, path]() {
                // Show the selected filename as confirmation
                Notification::create(
                    fmt::format("Icon selected: {}", path.filename().string()),
                    NotificationIcon::Success
                )->show();

                // Load it as a local sprite for preview
                auto container = m_mainLayer->getChildByIDRecursive("preview-background");

                auto previewIcon = CCSprite::create(path.string().c_str());
                auto previewIconShadow = CCSprite::create(path.string().c_str());
                
                auto oldPreviewIcon = container->getChildByID("preview-icon");
                auto oldPreviewIconShadow = container->getChildByID("preview-icon-shadow");
                
                if (previewIcon && previewIconShadow) {
                    if (container) {
                        if (oldPreviewIcon && oldPreviewIconShadow) {
                            oldPreviewIcon->removeFromParent();
                            oldPreviewIconShadow->removeFromParent();
                        }
                        
                        previewIcon->setID("preview-icon");
                        previewIcon->setPosition(container->getContentSize() / 2);
                        previewIcon->setScale(1.1);

                        previewIconShadow->setID("preview-icon-shadow");
                        previewIconShadow->setPosition({container->getContentWidth() / 2, container->getContentHeight() / 2 - 10});
                        previewIconShadow->setColor({0, 0, 0});
                        previewIconShadow->setScaleX(previewIcon->getScaleX());
                        previewIconShadow->setScaleY(previewIcon->getScaleY() * 1.2);
                        previewIconShadow->setOpacity(50);

                        container->addChild(previewIconShadow);
                        container->addChild(previewIcon);
                    }
                }
            });
        }
    );
}
void GauntletEditPopup::onPickBackground(CCObject* sender) {
    auto layer = SelectArtLayer::create(SelectArtType::Background, m_bgIndex);
    if (!layer) {
        return;
    }

    layer->show();
}

void GauntletEditPopup::selectArtClosed(SelectArtLayer* layer) {
    if (!layer) {
        return;
    }

    m_bgIndex = layer->m_art;
    updateBgIcon();
}

void GauntletEditPopup::updateBgIcon() {
    if (!m_bgIndexButton) {
        return;
    }

    auto icon = CCSprite::createWithSpriteFrameName(getBgIconSpriteName(m_bgIndex).c_str());
    if (!icon) {
        return;
    }

    m_bgIconSpr = icon;
    m_bgIndexButton->setNormalImage(icon);
}

void GauntletEditPopup::onPickNameColor(CCObject* sender) {
    m_colorPopup = ColorPickPopup::create(m_selectedColor);
    if (m_colorPopup) {
        m_colorPopup->setCallback([this](const cocos2d::ccColor4B& color) {
            m_selectedColor.r = color.r;
            m_selectedColor.g = color.g;
            m_selectedColor.b = color.b;
            if (m_colorSpr) {
                m_colorSpr->setColor({color.r, color.g, color.b});
            }
        });
        m_colorPopup->show();
    }
}

void GauntletEditPopup::onPickBGColor(CCObject* sender) {
    m_colorPopup = ColorPickPopup::create(m_selectedBGColor);
    if (m_colorPopup) {
        m_colorPopup->setCallback([this](const cocos2d::ccColor4B& color) {
            m_selectedBGColor.r = color.r;
            m_selectedBGColor.g = color.g;
            m_selectedBGColor.b = color.b;
            if (m_bgColorSpr) {
                m_bgColorSpr->setColor({color.r, color.g, color.b});
            }
        });
        m_colorPopup->show();
    }
}

void GauntletEditPopup::onSave(CCObject*) {
    m_data.name = m_nameInput->getString();
    m_data.description = m_descInput->getString();

    if (m_data.name.empty()) {
        Notification::create("Name is required.", NotificationIcon::Warning)->show();
        return;
    }

    m_loadingCircle->setVisible(true);

    // If a local icon was selected, upload it first then save
    if (m_pendingIconPath.has_value()) {
        m_uploadHolder.spawn(
            GauntletManagerAPI::get()->uploadIcon(m_pendingIconPath.value()),
            [this](web::WebResponse res) {
                if (!res.ok()) {
                    m_loadingCircle->setVisible(false);
                    Notification::create("Icon upload failed.", NotificationIcon::Error)->show();
                    return;
                }
                auto url = res.json().unwrapOr(matjson::Value())["url"].asString().unwrapOr("");
                if (!url.empty()) m_data.iconURL = url;
                m_pendingIconPath.reset();
                doSave();
            }
        );
    } else {
        doSave();
    }
}

void GauntletEditPopup::doSave() {
    auto future = m_data.id == 0
        ? GauntletManagerAPI::get()->create(m_data)
        : GauntletManagerAPI::get()->update(m_data);

    m_saveHolder.spawn(std::move(future), [this](web::WebResponse res) {
        m_loadingCircle->setVisible(false);
        if (!res.ok()) {
            Notification::create(
                fmt::format("Save failed: HTTP {}", res.code()),
                NotificationIcon::Error
            )->show();
            return;
        }
        Notification::create(
            m_data.id == 0 ? "Gauntlet created!" : "Gauntlet updated!",
            NotificationIcon::Success
        )->show();
        if (m_onSaved) m_onSaved();
        this->onClose(nullptr);
    });
}