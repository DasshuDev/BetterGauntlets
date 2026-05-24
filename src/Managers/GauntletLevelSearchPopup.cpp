#include "GauntletLevelSearchPopup.hpp"

using namespace geode::prelude;

GauntletLevelSearchPopup* GauntletLevelSearchPopup::create(
    int slotIndex,
    LevelPickCallback callback
) {
    auto ret = new GauntletLevelSearchPopup();
    if (ret->init(slotIndex, callback)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GauntletLevelSearchPopup::init(int slotIndex, LevelPickCallback callback) {
    if (!FLAlertLayer::init(150)) return false;

    m_slotIndex = slotIndex;
    m_callback  = callback;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    // Dark background
    auto bg = CCLayerColor::create({0, 0, 0, 150});
    m_mainLayer->addChild(bg, -1);

    // Popup background
    auto popupBg = CCScale9Sprite::create("GJ_square01.png");
    popupBg->setContentSize({380, 320});
    popupBg->setPosition(winSize / 2);
    m_mainLayer->addChild(popupBg);

    // Title
    auto title = CCLabelBMFont::create("Search Levels", "goldFont.fnt");
    title->setScale(0.7f);
    title->setPosition({winSize.width / 2, winSize.height / 2 + 130});
    m_mainLayer->addChild(title);

    // Search input background
    auto inputBg = CCScale9Sprite::create("square02_001.png");
    inputBg->setContentSize({260, 32});
    inputBg->setOpacity(100);
    inputBg->setPosition({winSize.width / 2 - 50, winSize.height / 2 + 95});
    m_mainLayer->addChild(inputBg);

    m_searchInput = CCTextInputNode::create(240, 28, "Level name...", "bigFont.fnt");
    m_searchInput->setPosition({winSize.width / 2 - 50, winSize.height / 2 + 95});
    m_searchInput->setDelegate(this);
    m_mainLayer->addChild(m_searchInput);

    // Search button
    auto searchMenu = CCMenu::create();
    searchMenu->setPosition({winSize.width / 2 + 155, winSize.height / 2 + 95});
    m_mainLayer->addChild(searchMenu);

    auto searchSpr = ButtonSprite::create("Go", "bigFont.fnt", "GJ_button_01.png");
    searchSpr->setScale(0.65f);
    auto searchBtn = CCMenuItemExt::createSpriteExtra(
        searchSpr, [this](CCMenuItemSpriteExtra*) { onSearch(nullptr); }
    );
    searchMenu->addChild(searchBtn);

    // Results area
    m_resultsLayer = CCLayer::create();
    m_resultsLayer->setPosition({0, 0});
    m_mainLayer->addChild(m_resultsLayer);

    // Loading circle
    m_loadingCircle = LoadingCircle::create();
    m_loadingCircle->setPosition(winSize / 2);
    m_loadingCircle->setVisible(false);
    m_mainLayer->addChild(m_loadingCircle, 10);

    // Close button
    auto closeMenu = CCMenu::create();
    closeMenu->setPosition({winSize.width / 2 - 180, winSize.height / 2 + 145});
    m_mainLayer->addChild(closeMenu);

    auto closeBtn = CCMenuItemExt::createSpriteExtra(
        CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"),
        [this](CCMenuItemSpriteExtra*) { onClose(nullptr); }
    );
    closeMenu->addChild(closeBtn);

    // Register with touch dispatcher
    this->registerWithTouchDispatcher();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(
        this, -500, true
    );

    // Set level manager delegate
    GameLevelManager::sharedState()->m_levelManagerDelegate = this;

    m_mainLayer->setZOrder(100);

    return true;
}

void GauntletLevelSearchPopup::onSearch(CCObject*) {
    auto query = std::string(m_searchInput->getString());
    if (query.empty()) return;

    m_resultsLayer->removeAllChildren();
    m_loadingCircle->setVisible(true);

    auto searchObj = GJSearchObject::create(SearchType::Search, query);
    GameLevelManager::sharedState()->getOnlineLevels(searchObj);
}

void GauntletLevelSearchPopup::loadLevelsFinished(CCArray* levels, char const*) {
    m_loadingCircle->setVisible(false);
    m_resultsLayer->removeAllChildren();

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    float yPos   = winSize.height / 2 + 60.f;
    int   count  = 0;

    for (auto* level : CCArrayExt<GJGameLevel*>(levels)) {
        if (count >= 5) break;
        buildResultRow(level, yPos);
        yPos -= 44.f;
        count++;
    }

    if (count == 0) {
        auto label = CCLabelBMFont::create("No results found.", "bigFont.fnt");
        label->setScale(0.5f);
        label->setPosition({winSize.width / 2, winSize.height / 2});
        m_resultsLayer->addChild(label);
    }
}

void GauntletLevelSearchPopup::loadLevelsFailed(char const*, int) {
    m_loadingCircle->setVisible(false);
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto label   = CCLabelBMFont::create("Search failed.", "bigFont.fnt");
    label->setScale(0.5f);
    label->setColor({255, 80, 80});
    label->setPosition({winSize.width / 2, winSize.height / 2});
    m_resultsLayer->addChild(label);
}

void GauntletLevelSearchPopup::buildResultRow(GJGameLevel* level, float yPos) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto rowBg = CCScale9Sprite::create("square02_001.png");
    rowBg->setContentSize({340, 40});
    rowBg->setOpacity(80);
    rowBg->setPosition({winSize.width / 2, yPos});
    m_resultsLayer->addChild(rowBg);

    auto nameLabel = CCLabelBMFont::create(level->m_levelName.c_str(), "bigFont.fnt");
    nameLabel->setScale(0.38f);
    nameLabel->setAnchorPoint({0, 0.5f});
    nameLabel->limitLabelWidth(170.f, 0.38f, 0.1f);
    nameLabel->setPosition({winSize.width / 2 - 160, yPos + 8});
    m_resultsLayer->addChild(nameLabel);

    auto creatorLabel = CCLabelBMFont::create(
        fmt::format("by {}", level->m_creatorName).c_str(), "goldFont.fnt"
    );
    creatorLabel->setScale(0.28f);
    creatorLabel->setAnchorPoint({0, 0.5f});
    creatorLabel->setPosition({winSize.width / 2 - 160, yPos - 8});
    m_resultsLayer->addChild(creatorLabel);

    auto starsLabel = CCLabelBMFont::create(
        fmt::format("{} *", level->m_stars).c_str(), "bigFont.fnt"
    );
    starsLabel->setScale(0.32f);
    starsLabel->setAnchorPoint({1, 0.5f});
    starsLabel->setPosition({winSize.width / 2 + 70, yPos});
    m_resultsLayer->addChild(starsLabel);

    auto menu = CCMenu::create();
    menu->setPosition({winSize.width / 2 + 130, yPos});
    m_resultsLayer->addChild(menu);

    auto selectSpr = ButtonSprite::create("Pick", "bigFont.fnt", "GJ_button_02.png");
    selectSpr->setScale(0.5f);

    int  lvlID      = level->m_levelID;
    auto lvlName    = level->m_levelName;
    auto lvlCreator = level->m_creatorName;
    int  lvlStars   = level->m_stars;

    auto selectBtn = CCMenuItemExt::createSpriteExtra(
        selectSpr,
        [this, lvlID, lvlName, lvlCreator, lvlStars](CCMenuItemSpriteExtra*) {
            SlotLevel picked;
            picked.id      = lvlID;
            picked.name    = lvlName;
            picked.creator = lvlCreator;
            picked.stars   = lvlStars;
            m_callback(picked);
            onClose(nullptr);
        }
    );
    menu->addChild(selectBtn);
}

void GauntletLevelSearchPopup::onClose(CCObject*) {
    this->removeFromParent();
}