#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/web.hpp>
#include "GauntletManagerAPI.hpp"

using namespace geode::prelude;

using EditSavedCallback = std::function<void()>;

class GauntletEditPopup : public Popup, public TextInputDelegate {
protected:
    bool init(float width, float height, char const* bg,
              GauntletEditData const& existing, EditSavedCallback onSaved);

    GauntletEditData  m_data;
    EditSavedCallback m_onSaved;

    TextInput* m_nameInput = nullptr;
    TextInput* m_descInput = nullptr;
    TextInput* m_urlInput = nullptr;
    CCLabelBMFont* m_previewTitle = nullptr;

    std::array<CCMenuItemSpriteExtra*, 5> m_slotBtns;
    std::array<CCLabelBMFont*, 5> m_slotLabels;

    int m_bgIndex;
    CCMenuItemSpriteExtra* m_bgIndexButton = nullptr;
    CCSprite* m_bgIconSpr = nullptr;

    ColorPickPopup* m_colorPopup = nullptr;
    ccColor3B m_selectedColor = {255, 255, 255};
    ccColor3B m_selectedBGColor = {255, 255, 255};
    CCSprite* m_colorSpr = nullptr;
    NineSlice* m_bgColorSpr = nullptr;

    async::TaskHolder<geode::Result<std::optional<std::filesystem::path>>> m_pickHolder;
    std::optional<std::filesystem::path> m_pendingIconPath;
    async::TaskHolder<web::WebResponse> m_saveHolder;
    async::TaskHolder<web::WebResponse> m_uploadHolder;

    std::string m_spritePath;

    LoadingCircle* m_loadingCircle = nullptr;

    void buildSlots();
    void refreshSlotLabel(int index);
    void onPickSlot(int slotIndex);
    void onPickIcon(CCObject* sender);
    void onPickNameColor(CCObject* sender);
    void onPickBGColor(CCObject* sender);
    void onPickBackground(CCObject* sender);
    void selectArtClosed(SelectArtLayer* layer);
    void updateBgIcon();
    void onSave(CCObject* sender);
    void doSave();

public:
    static GauntletEditPopup* create(
        GauntletEditData const& existing,
        EditSavedCallback onSaved
    );
};