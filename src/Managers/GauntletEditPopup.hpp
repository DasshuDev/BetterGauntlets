#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "GauntletManagerAPI.hpp"

using namespace geode::prelude;

using EditSavedCallback = std::function<void()>;

class GauntletEditPopup : public FLAlertLayer, public TextInputDelegate {
public:
    static GauntletEditPopup* create(
        GauntletEditData const& existing,
        EditSavedCallback onSaved
    );
    bool init(GauntletEditData const& existing, EditSavedCallback onSaved);

private:
    GauntletEditData  m_data;
    EditSavedCallback m_onSaved;

    CCTextInputNode* m_nameInput = nullptr;
    CCTextInputNode* m_descInput = nullptr;
    CCTextInputNode* m_urlInput  = nullptr;

    std::array<CCMenuItemSpriteExtra*, 5> m_slotBtns;
    std::array<CCLabelBMFont*, 5>         m_slotLabels;

    async::TaskHolder<geode::Result<std::optional<std::filesystem::path>>> m_pickHolder;
    async::TaskHolder<web::WebResponse> m_saveHolder;
    async::TaskHolder<web::WebResponse> m_uploadHolder;

    LoadingCircle* m_loadingCircle = nullptr;

    void buildSlots(CCNode* bg, CCSize winSize);
    void onPickSlot(int slotIndex);
    void onPickIcon(CCObject* sender);
    void onSave(CCObject* sender);
    void onClose(CCObject* sender);
    void refreshSlotLabel(int index);
};