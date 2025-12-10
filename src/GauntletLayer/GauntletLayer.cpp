#include <vector>
#include <cstdlib>
#include <ctime>
#include <cocos2d.h>
#include <Geode/binding/GauntletLayer.hpp>
#include <Geode/ui/Layout.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include <Geode/utils/cocos.hpp>

// Files
#include "GauntletLayer.hpp"
#include "../GauntletInfo/GauntletInfo.hpp"

using namespace geode::prelude;

#ifndef GEODE_IS_IOS
	#include <geode.custom-keybinds/include/Keybinds.hpp>
	using namespace keybinds;
#endif

CCNode* RedesignedGauntletLayer::getChildBySpriteFrameNameRecursive(cocos2d::CCNode* parent, char const* name) {
	return findFirstChildRecursive<cocos2d::CCNode>(parent, [=](auto* spr) {
		return isSpriteFrameName(spr, name);
	});
}

#ifndef GEODE_IS_IOS
	void RedesignedGauntletLayer::defineKeybind(const char* id, std::function<void()> callback) {
		this->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {
			if (event->isDown()) {
				callback();
			}
			return ListenerResult::Propagate;
		}, id);
	}
#endif

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

void RedesignedGauntletLayer::setupInfoButton() {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto infoBtnSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");

    auto infoBtn = CCMenuItemSpriteExtra::create(
        infoBtnSpr, this, menu_selector(RedesignedGauntletLayer::onInfoButtonClick)
    );

    infoBtn->setPosition(ccp(director->getScreenLeft() + 30, director->getScreenBottom() + 30));

    auto infoMenu = CCMenu::create();
    infoMenu->setPosition({0.f, 0.f});
    infoMenu->addChild(infoBtn);
    infoMenu->setID("gauntlet-info-menu"_spr);

    this->addChild(infoMenu);
}

void RedesignedGauntletLayer::onInfoButtonClick(CCObject* sender) {
	auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

	std::string gauntletName = GauntletNode::nameForType(m_gauntletType);
	std::string gauntletDesc = getGauntletInfo(GauntletType (m_gauntletType));

	auto popup = MDPopup::create(
		("The " + gauntletName + " Gauntlet").c_str(),
		gauntletDesc.c_str(),
		"OK"
	);
	
	auto date = fmt::format("Release Date: {}", getGauntletDate(GauntletType (m_gauntletType)));

	auto releaseDate = CCLabelBMFont::create(date.c_str(), "chatFont.fnt");
	releaseDate->setPosition({383.5, 25});
	releaseDate->setScale(0.5f);
	releaseDate->setAnchorPoint(ccp(1, 0));
	releaseDate->setOpacity(51);

	auto version = fmt::format("Version: {}", getGauntletVersion(GauntletType (m_gauntletType)));

	auto releaseVer = CCLabelBMFont::create(version.c_str(), "chatFont.fnt");
	releaseVer->setPosition({383.5, 15});
	releaseVer->setScale(0.5f);
	releaseVer->setAnchorPoint(ccp(1, 0));
	releaseVer->setOpacity(51);

	auto infoCredit = fmt::format("Description by: {}", getGauntletInfoCredits(GauntletType (m_gauntletType)));

	auto credit = CCLabelBMFont::create(infoCredit.c_str(), "chatFont.fnt");
	credit->setPosition({15.5, 15});
	credit->setScale(0.5f);
	credit->setAnchorPoint(ccp(0, 0));
	credit->setOpacity(51);

	auto popupChildren = popup->getChildren();
	auto child = static_cast<CCNode*>(popupChildren->objectAtIndex(0));
	child->setID("popup-child");
	log::info("Popup child: {} scaleY", child->getScaledContentHeight());

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
	if (!GauntletLayer::init(type)) return false;

	log::debug("Opened Gauntlet #{}", static_cast<int>(m_gauntletType));

	if (Loader::get()->getLoadedMod("jacob375.gauntletlevelvault")) {
		auto removeOGVaultBtn = getChildByIDRecursive("jacob375.gauntletlevelvault/gauntlet-levels");
		if (!removeOGVaultBtn) nullptr;
		else removeOGVaultBtn->removeFromParent();
	}

	auto director = CCDirector::sharedDirector();
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto gauntletShadowText = static_cast<CCLabelBMFont*>(this->getChildByID("title"));							// "title" is the shadow text
	gauntletShadowText->setFntFile("GR_OxygeneFontOutline.fnt"_spr);											// for whatever damn reason
	gauntletShadowText->setColor(ccc3(0, 0, 0));
	gauntletShadowText->setScale(0.750);
	gauntletShadowText->setPosition(ccp(winSize.width / 2 + 1, director->getScreenTop() - 32.5));

	auto gauntletTitleText = static_cast<CCLabelBMFont*>(this->getChildByID("title-shadow"));					// and "title-shadow" is the actual
	gauntletTitleText->setFntFile("GR_OxygeneFont.fnt"_spr);													// title text like what dude why.
	gauntletTitleText->setColor(ccc3(255, 255, 255));														   // geode devs please explain
	gauntletTitleText->setScale(0.750);																			// yourselves this instant >:(
	gauntletTitleText->setPosition(ccp(winSize.width / 2, director->getScreenTop() - 30)); 

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
		// case GauntletType::Random: { // 55
		// 	editCosmosGauntlet();
		// 	break;
		// }
		// case GauntletType::Chance: { // 56
		// 	editCosmosGauntlet();
		// 	break;
		// }
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

	#ifndef GEODE_IS_IOS
		this->defineKeybind("first-gauntlet-level"_spr, [this]() {
		RedesignedGauntletLayer::gauntletLevel(1); // default: numrow 1
		});
		this->defineKeybind("second-gauntlet-level"_spr, [this]() {
			RedesignedGauntletLayer::gauntletLevel(2); // default: numrow 2
		});
		this->defineKeybind("third-gauntlet-level"_spr, [this]() {
			RedesignedGauntletLayer::gauntletLevel(3); // default: numrow 3
		});
		this->defineKeybind("fourth-gauntlet-level"_spr, [this]() {
			RedesignedGauntletLayer::gauntletLevel(4); // default: numrow 4
		});
		this->defineKeybind("fifth-gauntlet-level"_spr, [this]() {
			RedesignedGauntletLayer::gauntletLevel(5); // default: numrow 5
		});
	#endif

	auto pathParent = CCLayerRGBA::create();
	if (!pathParent) nullptr;
	pathParent->setID("gauntlet-path"_spr);
	this->addChild(pathParent);

	std::vector<CCSprite*> pathVector = {};

	for (int d = 0; d < 32; d++) {
		auto dot = this->getChildByType<CCSprite>(d + 2);

		if (!dot) continue;

		auto pathEdit = Mod::get()->getSettingValue<bool>("enable-path-edits");
		if (pathEdit) {
			auto dotBlending = Mod::get()->getSettingValue<std::string>("path-blending");
			if (dotBlending == "Default") {
				dot->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
			}
			else if (dotBlending == "Additive") {
				dot->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });
			}

		}

		dot->setID(fmt::format("path-dot-{}", d + 1));
		pathVector.push_back(dot);

		if (d % 2 == 1) {
			dot->setColor(ccc3(210, 210, 210));
		} else {
			dot->setColor(ccc3(255, 255, 255));
		}
	}
	for (auto dot : pathVector) {
		dot->retain();
		dot->removeFromParentAndCleanup(false);
		pathParent->addChild(dot);
	}

	if (!Mod::get()->getSettingValue<bool>("toggle-paths")) {
		auto pathParent = this->getChildByID("gauntlet-path"_spr);
		pathParent->setVisible(false);
	}

	auto director = CCDirector::sharedDirector();
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	if (Loader::get()->getLoadedMod("jacob375.gauntletlevelvault")) {
		auto vaultMenu = CCMenu::create();
		if (!vaultMenu) return;
		vaultMenu->setID("level-vault-menu"_spr);
		vaultMenu->setPosition({0, 0});
		this->addChild(vaultMenu);

		auto type = m_gauntletType;

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

	auto disconnectSettings = Mod::get()->getSettingValue<bool>("disconnect-button");
	if (disconnectSettings) {

		auto disconnectMenu = CCMenu::create();
		if (!disconnectMenu) return;
		disconnectMenu->setID("disconnect-menu"_spr);
		disconnectMenu->setPosition(ccp(director->getScreenRight() - 45, director->getScreenBottom() + 20));
		disconnectMenu->setScale(0.5f);
		disconnectMenu->setContentSize({0, 0});
		this->addChild(disconnectMenu);

		auto bigFontBtnSpr = ButtonSprite::create("Disconnect", "goldFont.fnt", "GJ_button_04.png", .8f);
		auto bigFontBtn = CCMenuItemSpriteExtra::create(bigFontBtnSpr, this, menu_selector(RedesignedGauntletLayer::simulateDisconnect));
		bigFontBtn->setID("disconnect-button"_spr);
		
		disconnectMenu->addChild(bigFontBtn);
		
	}
	
	setupInfoButton();
}

void RedesignedGauntletLayer::editGauntlets() {

	auto director = CCDirector::sharedDirector();
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto refreshSpr = Mod::get()->getSettingValue<double>("rescale-refresh-spr");
	if (refreshSpr) {
		auto loadCircle = getChildByID("loading-circle");
		loadCircle->setScale(refreshSpr);
	}

	if (m_fields->m_loaded = true) {
		log::debug("Connected to Gauntlet #{}", static_cast<int>(m_gauntletType));
	} else {
		log::error("Failed to connect to server");
	}

	auto levelMenuPos = static_cast<CCMenu*>(this->getChildByIDRecursive("levels-menu"));
	if (!levelMenuPos) return;
	levelMenuPos->setPosition(0.f, 0.f);

	auto levelOne = static_cast<CCSprite*>(this->getChildByIDRecursive("level-1"));
	levelOne->setPosition({winSize.width / 2 - 180, winSize.height / 2 - 67});

	auto levelTwo = static_cast<CCSprite*>(this->getChildByIDRecursive("level-2"));
	levelTwo->setPosition({winSize.width / 2 - 90, winSize.height / 2 + 37});

	auto levelThree = static_cast<CCSprite*>(this->getChildByIDRecursive("level-3"));
	levelThree->setPosition({winSize.width / 2, winSize.height / 2 - 67});

	auto levelFour = static_cast<CCSprite*>(this->getChildByIDRecursive("level-4"));
	levelFour->setPosition({winSize.width / 2 + 90, winSize.height / 2 + 37});

	auto levelFive = static_cast<CCSprite*>(this->getChildByIDRecursive("level-5"));
	levelFive->setPosition({winSize.width / 2 + 180, winSize.height / 2 - 67});
		
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
										skullSpr->retain();
										skullSpr->removeFromParentAndCleanup(false);
										skullNode->addChild(skullSpr);
										skullSpr->release();
									}
									
									skullSpr->setID("skull-icon"_spr);
									skullSpr->setPositionY(10.f);
									if (hasCompletedLevel) {
										skullSpr->setColor(ccc3(128, 128, 128));
									}
								}
							}
						}
					}
				}
			}
		}
	}

	auto hover = Mod::get()->getSettingValue<bool>("islands-float");
	if (hover) {

		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		for (int i = 0; i < 5; i++) {
			auto levelMenu = getChildByIDRecursive(fmt::format("level-{}", i + 1));
			if (levelMenu) {
				float randomMoveUp = 2.0f + static_cast<float>(std::rand() % 15) / 10.0f;
				float randomMoveDown = 2.0f + static_cast<float>(std::rand() % 15) / 10.0f;

				auto moveUp = CCMoveBy::create(randomMoveUp, ccp(0, 5));
				auto moveDown = CCMoveBy::create(randomMoveDown, ccp(0, -5));

				auto easeMoveUp = CCEaseInOut::create(moveUp, 2.0f);
				auto easeMoveDown = CCEaseInOut::create(moveDown, 2.0f);

				auto hoverSequence = CCSequence::create(easeMoveUp, easeMoveDown, nullptr);

				auto levelHover = CCRepeatForever::create(hoverSequence);

				levelMenu->runAction(levelHover);
			}
		}
	}
}

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

void RedesignedGauntletLayer::simulateDisconnect(CCObject* obj) {
    auto levels = getChildByIDRecursive("levels-menu");
    if (levels) levels->setVisible(false);

    auto path = getChildByIDRecursive("gauntlet-path"_spr);
    if (path) path->setVisible(false);

    auto disconnectText = getChildByIDRecursive("try-again-text");
    if (disconnectText) disconnectText->setVisible(true);

	auto disconnectMenu = getChildByIDRecursive("disconnect-menu"_spr);
	if (disconnectMenu) disconnectMenu->setVisible(false);
}