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
#include "../GauntletInfo/GauntletInfo.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

CCNode* RedesignedGauntletLayer::getChildBySpriteFrameNameRecursive(cocos2d::CCNode* parent, char const* name) {
	return findFirstChildRecursive<cocos2d::CCNode>(parent, [=](auto* spr) {
		return isSpriteFrameName(spr, name);
	});
}

void RedesignedGauntletLayer::gauntletLevel(int desiredLevel) {
    // Get levels-menu safely
    auto levelsMenu = getChildByIDRecursive("levels-menu");
    if (!levelsMenu) {
        log::error("levels-menu not found!");
        return;
    }
    
    // Now safely get the gauntlet level
    auto gauntletLevel = levelsMenu->getChildByIDRecursive(fmt::format("level-{}", desiredLevel));
    if (!gauntletLevel) {
        log::error("level-{} not found!", desiredLevel);
        return;
    }
    
    auto gauntletSprite = gauntletLevel->getChildByType<GauntletSprite>(0);
    if (!gauntletSprite) return;
    
    for (auto node : CCArrayExt<CCNode*>(gauntletSprite->getChildren())) {
        if (typeinfo_cast<CCSpriteGrayscale*>(node)) return;
    }
    
    GauntletLayer::onLevel(gauntletLevel);
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
	releaseDate->setScale(0.5);
	releaseDate->setAnchorPoint(ccp(1, 0));
	releaseDate->setOpacity(51);
	
	CCLabelBMFont* releaseVer = CCLabelBMFont::create(version.c_str(), "chatFont.fnt");
	releaseVer->setPosition(383.5, 15);
	releaseVer->setScale(0.5);
	releaseVer->setAnchorPoint(ccp(1, 0));
	releaseVer->setOpacity(51);
	
	CCLabelBMFont* credit = CCLabelBMFont::create(infoCredit.c_str(), "chatFont.fnt");
	credit->setPosition(15.5, 15);
	credit->setScale(0.5);
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

inline void limitLabelWidth(CCLabelBMFont* label, float maxWidth, float originalScale = 1.0, float minScale = 0.5) {
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

	log::debug("Opened Gauntlet #{}", static_cast<int>(m_gauntletType));

	// if (Loader::get()->getLoadedMod("jacob375.gauntletlevelvault")) {
	// 	CCNode* removeOGVaultBtn = getChildByIDRecursive("jacob375.gauntletlevelvault/gauntlet-levels");
	// 	if (removeOGVaultBtn) {
	// 		removeOGVaultBtn->removeFromParent();
	// 	}
	// }

	CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

	auto exitAdjust = this->getChildByIDRecursive("exit-menu");
	if (exitAdjust) {
		exitAdjust->setPosition(24, 254.5);
		exitAdjust->setContentWidth(32.5);
		exitAdjust->setContentHeight(125);
		exitAdjust->updateLayout();
	} else {
		log::warn("exit-menu not found, skipping layout adjustment");
	}

	auto shadowText = static_cast<CCLabelBMFont*>(this->getChildByID("title"));
	if (shadowText) {
		shadowText->setFntFile("GR_OxygeneFontOutline.fnt"_spr);
		shadowText->setColor(ccc3(0, 0, 0));
		shadowText->setScale(0.750);
		shadowText->setPosition(ccp(winSize.width / 2 + 1, director->getScreenTop() - 32.5));
	}

	auto titleText = static_cast<CCLabelBMFont*>(this->getChildByID("title-shadow"));
	if (titleText) {
		titleText->setFntFile("GR_OxygeneFont.fnt"_spr);
		titleText->setColor(ccc3(255, 255, 255));
		titleText->setScale(0.750);
		titleText->setPosition(ccp(winSize.width / 2, director->getScreenTop() - 30));
	}

	CCSprite* floorLine = CCSprite::createWithSpriteFrameName("floorLine_001.png");
	if (floorLine) {
		floorLine->setID("floor-line");
		floorLine->setPosition({winSize.width / 2, titleText ? titleText->getPositionY() - 25 : winSize.height - 55});
		floorLine->setScaleX(.85);
		floorLine->setOpacity(100);
		this->addChild(floorLine);
	}

	auto bgLayer = getChildByID("background");
	if (bgLayer) {
		bgLayer->setZOrder(-99);
	}

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
		case GauntletType::Doom: { // 14 
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
		case GauntletType::Rune: { // 17 
			editRuneGauntlet();
			break;
		}
		case GauntletType::Force: { // 18 
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

	auto pathParent = CCNode::create();
	if (!pathParent) return;
	
	pathParent->setID("gauntlet-path"_spr);
	
	this->addChild(pathParent);

	for (int d = 0; d < 32; d++) {
		auto dot = this->getChildBySpriteFrameNameRecursive(this, "uiDot_001.png");
		if (!dot) continue;
		
		dot->retain();
		dot->removeFromParentAndCleanup(false);
		// dot->setVisible(false);
		
		pathParent->addChild(dot);
	}

	CCDirector* director = CCDirector::sharedDirector();
    CCSize winSize = director->getWinSize();

		auto type = m_gauntletType;
		auto vaultMenu = CCMenu::create();
		if (!vaultMenu) return;
		
		vaultMenu->setID("level-vault-menu"_spr);
		vaultMenu->setPosition({0, 0});
		
		this->addChild(vaultMenu);

		CCMenuItemSpriteExtra* vaultBtn = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_safeBtn_001.png"),
			this, menu_selector(RedesignedGauntletLayer::gauntletVault)
		);
		vaultBtn->setZOrder(5);
		vaultBtn->setPosition(ccp(director->getScreenRight() - 31, director->getScreenTop() - 30));

		if (Mod::get()->getSettingValue<bool>("vault-tooltip")) {
			auto vaultText = CCSprite::createWithSpriteFrameName("vaultText.png"_spr);
			vaultText->setPosition(ccp(vaultBtn->getPositionX() - 52.5, vaultBtn->getPositionY() - 37.5));
			vaultText->setScale(0.45);
			vaultText->setID("vault-text"_spr);

			vaultMenu->addChild(vaultText);
		}

		vaultMenu->addChild(vaultBtn);
	
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
	if (!levelsMenu) {
		log::error("levels-menu not found - cannot setup hover effect");
	}
	
	m_fields->m_levelsMenu = levelsMenu;

	for (int g = 0; g < 5; g++) {
		auto gauntletLevel = getChildByIDRecursive(fmt::format("level-{}", g + 1));
		if (gauntletLevel) {
			for (int i = 0; i < 5; i++) {
				auto sprite = static_cast<CCSprite*>(gauntletLevel->getChildByTag(i + 1));
				if (sprite) {
					sprite->setID(fmt::format("gauntlet-island-{}"_spr, g + 1));
					CCArray* children = sprite->getChildren();
					if (children && children->count() > 0) {

						auto levelNode = static_cast<GJGameLevel*>(m_levels->objectAtIndex(g));
						auto nameString = levelNode->m_levelName;
						std::string author = levelNode->m_creatorName;
						
						if (levelNode) {
							log::info("Level {}: {} by {}", g + 1, nameString, author);
						}
						auto shadow = static_cast<CCSprite*>(children->objectAtIndex(2));
						if (shadow) {
							shadow->setID("gauntlet-shadow"_spr);
						}
						auto island = static_cast<CCSprite*>(children->objectAtIndex(0));
						if (island) {
							island->setID("gauntlet-level"_spr);
						}
						auto stats = static_cast<CCSprite*>(children->objectAtIndex(1));
						if (stats) {
							stats->setID("gauntlet-stats"_spr);
							CCArray* statsChildren = stats->getChildren();
							if (statsChildren && statsChildren->count() > 0) {								
								auto starPos = static_cast<CCSprite*>(statsChildren->objectAtIndex(2));
								bool hasCompletedLevel = GameStatsManager::sharedState()->hasCompletedLevel(static_cast<GJGameLevel*>(m_levels->objectAtIndex(g)));
								if (starPos) {
									starPos->setID("star-icon"_spr);
									starPos->setAnchorPoint(ccp(0.5, 0.5));
									starPos->setPositionX(5);
								}								
								auto starLabel = static_cast<CCLabelBMFont*>(statsChildren->objectAtIndex(1));
								if (starLabel) {
									starLabel->setID("star-label"_spr);
									if (hasCompletedLevel) {
										starLabel->setColor(ccc3(255, 255, 50));
										auto star = CCParticleSystemQuad::create();
										if (star) {
											CCParticleSystemQuad* starParticles = GameToolbox::particleFromString(
												"30a-1a2a0a8a180a180a0a0a25a50a0a5a-8a0a0a10a5a0a0a0a1a0a1a0a0.25a0a1a0.05a0a0a0a0a1a0a1a0a1a0a0a0a0a0a0.35a0a0a0a20a0a0a0a1a2a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
												NULL,
												false
											);
											starParticles->setZOrder(1);
											starParticles->setPositionX(starPos->getPositionX());
											starParticles->setPositionY(starPos->getPositionY());
											stats->addChild(starParticles);
										}
									}
								}								
								auto nameModify = static_cast<CCLabelBMFont*>(statsChildren->objectAtIndex(0));
								if (nameModify) {
									nameModify->setID("level-name"_spr);
									nameModify->setPositionY(-11.5);
								}
								auto authorName = CCLabelBMFont::create(author.c_str(), "goldFont.fnt");
								if (authorName) {
									authorName->setID("creator-name"_spr);
									authorName->setAlignment(kCCTextAlignmentCenter);
									authorName->setPositionY(nameModify->getPositionY() - 11);
									authorName->setScale(0.4f);
									stats->addChild(authorName);
								}
								auto checkmarkSpr = this->getChildBySpriteFrameNameRecursive(stats, "GJ_completesIcon_001.png");
								if (checkmarkSpr) {
									checkmarkSpr->setID("checkmark-icon"_spr);
									checkmarkSpr->setAnchorPoint(ccp(0.5, 0.5));
									checkmarkSpr->setPosition(ccp(25.f, 2.f));
								}
								auto skullSpr = typeinfo_cast<CCSprite*>(getChildBySpriteFrameNameRecursive(stats, "miniSkull_001.png"));
								if (skullSpr) {

									auto skullNode = CCNode::create();
									skullNode->setID("skull-node"_spr);
									stats->addChild(skullNode);

									if (skullSpr->getParent()) {
										skullSpr->retain(); // Retain to prevent auto-deletion
										skullSpr->removeFromParentAndCleanup(false); // Cleanup false to preserve state
										skullNode->addChild(skullSpr);
										skullSpr->release(); // Release after reparenting
									}
									
									skullSpr->setID("skull-icon"_spr);
									skullSpr->setPositionY(10.f);
									if (hasCompletedLevel) {
										skullSpr->setColor(ccc3(128, 128, 128));
									}
								}
							}
						}
						// auto grayscaleSpr = typeinfo_cast<CCSpriteGrayscale*>(island);
						// if (grayscaleSpr) {
						// 	auto authorName = CCLabelBMFont::create(author.c_str(), "goldFont.fnt");
						// 	if (authorName) {
						// 		authorName->setID("creator-name"_spr);
						// 		authorName->setAlignment(kCCTextAlignmentCenter);
						// 		authorName->setPositionY(nameModify->getPositionY() - 11);
						// 		authorName->setScale(0.4f);
						// 		stats->addChild(authorName);
						// 	}
						// }
					}
				}
			}
		}
	}

	bool hover = Mod::get()->getSettingValue<bool>("level-hover");
	if (hover) {
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		
		for (int i = 0; i < 5; i++) {
			CCNode* levels = m_fields->m_levelsMenu->getChildByIDRecursive(fmt::format("level-{}", i + 1));
			if (!levels) continue;
			
			float randomMoveUp = 2.0 + static_cast<float>(std::rand() % 15) / 10.0;
			float randomMoveDown = 2.0 + static_cast<float>(std::rand() % 15) / 10.0;

			CCMoveBy* moveUp = CCMoveBy::create(randomMoveUp, ccp(0, 5));
			CCMoveBy* moveDown = CCMoveBy::create(randomMoveDown, ccp(0, -5));
			
			CCEaseInOut* easeMoveUp = CCEaseInOut::create(moveUp, 2.0);
			CCEaseInOut* easeMoveDown = CCEaseInOut::create(moveDown, 2.0);
			
			CCSequence* hoverSequence = CCSequence::create(easeMoveUp, easeMoveDown, nullptr);
			
			CCRepeatForever* levelHover = CCRepeatForever::create(hoverSequence);
			levels->runAction(levelHover);
		}
	}
}

void RedesignedGauntletLayer::gauntletVault(CCObject* obj) {
	CCDirector* director = CCDirector::sharedDirector();
	CCSize winSize = director->getWinSize();

    auto savedGauntlets = GameLevelManager::sharedState()->m_savedGauntlets;
	auto getGauntletType = savedGauntlets->objectForKey(std::to_string(static_cast<int>(m_gauntletType)));
	auto gauntletLevels = static_cast<GJMapPack*>(getGauntletType);
	auto searchObject = GJSearchObject::create(SearchType::Type19, gauntletLevels->m_levelStrings);
	auto browserLayer = LevelBrowserLayer::create(searchObject);
	
	auto browserBG = static_cast<CCSprite*>(browserLayer->getChildByIDRecursive("background"));
	auto browserBL = static_cast<CCSprite*>(browserLayer->getChildByIDRecursive("left-corner"));
	auto browserBR = static_cast<CCSprite*>(browserLayer->getChildByIDRecursive("right-corner"));
	auto grayscaleTL = CCSpriteGrayscale::createWithSpriteFrameName("GJ_sideArt_001.png");
	auto grayscaleTR = CCSpriteGrayscale::createWithSpriteFrameName("GJ_sideArt_001.png");
	auto floor = CCSprite::createWithSpriteFrameName("gauntletGround_001.png"_spr);
	
	auto floorSize = floor->getContentSize();
	
	float scaleX = winSize.width / floorSize.width;
	float scaleY = winSize.height / floorSize.height;
	float scale = std::min(scaleX, scaleY);
	
	floor->setScale(scale);
	floor->setPosition(ccp(winSize.width / 2, floorSize.height * scale / 2));
	floor->setAnchorPoint(ccp(0.5, 0.5));
	floor->setZOrder(-2);
	floor->setColor(ccc3(175, 175, 175));
	browserLayer->addChild(floor);
	
	grayscaleTL->setPosition({CCDirector::get()->getScreenLeft() - 1, CCDirector::get()->getScreenTop() + 1});
	grayscaleTL->setAnchorPoint(ccp(0, 1));
	grayscaleTL->setColor(ccc3(67, 67, 67));
	grayscaleTL->setFlipY(true);
	grayscaleTL->setZOrder(-1);
	browserLayer->addChild(grayscaleTL);

	grayscaleTR->setPosition({CCDirector::get()->getScreenRight() + 1, CCDirector::get()->getScreenTop() + 1});
	grayscaleTR->setAnchorPoint(ccp(1, 1));
	grayscaleTR->setColor(ccc3(67, 67, 67));
	grayscaleTR->setFlipX(true);
	grayscaleTR->setFlipY(true);
	grayscaleTR->setZOrder(-1);
	browserLayer->addChild(grayscaleTR);
	
	browserBG->setColor(ccc3(34, 34, 34));
	browserBL->setVisible(false);
	browserBR->setVisible(false);
    
	auto scene = CCScene::create();
	scene->addChild(browserLayer);
    
	auto transition = CCTransitionFade::create(0.5, scene);
	CCDirector::sharedDirector()->pushScene(transition);
}