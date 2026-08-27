#include "GauntletLayer.hpp"
#include "../GauntletInfo/GauntletInfo.hpp"
#include "../GauntletSelectLayer/GauntletSelectLayer.hpp"
#include "../Hooks/Custom Hooks/GauntletCompletionPopup.hpp"
#include "HueLuminanceTo.hpp"
#include <Geode/Geode.hpp>
#include <Geode/binding/CCSpriteWithHue.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/Layout.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include <Geode/utils/cocos.hpp>
#include <cocos2d.h>
#include <cstdlib>
#include <ctime>
#include <unordered_map>

using namespace geode::prelude;

static std::unordered_map<int, std::vector<bool>> m_lastKnownLockedStates;
static std::unordered_map<int, bool> m_lastKnownGauntletCompletion;

// Helpers

inline void limitLabelWidth(CCLabelBMFont *label, float maxWidth,
                            float originalScale = 1.0, float minScale = 0.5) {
  if (!label)
    return;
  float contentWidth = label->getContentSize().width * originalScale;
  if (contentWidth > maxWidth) {
    float newScale = maxWidth / label->getContentSize().width;
    newScale = std::max(newScale, minScale);
    label->setScale(newScale);
  } else {
    label->setScale(originalScale);
  }
}

// create / scene

CCScene *BetterGauntletLayer::scene(GauntletType type) {
  auto layer = BetterGauntletLayer::create(type);
  if (!layer)
    return nullptr;
  auto sc = CCScene::create();
  sc->addChild(layer);
  return sc;
}

BetterGauntletLayer *BetterGauntletLayer::create(GauntletType type) {
  auto ret = new BetterGauntletLayer();
  if (ret && ret->init(type)) {
    ret->autorelease();
    return ret;
  }
  delete ret;
  return nullptr;
}

// destructor

BetterGauntletLayer::~BetterGauntletLayer() {
  auto glm = GameLevelManager::get();
  if (glm->m_levelManagerDelegate == this)
    glm->m_levelManagerDelegate = nullptr;
  if (m_levels) {
    m_levels->release();
    m_levels = nullptr;
  }
}

// init

bool BetterGauntletLayer::init(GauntletType type) {
  if (!CCLayer::init())
    return false;

  m_gauntletType = type;
  setKeypadEnabled(true);
  setKeyboardEnabled(true);

  log::debug("BetterGauntletLayer opened for Gauntlet #{}",
             static_cast<int>(m_gauntletType));

  CCDirector *director = CCDirector::sharedDirector();
  CCSize winSize = director->getWinSize();

  // Preload the gauntlet sprite sheet (vanilla loads this in its own init)
  CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(
      "GauntletSheet.plist");

  // Background
  m_backgroundSprite = CCSprite::create("game_bg_01_001.png");
  if (m_backgroundSprite) {
    m_backgroundSprite->setID("background");
    m_backgroundSprite->setColor(ccc3(34, 34, 34));
    m_backgroundSprite->setZOrder(-99);
    m_backgroundSprite->setPosition({0, 0});
    m_backgroundSprite->setAnchorPoint({0, 0});
    float scaleX = winSize.width / m_backgroundSprite->getContentWidth();
    float scaleY = winSize.height / m_backgroundSprite->getContentHeight();
    m_backgroundSprite->setScale(std::max(scaleX, scaleY));
    this->addChild(m_backgroundSprite);
  }

  // Title
  std::string gauntletName = GauntletNode::nameForType(m_gauntletType);
  std::string titleStr = "The " + gauntletName + " Gauntlet";

  auto shadowText =
      CCLabelBMFont::create(titleStr.c_str(), "GR_OxygeneFontOutline.fnt"_spr);
  shadowText->setID("title");
  shadowText->setColor(ccc3(0, 0, 0));
  shadowText->setScale(0.750);
  shadowText->setOpacity(100);
  shadowText->setPosition(
      ccp(winSize.width / 2 + 1, director->getScreenTop() - 32.5));
  this->addChild(shadowText, 0);

  auto titleText =
      CCLabelBMFont::create(titleStr.c_str(), "GR_OxygeneFont.fnt"_spr);
  titleText->setID("title-shadow");
  titleText->setColor(ccc3(255, 255, 255));
  titleText->setScale(0.750);
  titleText->setPosition(ccp(winSize.width / 2, director->getScreenTop() - 30));
  this->addChild(titleText, 1);

  // Floor line
  CCSprite *floorLine =
      CCSprite::createWithSpriteFrameName("floorLine_001.png");
  if (floorLine) {
    floorLine->setID("floor-line");
    floorLine->setPosition({winSize.width / 2, titleText->getPositionY() - 25});
    floorLine->setScaleX(.85);
    floorLine->setOpacity(100);
    this->addChild(floorLine);
  }

  // Exit menu
  auto backMenu = CCMenu::create();
  backMenu->setID("exit-menu");
  backMenu->setPosition(24, director->getScreenTop() - 23);
  backMenu->setContentWidth(32.5);
  backMenu->setContentHeight(125);
  this->addChild(backMenu, 5);

  auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
  auto backBtn = CCMenuItemSpriteExtra::create(
      backSpr, this, menu_selector(BetterGauntletLayer::onBack));
  backMenu->addChild(backBtn);

  // Loading spinner
  auto loadCircle = CCSprite::create("loadingCircle.png");
  loadCircle->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
  loadCircle->runAction(
      CCRepeatForever::create(CCRotateBy::create(1.0f, 360.0f)));
  loadCircle->setID("loading-circle");
  loadCircle->setPosition(winSize / 2);
  this->addChild(loadCircle, 10);

  // Apply per-gauntlet theme (background, corners, particles)
  switch (m_gauntletType) {
  default:                          editGauntletFallback();     break;
  case GauntletType::Fire:          editFireGauntlet();         break;
  case GauntletType::Ice:           editIceGauntlet();          break;
  case GauntletType::Poison:        editPoisonGauntlet();       break;
  case GauntletType::Shadow:        editShadowGauntlet();       break;
  case GauntletType::Lava:          editLavaGauntlet();         break;
  case GauntletType::Bonus:         editBonusGauntlet();        break;
  case GauntletType::Chaos:         editChaosGauntlet();        break;
  case GauntletType::Demon:         editDemonGauntlet();        break;
  case GauntletType::Time:          editTimeGauntlet();         break;
  case GauntletType::Crystal:       editCrystalGauntlet();      break;
  case GauntletType::Magic:         editMagicGauntlet();        break;
  case GauntletType::Spike:         editSpikeGauntlet();        break;
  case GauntletType::Monster:       editMonsterGauntlet();      break;
  case GauntletType::Doom:          editDoomGauntlet();         break;
  case GauntletType::Death:         editDeathGauntlet();        break;
  case GauntletType::Forest:        editForestGauntlet();       break;
  case GauntletType::Rune:          editRuneGauntlet();         break;
  case GauntletType::Force:         editForceGauntlet();        break;
  case GauntletType::Spooky:        editSpookyGauntlet();       break;
  case GauntletType::Dragon:        editDragonGauntlet();       break;
  case GauntletType::Water:         editWaterGauntlet();        break;
  case GauntletType::Haunted:       editHauntedGauntlet();      break;
  case GauntletType::Acid:          editAcidGauntlet();         break;
  case GauntletType::Witch:         editWitchGauntlet();        break;
  case GauntletType::Power:         editPowerGauntlet();        break;
  case GauntletType::Potion:        editPotionGauntlet();       break;
  case GauntletType::Snake:         editSnakeGauntlet();        break;
  case GauntletType::Toxic:         editToxicGauntlet();        break;
  case GauntletType::Halloween:     editHalloweenGauntlet();    break;
  case GauntletType::Treasure:      editTreasureGauntlet();     break;
  case GauntletType::Ghost:         editGhostGauntlet();        break;
  case GauntletType::Spider:        editSpiderGauntlet();       break;
  case GauntletType::Gem:           editGemGauntlet();          break;
  case GauntletType::Inferno:       editInfernoGauntlet();      break;
  case GauntletType::Portal:        editPortalGauntlet();       break;
  case GauntletType::Strange:       editStrangeGauntlet();      break;
  case GauntletType::Fantasy:       editFantasyGauntlet();      break;
  case GauntletType::Christmas:     editChristmasGauntlet();    break;
  case GauntletType::Surprise:      editSurpriseGauntlet();     break;
  case GauntletType::Mystery:       editMysteryGauntlet();      break;
  case GauntletType::Cursed:        editCursedGauntlet();       break;
  case GauntletType::Cyborg:        editCyborgGauntlet();       break;
  case GauntletType::Castle:        editCastleGauntlet();       break;
  case GauntletType::Grave:         editGraveGauntlet();        break;
  case GauntletType::Temple:        editTempleGauntlet();       break;
  case GauntletType::World:         editWorldGauntlet();        break;
  case GauntletType::Galaxy:        editGalaxyGauntlet();       break;
  case GauntletType::Universe:      editUniverseGauntlet();     break;
  case GauntletType::Discord:       editDiscordGauntlet();      break;
  case GauntletType::Split:         editSplitGauntlet();        break;
  case GauntletType::NCS:           editNCSGauntlet1();         break;
  case GauntletType::NCS2:          editNCSGauntlet2();         break;
  case GauntletType::Space:         editSpaceGauntlet();        break;
  case GauntletType::Cosmos:        editCosmosGauntlet();       break;
  case GauntletType::Random:        editRandomGauntlet();       break;
  case GauntletType::Chance:        editChanceGauntlet();       break;
  case GauntletType::Love:          editLoveGauntlet();         break;
  case GauntletType::Cinema:        editCinemaGauntlet();       break;
  case GauntletType::Future:        editFutureGauntlet();       break;
  case GauntletType::Utopia:        editUtopiaGauntlet();       break;
  }

  // Fetch levels from RobTop servers
  GameLevelManager::get()->m_levelManagerDelegate = this;
  GameLevelManager::get()->getGauntletLevels(static_cast<int>(m_gauntletType));

  return true;
}

// Level loading delegates

void BetterGauntletLayer::loadLevelsFinished(CCArray *levels, char const *key, int type) {
  auto glm = GameLevelManager::get();
  if (glm->m_levelManagerDelegate == this)
    glm->m_levelManagerDelegate = nullptr;

  if (auto lc = getChildByIDRecursive("loading-circle"))
    lc->removeFromParent();

  if (!levels || levels->count() == 0) {
    Notification::create("No levels returned for this gauntlet.", NotificationIcon::Error)->show();
    return;
  }

  m_levels = levels;
  m_levels->retain();

  setupGauntlet(levels);
}

void BetterGauntletLayer::loadLevelsFailed(char const *key, int type) {
  auto glm = GameLevelManager::get();
  if (glm->m_levelManagerDelegate == this)
    glm->m_levelManagerDelegate = nullptr;

  if (auto lc = getChildByIDRecursive("loading-circle"))
    lc->removeFromParent();

  Notification::create("Failed to load gauntlet levels.", NotificationIcon::Error, 1.5)->show();
}

// setupGauntlet - builds vault + info, then creates level buttons

void BetterGauntletLayer::setupGauntlet(CCArray *levels) {
  CCDirector *director = CCDirector::sharedDirector();
  CCSize winSize = director->getWinSize();

  // Vault button
  CCMenu *vaultMenu = CCMenu::create();
  vaultMenu->setID("level-vault-menu"_spr);
  vaultMenu->setPosition({0, 0});
  this->addChild(vaultMenu);

  CCMenuItemSpriteExtra *vaultBtn = CCMenuItemSpriteExtra::create(
      CCSprite::createWithSpriteFrameName("GJ_safeBtn_001.png"), this,
      menu_selector(BetterGauntletLayer::gauntletVault));
  vaultBtn->setZOrder(5);
  vaultBtn->setPosition(
      ccp(director->getScreenRight() - 31, director->getScreenTop() - 30));

  if (Mod::get()->getSettingValue<bool>("vault-tooltip")) {
    CCSprite *vaultText =
        CCSprite::createWithSpriteFrameName("vaultText.png"_spr);
    vaultText->setPosition(
        ccp(vaultBtn->getPositionX() - 52.5, vaultBtn->getPositionY() - 37.5));
    vaultText->setScale(0.45);
    vaultText->setID("vault-text"_spr);
    vaultMenu->addChild(vaultText);
  }
  vaultMenu->addChild(vaultBtn);

  // Info button
  setupInfo();

  // Build the level island buttons
  editGauntlets();
}

void BetterGauntletLayer::editGauntlets() {
  if (!m_levels || m_loaded)
    return;
  m_loaded = true;

  CCDirector *director = CCDirector::sharedDirector();
  CCSize winSize = director->getWinSize();

  m_levelsMenu = CCMenu::create();
  m_levelsMenu->setPosition({winSize.width / 2, winSize.height / 2 - 15});
  m_levelsMenu->setID("level-array"_spr);
  m_levelsMenu->ignoreAnchorPointForPosition(false);
  this->addChild(m_levelsMenu);

  int levelCount = std::min(static_cast<int>(m_levels->count()), 5);
  auto &lastLockedStates =
      m_lastKnownLockedStates[static_cast<int>(m_gauntletType)];

  for (int i = 0; i < levelCount; i++) {
    GJGameLevel *levelNode =
        static_cast<GJGameLevel *>(m_levels->objectAtIndex(i));
    if (!levelNode)
      continue;

    bool hasCompletedLevel =
        GameStatsManager::sharedState()->hasCompletedLevel(levelNode);
    int id = levelNode->m_levelID.value();
    gd::string name = levelNode->m_levelName;
    gd::string user = levelNode->m_creatorName;

    GJGameLevel *savedLevel =
        GameLevelManager::sharedState()->getSavedGauntletLevel(id);
    int percent = 0;
    if (savedLevel)
      percent = savedLevel->m_normalPercent.value();

    // Island container
    CCNode *levelSpr = CCNode::create();
    levelSpr->setID(fmt::format("gauntlet-{}", i + 1));
    levelSpr->setAnchorPoint({0.5, 0.5});
    levelSpr->setContentSize({70, 80});

    CCSpriteWithHue *islandSpr = CCSpriteWithHue::createWithSpriteFrameName(
        GauntletNode::frameForType(m_gauntletType).c_str());
    islandSpr->setPosition(levelSpr->getContentSize() / 2);
    islandSpr->setID(fmt::format("island-{}", i + 1));

    CCSprite *shadowSpr = CCSprite::createWithSpriteFrameName(
        GauntletNode::frameForType(m_gauntletType).c_str());
    shadowSpr->setColor(ccc3(0, 0, 0));
    shadowSpr->setOpacity(75);
    shadowSpr->setScaleY(1.2);
    shadowSpr->setPosition(
        {islandSpr->getPositionX(), islandSpr->getPositionY() - 10});
    shadowSpr->setID(fmt::format("island-{}-shadow", i + 1));

    levelSpr->addChild(islandSpr);
    levelSpr->addChild(shadowSpr, -1);

    // Labels
    CCLabelBMFont *levelName =
        CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
    limitLabelWidth(levelName, 120.0, 0.4, 0.3);
    levelName->setID("level-name"_spr);
    levelName->setPosition(
        {islandSpr->getPositionX(), islandSpr->getPositionY() - 10});

    CCLabelBMFont *authorName =
        CCLabelBMFont::create(user.c_str(), "goldFont.fnt");
    limitLabelWidth(authorName, 120.0, 0.4, 0.25);
    authorName->setID("creator-name"_spr);
    authorName->setAlignment(kCCTextAlignmentCenter);
    authorName->setPosition(
        {levelName->getPositionX(), levelName->getPositionY() - 10});

    // Star row
    CCNode *starNode = CCNode::create();
    starNode->setPosition(
        {levelName->getPositionX(), levelName->getPositionY() - 27.5f});
    starNode->setID("star-node"_spr);
    starNode->setScale(0.65);
    starNode->setAnchorPoint({0.5, 0.5});
    starNode->setLayout(
        RowLayout::create()->setGap(5)->setAutoGrowAxis(true)->setAxisAlignment(
            AxisAlignment::Center));

    CCLabelBMFont *starCount = CCLabelBMFont::create(
        fmt::format("{}", levelNode->m_stars.value()).c_str(), "bigFont.fnt");
    starCount->setID("star-count"_spr);
    starCount->setScale(0.65);

    CCSprite *starSpr =
        CCSprite::createWithSpriteFrameName("GJ_bigStar_001.png");
    starSpr->setID("star-icon"_spr);
    starSpr->setAnchorPoint(ccp(0.5, 0.5));
    starSpr->setScale(0.65);

    if (hasCompletedLevel) {
      starCount->setColor(ccc3(255, 255, 50));
      CCSprite *checkmarkSpr =
          CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
      checkmarkSpr->setID("checkmark-icon"_spr);
      checkmarkSpr->setAnchorPoint(ccp(0.5, 0.5));
      checkmarkSpr->setPosition(
          {islandSpr->getPositionX() + 15, islandSpr->getPositionY() + 15});
      levelSpr->addChild(checkmarkSpr);
    }

    levelSpr->addChild(levelName);
    levelSpr->addChild(authorName);
    levelSpr->addChild(starNode);
    starNode->addChild(starCount);
    starNode->addChild(starSpr);
    starNode->updateLayout();

    // Lock check
    bool isLocked = false;
    if (i > 0) {
      GJGameLevel *previousLevel = static_cast<GJGameLevel *>(m_levels->objectAtIndex(i - 1));
      isLocked = !GameStatsManager::sharedState()->hasCompletedLevel(previousLevel);
    }

    bool wasLocked = i < static_cast<int>(lastLockedStates.size()) ? lastLockedStates[i] : isLocked;
    bool justUnlocked = wasLocked && !isLocked;

    m_lockedStates.push_back(isLocked);

    if (isLocked || justUnlocked) {
      m_lockSprite = CCSprite::createWithSpriteFrameName("GJ_lock_001.png");
      m_lockSprite->setPosition({islandSpr->getPositionX(), islandSpr->getPositionY() - 15});
      m_lockSprite->setID("gauntlet-lock"_spr);
      levelSpr->addChild(m_lockSprite);
      islandSpr->setColor(ccc3(128, 128, 128));

      levelName->setVisible(false);
      authorName->setVisible(false);
      starNode->setVisible(false);
    }

    // Button
    CCMenuItemSpriteExtra *btn = CCMenuItemSpriteExtra::create(
        levelSpr, this,
        isLocked ? menu_selector(BetterGauntletLayer::onLocked) : menu_selector(BetterGauntletLayer::onLevel));
    btn->setUserObject(levelNode);
    btn->setTag(i);
    btn->setID(fmt::format("level-{}", i + 1));

    float posFarX = 185;
    float posCloseX = posFarX / 2;
    float posY = 50;

    switch (i + 1) {
    case 1: btn->setPosition(winSize.width / 2 - posFarX, winSize.height / 2 - posY); break;
    case 2: btn->setPosition(winSize.width / 2 - posCloseX, winSize.height / 2 + posY); break;
    case 3: btn->setPosition(winSize.width / 2, winSize.height / 2 - posY); break;
    case 4: btn->setPosition(winSize.width / 2 + posCloseX, winSize.height / 2 + posY); break;
    case 5: btn->setPosition(winSize.width / 2 + posFarX, winSize.height / 2 - posY); break;
    }

    m_levelsMenu->addChild(btn);

    if (justUnlocked)
      playUnlockAnimation(levelSpr, i);
  }

  lastLockedStates = m_lockedStates;

  // Hover animation
  bool hover = Mod::get()->getSettingValue<bool>("level-hover");
  if (hover) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = 0; i < levelCount; i++) {
      CCNode *lvl = m_levelsMenu->getChildByIDRecursive(
          fmt::format("gauntlet-{}", i + 1));
      if (!lvl)
        continue;

      float randomMoveUp = 2.0 + static_cast<float>(std::rand() % 15) / 10.0;
      float randomMoveDown = 2.0 + static_cast<float>(std::rand() % 15) / 10.0;

      CCMoveBy *moveUp = CCMoveBy::create(randomMoveUp, ccp(0, 5));
      CCMoveBy *moveDown = CCMoveBy::create(randomMoveDown, ccp(0, -5));
      CCEaseInOut *easeMoveUp = CCEaseInOut::create(moveUp, 2.0);
      CCEaseInOut *easeMoveDown = CCEaseInOut::create(moveDown, 2.0);
      CCSequence *hoverSequence = CCSequence::create(easeMoveUp, easeMoveDown, nullptr);
      CCRepeatForever *levelHover = CCRepeatForever::create(hoverSequence);
      lvl->runAction(levelHover);
    }
  }

  checkGauntletCompletion();
}

void BetterGauntletLayer::onEnter() {
  CCLayer::onEnter();
  checkForUnlocks();
  checkGauntletCompletion();
}

// Check gauntlet completion
void BetterGauntletLayer::checkGauntletCompletion() {
  if (!m_levels) return;

  int levelCount = std::min(static_cast<int>(m_levels->count()), 5);
  if (levelCount == 0) return;

  bool isComplete = true;
  for (int i = 0; i < levelCount; i++) {
    auto level = static_cast<GJGameLevel *>(m_levels->objectAtIndex(i));
    if (!level || !GameStatsManager::sharedState()->hasCompletedLevel(level)) {
      isComplete = false;
      break;
    }
  }

  int key = static_cast<int>(m_gauntletType);
  auto it = m_lastKnownGauntletCompletion.find(key);
  bool wasComplete = it != m_lastKnownGauntletCompletion.end() ? it->second : isComplete;
  m_lastKnownGauntletCompletion[key] = isComplete;

  if (!isComplete) return;

  bool rewardUnclaimed = !GameStatsManager::sharedState()->isGauntletChestUnlocked(key);
  if (wasComplete && !rewardUnclaimed) return;

  auto titleLabel = static_cast<CCLabelBMFont *>(getChildByID("title-shadow"));
  auto highlightLabel = static_cast<CCLabelBMFont *>(getChildByID("title-highlight"_spr));

  ccColor3B titleColor = titleLabel ? titleLabel->getColor() : ccWHITE;
  ccColor3B highlightColor = highlightLabel ? highlightLabel->getColor() : ccWHITE;

  if (auto popup = GauntletCompletionPopup::create(m_gauntletType, titleColor, highlightColor)) {
    this->addChild(popup, 1000);
  }
}

// Check for previously beaten level
void BetterGauntletLayer::checkForUnlocks() {
  if (!m_levels || !m_levelsMenu || m_lockedStates.empty())
    return;

  int levelCount = std::min(static_cast<int>(m_levels->count()), 5);
  for (int i = 0; i < levelCount && i < static_cast<int>(m_lockedStates.size());
       i++) {
    if (!m_lockedStates[i])
      continue;

    GJGameLevel *previousLevel =
        static_cast<GJGameLevel *>(m_levels->objectAtIndex(i - 1));
    bool stillLocked =
        !GameStatsManager::sharedState()->hasCompletedLevel(previousLevel);
    if (stillLocked)
      continue;

    m_lockedStates[i] = false;
    auto levelSpr =
        m_levelsMenu->getChildByIDRecursive(fmt::format("gauntlet-{}", i + 1));
    if (levelSpr)
      playUnlockAnimation(levelSpr, i);
  }
}

constexpr float kUnlockSfxPeakDelay = 1.5f;

// Play new unlock animation
void BetterGauntletLayer::playUnlockAnimation(CCNode *levelSpr, int index) {
  auto btn = static_cast<CCMenuItemSpriteExtra *>(levelSpr->getParent());
  if (btn)
    btn->setTarget(this, menu_selector(BetterGauntletLayer::onLevel));

  auto islandSpr = static_cast<CCSpriteWithHue *>(
      levelSpr->getChildByID(fmt::format("island-{}", index + 1)));
  auto lockSpr = levelSpr->getChildByID("gauntlet-lock"_spr);
  auto levelName =
      static_cast<CCLabelBMFont *>(levelSpr->getChildByID("level-name"_spr));
  auto authorName =
      static_cast<CCLabelBMFont *>(levelSpr->getChildByID("creator-name"_spr));
  auto starNode = levelSpr->getChildByID("star-node"_spr);

  FMODAudioEngine::sharedEngine()->playEffect("unlockGauntlet.ogg");

  if (lockSpr) {
    lockSpr->runAction(
        CCSequence::create(CCFadeOut::create(kUnlockSfxPeakDelay), CCRemoveSelf::create(), nullptr));
  }

  playUnlockParticlesIn(levelSpr, index);
  if (islandSpr)
    islandShake(islandSpr, kUnlockSfxPeakDelay);

  // Name/author/star pop in instantly at the sfx peak, no fade
  for (auto label : {levelName, authorName}) {
    if (!label)
      continue;
    label->runAction(CCSequence::create(
        CCDelayTime::create(kUnlockSfxPeakDelay), CCShow::create(), nullptr));
  }

  if (starNode) {
    starNode->setScale(0.65f);
    starNode->runAction(CCSequence::create(
        CCDelayTime::create(kUnlockSfxPeakDelay), CCShow::create(), nullptr));
  }

  if (islandSpr) {
    islandSpr->setColor(ccc3(128, 128, 128));
    islandSpr->runAction(CCSequence::create(
        CCDelayTime::create(kUnlockSfxPeakDelay),
        CCCallFuncN::create(
            this, callfuncN_selector(BetterGauntletLayer::onUnlockPeak)),
        CCSpawn::create(
            CCTintTo::create(0.3f, 255, 255, 255),
            HueLuminanceTo::create(0.6f, 1, 0),
            CCSequence::create(
                CCEaseInOut::create(CCScaleTo::create(0.25f, 1.35f), 2),
                CCEaseBackOut::create(CCScaleTo::create(0.35f, 1)), nullptr),
            nullptr),
        nullptr));
  }
}

// Fires at the sfx's "unlock" peak (pSender is the island sprite mid-pulse)
void BetterGauntletLayer::onUnlockPeak(CCNode *sender) {
  auto levelSpr = sender->getParent();
  if (!levelSpr)
    return;
  auto btn = levelSpr->getParent();
  playUnlockParticlesOut(levelSpr, btn ? btn->getTag() : 0);
}

CCFiniteTimeAction *
BetterGauntletLayer::generateShakeAction(CCPoint originalPos, float xyOffset, float duration) {
  float dx = CCRANDOM_MINUS1_1() * xyOffset;
  float dy = CCRANDOM_MINUS1_1() * xyOffset;
  return CCMoveTo::create(duration, {originalPos.x + dx, originalPos.y + dy});
}

void BetterGauntletLayer::islandShake(CCSprite *islandSpr, float duration) {
  constexpr float stepDuration = 0.005f;
  constexpr float xyOffset = 3;

  CCPoint originalPos = islandSpr->getPosition();
  int steps = static_cast<int>(duration / stepDuration);

  auto shakeSteps = CCArray::create();
  for (int i = 0; i < steps; i++) {
    shakeSteps->addObject(
        generateShakeAction(originalPos, xyOffset, stepDuration));
  }
  shakeSteps->addObject(CCMoveTo::create(stepDuration, originalPos));

  islandSpr->runAction(CCSequence::create(shakeSteps));
}

void BetterGauntletLayer::playUnlockParticlesIn(CCNode *levelSpr, int index) {
  auto particlesIn = GameToolbox::particleFromString(
      "150a1a2a0a75a-180a180a0a0a75a75a0a0a-2000a0a0a0a3a0a0a62a1a0a1a0a1a0a0."
      "35a0.15a0a0a0a87a1a0a1a0a1a0a0.15a0.05a0.2a0a0.5a0."
      "15a75a25a0a0a0a0a0a2a1a0a0a0a0a0a5a0a0a0a0a0a0a0a0a0a0a0a0",
      NULL, false);
  particlesIn->setPosition(levelSpr->getContentSize() / 2);
  particlesIn->setID(fmt::format("unlock-particles-in-{}", index + 1));
  particlesIn->setAutoRemoveOnFinish(true);
  levelSpr->addChild(particlesIn, -1);
}

void BetterGauntletLayer::playUnlockParticlesOut(CCNode *levelSpr, int index) {
  auto particlesOut = GameToolbox::particleFromString(
      "20a-1a0.75a0a-1a180a180a0a380a0a0a0a0a0a0a0a0a3a0a0a62a1a0a1a0a1a0a0."
      "35a0.15a0a0a0a87a1a0a1a0a1a0a0.15a0.05a0a0a0.5a0."
      "35a0a0a20a15a0a0a0a2a1a0a0a0a0a0a5a0a0a0a0a0a0a0a0a0a3.61a0a0",
      NULL, false);
  particlesOut->setPosition(levelSpr->getContentSize() / 2);
  particlesOut->setID(fmt::format("unlock-particles-out-{}", index + 1));
  particlesOut->setAutoRemoveOnFinish(true);
  levelSpr->addChild(particlesOut, -1);
}

// Info

void BetterGauntletLayer::setupInfo() {
  CCDirector *director = CCDirector::sharedDirector();

  auto infoBtnSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
  auto infoBtn = CCMenuItemSpriteExtra::create(
      infoBtnSpr, this, menu_selector(BetterGauntletLayer::onInfo));
  infoBtn->setPosition(
      ccp(director->getScreenLeft() + 30, director->getScreenBottom() + 30));

  auto infoMenu = CCMenu::create();
  infoMenu->setPosition(0, 0);
  infoMenu->addChild(infoBtn);
  infoMenu->setID("gauntlet-info-menu"_spr);
  this->addChild(infoMenu);

  if (Mod::get()->getSettingValue<bool>("debug-reward-button")) {
    auto debugBtnSpr = ButtonSprite::create("Debug", "goldFont.fnt", "GJ_button_04.png", 0.75);
    auto debugBtn = CCMenuItemSpriteExtra::create(
        debugBtnSpr, this, menu_selector(BetterGauntletLayer::onDebugReward));
    debugBtn->setScale(0.5);
    debugBtn->m_baseScale = 0.5;
    debugBtn->setPosition(
        ccp(director->getScreenRight() - 45, director->getScreenBottom() + 30));

    auto debugMenu = CCMenu::create();
    debugMenu->setPosition(0, 0);
    debugMenu->addChild(debugBtn);
    debugMenu->setID("gauntlet-debug-menu"_spr);
    this->addChild(debugMenu);
  }
}

void BetterGauntletLayer::onDebugReward(CCObject *sender) {
  auto titleLabel = static_cast<CCLabelBMFont *>(getChildByID("title-shadow"));
  auto highlightLabel = static_cast<CCLabelBMFont *>(getChildByID("title-highlight"_spr));

  ccColor3B titleColor = titleLabel ? titleLabel->getColor() : ccWHITE;
  ccColor3B highlightColor = highlightLabel ? highlightLabel->getColor() : ccWHITE;

  if (auto popup = GauntletCompletionPopup::createDebug(m_gauntletType, titleColor, highlightColor)) {
    this->addChild(popup, 1000);
  }
}

void BetterGauntletLayer::onInfo(CCObject *sender) {
  std::string gauntletName = GauntletNode::nameForType(m_gauntletType);
  std::string gauntletDesc = getGauntletInfo(m_gauntletType);

  MDPopup *popup =
      MDPopup::create(("The " + gauntletName + " Gauntlet").c_str(), gauntletDesc.c_str(), "OK");

  std::string date = fmt::format("Released on: {}", getGauntletDate(m_gauntletType));
  std::string version = fmt::format("Version: {}", getGauntletVersion(m_gauntletType));
  std::string infoCredit = fmt::format("Description by: {}", getGauntletInfoCredits(m_gauntletType));

  CCLabelBMFont *releaseDate = CCLabelBMFont::create(date.c_str(), "chatFont.fnt");
  releaseDate->setPosition(383.5, 25);
  releaseDate->setScale(0.5);
  releaseDate->setAnchorPoint(ccp(1, 0));
  releaseDate->setOpacity(51);

  CCLabelBMFont *releaseVer = CCLabelBMFont::create(version.c_str(), "chatFont.fnt");
  releaseVer->setPosition(383.5, 15);
  releaseVer->setScale(0.5);
  releaseVer->setAnchorPoint(ccp(1, 0));
  releaseVer->setOpacity(51);

  CCLabelBMFont *credit = CCLabelBMFont::create(infoCredit.c_str(), "chatFont.fnt");
  credit->setPosition(15.5, 15);
  credit->setScale(0.5);
  credit->setAnchorPoint(ccp(0, 0));
  credit->setOpacity(51);

  CCArray *popupChildren = popup->getChildren();
  CCNode *child = static_cast<CCNode *>(popupChildren->objectAtIndex(0));
  child->setID("info"_spr);
  child->addChild(releaseDate);
  child->addChild(releaseVer);
  child->addChild(credit);

  popup->show();
}

// Callbacks

void BetterGauntletLayer::keyBackClicked() {
  onBack(nullptr);
}

void BetterGauntletLayer::onBack(CCObject *sender) {
  if (m_exiting)
    return;
  m_exiting = true;

  if (CCScene::get()->getUserFlag("from-redash"_spr)) {
    CCDirector::get()->popSceneWithTransition(0.5f, kPopTransitionFade);
    return;
  }

  auto scene = BetterGauntletSelectLayer::scene();
  if (scene) {
    CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f, scene));
  }
}

void BetterGauntletLayer::onLocked(CCObject *sender) {
  auto button = static_cast<CCMenuItemSpriteExtra *>(sender);
  auto levelSpr = static_cast<CCNode *>(button->getNormalImage());
  if (!levelSpr || !levelSpr->getChildren() ||
      levelSpr->getChildren()->count() < 2)
    return;
  auto islandSpr =
      static_cast<CCSprite *>(levelSpr->getChildren()->objectAtIndex(1));
  if (!islandSpr)
    return;

  ccColor3B originalColor = ccc3(128, 128, 128);
  auto turnRed = CCTintTo::create(0, 192, 128, 128);
  auto resetColor =
      CCTintTo::create(0.25, originalColor.r, originalColor.g, originalColor.b);
  auto sequence = CCSequence::create(turnRed, resetColor, nullptr);
  islandSpr->runAction(sequence);
}

void BetterGauntletLayer::onLevel(CCObject *sender) {
  auto button = static_cast<CCMenuItemSpriteExtra *>(sender);
  auto levelNode = static_cast<GJGameLevel *>(button->getUserObject());
  if (!levelNode)
    return;

  auto lil = LevelInfoLayer::create(levelNode, false);
  auto scene = CCScene::create();
  scene->addChild(lil);
  if (CCScene::get()->getUserFlag("from-redash"_spr)) {
    scene->setUserFlag("from-redash"_spr, true);
  }
  CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, scene));
}

void BetterGauntletLayer::gauntletVault(CCObject *obj) {
  CCDirector *director = CCDirector::sharedDirector();
  CCSize winSize = director->getWinSize();

  CCDictionary *savedGauntlets =
      GameLevelManager::sharedState()->m_savedGauntlets;
  CCObject *getGauntletType = savedGauntlets->objectForKey(
      std::to_string(static_cast<int>(m_gauntletType)));
  if (!getGauntletType)
    return;

  GJMapPack *gauntletLevels = static_cast<GJMapPack *>(getGauntletType);
  GJSearchObject *searchObject = GJSearchObject::create(
      SearchType::Type19, gauntletLevels->m_levelStrings);
  LevelBrowserLayer *browserLayer = LevelBrowserLayer::create(searchObject);

  CCSprite *browserBG = static_cast<CCSprite *>(
      browserLayer->getChildByIDRecursive("background"));
  auto browserBL = browserLayer->getChildByIDRecursive("left-corner");
  auto browserBR = browserLayer->getChildByIDRecursive("right-corner");

  CCSpriteGrayscale *grayscaleTL =
      CCSpriteGrayscale::createWithSpriteFrameName("GJ_sideArt_001.png");
  CCSpriteGrayscale *grayscaleTR =
      CCSpriteGrayscale::createWithSpriteFrameName("GJ_sideArt_001.png");
  CCSprite *floor =
      CCSprite::createWithSpriteFrameName("gauntletGround_001.png"_spr);

  CCSize floorSize = floor->getContentSize();
  float scaleX = winSize.width / floorSize.width;
  float scaleY = winSize.height / floorSize.height;
  float scale = std::min(scaleX, scaleY);

  floor->setScale(scale);
  floor->setPosition(ccp(winSize.width / 2, floorSize.height * scale / 2));
  floor->setAnchorPoint(ccp(0.5, 0.5));
  floor->setZOrder(-2);
  floor->setColor(ccc3(175, 175, 175));
  browserLayer->addChild(floor);

  grayscaleTL->setPosition(
      {director->getScreenLeft() - 1, director->getScreenTop() + 1});
  grayscaleTL->setAnchorPoint(ccp(0, 1));
  grayscaleTL->setColor(ccc3(67, 67, 67));
  grayscaleTL->setFlipY(true);
  grayscaleTL->setZOrder(-1);
  browserLayer->addChild(grayscaleTL);

  grayscaleTR->setPosition(
      {director->getScreenRight() + 1, director->getScreenTop() + 1});
  grayscaleTR->setAnchorPoint(ccp(1, 1));
  grayscaleTR->setColor(ccc3(67, 67, 67));
  grayscaleTR->setFlipX(true);
  grayscaleTR->setFlipY(true);
  grayscaleTR->setZOrder(-1);
  browserLayer->addChild(grayscaleTR);

  if (browserBG)
    browserBG->setColor(ccc3(34, 34, 34));
  if (browserBL)
    browserBL->setVisible(false);
  if (browserBR)
    browserBR->setVisible(false);

  auto scene = CCScene::create();
  scene->addChild(browserLayer);
  auto transition = CCTransitionFade::create(0.5, scene);
  CCDirector::sharedDirector()->pushScene(transition);
}
