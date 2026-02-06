#include "LevelInfoPopup.hpp"
#include <Geode/ui/ProgressBar.hpp>

using namespace geode::prelude;

bool LevelInfoPopup::init(GJGameLevel* level) {
    // Initialize the popup with width=350, height=250
    if (!Popup::init(350.f, 250.f))
        return false;
    
    m_level = level;
    
    auto winSize = this->m_mainLayer->getContentSize();

    // ===== Name/Author =====
    
    // Level Name Label
    CCLabelBMFont* levelName = CCLabelBMFont::create(
        level->m_levelName.c_str(),
        "bigFont.fnt"
    );
    levelName->setPosition(winSize / 2 + ccp(0, 105));
    levelName->setScale(0.75);
    this->m_mainLayer->addChild(levelName);

    // Author label
    auto authorLabel = CCLabelBMFont::create(
        fmt::format("By {}", level->m_creatorName.c_str()).c_str(),
        "goldFont.fnt"
    );
    authorLabel->setPosition(winSize / 2 + ccp(0, 85));
    authorLabel->setScale(0.75);
    this->m_mainLayer->addChild(authorLabel);
    
    // ===== Percentage =====

    // Get level stats
    int levelID = level->m_levelID.value();
    GJGameLevel* savedLevel = GameLevelManager::sharedState()->getSavedGauntletLevel(levelID);
    
    int normal = 0;
    int practice = 0;

    if (savedLevel->m_normalPercent) normal = savedLevel->m_normalPercent.value();
    if (savedLevel->m_practicePercent) practice = savedLevel->m_practicePercent;
    
    // Normal Percentage Label
    ProgressBar* normalProgress = ProgressBar::create();
    CCLabelBMFont* normalPercentLabel = CCLabelBMFont::create(
        "Normal Mode",
        "bigFont.fnt"
    );

    normalPercentLabel->setPosition(winSize / 2 + ccp(0, -30));
    normalPercentLabel->setScale(0.4);

    normalProgress->setStyle(geode::ProgressBarStyle::Solid);
    normalProgress->updateProgress(normal);
    normalProgress->setFillColor(ccc3(0, 255, 0));
    normalProgress->showProgressLabel(true);
    normalProgress->setAnchorPoint(ccp(0.5, 0.5));
    normalProgress->setScale(0.65);
    normalProgress->setPosition(winSize / 2 + ccp(0, -45));

    this->m_mainLayer->addChild(normalPercentLabel);
    this->m_mainLayer->addChild(normalProgress);

    // Practice Percentage Label
    ProgressBar* practiceProgress = ProgressBar::create();
    CCLabelBMFont* practicePercentLabel = CCLabelBMFont::create(
        "Practice Mode",
        "bigFont.fnt"
    );

    practicePercentLabel->setPosition(winSize / 2 + ccp(0, -65));
    practicePercentLabel->setScale(0.4);
    
    practiceProgress->setStyle(geode::ProgressBarStyle::Solid);
    practiceProgress->updateProgress(practice);
    practiceProgress->setFillColor(ccc3(0, 255, 255));
    practiceProgress->showProgressLabel(true);
    practiceProgress->setAnchorPoint(ccp(0.5, 0.5));
    practiceProgress->setScale(0.65);
    practiceProgress->setPosition(winSize / 2 + ccp(0, -80));

    this->m_mainLayer->addChild(practicePercentLabel);
    this->m_mainLayer->addChild(practiceProgress);

    // ===== Play =====
    
    // Play button
    CCSprite* playBtnSpr = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
    CCMenuItemSpriteExtra* playBtn = CCMenuItemSpriteExtra::create(
        playBtnSpr,
        this,
        menu_selector(LevelInfoPopup::onPlay)
    );
    playBtn->setPosition(winSize / 2 + ccp(0, 25));
    
    auto menu = CCMenu::create();
    menu->addChild(playBtn);
    menu->setPosition(0, 0);
    this->m_mainLayer->addChild(menu);
    
    return true;
}

LevelInfoPopup* LevelInfoPopup::create(GJGameLevel* level) {
    auto popup = new LevelInfoPopup();
    if (popup->init(level)) {
        popup->autorelease();
        return popup;
    }
    delete popup;
    return nullptr;
}

void LevelInfoPopup::onPlay(CCObject* sender) {
    GameLevelManager::sharedState()->downloadLevel(m_level->m_levelID.value(), true, 0);
    this->onClose(sender);
}