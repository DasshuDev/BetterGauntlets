#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/web.hpp>
#include "GauntletManagerAPI.hpp"

using namespace geode::prelude;

using EditSavedCallback = std::function<void(GauntletEditData const&)>;

class GauntletEditPopup :
    public Popup,
    public TextInputDelegate,
    public SelectArtDelegate,
    public LevelManagerDelegate,
    public UserInfoDelegate,
    public FLAlertLayerProtocol
{
protected:
    bool init(float width, float height, char const* bg, GauntletEditData const& existing, EditSavedCallback onSaved);

    GauntletEditData  m_data;
    EditSavedCallback m_onSaved;

    // Layer swap
    CCMenu* m_GSLPreview = nullptr;
    CCMenu* m_GLPreview = nullptr;

    // Text inputs
    TextInput* m_nameInput = nullptr;
    TextInput* m_descInput = nullptr;
    TextInput* m_levelInput = nullptr;
    TextInput* m_infoDateInput = nullptr;
    TextInput* m_infoVersionInput = nullptr;
    TextInput* m_infoAccIDInput = nullptr;

    // Level fetch state
    bool m_searchingLevel = false;
    bool m_searchingByName = false;
    bool m_searchingUser = false;
    int m_pendingSlotIndex = -1;
    int m_selectedSlot = 0;
    std::string m_pendingSearchKey;

    // Level search UI (in m_GLPreview)
    TextInput* m_levelSearchInput = nullptr;
    CCLayer* m_levelResultsLayer = nullptr;
    std::array<CCMenuItemSpriteExtra*, 5> m_slotSelectBtns;
    std::array<CCLabelBMFont*, 5> m_slotStatusLabels;

    // Preview labels
    CCLabelBMFont* m_previewTitle = nullptr;
    CCLabelBMFont* m_previewTitleShadow = nullptr;
    CCLabelBMFont* m_gauntletText = nullptr;
    CCLabelBMFont* m_gauntletTextShadow = nullptr;
    TextArea* m_description = nullptr;
    NineSlice* m_previewBG = nullptr;

    // Info strings (set by push buttons)
    std::string m_infoDate;
    std::string m_infoVersion;
    std::string m_infoSuggester;
    int m_infoAccID = 0;

    // Level slots
    std::array<CCMenuItemSpriteExtra*, 5> m_slotBtns;
    std::array<CCLabelBMFont*, 5> m_slotLabels;

    // Background picker
    int m_bgIndex = 1;
    CCMenuItemSpriteExtra* m_bgIndexButton = nullptr;
    CCSprite* m_bgIconSpr = nullptr;

    // Color pickers
    ColorPickPopup* m_colorPopup = nullptr;
    CCSprite* m_colorSprName = nullptr;
    ccColor3B m_selectedColor = {255, 255, 255};
    CCSprite* m_colorSprBG = nullptr;
    ccColor3B m_selectedBGColor = {255, 255, 255};
    CCSprite* m_colorSprNode = nullptr;
    ccColor3B m_selectedNodeColor = {255, 255, 255};
    CCSprite* m_colorAccent1 = nullptr;
    ccColor3B m_selectedAccentColor1 = {255, 255, 255};
    CCSprite* m_colorAccent2 = nullptr;
    ccColor3B m_selectedAccentColor2 = {255, 255, 255};

    // Async holders
    async::TaskHolder<geode::Result<std::optional<std::filesystem::path>>> m_pickHolder;
    std::optional<std::filesystem::path> m_pendingIconPath;
    async::TaskHolder<web::WebResponse> m_saveHolder;
    async::TaskHolder<web::WebResponse> m_uploadHolder;
    async::TaskHolder<web::WebResponse> m_webIconHolder;

    std::string m_spritePath;
    LoadingCircle* m_loadingCircle = nullptr;

    // Methods
    void onClose(CCObject* sender) override;
    void FLAlert_Clicked(FLAlertLayer* alert, bool btn2) override;

    // Level slots
    void onPickSlot(int slotIndex);
    void onSelectSlot(int slotIndex);
    void onSearchLevel(CCObject* sender);
    void buildLevelResults(CCArray* levels);
    void refreshSlotLabel(int index);
    void refreshAllSlotStatus();
    void loadLevelsFinished(CCArray* levels, char const* key) override;
    void loadLevelsFailed(char const* key, int p1) override;

    // Icon
    void onPickIcon(CCObject* sender);

    // Colors
    void onPickNameColor(CCObject* sender);
    void onPickBGColor(CCObject* sender);
    void onPickNodeColor(CCObject* sender);
    void onPickAcc1Color(CCObject* sender);
    void onPickAcc2Color(CCObject* sender);

    // Background
    void onPickBackground(CCObject* sender);
    void selectArtClosed(SelectArtLayer* layer) override;
    void updateBgIcon();

    // Preview
    void updatePreviewName(CCObject* sender);
    void updateDescription(CCObject* sender);
    void onPreviewInfo(CCObject* sender);

    // Info fields
    void updateInfoDate(CCObject* sender);
    void updateInfoVersion(CCObject* sender);
    void updateInfoAccID(CCObject* sender);
    void getUserInfoFinished(GJUserScore* score) override;
    void getUserInfoFailed(int p1) override;

    // Layer swap
    void onSwapLayer(CCObject* sender);

    // Save
    void onSave(CCObject* sender);
    void doSave();

public:
    static GauntletEditPopup* create(
        GauntletEditData const& existing,
        EditSavedCallback onSaved
    );
};