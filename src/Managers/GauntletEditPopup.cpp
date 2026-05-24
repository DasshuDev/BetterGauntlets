#include "GauntletEditPopup.hpp"
#include "GauntletLevelSearchPopup.hpp"

using namespace geode::prelude;

GauntletEditPopup* GauntletEditPopup::create(
    GauntletEditData const& existing,
    EditSavedCallback onSaved
) {
    auto ret = new GauntletEditPopup();
    if (ret->init(existing, onSaved)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GauntletEditPopup::init(GauntletEditData const& existing, EditSavedCallback onSaved) {
    auto titleStr = existing.id == 0 ? "Create Gauntlet" : "Edit Gauntlet";

    if (!FLAlertLayer::init(
        nullptr,                  // delegate
        titleStr,                 // title
        "",                       // content (unused)
        "Close",                  // btn1
        nullptr,                  // btn2
        440.f,                    // width
        true,                     // scroll
        340.f,                    // height
        1.f                       // bgOpacity
    )) return false;

    m_data    = existing;
    m_onSaved = onSaved;

    // Use m_buttonMenu for our close button, m_mainLayer for content
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto bg      = m_mainLayer;

    float topY = winSize.height / 2 + 120.f;

    // Gauntlet Name
    auto nameLabel = CCLabelBMFont::create("Name", "goldFont.fnt");
    nameLabel->setScale(0.45f);
    nameLabel->setAnchorPoint({0, 0.5f});
    nameLabel->setPosition({winSize.width / 2 - 195, topY});
    bg->addChild(nameLabel);

    auto nameBg = CCScale9Sprite::create("square02_001.png");
    nameBg->setContentSize({240, 28});
    nameBg->setOpacity(100);
    nameBg->setPosition({winSize.width / 2 - 60, topY});
    bg->addChild(nameBg);

    m_nameInput = CCTextInputNode::create(220, 24, "Gauntlet name...", "bigFont.fnt");
    m_nameInput->setPosition({winSize.width / 2 - 60, topY});
    m_nameInput->setDelegate(this);
    if (!m_data.name.empty()) m_nameInput->setString(m_data.name.c_str());
    bg->addChild(m_nameInput);

    // ── Name color picker ──────────────────────────────────────────────────
    auto nameColorLabel = CCLabelBMFont::create("Name Color", "goldFont.fnt");
    nameColorLabel->setScale(0.4f);
    nameColorLabel->setAnchorPoint({0, 0.5f});
    nameColorLabel->setPosition({winSize.width / 2 + 90, topY + 8});
    bg->addChild(nameColorLabel);

    auto nameColorMenu = CCMenu::create();
    nameColorMenu->setPosition({winSize.width / 2 + 185, topY});
    bg->addChild(nameColorMenu);

    auto nameColorSwatch = CCScale9Sprite::create("square02_001.png");
    nameColorSwatch->setContentSize({28, 28});
    nameColorSwatch->setColor(m_data.nameColor);
    nameColorSwatch->setID("name-color-swatch");

    auto nameColorBtn = CCMenuItemExt::createSpriteExtra(
        nameColorSwatch,
        [this](CCMenuItemSpriteExtra*) {
            ColorPickPopup::create(m_data.nameColor)->show();
        }
    );
    nameColorMenu->addChild(nameColorBtn);

    // ── Background color picker ────────────────────────────────────────────
    float colorY = topY - 42.f;

    auto bgColorLabel = CCLabelBMFont::create("BG Color", "goldFont.fnt");
    bgColorLabel->setScale(0.4f);
    bgColorLabel->setAnchorPoint({0, 0.5f});
    bgColorLabel->setPosition({winSize.width / 2 - 195, colorY});
    bg->addChild(bgColorLabel);

    auto bgColorMenu = CCMenu::create();
    bgColorMenu->setPosition({winSize.width / 2 - 110, colorY});
    bg->addChild(bgColorMenu);

    auto bgColorSwatch = CCScale9Sprite::create("square02_001.png");
    bgColorSwatch->setContentSize({28, 28});
    bgColorSwatch->setColor(m_data.bgColor);
    bgColorSwatch->setID("bg-color-swatch");

    auto bgColorBtn = CCMenuItemExt::createSpriteExtra(
        bgColorSwatch,
        [this](CCMenuItemSpriteExtra*) {
            ColorPickPopup::create(m_data.bgColor)->show();
        }
    );
    bgColorMenu->addChild(bgColorBtn);

    // Icon
    float iconY = colorY - 46.f;

    auto iconLabel = CCLabelBMFont::create("Icon", "goldFont.fnt");
    iconLabel->setScale(0.45f);
    iconLabel->setAnchorPoint({0, 0.5f});
    iconLabel->setPosition({winSize.width / 2 - 195, iconY + 14});
    bg->addChild(iconLabel);

    auto urlBg = CCScale9Sprite::create("square02_001.png");
    urlBg->setContentSize({220, 28});
    urlBg->setOpacity(100);
    urlBg->setPosition({winSize.width / 2 - 70, iconY + 14});
    bg->addChild(urlBg);

    m_urlInput = CCTextInputNode::create(200, 24, "Icon URL (optional)...", "chatFont.fnt");
    m_urlInput->setPosition({winSize.width / 2 - 70, iconY + 14});
    m_urlInput->setDelegate(this);
    if (!m_data.iconURL.empty()) m_urlInput->setString(m_data.iconURL.c_str());
    bg->addChild(m_urlInput);

    auto iconMenu = CCMenu::create();
    iconMenu->setPosition({winSize.width / 2 + 170, iconY + 14});
    bg->addChild(iconMenu);

    auto fileSpr = ButtonSprite::create("Browse", "bigFont.fnt", "GJ_button_01.png");
    fileSpr->setScale(0.55f);
    auto fileBtn = CCMenuItemExt::createSpriteExtra(
        fileSpr,
        [this](CCMenuItemSpriteExtra*) { onPickIcon(nullptr); }
    );
    iconMenu->addChild(fileBtn);

    float descY = iconY - 50.f;

    // Description
    auto descLabel = CCLabelBMFont::create("Description", "goldFont.fnt");
    descLabel->setScale(0.45f);
    descLabel->setAnchorPoint({0, 0.5f});
    descLabel->setPosition({winSize.width / 2 - 195, descY + 14});
    bg->addChild(descLabel);

    auto descBg = CCScale9Sprite::create("square02_001.png");
    descBg->setContentSize({380, 44});
    descBg->setOpacity(100);
    descBg->setPosition({winSize.width / 2, descY - 8});
    bg->addChild(descBg);

    m_descInput = CCTextInputNode::create(360, 40, "Gauntlet description...", "chatFont.fnt");
    m_descInput->setPosition({winSize.width / 2, descY - 8});
    m_descInput->setDelegate(this);
    if (!m_data.description.empty())
        m_descInput->setString(m_data.description.c_str());
    bg->addChild(m_descInput);

    buildSlots(bg, winSize);

    // Save
    auto saveMenu = CCMenu::create();
    saveMenu->setPosition({winSize.width / 2 + 150, winSize.height / 2 - 140});
    bg->addChild(saveMenu);

    auto saveSpr = ButtonSprite::create(
        existing.id == 0 ? "Create" : "Save",
        "goldFont.fnt", "GJ_button_01.png"
    );
    auto saveBtn = CCMenuItemExt::createSpriteExtra(
        saveSpr,
        [this](CCMenuItemSpriteExtra*) { onSave(nullptr); }
    );
    saveMenu->addChild(saveBtn);

    // Loading circle
    m_loadingCircle = LoadingCircle::create();
    m_loadingCircle->setPosition({winSize.width / 2, winSize.height / 2});
    m_loadingCircle->setVisible(false);
    bg->addChild(m_loadingCircle, 10);

    return true;
}

void GauntletEditPopup::buildSlots(CCNode* bg, CCSize winSize) {
    float slotY   = winSize.height / 2 - 80.f;
    float spacing = 72.f;
    float totalW  = 4 * spacing;
    float startX  = (winSize.width - totalW) / 2.f;

    auto slotHeader = CCLabelBMFont::create("Levels (tap to search)", "goldFont.fnt");
    slotHeader->setScale(0.4f);
    slotHeader->setPosition({winSize.width / 2, slotY + 44});
    bg->addChild(slotHeader);

    auto slotMenu = CCMenu::create();
    slotMenu->setPosition({0, 0});
    bg->addChild(slotMenu);

    for (int i = 0; i < 5; i++) {
        float x = startX + i * spacing;

        auto slotBg = CCScale9Sprite::create("square02_001.png");
        slotBg->setContentSize({64, 64});
        slotBg->setOpacity(80);
        slotBg->setPosition({x, slotY});
        bg->addChild(slotBg);

        auto numLabel = CCLabelBMFont::create(
            fmt::format("{}", i + 1).c_str(), "goldFont.fnt"
        );
        numLabel->setScale(0.35f);
        numLabel->setPosition({x, slotY + 24});
        bg->addChild(numLabel);

        auto& slot    = m_data.slots[i];
        auto  text    = slot.id > 0 ? slot.name : "Empty";
        auto infoLabel = CCLabelBMFont::create(text.c_str(), "chatFont.fnt");
        infoLabel->setScale(0.3f);
        infoLabel->limitLabelWidth(60.f, 0.3f, 0.1f);
        infoLabel->setPosition({x, slotY - 4});
        infoLabel->setColor(slot.id > 0 ? ccColor3B{100, 255, 100} : ccColor3B{255, 255, 255});
        bg->addChild(infoLabel);
        m_slotLabels[i] = infoLabel;

        auto tapSpr = CCSprite::create();
        tapSpr->setContentSize({64, 64});

        int idx = i;
        auto tapBtn = CCMenuItemExt::createSpriteExtra(
            tapSpr,
            [this, idx](CCMenuItemSpriteExtra*) { onPickSlot(idx); }
        );
        tapBtn->setPosition({x, slotY});
        slotMenu->addChild(tapBtn);
        m_slotBtns[i] = tapBtn;
    }
}

void GauntletEditPopup::refreshSlotLabel(int index) {
    auto& slot  = m_data.slots[index];
    auto  label = m_slotLabels[index];
    if (!label) return;
    auto text = slot.id > 0 ? slot.name : "Empty";
    label->setString(text.c_str());
    label->limitLabelWidth(60.f, 0.3f, 0.1f);
    label->setColor(slot.id > 0 ? ccColor3B{100, 255, 100} : ccColor3B{255, 255, 255});
}

void GauntletEditPopup::onPickSlot(int slotIndex) {
    auto popup = GauntletLevelSearchPopup::create(
        slotIndex,
        [this, slotIndex](SlotLevel const& picked) {
            m_data.slots[slotIndex] = picked;
            refreshSlotLabel(slotIndex);
        }
    );
    popup->show();
}

void GauntletEditPopup::onPickIcon(CCObject*) {
    m_pickHolder.spawn(
        file::pick(
            file::PickMode::OpenFile,
            file::FilePickOptions{
                dirs::getGameDir(),
                {{ "Images", { "*.png", "*.jpg", "*.jpeg" } }}
            }
        ),
        [this](geode::Result<std::optional<std::filesystem::path>> result) {
            if (!result) return;                    // Result itself failed
            if (!result.unwrap().has_value()) return; // user cancelled

            auto path = result.unwrap().value();
            m_loadingCircle->setVisible(true);

            m_uploadHolder.spawn(
                GauntletManagerAPI::get()->uploadIcon(path),
                [this](web::WebResponse res) {
                    m_loadingCircle->setVisible(false);
                    if (!res.ok()) {
                        Notification::create(
                            "Icon upload failed.", NotificationIcon::Error
                        )->show();
                        return;
                    }
                    auto json = res.json().unwrapOr(matjson::Value());
                    auto url  = json["url"].asString().unwrapOr("");
                    if (url.empty()) return;
                    m_data.iconURL = url;
                    m_urlInput->setString(url.c_str());
                    Notification::create(
                        "Icon uploaded!", NotificationIcon::Success
                    )->show();
                }
            );
        }
    );
}

void GauntletEditPopup::onSave(CCObject*) {
    m_data.name        = m_nameInput->getString();
    m_data.description = m_descInput->getString();

    auto urlStr = std::string(m_urlInput->getString());
    if (!urlStr.empty()) m_data.iconURL = urlStr;

    if (m_data.name.empty()) {
        Notification::create("Name is required.", NotificationIcon::Warning)->show();
        return;
    }

    m_loadingCircle->setVisible(true);

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
        this->removeFromParent();
    });
}

void GauntletEditPopup::onClose(CCObject*) {
    this->removeFromParent();
}