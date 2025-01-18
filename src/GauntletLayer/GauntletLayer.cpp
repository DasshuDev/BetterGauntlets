#include <Geode/Geode.hpp>
#include <Geode/modify/GauntletLayer.hpp>
#include <Geode/loader/SettingV3.hpp>
#include "cocos2d.h"
#include "../Gauntlets/editGauntletSingle.hpp"

// gauntlet files
#include "../Gauntlets/fire.hpp"

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS
#include <geode.custom-keybinds/include/Keybinds.hpp>
using namespace keybinds;
#endif

class $modify(RedesignedGauntletLayer, GauntletLayer) {
    struct Fields {
        bool m_loaded = false;
    };

	cocos2d::CCNode* getChildBySpriteFrameNameRecursive(cocos2d::CCNode* parent, char const* name) {
        return findFirstChildRecursive<cocos2d::CCNode>(parent, [=](auto* spr) {
            return isSpriteFrameName(spr, name);
        });
    }

    #ifdef GEODE_IS_WINDOWS
    void defineKeybind(const char* id, std::function<void()> callback) {
        this->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {
            if (event->isDown()) {
                callback();
            }
            return ListenerResult::Propagate;
        }, id);
    }
    void gauntletLevel(int desiredLevel) {
        if (const auto gauntletLevel = getChildByIDRecursive("levels-menu")->getChildByIDRecursive(fmt::format("level-{}", desiredLevel))) {
            auto gauntletSprite = gauntletLevel->getChildByType<GauntletSprite>(0);
            if (!gauntletSprite) return;
            for (auto node : CCArrayExt<CCNode*>(gauntletSprite->getChildren())) {
                if (typeinfo_cast<CCSpriteGrayscale*>(node)) return;
            }
            GauntletLayer::onLevel(gauntletLevel);
        }
    }
    #endif

	bool init(GauntletType p0) {
		if (!GauntletLayer::init(p0))
			return false;

		auto main = getChildByID("main-layer");

		auto director = CCDirector::sharedDirector();
		auto winSize = CCDirector::sharedDirector()->getWinSize();

		// "title" is the shadow text for whatever damn reason
		auto gauntletShadowText = static_cast<CCLabelBMFont*>(this->getChildByID("title"));
		gauntletShadowText->setFntFile("GR_OxygeneFontOutline.fnt"_spr);
		gauntletShadowText->setColor(ccc3(0, 0, 0));
		gauntletShadowText->setScale(0.750);
		gauntletShadowText->setPositionX(winSize.width / 2 + 1);
		gauntletShadowText->setPositionY(winSize.height - 32.5);

		// and "title-shadow" is the actual title text like what dude why
		auto gauntletTitleText = static_cast<CCLabelBMFont*>(this->getChildByID("title-shadow"));
		gauntletTitleText->setFntFile("GR_OxygeneFont.fnt"_spr);
		gauntletTitleText->setColor(ccc3(255, 255, 255));
		gauntletTitleText->setScale(0.750);
		gauntletTitleText->setPositionX(winSize.width / 2);
		gauntletTitleText->setPositionY(winSize.height - 30);

		auto bgLayer = getChildByID("background");
		bgLayer->setZOrder(-10);

		auto loadingCircleNode = getChildBySpriteFrameNameRecursive(this, "loadingCircle-uhd.png");
		if (loadingCircleNode) {
			loadingCircleNode->setID("loading-spinner");
            // loadingCircleNode->setContentSize(ccp(0, 0));
            // loadingCircleNode->setAnchorPoint(ccp(0.5, 0.5));
            loadingCircleNode->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        }
        // auto rescaleLC = Mod::get()->getSettingValue<double>("loading-circle-scale");
        // if (rescaleLC) {
        //     loadingCircleNode->setScale(rescaleLC);
        // }

		switch(m_gauntletType) {
			default: break;

			case GauntletType::Fire: { // 1
				// editGauntletSingle("Fire", ccc3(63, 29, 9), ccc3(255, 255, 0), ccc3(231, 79, 0));
				// GameManager::get()->loadBackground(14);
				
				// auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_14_001.png");
				// this->m_backgroundSprite->setTexture(bgTexture);

				// auto bgParticleNode = CCParticleSystemQuad::create();
				// if (bgParticleNode) {
				// 	CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("50a-1a2a0.45a20a90a90a0a0a300a0a-44a140a0a0a0a196a25a15a0a40a1a0a0.75a0a0a0a1a0.05a0a0a0a25a1a0a0.25a0a0a0a0.5a0.15a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a0.6a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
				// 	bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenBottom() - 10));
				// 	bgParticles->setZOrder(-2);
				// 	this->addChild(bgParticles);
				// }

				// for (int i = 0; i < 5; i++) {
				// 	auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
				// 	if (level) {
				// 		auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
				// 		if (name) {
				// 			name->setColor(ccc3(255, 185, 80));
				// 		}
				// 	}
				// }
				editFireGauntlet(this);
				break;
			}
			case GauntletType::Ice: { // 2
				editGauntletSingle::editGauntletSingle("Ice", ccc3(8, 68, 83), ccc3(192, 236, 255), ccc3(0, 174, 255));
				GameManager::get()->loadBackground(36);
				
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_36_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				auto bgParticleNode = CCParticleSystemQuad::create();
				if (bgParticleNode) {
					CCParticleSystemQuad* bgParticles = GameToolbox::particleFromString("50a-1a2a0.45a3a90a90a0a0a300a0a0a-403a0a0a0a0a10a5a-45a0a0.619608a0a1a0a0.996078a0a1a0.05a0a0a-45a0a0a0a0.545098a0a1a0a0.5a0.15a0a0a0.25a0a0a0a0a0a0a0a0a2a1a0a0a0a96a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
					bgParticles->setPosition(ccp(winSize.width / 2, director->getScreenTop() + 10));
					bgParticles->setZOrder(-2);
					this->addChild(bgParticles);
				}

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(197, 237, 255));
						}
					}
				}
				break;
			}
			case GauntletType::Poison: { // 3
				editGauntletSingle("Poison", ccc3(20, 63, 9), ccc3(229, 255, 0), ccc3(80, 179, 0));
				GameManager::get()->loadBackground(31);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_31_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(210, 255, 201));
						}
					}
				}
				break;
			}
			case GauntletType::Shadow: { // 4
				editGauntletSingle("Shadow", ccc3(46, 19, 45), ccc3(177, 1, 255), ccc3(59, 0, 141));
				GameManager::get()->loadBackground(27);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_27_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);
				m_backgroundSprite->setPositionY(-175);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(233, 161, 255));
						}
					}
				}
				break;
			}
			case GauntletType::Lava: { // 5
				editGauntletSingle("Lava", ccc3(63, 9, 9), ccc3(255, 209, 56), ccc3(255, 51, 0));
				GameManager::get()->loadBackground(57);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_57_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(255, 173, 148));
						}
					}
				}
				break;
			}
			case GauntletType::Bonus: { // 6
				editGauntletSingle("Bonus", ccc3(30, 54, 85), ccc3(255, 241, 20), ccc3(226, 102, 0));
				GameManager::get()->loadBackground(19);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_19_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(255, 229, 144));
						}
					}
				}
				break;
			}
			case GauntletType::Chaos: { // 7
				editGauntletSingle("Chaos", ccc3(33, 12, 6), ccc3(255, 145, 0), ccc3(255, 18, 0));
				GameManager::get()->loadBackground(14);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_14_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(255, 201, 180));
						}
					}
				}
				break;
			}
			case GauntletType::Demon: { // 8
				editGauntletSingle("Demon", ccc3(29, 29, 29), ccc3(202, 81, 81), ccc3(99, 3, 3));
				GameManager::get()->loadBackground(38);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_38_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(255, 192, 167));
						}
					}
				}
				break;
			}
			case GauntletType::Time: { // 9
				editGauntletSingle("Time", ccc3(55, 79, 107), ccc3(248, 219, 138), ccc3(243, 100, 17));
				GameManager::get()->loadBackground(17);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_17_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(202, 249, 255));
						}
					}
				}
				break;
			}
			case GauntletType::Crystal: { // 10
				editGauntletSingle("Crystal", ccc3(51, 2, 31), ccc3(248, 223, 255), ccc3(246, 58, 218));
				GameManager::get()->loadBackground(29);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_29_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(253, 226, 255));
						}
					}
				}
				break;
			}
			case GauntletType::Magic: { // 11
				editGauntletSingle("Magic", ccc3(14, 16, 51), ccc3(77, 250, 255), ccc3(36, 52, 145));
				GameManager::get()->loadBackground(32);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_32_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(195, 253, 255));
						}
					}
				}
				break;
			}
			case GauntletType::Spike: { // 12
				editGauntletSingle("Spike", ccc3(54, 37, 4), ccc3(245, 174, 68), ccc3(124, 91, 1));
				GameManager::get()->loadBackground(15);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_15_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(245, 222, 160));
						}
					}
				}
				break;
			}
			case GauntletType::Monster: { // 13
				editGauntletSingle("Monster", ccc3(23, 58, 28), ccc3(65, 214, 35), ccc3(8, 110, 13));
				GameManager::get()->loadBackground(38);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_38_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(151, 192, 146));
						}
					}
				}
				break;
			}
			case GauntletType::Doom: { // 14
				editGauntletSingle("Doom", ccc3(48, 36, 33), ccc3(141, 133, 128), ccc3(75, 69, 60));
				GameManager::get()->loadBackground(35);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_35_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);
					m_backgroundSprite->setAnchorPoint(ccp(0.500f, 0.0f));
					m_backgroundSprite->setPositionX(winSize.width / 2);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(212, 156, 156));
						}
					}
				}
				break;
			}
			case GauntletType::Death: { // 15
				editGauntletSingle("Death", ccc3(22, 22, 22), ccc3(100, 100, 100), ccc3(48, 48, 48));
				GameManager::get()->loadBackground(16);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_16_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(153, 106, 106));
						}
					}
				}
				break;
			}
			case GauntletType::Forest: { // 16
				editGauntletSingle("Forest", ccc3(33, 68, 34), ccc3(148, 255, 98), ccc3(61, 165, 20));
				GameManager::get()->loadBackground(26);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_26_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(154, 214, 148));
						}
					}
				}
				break;
			}
			case GauntletType::Rune: { // 17
				editGauntletSingle("Rune", ccc3(46, 18, 0), ccc3(227, 130, 33), ccc3(100, 28, 0));
				GameManager::get()->loadBackground(15);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_15_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(255, 230, 161));
						}
					}
				}
				break;
			}
			case GauntletType::Force: { // 18
				editGauntletSingle("Force", ccc3(53, 50, 64), ccc3(130, 112, 193), ccc3(160, 19, 74));
				GameManager::get()->loadBackground(42);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_42_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(245, 166, 255));
						}
					}
				}
				break;
			}
			case GauntletType::Spooky: { // 19
				editGauntletSingle("Spooky", ccc3(55, 167, 173), ccc3(232, 52, 24), ccc3(109, 43, 17));
				GameManager::get()->loadBackground(16);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_16_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(139, 180, 175));
						}
					}
				}
				break;
			}
			case GauntletType::Dragon: { // 20
				editGauntletSingle("Dragon", ccc3(46, 0, 0), ccc3(201, 33, 33), ccc3(92, 35, 35));
				GameManager::get()->loadBackground(30);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_30_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(255, 188, 157));
						}
					}
				}
				break;
			}
			case GauntletType::Water: { // 21
				editGauntletSingle("Water", ccc3(13, 11, 48), ccc3(78, 212, 230), ccc3(18, 80, 151));
				GameManager::get()->loadBackground(2);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_02_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(175, 210, 255));
						}
					}
				}
				break;
			}
			case GauntletType::Haunted: { // 22
				editGauntletSingle("Haunted", ccc3(28, 36, 33), ccc3(255, 255, 110), ccc3(72, 45, 54));
				GameManager::get()->loadBackground(16);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_16_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(255, 250, 202));
						}
					}
				}
				break;
			}
			case GauntletType::Acid: { // 23
				editGauntletSingle("Acid", ccc3(45, 38, 17), ccc3(208, 255, 0), ccc3(162, 255, 24));
				GameManager::get()->loadBackground(24);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_24_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(209, 255, 207));
						}
					}
				}
				break;
			}
			case GauntletType::Witch: { // 24
				editGauntletSingle("Witch", ccc3(0, 44, 25), ccc3(66, 99, 125), ccc3(35, 13, 46));
				GameManager::get()->loadBackground(26);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_26_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(170, 184, 196));
						}
					}
				}
				break;
			}
			case GauntletType::Power: { // 25
				editGauntletSingle("Power", ccc3(72, 58, 42), ccc3(206, 174, 149), ccc3(116, 87, 56));
				GameManager::get()->loadBackground(38);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_38_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(226, 212, 164));
						}
					}
				}
				break;
			}
			case GauntletType::Potion: { // 26
				editGauntletSingle("Potion", ccc3(58, 30, 52), ccc3(255, 0, 144), ccc3(139, 0, 148));
				GameManager::get()->loadBackground(25);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_25_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(246, 209, 255));
						}
					}
				}
				break;
			}
			case GauntletType::Snake: { // 27
				editGauntletSingle("Snake", ccc3(66, 31, 114), ccc3(209, 167, 107), ccc3(110, 57, 34));
				GameManager::get()->loadBackground(21);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_21_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(189, 164, 128));
						}
					}
				}
				break;
			}
			case GauntletType::Toxic: { // 28
				editGauntletSingle("Toxic", ccc3(41, 4, 43), ccc3(0, 248, 0), ccc3(9, 87, 8));
				GameManager::get()->loadBackground(31);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_31_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(165, 255, 153));
						}
					}
				}
				break;
			}
			case GauntletType::Halloween: { // 29
				editGauntletSingle("Halloween", ccc3(39, 36, 12), ccc3(255, 100, 3), ccc3(173, 34, 34));
				GameManager::get()->loadBackground(16);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_16_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(255, 204, 145));
						}
					}
				}
				break;
			}
			case GauntletType::Treasure: { // 30
				editGauntletSingle("Treasure", ccc3(29, 39, 33), ccc3(249, 129, 48), ccc3(136, 39, 0));
				GameManager::get()->loadBackground(42);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_42_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(255, 239, 170));
						}
					}
				}
				break;
			}
			case GauntletType::Ghost: { // 31
				editGauntletSingle("Ghost", ccc3(41, 41, 41), ccc3(226, 226, 226), ccc3(153, 153, 153));
				GameManager::get()->loadBackground(16);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_16_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(216, 216, 216));
						}
					}
				}
				break;
			}
			case GauntletType::Spider: { // 32
				editGauntletSingle("Spider", ccc3(44, 52, 56), ccc3(141, 192, 209), ccc3(75, 66, 91));
				GameManager::get()->loadBackground(23);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_23_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(219, 181, 255));
						}
					}
				}
				break;
			}
			case GauntletType::Gem: { // 33
				editGauntletSingle("Gem", ccc3(39, 38, 66), ccc3(201, 246, 229), ccc3(188, 129, 220));
				GameManager::get()->loadBackground(14);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_14_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(207, 255, 255));
						}
					}
				}
				break;
			}
			case GauntletType::Inferno: { // 34
				editGauntletSingle("Inferno", ccc3(70, 24, 54), ccc3(255, 223, 67), ccc3(208, 58, 0));
				GameManager::get()->loadBackground(21);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_21_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(252, 174, 110));
						}
					}
				}
				break;
			}
			case GauntletType::Portal: { // 35
				editGauntletSingle("Portal", ccc3(31, 49, 77), ccc3(255, 50, 140), ccc3(109, 34, 86));
				GameManager::get()->loadBackground(57);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_57_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(255, 196, 225));
						}
					}
				}
				break;
			}
			case GauntletType::Strange: { // 36
				editGauntletSingle("Strange", ccc3(42, 52, 3), ccc3(203, 134, 255), ccc3(112, 59, 117));
				GameManager::get()->loadBackground(17);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_17_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(225, 184, 226));
						}
					}
				}
				break;
			}
			case GauntletType::Fantasy: { // 37
				editGauntletSingle("Fantasy", ccc3(49, 57, 24), ccc3(185, 255, 92), ccc3(73, 126, 38));
				GameManager::get()->loadBackground(18);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_18_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(212, 255, 143));
						}
					}
				}
				break;
			}
			case GauntletType::Christmas: { // 38
				editGauntletSingle("Christmas", ccc3(19, 43, 68), ccc3(158, 255, 250), ccc3(68, 118, 133));
				GameManager::get()->loadBackground(29);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_29_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(192, 245, 240));
						}
					}
				}
				break;
			}
			case GauntletType::Surprise: { // 39
				editGauntletSingle("Surprise", ccc3(42, 52, 3), ccc3(213, 52, 40), ccc3(100, 41, 18));
				GameManager::get()->loadBackground(46);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_46_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(233, 153, 117));
						}
					}
				}
				break;
			}
			case GauntletType::Mystery: { // 40
				editGauntletSingle("Mystery", ccc3(75, 51, 1), ccc3(255, 233, 23), ccc3(255, 146, 0));
				GameManager::get()->loadBackground(49);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_49_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(255, 223, 80));
						}
					}
				}
				break;
			}
			case GauntletType::Cursed: { // 41
				editGauntletSingle("Cursed", ccc3(46, 14, 43), ccc3(81, 255, 0), ccc3(8, 121, 4));
				GameManager::get()->loadBackground(37);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_37_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(147, 255, 133));
						}
					}
				}
				break;
			}
			case GauntletType::Cyborg: { // 42
				editGauntletSingle("Cyborg", ccc3(50, 50, 50), ccc3(182, 228, 237), ccc3(63, 88, 123));
				GameManager::get()->loadBackground(45);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_45_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(187, 212, 212));
						}
					}
				}
				break;
			}
			case GauntletType::Castle: { // 43
				editGauntletSingle("Castle", ccc3(50, 67, 87), ccc3(190, 214, 230), ccc3(105, 119, 136));
				GameManager::get()->loadBackground(19);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_19_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(221, 203, 179));
						}
					}
				}
				break;
			}
			case GauntletType::Grave: { // 44
				editGauntletSingle("Grave", ccc3(23, 32, 31), ccc3(176, 195, 207), ccc3(80, 66, 87));
				GameManager::get()->loadBackground(16);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_16_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(175, 225, 160));
						}
					}
				}
				break;
			}
			case GauntletType::Temple: { // 45
				editGauntletSingle("Temple", ccc3(51, 59, 21), ccc3(255, 226, 184), ccc3(116, 93, 63));
				GameManager::get()->loadBackground(30);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_30_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(226, 215, 163));
						}
					}
				}
				break;
			}
			case GauntletType::World: { // 46
				editGauntletSingle("World", ccc3(79, 80, 130), ccc3(122, 233, 31), ccc3(54, 109, 33));
				GameManager::get()->loadBackground(27);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_27_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(175, 255, 166));
						}
					}
				}
				break;
			}
			case GauntletType::Galaxy: { // 47
				editGauntletSingle("Galaxy", ccc3(40, 26, 44), ccc3(255, 180, 255), ccc3(181, 9, 149));
				GameManager::get()->loadBackground(21);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_21_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);
				m_backgroundSprite->setPositionY(-250);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(255, 151, 243));
						}
					}
				}
				break;
			}
			case GauntletType::Universe: { // 48
				editGauntletSingle("Universe", ccc3(36, 70, 105), ccc3(255, 252, 104), ccc3(255, 1, 136));
				GameManager::get()->loadBackground(32);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_32_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);
				m_backgroundSprite->setPositionY(-275);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(255, 197, 103));
						}
					}
				}
				break;
			}
			case GauntletType::Discord: { // 49
				editGauntletSingle("Discord", ccc3(44, 47, 51), ccc3(88, 101, 212), ccc3(35, 41, 99));
				GameManager::get()->loadBackground(10);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_10_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
				auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1, i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(145, 175, 255));
						}
						for (int c = 0; c < 5; c++) {
							auto sprite = static_cast<CCSprite*>(level->getChildByTag(c + 1));
							
							if (sprite) {
								CCArray* children = sprite->getChildren();
								
								if (children && children->count() > 0) {
									
									auto swapSprite = static_cast<CCSprite*>(children->objectAtIndex(0));

									auto updatedSprite = Mod::get()->getSettingValue<bool>("update-discord-sprite");
									if (!updatedSprite) {
										// :3
									} else {
										swapSprite->setDisplayFrame(CCSprite::create("island_discord_002.png"_spr)->displayFrame());
									}
								}
							}				
						}
					}
				}
				break;
			}
			case GauntletType::Split: { // 50
				editGauntletSingle("Split", ccc3(49, 50, 65), ccc3(255, 110, 237), ccc3(113, 60, 107));
				GameManager::get()->loadBackground(9);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_09_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(255, 155, 240));
						}
					}
				}
				break;
			}
			case GauntletType::NCS: { // 51
				editGauntletSingle("NCS I", ccc3(16, 131, 131), ccc3(32, 254, 254), ccc3(9, 71, 71));
				GameManager::get()->loadBackground(21);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_21_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);
				m_backgroundSprite->setPositionY(-250);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(170, 245, 255));
						}
					}
				}
				break;
			}
			case GauntletType::NCS2: { // 52
				editGauntletSingle("NCS II", ccc3(136, 81, 136), ccc3(255, 152, 255), ccc3(80, 47, 80));
				GameManager::get()->loadBackground(21);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_21_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);
				m_backgroundSprite->setPositionY(-250);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(255, 160, 250));
						}
					}
				}
				break;
			}
			case GauntletType::Space: { // 53
				editGauntletSingle("Space", ccc3(38, 79, 87), ccc3(119, 188, 235), ccc3(19, 77, 145));
				GameManager::get()->loadBackground(13);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_13_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(128, 187, 229));
						}
					}
				}
				break;
			}
			case GauntletType::Cosmos: { // 54
				editGauntletSingle("Cosmos", ccc3(53, 36, 20), ccc3(252, 226, 128), ccc3(254, 84, 0));
				GameManager::get()->loadBackground(13);
				auto bgTexture = CCTextureCache::sharedTextureCache()->textureForKey("game_bg_13_001.png");
				this->m_backgroundSprite->setTexture(bgTexture);

				for (int i = 0; i < 5; i++) {
					auto level = getChildByIDRecursive(fmt::format("level-{}", i + 1));
					if (level) {
						auto name = static_cast<CCLabelBMFont*>(level->getChildByIDRecursive("level-name"));
						if (name) {
							name->setColor(ccc3(255, 208, 80));
						}
					}
				}
				break;
			}
		}

		if (this->m_levels != nullptr && !m_fields->m_loaded)
			editGauntlets();

		return true;
	}

	void loadLevelsFinished(CCArray* p0, char const* p1, int p2) {
		GauntletLayer::loadLevelsFinished(p0, p1, p2);

		#ifdef GEODE_IS_WINDOWS
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

		if (m_fields->m_loaded) return;

		editGauntlets();
	}

	void editGauntlets() {

	auto pathParent = CCNode::create();
	pathParent->setID("gauntlet-path");
	this->addChild(pathParent);

	std::vector<CCSprite*> pathVector = {};

	for (int d = 0; d < 32; d++) {
		auto dot = this->getChildByType<CCSprite>(d + 2);
		if (!dot) continue;
		dot->setID(fmt::format("path-{}", d + 1));
		pathVector.push_back(dot);
	}
	for (auto dot : pathVector) {
		dot->retain();
		dot->removeFromParentAndCleanup(false);
		pathParent->addChild(dot);
	}

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto levelMenuPos = static_cast<CCMenu*>(this->getChildByIDRecursive("levels-menu"));
	if (!levelMenuPos) return;
		levelMenuPos->setPosition(0, 0);
	
	auto levelOne = static_cast<CCSprite*>(this->getChildByIDRecursive("level-1"));
	levelOne->setPositionX(winSize.width / 2 - 180);
	levelOne->setPositionY(winSize.height / 2 - 70);

	auto levelTwo = static_cast<CCSprite*>(this->getChildByIDRecursive("level-2"));
	levelTwo->setPositionX(winSize.width / 2 - 90);
	levelTwo->setPositionY(winSize.height / 2 + 40);;

	auto levelThree = static_cast<CCSprite*>(this->getChildByIDRecursive("level-3"));
	levelThree->setPositionX(winSize.width / 2);
	levelThree->setPositionY(winSize.height / 2 - 70);

	auto levelFour = static_cast<CCSprite*>(this->getChildByIDRecursive("level-4"));
	levelFour->setPositionX(winSize.width / 2 + 90);
	levelFour->setPositionY(winSize.height / 2 + 40);

	auto levelFive = static_cast<CCSprite*>(this->getChildByIDRecursive("level-5"));
	levelFive->setPositionX(winSize.width / 2 + 180);
	levelFive->setPositionY(winSize.height / 2 - 70);
	
	for (int g = 0; g < 5; g++) {
		auto gauntletLevel = getChildByIDRecursive(fmt::format("level-{}", g + 1));
		if (gauntletLevel) {
			for (int i = 0; i < 5; i++) {
				auto sprite = static_cast<CCSprite*>(gauntletLevel->getChildByTag(i + 1));
				if (sprite) {
					sprite->setID(fmt::format("gauntlet-island-{}", g + 1));
					CCArray* children = sprite->getChildren();
					if (children && children->count() > 0) {
						auto shadow = static_cast<CCSprite*>(children->objectAtIndex(2));
						auto island = static_cast<CCSprite*>(children->objectAtIndex(0));
						auto stats = static_cast<CCSprite*>(children->objectAtIndex(1));
						if (shadow) {
							shadow->setID("gauntlet-shadow");
						}
						if (island) {
							island->setID("gauntlet-level");
						}
						if (stats) {
							stats->setID("gauntlet-stats");
							CCArray* statsChildren = stats->getChildren();
							if (statsChildren && statsChildren->count() > 0) {
								auto starLabel = static_cast<CCLabelBMFont*>(statsChildren->objectAtIndex(1));
								auto starPos = static_cast<CCSprite*>(statsChildren->objectAtIndex(2));
								bool hasCompletedLevel = GameStatsManager::sharedState()->hasCompletedLevel(static_cast<GJGameLevel*>(m_levels->objectAtIndex(g)));
								if (starPos) {
									starPos->setID("star-icon");
									starPos->setAnchorPoint(ccp(0.5, 0.5));
									starPos->setPositionX(5);
								}
								if (starLabel) {
									starLabel->setID("star-label");
									if (hasCompletedLevel) {
										starLabel->setColor(ccc3(255, 255, 50));
										auto star = CCParticleSystemQuad::create();
										if (star) {
											CCParticleSystemQuad* starParticles = GameToolbox::particleFromString("30a-1a2a0a8a180a180a0a0a25a50a0a5a-8a0a0a10a5a0a0a0a1a0a1a0a0.25a0a1a0.05a0a0a0a0a1a0a1a0a1a0a0a0a0a0a0.35a0a0a0a20a0a0a0a1a2a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0", NULL, false);
											starParticles->setZOrder(1);
											starParticles->setPositionX(starPos->getPositionX());
											starParticles->setPositionY(starPos->getPositionY());
											stats->addChild(starParticles);
										}
									}
								}
								auto nameModify = static_cast<CCLabelBMFont*>(statsChildren->objectAtIndex(0));
								if (nameModify) {
									nameModify->setID("level-name");
									nameModify->setPositionY(-20);
								}
							}
						}
					}
				}
			}
		}
	}

	m_fields->m_loaded = true;
	log::debug("Opened Gauntlet #{}", static_cast<int>(m_gauntletType));
	
	}

	// void editGauntletSingle(std::string title, ccColor3B bgColor, ccColor3B titleColor, ccColor3B highlightColor) {
	
	// 	auto winSize = CCDirector::sharedDirector()->getWinSize();
	// 	auto director = CCDirector::sharedDirector();

	// 	auto gauntletOutlineText = CCLabelBMFont::create(std::string(title + " Gauntlet").c_str(), "GR_OxygeneFontOutline.fnt"_spr);
	// 	auto gauntletHighlightText = CCLabelBMFont::create(std::string(title + " Gauntlet").c_str(), "GR_OxygeneFontHighlight.fnt"_spr);

	// 	auto gauntletTitleText = static_cast<CCLabelBMFont*>(this->getChildByID("title-shadow"));
	// 	if (!gauntletTitleText) return;
	// 	auto gauntletShadowText = static_cast<CCLabelBMFont*>(this->getChildByID("title"));
	// 	if (!gauntletShadowText) return;

	// 	auto cornerParent = CCNode::create();
	// 	cornerParent->setID("corners");
	// 	cornerParent->setZOrder(-1);
	// 	this->addChild(cornerParent);
		
	// 	auto TLCornerParent = CCNode::create();
	// 	TLCornerParent->setID("top-left");
	// 	TLCornerParent->setPositionY(director->getScreenTop());
	// 	cornerParent->addChild(TLCornerParent);
		
	// 	auto TRCornerParent = CCNode::create();
	// 	TRCornerParent->setID("top-right");
	// 	TRCornerParent->setPositionY(director->getScreenTop());
	// 	cornerParent->addChild(TRCornerParent);

	// 	// Left Corner
	// 	auto TLCorner = CCSprite::create("GR_gauntletCorner_001.png"_spr);
	// 	TLCorner->setID("TL-corner");
	// 	TLCorner->setAnchorPoint(ccp(0, 0));
	// 	TLCorner->setPosition(ccp(director->getScreenLeft(), director->getScreenBottom()));
	// 	TLCorner->setScaleY(TLCorner->getScaleY() * -1);
	// 	TLCornerParent->addChild(TLCorner);
		
	// 	auto TLCornerColor = CCSprite::create("GR_gauntletCornerColor_002.png"_spr);
	// 	TLCornerColor->setID("TL-color");
	// 	TLCornerColor->setAnchorPoint(ccp(0, 0));
	// 	TLCornerColor->setZOrder(1);
	// 	TLCornerColor->setPosition(ccp(director->getScreenLeft(), director->getScreenBottom()));
	// 	TLCornerColor->setScaleY(TLCornerColor->getScaleY() * -1);
	// 	TLCornerParent->addChild(TLCornerColor);

	// 	auto TLCornerHighlight = CCSprite::create("GR_gauntletCornerHighlight_001.png"_spr);
	// 	TLCornerHighlight->setID("TL-highlight");
	// 	TLCornerHighlight->setAnchorPoint(ccp(0, 0));
	// 	TLCornerHighlight->setZOrder(1);
	// 	TLCornerHighlight->setPosition(ccp(director->getScreenLeft(), director->getScreenBottom()));
	// 	TLCornerHighlight->setScaleY(TLCornerHighlight->getScaleY() * -1);
	// 	TLCornerParent->addChild(TLCornerHighlight);
		
	// 	// Right Corner
	// 	auto TRCorner = CCSprite::create("GR_gauntletCorner_001.png"_spr);
	// 	TRCorner->setID("TR-corner");
	// 	TRCorner->setAnchorPoint(ccp(0, 0));
	// 	TRCorner->setPosition(ccp(director->getScreenRight(), director->getScreenBottom()));
	// 	TRCorner->setScaleY(TRCorner->getScaleY() * -1);
	// 	TRCorner->setScaleX(TRCorner->getScaleX() * -1);
	// 	TRCornerParent->addChild(TRCorner);
		
	// 	auto TRCornerColor = CCSprite::create("GR_gauntletCornerColor_002.png"_spr);
	// 	TRCornerColor->setID("TR-color");
	// 	TRCornerColor->setAnchorPoint(ccp(0, 0));
	// 	TRCornerColor->setZOrder(1);
	// 	TRCornerColor->setPosition(ccp(director->getScreenRight(), director->getScreenBottom()));
	// 	TRCornerColor->setScaleY(TRCornerColor->getScaleY() * -1);
	// 	TRCornerColor->setScaleX(TRCornerColor->getScaleX() * -1);
	// 	TRCornerParent->addChild(TRCornerColor);

	// 	auto TRCornerHighlight = CCSprite::create("GR_gauntletCornerHighlight_001.png"_spr);
	// 	TRCornerHighlight->setID("TR-highlight");
	// 	TRCornerHighlight->setAnchorPoint(ccp(0, 0));
	// 	TRCornerHighlight->setZOrder(1);
	// 	TRCornerHighlight->setPosition(ccp(director->getScreenRight(), director->getScreenBottom()));
	// 	TRCornerHighlight->setScaleY(TRCornerHighlight->getScaleY() * -1);
	// 	TRCornerHighlight->setScaleX(TRCornerHighlight->getScaleX() * -1);
	// 	TRCornerParent->addChild(TRCornerHighlight);

	// 	// Text Highlights
	// 	gauntletHighlightText->setID("title-highlight");
	// 	gauntletHighlightText->setFntFile("GR_OxygeneFontHighlight.fnt"_spr);
	// 	gauntletHighlightText->setScale(0.75);
	// 	gauntletHighlightText->setPositionX(winSize.width / 2 + 2);
	// 	gauntletHighlightText->setPositionY(winSize.height - 30);

	// 	// Text Outlines
	// 	gauntletOutlineText->setID("title-outline");
	// 	gauntletOutlineText->setFntFile("GR_OxygeneFontOutline.fnt"_spr);
	// 	gauntletOutlineText->setScale(0.75);
	// 	gauntletOutlineText->setPositionX(winSize.width / 2 - 1.5);
	// 	gauntletOutlineText->setPositionY(winSize.height - 30);

	// 	// Shadow
	// 	gauntletShadowText->setString(std::string(title + " Gauntlet").c_str());
		
	// 	// Colors
	// 	m_backgroundSprite->setColor(bgColor);
	// 	gauntletTitleText->setColor(titleColor);
	// 	gauntletHighlightText->setColor(highlightColor);
	// 	TLCornerColor->setColor(titleColor);
	// 	TRCornerColor->setColor(titleColor);
	// 	TLCornerHighlight->setColor(highlightColor);
	// 	TRCornerHighlight->setColor(highlightColor);
		
	// 	this->addChild(gauntletOutlineText);
	// 	this->addChild(gauntletHighlightText);
		
	// 	gauntletHighlightText->setZOrder(37);
	// }
};