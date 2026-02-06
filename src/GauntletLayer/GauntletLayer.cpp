#include <cstdlib>
#include <ctime>
#include <cocos2d.h>
#include <Geode/binding/GauntletLayer.hpp>
#include <Geode/ui/Layout.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include "GauntletLayer.hpp"
#include "LevelInfoPopup.hpp"
#include "../GauntletInfo/GauntletInfo.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

// #ifndef GEODE_IS_IOS
// 	#include <geode.custom-keybinds/include/Keybinds.hpp>
// 	using namespace keybinds;
// #endif

CCNode* RedesignedGauntletLayer::getChildBySpriteFrameNameRecursive(cocos2d::CCNode* parent, char const* name) {
	return findFirstChildRecursive<cocos2d::CCNode>(parent, [=](auto* spr) {
		return isSpriteFrameName(spr, name);
	});
}

// #ifndef GEODE_IS_IOS
// 	void RedesignedGauntletLayer::defineKeybind(const char* id, std::function<void()> callback) {
// 		this->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {
// 			if (event->isDown()) {
// 				callback();
// 			}
// 			return ListenerResult::Propagate;
// 		}, id);
// 	}
// #endif

void RedesignedGauntletLayer::gauntletLevel(int desiredLevel) {
	if (const auto gauntletLevel = getChildByIDRecursive("levels-menu")->getChildByIDRecursive(fmt::format("level-{}", desiredLevel))) {
		auto gauntletSprite = gauntletLevel->getChildByType<GauntletSprite>(0);
		if (!gauntletSprite) return;
		for (auto node : CCArrayExt<CCNode*>(gauntletSprite->getChildren())) {
			if (typeinfo_cast<CCSpriteGrayscale*>(node)) return;
		}
		GauntletLayer::onLevel(gauntletLevel);
	}
}

void RedesignedGauntletLayer::setupInfo() {
    CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

    auto infoBtnSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");

    auto infoBtn = CCMenuItemSpriteExtra::create(
        infoBtnSpr, this, menu_selector(RedesignedGauntletLayer::onInfo)
    );

    infoBtn->setPosition(ccp(director->getScreenLeft() + 30, director->getScreenBottom() + 30));

    auto infoMenu = CCMenu::create();
    infoMenu->setPosition(0, 0);
    infoMenu->addChild(infoBtn);
    infoMenu->setID("gauntlet-info-menu"_spr);

    this->addChild(infoMenu);
}

void RedesignedGauntletLayer::onInfo(CCObject* sender) {
	CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

	std::string gauntletName = GauntletNode::nameForType(m_gauntletType);
	std::string gauntletDesc = getGauntletInfo(GauntletType (m_gauntletType));

	MDPopup* popup = MDPopup::create(
		("The " + gauntletName + " Gauntlet").c_str(),
		gauntletDesc.c_str(),
		"OK"
	);
	
	std::string date = fmt::format("Released on: {}", getGauntletDate(GauntletType (m_gauntletType)));
	std::string version = fmt::format("Version: {}", getGauntletVersion(GauntletType (m_gauntletType)));
	std::string infoCredit = fmt::format("Description by: {}", getGauntletInfoCredits(GauntletType (m_gauntletType)));
	
	CCLabelBMFont* releaseDate = CCLabelBMFont::create(date.c_str(), "chatFont.fnt");
	releaseDate->setPosition(383.5, 25);
	releaseDate->setScale(0.5f);
	releaseDate->setAnchorPoint(ccp(1, 0));
	releaseDate->setOpacity(51);
	
	CCLabelBMFont* releaseVer = CCLabelBMFont::create(version.c_str(), "chatFont.fnt");
	releaseVer->setPosition(383.5, 15);
	releaseVer->setScale(0.5f);
	releaseVer->setAnchorPoint(ccp(1, 0));
	releaseVer->setOpacity(51);
	
	CCLabelBMFont* credit = CCLabelBMFont::create(infoCredit.c_str(), "chatFont.fnt");
	credit->setPosition(15.5, 15);
	credit->setScale(0.5f);
	credit->setAnchorPoint(ccp(0, 0));
	credit->setOpacity(51);

	CCArray* popupChildren = popup->getChildren();
	CCNode* child = static_cast<CCNode*>(popupChildren->objectAtIndex(0));
	
	child->setID("info"_spr);
	child->addChild(releaseDate);
	child->addChild(releaseVer);
	child->addChild(credit);

	popup->show();
}

inline void limitLabelWidth(CCLabelBMFont* label, float maxWidth, float originalScale = 1.0f, float minScale = 0.5f) {
    if (!label) return;

    float contentWidth = label->getContentSize().width * originalScale;
    if (contentWidth > maxWidth) {
        float newScale = maxWidth / label->getContentSize().width;
        newScale = std::max(newScale, minScale);
        label->setScale(newScale);
    } else {
        label->setScale(originalScale);
    }
}

bool RedesignedGauntletLayer::init(GauntletType type) {
	if (!GauntletLayer::init(type)) {
		Notification::create("Failed to gather Gauntlet data, reloading...", NotificationIcon::Loading)->show();
		return false;
	}

	this->setTouchEnabled(true);
    this->setTouchMode(cocos2d::kCCTouchesOneByOne);
    this->setTouchPriority(-128); // Adjust priority as needed

	log::debug("Opened Gauntlet #{}", static_cast<int>(m_gauntletType));

	if (Loader::get()->getLoadedMod("jacob375.gauntletlevelvault")) {
		CCNode* removeOGVaultBtn = getChildByIDRecursive("jacob375.gauntletlevelvault/gauntlet-levels");
		if (removeOGVaultBtn) {
			removeOGVaultBtn->removeFromParent();
		}
	}

	CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

	auto exitAdjust = this->getChildByIDRecursive("exit-menu");
	if (!exitAdjust) return false;
	exitAdjust->setPosition(24, 254.5);
	exitAdjust->setContentWidth(32.5);
	exitAdjust->setContentHeight(125);
	exitAdjust->updateLayout();

	auto shadowText = static_cast<CCLabelBMFont*>(this->getChildByID("title"));
	shadowText->setFntFile("GR_OxygeneFontOutline.fnt"_spr);
	shadowText->setColor(ccc3(0, 0, 0));
	shadowText->setScale(0.750);
	shadowText->setPosition(ccp(winSize.width / 2 + 1, director->getScreenTop() - 32.5));

	auto titleText = static_cast<CCLabelBMFont*>(this->getChildByID("title-shadow"));
	titleText->setFntFile("GR_OxygeneFont.fnt"_spr);
	titleText->setColor(ccc3(255, 255, 255));
	titleText->setScale(0.750);
	titleText->setPosition(ccp(winSize.width / 2, director->getScreenTop() - 30)); 

	CCSprite* floorLine = CCSprite::createWithSpriteFrameName("floorLine_001.png");
	floorLine->setID("floor-line");
	floorLine->setPosition({winSize.width / 2, titleText->getPositionY() - 25});
	floorLine->setScaleX(.85);
	floorLine->setOpacity(100);
	this->addChild(floorLine);

	auto bgLayer = getChildByID("background");
	bgLayer->setZOrder(-99);

	switch(m_gauntletType) {
		default: { // 0 (Fallback/Unsupported)
			editGauntletFallback();
			break;
		}
		case GauntletType::Fire: { // 1
			editFireGauntlet();
			break;
		}
		case GauntletType::Ice: { // 2
			editIceGauntlet();
			break;
		}
		case GauntletType::Poison: { // 3
			editPoisonGauntlet();
			break;
		}
		case GauntletType::Shadow: { // 4
			editShadowGauntlet();
			break;
		}
		case GauntletType::Lava: { // 5
			editLavaGauntlet();
			break;
		}
		case GauntletType::Bonus: { // 6
			editBonusGauntlet();
			break;
		}
		case GauntletType::Chaos: { // 7
			editChaosGauntlet();
			break;
		}
		case GauntletType::Demon: { // 8
			editDemonGauntlet();
			break;
		}
		case GauntletType::Time: { // 9
			editTimeGauntlet();
			break;
		}
		case GauntletType::Crystal: { // 10
			editCrystalGauntlet();
			break;
		}
		case GauntletType::Magic: { // 11
			editMagicGauntlet();
			break;
		}
		case GauntletType::Spike: { // 12
			editSpikeGauntlet();
			break;
		}
		case GauntletType::Monster: { // 13 UNFINISHED
			editMonsterGauntlet();
			break;
		}
		case GauntletType::Doom: { // 14 UNFINISHED
			editDoomGauntlet();
			break;
		}
		case GauntletType::Death: { // 15
			editDeathGauntlet();
			break;
		}
		case GauntletType::Forest: { // 16
			editForestGauntlet();
			break;
		}
		case GauntletType::Rune: { // 17 UNFINISHED
			editRuneGauntlet();
			break;
		}
		case GauntletType::Force: { // 18 UNFINISHED
			editForceGauntlet();
			break;
		}
		case GauntletType::Spooky: { // 19 UNFINISHED
			editSpookyGauntlet();
			break;
		}
		case GauntletType::Dragon: { // 20 UNFINISHED
			editDragonGauntlet();
			break;
		}
		case GauntletType::Water: { // 21
			editWaterGauntlet();
			break;
		}
		case GauntletType::Haunted: { // 22 UNFINISHED
			editHauntedGauntlet();
			break;
		}
		case GauntletType::Acid: { // 23 UNFINISHED
			editAcidGauntlet();
			break;
		}
		case GauntletType::Witch: { // 24 UNFINISHED
			editWitchGauntlet();
			break;
		}
		case GauntletType::Power: { // 25 UNFINISHED
			editPowerGauntlet();
			break;
		}
		case GauntletType::Potion: { // 26 UNFINISHED
			editPotionGauntlet();
			break;
		}
		case GauntletType::Snake: { // 27 UNFINISHED
			editSnakeGauntlet();
			break;
		}
		case GauntletType::Toxic: { // 28 UNFINISHED
			editToxicGauntlet();
			break;
		}
		case GauntletType::Halloween: { // 29 UNFINISHED
			editHalloweenGauntlet();
			break;
		}
		case GauntletType::Treasure: { // 30 UNFINISHED
			editTreasureGauntlet();
			break;
		}
		case GauntletType::Ghost: { // 31 UNFINISHED
			editGhostGauntlet();
			break;
		}
		case GauntletType::Spider: { // 32 UNFINISHED
			editSpiderGauntlet();
			break;
		}
		case GauntletType::Gem: { // 33 UNFINISHED
			editGemGauntlet();
			break;
		}
		case GauntletType::Inferno: { // 34 UNFINISHED
			editInfernoGauntlet();
			break;
		}
		case GauntletType::Portal: { // 35 UNFINISHED
			editPortalGauntlet();
			break;
		}
		case GauntletType::Strange: { // 36 UNFINISHED
			editStrangeGauntlet();
			break;
		}
		case GauntletType::Fantasy: { // 37 UNFINISHED
			editFantasyGauntlet();
			break;
		}
		case GauntletType::Christmas: { // 38
			editChristmasGauntlet();
			break;
		}
		case GauntletType::Surprise: { // 39 UNFINISHED
			editSurpriseGauntlet();
			break;
		}
		case GauntletType::Mystery: { // 40 UNFINISHED
			editMysteryGauntlet();
			break;
		}
		case GauntletType::Cursed: { // 41 UNFINISHED
			editCursedGauntlet();
			break;
		}
		case GauntletType::Cyborg: { // 42 UNFINISHED
			editCyborgGauntlet();
			break;
		}
		case GauntletType::Castle: { // 43 UNFINISHED
			editCastleGauntlet();
			break;
		}
		case GauntletType::Grave: { // 44 UNFINISHED
			editGraveGauntlet();
			break;
		}
		case GauntletType::Temple: { // 45 UNFINISHED
			editTempleGauntlet();
			break;
		}
		case GauntletType::World: { // 46 UNFINISHED
			editWorldGauntlet();
			break;
		}
		case GauntletType::Galaxy: { // 47 UNFINISHED
			editGalaxyGauntlet();
			break;
		}
		case GauntletType::Universe: { // 48 UNFINISHED
			editUniverseGauntlet();
			break;
		}
		case GauntletType::Discord: { // 49 UNFINISHED
			editDiscordGauntlet();
			break;
		}
		case GauntletType::Split: { // 50
			editSplitGauntlet();
			break;
		}
		case GauntletType::NCS: { // 51
			editNCSGauntlet1();
			break;
		}
		case GauntletType::NCS2: { // 52
			editNCSGauntlet2();
			break;
		}
		case GauntletType::Space: { // 53
			editSpaceGauntlet();
			break;
		}
		case GauntletType::Cosmos: { // 54
			editCosmosGauntlet();
			break;
		}
		case GauntletType::Random: { // 55
			editRandomGauntlet();
			break;
		}
		case GauntletType::Chance: { // 56
			editChanceGauntlet();
			break;
		}
		case GauntletType::Love: { // 57
			editLoveGauntlet();
			break;
		}
		case GauntletType::Cinema: { // 58
			editCinemaGauntlet();
			break;
		}
		case GauntletType::Future: { // 59
			editFutureGauntlet();
			break;
		}
		case GauntletType::Utopia: { // 60
			editUtopiaGauntlet();
			break;
		}
	}

	if (this->m_levels != nullptr && !m_fields->m_loaded)
		editGauntlets();
		
	return true;
}

void RedesignedGauntletLayer::loadLevelsFinished(CCArray* p0, char const* p1, int p2) {
    GauntletLayer::loadLevelsFinished(p0, p1, p2);

	if (m_fields->m_loaded) return;

	editGauntlets();
}

void RedesignedGauntletLayer::setupGauntlet(CCArray* levels) {
    GauntletLayer::setupGauntlet(levels);

	// #ifndef GEODE_IS_IOS
	// 	this->defineKeybind("first-gauntlet-level"_spr, [this]() {
	// 	RedesignedGauntletLayer::gauntletLevel(1); // default: numrow 1
	// 	});
	// 	this->defineKeybind("second-gauntlet-level"_spr, [this]() {
	// 		RedesignedGauntletLayer::gauntletLevel(2); // default: numrow 2
	// 	});
	// 	this->defineKeybind("third-gauntlet-level"_spr, [this]() {
	// 		RedesignedGauntletLayer::gauntletLevel(3); // default: numrow 3
	// 	});
	// 	this->defineKeybind("fourth-gauntlet-level"_spr, [this]() {
	// 		RedesignedGauntletLayer::gauntletLevel(4); // default: numrow 4
	// 	});
	// 	this->defineKeybind("fifth-gauntlet-level"_spr, [this]() {
	// 		RedesignedGauntletLayer::gauntletLevel(5); // default: numrow 5
	// 	});
	// #endif

	auto pathParent = CCNode::create();
	if (!pathParent) return;
	
	pathParent->setID("gauntlet-path"_spr);
	
	this->addChild(pathParent);

	for (int d = 0; d < 32; d++) {
		auto dot = this->getChildBySpriteFrameNameRecursive(this, "uiDot_001.png");
		if (!dot) continue;
		
		dot->retain();
		dot->removeFromParentAndCleanup(false);
		dot->setVisible(false);
		
		pathParent->addChild(dot);
	}

	CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

	if (Loader::get()->getLoadedMod("jacob375.gauntletlevelvault")) {
		auto type = m_gauntletType;
		auto vaultMenu = CCMenu::create();
		if (!vaultMenu) return;
		
		vaultMenu->setID("level-vault-menu"_spr);
		vaultMenu->setPosition({0, 0});
		
		this->addChild(vaultMenu);

		auto vaultBtnSpr = CircleButtonSprite::createWithSpriteFrameName(
			GauntletNode::frameForType(static_cast<GauntletType>(type)).c_str(), 1.0f
		);

		auto vaultBtn = CCMenuItemSpriteExtra::create(vaultBtnSpr, this, menu_selector(RedesignedGauntletLayer::gauntletVault));
		vaultBtn->setZOrder(5);
		vaultBtn->setPosition(ccp(director->getScreenRight() - 31, director->getScreenTop() - 30));

		if (Mod::get()->getSettingValue<bool>("vault-tooltip")) {
			auto vaultText = CCSprite::createWithSpriteFrameName("vaultText.png"_spr);
			vaultText->setPosition(ccp(vaultBtn->getPositionX() - 47.5, vaultBtn->getPositionY() - 32.5));
			vaultText->setScale(0.45f);
			vaultText->setID("vault-text"_spr);

			vaultMenu->addChild(vaultText);
		}

		vaultMenu->addChild(vaultBtn);
	}
	
	setupInfo();
}

void RedesignedGauntletLayer::editGauntlets() {

	CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();
	GauntletType type = m_gauntletType;

	auto refreshSpr = Mod::get()->getSettingValue<double>("rescale-refresh-spr");
	if (refreshSpr) {
		auto loadCircle = getChildByID("loading-circle");
		loadCircle->setScale(refreshSpr);
	}

	CCMenu* levelsMenu = static_cast<CCMenu*>(this->getChildByIDRecursive("levels-menu"));
	if (!levelsMenu) log::error("error loading levels menu");
	levelsMenu->setPosition(0, 0);
	levelsMenu->setVisible(false);
	levelsMenu->setPositionY(-1000);

	m_fields->m_levelsMenu = CCMenu::create();
	m_fields->m_levelsMenu->setPosition(0, 0);
	m_fields->m_levelsMenu->setID("level-array"_spr);
	m_fields->m_levelsMenu->setAnchorPoint(ccp(0, 0));
	m_fields->m_levelsMenu->setContentSize({winSize.width * 2, winSize.height - 90});
	m_fields->m_levelsMenu->setScale(1.15);
	this->addChild(m_fields->m_levelsMenu);

    
    for (int i = 1; i < 6; i++) {
		CCNode* level = getChildByIDRecursive(fmt::format("level-{}", i));
		if (!level) {
			log::warn("level-{} not found!", i);
			continue;
		}

		// Sprites    
		CCNode* levelBtn = getChildByIDRecursive(fmt::format("level-{}", i));
		CCSprite* island = static_cast<CCSprite*>(level->getChildByTag(1));
		if (!levelBtn) continue;
		if (!island) continue;

		// Island Children
		CCArray* children = island->getChildren();
		if (!children || children->count() == 0) continue;
		
		// Stats
		GJGameLevel* levelNode = static_cast<GJGameLevel*>(m_levels->objectAtIndex(i - 1));
		bool hasCompletedLevel = GameStatsManager::sharedState()->hasCompletedLevel(levelNode);
		int id = levelNode->m_levelID.value();
		gd::string name = levelNode->m_levelName;
		gd::string user = levelNode->m_creatorName;
		GJGameLevel* savedLevel = GameLevelManager::sharedState()->getSavedGauntletLevel(id);
		
		// Percentage
		int percent = 0;
		if (savedLevel) {
			percent = savedLevel->m_normalPercent.value();
		}
		// Log level info
		log::info("#{}: {} by {}; {}%", i, name, user, percent);

		// ===== Button Creation =====

		// Island Nodes
		CCNode* levelSpr = CCNode::create();
		levelSpr->setID(fmt::format("gauntlet-{}", i));
		levelSpr->setAnchorPoint({0.5, 0.5});
		levelSpr->setContentSize({70, 80});

		CCSprite* islandSpr = CCSprite::createWithSpriteFrameName(
			GauntletNode::frameForType(m_gauntletType).c_str()
		);
		islandSpr->setPosition(levelSpr->getContentSize() / 2);
		
		CCSprite* shadowSpr = CCSprite::createWithSpriteFrameName(
			GauntletNode::frameForType(m_gauntletType).c_str()
		);
		shadowSpr->setColor(ccc3(0, 0, 0));
		shadowSpr->setOpacity(75);
		shadowSpr->setScaleY(1.2);
		shadowSpr->setPosition({islandSpr->getPositionX(), islandSpr->getPositionY() - 10});
		shadowSpr->setZOrder(islandSpr->getZOrder() - 1);

		levelSpr->addChild(islandSpr);
		levelSpr->addChild(shadowSpr);

		// ===== Node Children =====

		// Stats
		CCLabelBMFont* levelName = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
		limitLabelWidth(levelName, 120.0f, 0.4f, 0.3f);
		// levelName->setPositionY(-11.5);
		levelName->setID("level-name"_spr);
		levelName->setPosition({islandSpr->getPositionX(), islandSpr->getPositionY() - 10});

		CCLabelBMFont* authorName = CCLabelBMFont::create(user.c_str(), "goldFont.fnt");
		limitLabelWidth(authorName, 120.0f, 0.4f, 0.25f);
		// authorName->setPositionY(levelName->getPositionY() - 11);
		authorName->setID("creator-name"_spr);
		authorName->setAlignment(kCCTextAlignmentCenter);
		authorName->setPosition({levelName->getPositionX(), levelName->getPositionY() - 10});

		// Star Icon and Count
		CCLabelBMFont* starCount = CCLabelBMFont::create(
			fmt::format("{}", levelNode->m_stars.value()).c_str(),
			"bigFont.fnt"
		);
		starCount->setID("star-count"_spr);
		starCount->setPositionX(5);
		starCount->setScale(0.5);

		CCSprite* starSpr = CCSprite::createWithSpriteFrameName("GJ_bigStar_001.png");
		starSpr->setID("star-icon"_spr);
		starSpr->setAnchorPoint(ccp(0.5, 0.5));
		starSpr->setPositionX(5);
		starSpr->setScale(0.5);
		
		// On Level Completion
		if (hasCompletedLevel) {
			starCount->setColor(ccc3(255, 255, 50));
			
			// Checkmark icon
			CCSprite* checkmarkSpr = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
			checkmarkSpr->setID("checkmark-icon"_spr);
			checkmarkSpr->setAnchorPoint(ccp(0.5, 0.5));
			checkmarkSpr->setPosition(ccp(25, 2));
			levelSpr->addChild(checkmarkSpr);

		}
		levelSpr->addChild(levelName);
		levelSpr->addChild(authorName);
		levelSpr->addChild(starCount);
		levelSpr->addChild(starSpr);

		// Skull icon
		CCSprite* skullSpr = CCSprite::createWithSpriteFrameName("miniSkull_001.png");
		if (m_fields->m_levelsMenu->getChildByTag(4)) {
			skullSpr->setID("skull-icon"_spr);
			skullSpr->setPositionY(10);
			levelSpr->addChild(skullSpr);
		}

		// Is Level Locked?
		GJGameLevel* lockNode = static_cast<GJGameLevel*>(m_levels->objectAtIndex(i - 1));
		bool isLocked = false;
		if (i > 1) {
			GJGameLevel* previousLevel = static_cast<GJGameLevel*>(m_levels->objectAtIndex(i - 2));
			isLocked = !GameStatsManager::sharedState()->hasCompletedLevel(previousLevel);
		}
		if (isLocked) {
			CCSprite* lockSpr = CCSprite::createWithSpriteFrameName("GJ_lock_001.png");
			lockSpr->setPosition({islandSpr->getPositionX(), islandSpr->getPositionY() - 15});
			lockSpr->setScale(0.75);
			levelSpr->addChild(lockSpr);
			islandSpr->setColor(ccc3(128, 128, 128));

			levelName->setVisible(false);
			authorName->setVisible(false);
			starCount->setVisible(false);
			starSpr->setVisible(false);
			skullSpr->setVisible(false);
		}

		CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(
			levelSpr,
			this,
			isLocked 
			? menu_selector(RedesignedGauntletLayer::onLocked) 
			: menu_selector(RedesignedGauntletLayer::onLevelInfo)
		);
		btn->setUserObject(levelNode);
		btn->setTag(i - 1);
		btn->setID(fmt::format("level-{}", i));

		m_fields->m_levelsMenu->addChild(btn);
		
		CCNode* btnPos = m_fields->m_levelsMenu->getChildByID(fmt::format("level-{}", i));
		CCNode* previousPosX = m_fields->m_levelsMenu->getChildByID(fmt::format("level-{}", i - 1));

		if (i == 1) btn->setPosition(winSize.width / 5, 90);
		if (i == 2) btn->setPosition(previousPosX->getPositionX() + 150, 160);
		if (i == 3) btn->setPosition(previousPosX->getPositionX() + 150, 100);
		if (i == 4) btn->setPosition(previousPosX->getPositionX() + 150, 150);
		if (i == 5) btn->setPosition(previousPosX->getPositionX() + 150, 110);

	}
	
	bool hover = Mod::get()->getSettingValue<bool>("level-hover");
	if (hover) {
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		
		for (int i = 0; i < 5; i++) {
			CCNode* levels = m_fields->m_levelsMenu->getChildByIDRecursive(fmt::format("level-{}", i + 1));
			if (!levels) log::error("Levels node missing!");
			
			float randomMoveUp = 2.0f + static_cast<float>(std::rand() % 15) / 10.0f;
			float randomMoveDown = 2.0f + static_cast<float>(std::rand() % 15) / 10.0f;

			CCMoveBy* moveUp = CCMoveBy::create(randomMoveUp, ccp(0, 5));
			CCMoveBy* moveDown = CCMoveBy::create(randomMoveDown, ccp(0, -5));
			
			CCEaseInOut* easeMoveUp = CCEaseInOut::create(moveUp, 2.0f);
			CCEaseInOut* easeMoveDown = CCEaseInOut::create(moveDown, 2.0f);
			
			CCSequence* hoverSequence = CCSequence::create(easeMoveUp, easeMoveDown, nullptr);
			
			CCRepeatForever* levelHover = CCRepeatForever::create(hoverSequence);
			
			levelHover->setTag(999);
			levels->runAction(levelHover);
		}
	}
}

void RedesignedGauntletLayer::onLocked(CCObject* sender) {
    auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto levelSpr = static_cast<CCNode*>(button->getNormalImage());
    auto islandSpr = static_cast<CCSprite*>(levelSpr->getChildren()->objectAtIndex(1));
    if (!islandSpr) return;
	
    ccColor3B originalColor = ccc3(128, 128, 128);
    
    islandSpr->stopActionByTag(100);
    
    auto turnRed = CCTintTo::create(0, 192, 128, 128);
    auto resetColor = CCTintTo::create(0.25f, originalColor.r, originalColor.g, originalColor.b);
    
    auto sequence = CCSequence::create(turnRed, resetColor, nullptr);
    sequence->setTag(100);
    islandSpr->runAction(sequence);
    
    log::error("Level locked!");
}

// Gauntlet Level Vault
void RedesignedGauntletLayer::gauntletVault(CCObject* obj) {
    auto savedGauntlets = GameLevelManager::sharedState()->m_savedGauntlets;
    auto getGauntletType = savedGauntlets->objectForKey(std::to_string(static_cast<int>(m_gauntletType)));
    auto gauntletLevels = static_cast<GJMapPack*>(getGauntletType);
    auto searchObject = GJSearchObject::create(SearchType::Type19, gauntletLevels->m_levelStrings);
    auto browserLayer = LevelBrowserLayer::create(searchObject);
    auto scene = CCScene::create();
    scene->addChild(browserLayer);
    auto transition = CCTransitionFade::create(0.5, scene);
    CCDirector::sharedDirector()->pushScene(transition);
}

void RedesignedGauntletLayer::onLevelInfo(CCObject* sender) {
    auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto level = static_cast<GJGameLevel*>(button->getUserObject());
    LevelInfoPopup::create(level)->show();
}

bool RedesignedGauntletLayer::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    if (!m_fields->m_levelsMenu) {
		log::error("m_levelsMenu not found");
		return false;
	}
    
	log::info("touching");

    auto touchLoc = touch->getLocation();
    auto local = m_fields->m_levelsMenu->convertToNodeSpace(touchLoc);
    auto size = m_fields->m_levelsMenu->getContentSize();
    
    if (local.x >= 0 && local.x <= size.width && local.y >= 0 && local.y <= size.height) {
        m_fields->m_dragging = true;
        m_fields->m_touchStartLoc = touchLoc;
        m_fields->m_menuStartPos = m_fields->m_levelsMenu->getPosition();
        m_fields->m_touchLastLoc = touchLoc;
        m_fields->m_touchLastTime = std::chrono::steady_clock::now();
        m_fields->m_velocity = ccp(0, 0);
        m_fields->m_flinging = false;
        return true;
    }
    return false;
}

void RedesignedGauntletLayer::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    if (m_fields->m_dragging) {
        // Just use the 'touch' parameter directly

	log::info("dragging");

        auto touchLoc = touch->getLocation();
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> dt = now - m_fields->m_touchLastTime;
        float secs = dt.count();
        if (secs <= 0.0f) secs = 1e-6f;
        
        CCPoint delta = ccpSub(touchLoc, m_fields->m_touchStartLoc);
        CCPoint newPos = ccpAdd(m_fields->m_menuStartPos, delta);
        
        // Compute velocity
        m_fields->m_velocity = ccp(
            (touchLoc.x - m_fields->m_touchLastLoc.x) / secs,
            (touchLoc.y - m_fields->m_touchLastLoc.y) / secs
        );
        m_fields->m_touchLastLoc = touchLoc;
        m_fields->m_touchLastTime = now;
        
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto content = m_fields->m_levelsMenu->getContentSize();
        
        float minX = std::min(0.0f, winSize.width - content.width);
        float maxX = std::max(0.0f, winSize.width - content.width);
        float minY = std::min(0.0f, winSize.height - content.height);
        float maxY = std::max(0.0f, winSize.height - content.height);
        
        newPos.x = std::max(minX, std::min(maxX, newPos.x));
        newPos.y = std::max(minY, std::min(maxY, newPos.y));
        
        m_fields->m_levelsMenu->setPosition(newPos);
        updateParallax(newPos);
    }
}

void RedesignedGauntletLayer::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    if (m_fields->m_dragging) {
        m_fields->m_dragging = false;

		log::info("stopped");
        
        float speed = std::hypotf(m_fields->m_velocity.x, m_fields->m_velocity.y);
        if (speed > 300.0f) {
            m_fields->m_flinging = true;
        } else {
            m_fields->m_velocity = ccp(0, 0);
            m_fields->m_flinging = false;
        }
    }
}

void RedesignedGauntletLayer::updateParallax(CCPoint const& menuPos) {
    if (!m_fields->m_bgSprite || !m_fields->m_bgSprite2) return;
    
    CCPoint offset = ccpSub(menuPos, m_fields->m_menuOriginPos);
    CCPoint parallaxOffset = ccpMult(offset, m_fields->m_parallax);
    
    m_fields->m_bgSprite->setPosition(ccpAdd(m_fields->m_bgStartPos, parallaxOffset));
}