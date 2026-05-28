#include "GauntletLevelSearchPopup.hpp"

using namespace geode::prelude;

GauntletLevelSearchPopup* GauntletLevelSearchPopup::create(
    int slotIndex, LevelPickCallback callback
) {
    auto ret = new GauntletLevelSearchPopup();
    if (ret && ret->init(400.f, 300.f, "GJ_square05.png", slotIndex, callback)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GauntletLevelSearchPopup::init(float width, float height, char const* bg, int slotIndex, LevelPickCallback callback) {
    if (!Popup::init(width, height, bg)) return false;

    m_slotIndex = slotIndex;
    m_callback  = callback;

    // Title
    auto title = CCLabelBMFont::create(
        fmt::format("Pick Level for Slot {}", slotIndex + 1).c_str(), "goldFont.fnt"
    );
    title->setPosition(m_size.width / 2, m_size.height - 20);
    title->setScale(0.6f);
    m_mainLayer->addChild(title);

    // Search row
    auto inputBg = CCScale9Sprite::create("square02_001.png");
    inputBg->setContentSize({270, 28});
    inputBg->setOpacity(100);
    inputBg->setPosition({m_size.width / 2 - 50, m_size.height - 50});
    m_mainLayer->addChild(inputBg);

    m_searchInput = CCTextInputNode::create(250, 24, "Level name...", "bigFont.fnt");
    m_searchInput->setPosition({m_size.width / 2 - 50, m_size.height - 50});
    m_searchInput->setDelegate(this);
    m_mainLayer->addChild(m_searchInput);

    auto searchMenu = CCMenu::create();
    searchMenu->setPosition({m_size.width - 35, m_size.height - 50});
    m_mainLayer->addChild(searchMenu);

    auto searchSpr = ButtonSprite::create("Go", "bigFont.fnt", "GJ_button_01.png");
    searchSpr->setScale(0.6f);
    auto searchBtn = CCMenuItemExt::createSpriteExtra(
        searchSpr, [this](CCMenuItemSpriteExtra*) { onSearch(nullptr); }
    );
    searchMenu->addChild(searchBtn);

    // Results area
    auto resultsBg = CCScale9Sprite::create("square02b_001.png");
    resultsBg->setContentSize({m_size.width - 20, m_size.height - 90});
    resultsBg->setColor({0, 0, 0});
    resultsBg->setOpacity(80);
    resultsBg->setPosition({m_size.width / 2, (m_size.height - 90) / 2 + 5});
    m_mainLayer->addChild(resultsBg);

    m_resultsLayer = CCLayer::create();
    m_resultsLayer->setPosition({0, 0});
    m_mainLayer->addChild(m_resultsLayer);

    // Loading circle
    m_loadingCircle = LoadingCircle::create();
    m_loadingCircle->setPosition(m_size / 2);
    m_loadingCircle->setVisible(false);
    m_mainLayer->addChild(m_loadingCircle, 10);

    GameLevelManager::sharedState()->m_levelManagerDelegate = this;

    return true;
}

void GauntletLevelSearchPopup::onSearch(CCObject*) {
    auto query = std::string(m_searchInput->getString());
    if (query.empty()) return;
    m_resultsLayer->removeAllChildren();
    m_loadingCircle->setVisible(true);
    GameLevelManager::sharedState()->getOnlineLevels(
        GJSearchObject::create(SearchType::Search, query)
    );
}

void GauntletLevelSearchPopup::loadLevelsFinished(CCArray* levels, char const*) {
    m_loadingCircle->setVisible(false);
    m_resultsLayer->removeAllChildren();

    float yPos = m_size.height - 80.f;
    int   count = 0;

    for (auto* level : CCArrayExt<GJGameLevel*>(levels)) {
        if (count >= 5) break;
        buildResultRow(level, yPos);
        yPos -= 42.f;
        count++;
    }

    if (count == 0) {
        auto label = CCLabelBMFont::create("No results found.", "bigFont.fnt");
        label->setScale(0.45f);
        label->setColor({180, 180, 180});
        label->setPosition({m_size.width / 2, m_size.height / 2 - 10});
        m_resultsLayer->addChild(label);
    }
}

void GauntletLevelSearchPopup::loadLevelsFailed(char const*, int) {
    m_loadingCircle->setVisible(false);
    auto label = CCLabelBMFont::create("Search failed.", "bigFont.fnt");
    label->setScale(0.45f);
    label->setColor({255, 80, 80});
    label->setPosition({m_size.width / 2, m_size.height / 2 - 10});
    m_resultsLayer->addChild(label);
}

void GauntletLevelSearchPopup::buildResultRow(GJGameLevel* level, float yPos) {
    auto rowBg = CCScale9Sprite::create("square02_001.png");
    rowBg->setContentSize({m_size.width - 30, 38});
    rowBg->setOpacity(60);
    rowBg->setPosition({m_size.width / 2, yPos});
    m_resultsLayer->addChild(rowBg);

    auto nameLabel = CCLabelBMFont::create(level->m_levelName.c_str(), "bigFont.fnt");
    nameLabel->setScale(0.36f);
    nameLabel->setAnchorPoint({0, 0.5f});
    nameLabel->limitLabelWidth(180.f, 0.36f, 0.1f);
    nameLabel->setPosition({14, yPos + 8});
    m_resultsLayer->addChild(nameLabel);

    auto creatorLabel = CCLabelBMFont::create(
        fmt::format("by {}", level->m_creatorName).c_str(), "goldFont.fnt"
    );
    creatorLabel->setScale(0.26f);
    creatorLabel->setAnchorPoint({0, 0.5f});
    creatorLabel->setPosition({14, yPos - 8});
    m_resultsLayer->addChild(creatorLabel);

    auto starsLabel = CCLabelBMFont::create(
        fmt::format("{} *", level->m_stars).c_str(), "bigFont.fnt"
    );
    starsLabel->setScale(0.3f);
    starsLabel->setAnchorPoint({1, 0.5f});
    starsLabel->setPosition({m_size.width - 75, yPos});
    m_resultsLayer->addChild(starsLabel);

    auto menu = CCMenu::create();
    menu->setPosition({m_size.width - 35, yPos});
    m_resultsLayer->addChild(menu);

    auto selectSpr = ButtonSprite::create("Pick", "bigFont.fnt", "GJ_button_02.png");
    selectSpr->setScale(0.45f);

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
            this->onClose(nullptr);
        }
    );
    menu->addChild(selectBtn);
}