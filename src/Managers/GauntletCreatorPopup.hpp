#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/web.hpp>
#include "../APIs/GauntletManagerAPI.hpp"
#include "PagedPopup.hpp"

using namespace geode::prelude;

using EditSavedCallback = std::function<void(GauntletEditData const&)>;

struct LevelRewardEntry {
    int levelId = 0;
    std::string levelName;
    std::string creatorName;
    int reward = 0;
};

class GauntletCreatorPopup :
    public PagedPopup,
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
    CCMenu* m_infoPreview = nullptr;

    // Text inputs
    TextInput* m_nameInput = nullptr;
    TextInput* m_descInput = nullptr;
    TextInput* m_infoDateInput = nullptr;
    TextInput* m_infoVersionInput = nullptr;
    TextInput* m_infoAccIDInput = nullptr;
    TextInput* m_rewardCoinsInput = nullptr;

    // Fetch state
    bool m_searchingUser = false;
    bool m_searchingLevel = false;
    std::string m_pendingSearchKey;

    // Level search UI (in m_GLPreview)
    CCClippingNode* m_clippingNode = nullptr;
    CCSprite* m_GLBackground = nullptr;
    CCSprite* m_leftCornerAccent1 = nullptr;
    CCSprite* m_leftCornerAccent2 = nullptr;
    CCSprite* m_rightCornerAccent1 = nullptr;
    CCSprite* m_rightCornerAccent2 = nullptr;
    TextInput* m_levelSearchInput = nullptr;

    // Preview labels
    CCLabelBMFont* m_previewTitle = nullptr;
    CCLabelBMFont* m_previewTitleShadow = nullptr;
    CCLabelBMFont* m_gauntletText = nullptr;
    CCLabelBMFont* m_gauntletTextShadow = nullptr;
    NineSlice* m_previewBG = nullptr;

    // Info strings (set by push buttons)
    std::string m_infoDate;
    std::string m_infoVersion;
    std::string m_infoSuggester;
    int m_infoAccID = 0;

    // Level slots
    std::vector<LevelRewardEntry> m_levels;

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
    async::TaskHolder<web::WebResponse> m_uploadHolder;
    async::TaskHolder<web::WebResponse> m_webIconHolder;
    std::array<async::TaskHolder<web::WebResponse>, 5> m_slotBgHolders;


    // Methods
    void onClose(CCObject* sender) override;
    void FLAlert_Clicked(FLAlertLayer* alert, bool btn2) override;

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
    void onAddLevel(CCObject* sender);
    void refreshLevels();
    void loadLevelsFinished(CCArray* levels, char const* key) override;
    void loadLevelsFailed(char const* key, int p1) override;

    // Pages: 0 = basic info/colors (GSL preview), 1 = levels (GL preview), 2 = info fields
    int  getPageCount() const override { return 3; }
    void onPageChanged(int page) override;

    // Save
    void onSave(CCObject* sender);
    void doSave();

public:
    static GauntletCreatorPopup* create(
        GauntletEditData const& existing,
        EditSavedCallback onSaved
    );
};