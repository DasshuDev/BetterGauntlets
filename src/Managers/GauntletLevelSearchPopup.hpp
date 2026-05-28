#pragma once
#include <Geode/Geode.hpp>
#include "GauntletManagerAPI.hpp"

using namespace geode::prelude;

using LevelPickCallback = std::function<void(SlotLevel const&)>;

class GauntletLevelSearchPopup : public Popup, public TextInputDelegate, public LevelManagerDelegate {
public:
    static GauntletLevelSearchPopup* create(int slotIndex, LevelPickCallback callback);
    bool init(float width, float height, char const* bg, int slotIndex, LevelPickCallback callback);

private:
    int               m_slotIndex;
    LevelPickCallback m_callback;
    CCTextInputNode*  m_searchInput   = nullptr;
    CCLayer*          m_resultsLayer  = nullptr;
    LoadingCircle*    m_loadingCircle = nullptr;

    void onSearch(CCObject* sender);
    void buildResultRow(GJGameLevel* level, float yPos);

    // LevelManagerDelegate overrides
    void loadLevelsFinished(CCArray* levels, char const* key) override;
    void loadLevelsFailed(char const* key, int p1) override;
};