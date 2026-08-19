#include "CustomGauntletLayer.hpp"
#include "../GauntletSelectLayer/GauntletSelectLayer.hpp"
#include "../Data/CustomGauntletManager.hpp"
#include "../GauntletLayer/HueLuminanceTo.hpp"
#include "CustomGauntletCompletionPopup.hpp"
#include <Geode/binding/CCSpriteWithHue.hpp>

using namespace geode::prelude;

// Helpers 

static void limitLabel(CCLabelBMFont* lbl, float max, float orig, float minS) {
    if (!lbl) return;
    float w = lbl->getContentSize().width * orig;
    lbl->setScale(w > max ? std::max(max / lbl->getContentSize().width, minS) : orig);
}

// create / scene 

CCScene* CustomGauntletLayer::scene(CustomGauntletData const& data) {
    auto layer = CustomGauntletLayer::create(data);
    if (!layer) return nullptr;
    auto sc = CCScene::create();
    sc->addChild(layer);
    return sc;
}

CustomGauntletLayer* CustomGauntletLayer::create(CustomGauntletData const& data) {
    auto ret = new CustomGauntletLayer();
    if (ret && ret->init(data)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CustomGauntletLayer::init(CustomGauntletData const& data) {
    if (!CCLayer::init()) return false;

    m_data = data;

    setKeypadEnabled(true);

    buildBackground();
    buildCorners();
    buildTitle();

    // Loading circle while levels fetch
    auto loadCircle = LoadingCircle::create();
    loadCircle->setID("loading-circle");
    loadCircle->setPosition(CCDirector::get()->getWinSize() / 2);
    loadCircle->ignoreAnchorPointForPosition(false);
    loadCircle->show();
    addChild(loadCircle, 10);

    // Back button
    auto backMenu = CCMenu::create();
    backMenu->setID("exit-menu");
    backMenu->setPosition({24, CCDirector::get()->getScreenTop() - 23});
    addChild(backMenu, 5);

    auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    auto backBtn = CCMenuItemExt::createSpriteExtra(backSpr,
        [this](CCMenuItemSpriteExtra*) { onBack(nullptr); });
    backMenu->addChild(backBtn);

    // Info button
    auto infoMenu = CCMenu::create();
    infoMenu->setID("gauntlet-info-menu"_spr);
    infoMenu->setPosition({0, 0});
    addChild(infoMenu, 5);

    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    auto infoBtn = CCMenuItemSpriteExtra::create(
        infoSpr, this, menu_selector(CustomGauntletLayer::onInfo));
    infoBtn->setPosition(
        ccp(CCDirector::get()->getScreenLeft() + 30,
            CCDirector::get()->getScreenBottom() + 30));
    infoMenu->addChild(infoBtn);

    // Debug-only: preview the completion popup/reward animation without
    // touching real CustomGauntletManager claim/reward state.
    if (Mod::get()->getSettingValue<bool>("debug-reward-button")) {
        auto debugBtnSpr = ButtonSprite::create("Debug", "goldFont.fnt", "GJ_button_04.png", 0.75);
        auto debugBtn = CCMenuItemSpriteExtra::create(
            debugBtnSpr, this, menu_selector(CustomGauntletLayer::onDebugReward));
        debugBtn->setScale(0.5);
        debugBtn->m_baseScale = 0.5;
        debugBtn->setPosition(
            ccp(CCDirector::get()->getScreenRight() - 45, CCDirector::get()->getScreenBottom() + 30));

        auto debugMenu = CCMenu::create();
        debugMenu->setPosition(0, 0);
        debugMenu->addChild(debugBtn);
        debugMenu->setID("gauntlet-debug-menu"_spr);
        this->addChild(debugMenu);
    }

    // Load island icon async
    loadIslandIcon();

    // Kick off level fetch
    GameLevelManager::get()->m_levelManagerDelegate = this;
    loadLevels();

    return true;
}

// Background 

void CustomGauntletLayer::buildBackground() {
    auto winSize = CCDirector::get()->getWinSize();

    // Solid BG colour
    auto bg = CCSprite::create(
        fmt::format("game_bg_{:02d}_001.png", m_data.bgIndex).c_str());
    if (!bg) bg = CCSprite::create("game_bg_01_001.png");

    if (bg) {
        bg->setID("background");
        bg->setColor(m_data.bgColor);
        bg->setZOrder(-99);
        bg->setPosition({0, -20});
        bg->setAnchorPoint({0, 0});
        float scaleX = winSize.width  / bg->getContentWidth();
        float scaleY = winSize.height / bg->getContentHeight();
        bg->setScale(std::max(scaleX, scaleY));
        addChild(bg);
    }

    // Floor line
    auto floorLine = CCSprite::createWithSpriteFrameName("floorLine_001.png");
    if (floorLine) {
        floorLine->setID("floor-line");
        floorLine->setScaleX(0.85);
        floorLine->setOpacity(100);
        floorLine->setPosition({winSize.width / 2, winSize.height - 55});
        this->addChild(floorLine);
    }
}

// Corner builder

void CustomGauntletLayer::buildCorners() {
    auto dir = CCDirector::get();

    auto makeCorner = [&](
        float x,
        float y,
        bool flipX,
        bool flipY,
        ccColor3B color,
        ccColor3B highlight
    ) {

        auto parent = CCNode::create();
        parent->setPosition({x, y});
        addChild(parent, 1);

        auto cornerBase = CCSprite::createWithSpriteFrameName("GR_gauntletCorner_001.png"_spr);
        cornerBase->setAnchorPoint({0, 0});
        if (flipX) cornerBase->setScaleX(cornerBase->getScaleX() * -1);
        if (flipY) cornerBase->setScaleY(cornerBase->getScaleY() * -1);
        parent->addChild(cornerBase);

        auto color1 = CCSprite::createWithSpriteFrameName("GR_gauntletCornerColor_001.png"_spr);
        color1->setAnchorPoint({0, 0});
        color1->setZOrder(1);
        color1->setColor(color);
        if (flipX) color1->setScaleX(color1->getScaleX() * -1);
        if (flipY) color1->setScaleY(color1->getScaleY() * -1);
        parent->addChild(color1);

        auto color2 = CCSprite::createWithSpriteFrameName("GR_gauntletCornerHighlight_001.png"_spr);
        color2->setAnchorPoint({0, 0});
        color2->setZOrder(1);
        color2->setColor(highlight);
        if (flipX) color2->setScaleX(color2->getScaleX() * -1);
        if (flipY) color2->setScaleY(color2->getScaleY() * -1);
        parent->addChild(color2);
    };

    // Top-left
    makeCorner(
        dir->getScreenLeft() - 1,
        dir->getScreenTop() + 1,
        false,
        true,
        m_data.accentColor1,
        m_data.accentColor2
    );

    // Top-right
    makeCorner(
        dir->getScreenRight() + 1,
        dir->getScreenTop() + 1,
        true,
        true,
        m_data.accentColor1,
        m_data.accentColor2
    );
}

// Title builder

void CustomGauntletLayer::buildTitle() {
    auto winSize = CCDirector::get()->getWinSize();
    std::string title = "The " + m_data.name + " Gauntlet";

    auto titleNode = CCNode::create();
    titleNode->setPosition({winSize.width / 2 + 1, winSize.height - 32.5f});
    titleNode->setID("title-node");
    this->addChild(titleNode);

    // Main title
    m_titleLabel = CCLabelBMFont::create(title.c_str(), "GR_OxygeneFont.fnt"_spr);
    m_titleLabel->setID("title");
    m_titleLabel->setScale(0.75);
    m_titleLabel->setColor(m_data.nameColor);
    m_titleLabel->setPosition({winSize.width / 2, winSize.height - 30});
    m_titleLabel->setZOrder(37);
    this->addChild(m_titleLabel);

    // Highlight
    auto highlight = CCLabelBMFont::create(title.c_str(), "GR_OxygeneFontHighlight.fnt"_spr);
    highlight->setID("title-highlight");
    highlight->setScale(0.75);
    highlight->setColor(m_data.accentColor2);
    highlight->setPosition({m_titleLabel->getPositionX() + 2, winSize.height - 30});
    highlight->setZOrder(38);
    this->addChild(highlight);

    // Outline
    auto outline = CCLabelBMFont::create(title.c_str(), "GR_OxygeneFontOutline.fnt"_spr);
    outline->setID("title-outline");
    outline->setScale(0.75);
    outline->setColor({ 255, 255, 255 });
    outline->setPosition(m_titleLabel->getPositionX() - 1.5, m_titleLabel->getPositionY());
    outline->setZOrder(36);
    this->addChild(outline);

    // Shadow
    auto shadow = CCLabelBMFont::create(title.c_str(), "GR_OxygeneFontOutline.fnt"_spr);
    shadow->setID("title-shadow");
    shadow->setScale(0.75);
    shadow->setColor({0, 0, 0});
    shadow->setOpacity(100);
    shadow->setPosition({outline->getPositionX() + 3.5f, outline->getPositionY() - 3.5f});
    shadow->setZOrder(35);
    this->addChild(shadow);
}

// Icon builder

void CustomGauntletLayer::loadIslandIcon() {
    if (m_data.iconURL.empty()) return;

    m_iconHolder.spawn(
        web::WebRequest().get(m_data.iconURL),
        [this](web::WebResponse res) {
            if (!res.ok()) return;
            auto bytes = res.data();
            Ref<CustomGauntletLayer> self(this);
            queueInMainThread([self, bytes]() {
                if (!self->getParent()) return;

                auto img = new CCImage();
                if (!img->initWithImageData(
                        const_cast<unsigned char*>(bytes.data()), bytes.size())) {
                    delete img; return;
                }
                auto tex = new CCTexture2D();
                tex->initWithImage(img);
                delete img;

                if (self->m_levelsMenu) {
                    for (int i = 1; i <= 5; i++) {
                        auto btn = self->m_levelsMenu->getChildByID(
                            fmt::format("level-{}", i).c_str());
                        if (!btn) continue;
                        auto levelSpr = static_cast<CCNode*>(
                            static_cast<CCMenuItemSpriteExtra*>(btn)->getNormalImage());
                        if (auto island = static_cast<CCSprite*>(
                                levelSpr->getChildByID(fmt::format("island-{}", i).c_str()))) {
                            island->setTexture(tex);
                        }
                        if (auto shadow = static_cast<CCSprite*>(
                                levelSpr->getChildByID(
                                    fmt::format("island-{}-shadow", i).c_str()))) {
                            shadow->setTexture(tex);
                        }
                    }
                }
                tex->release();
            });
        }
    );
}

// Level loader

void CustomGauntletLayer::loadLevels() {
    std::string idList;
    for (int i = 0; i < 5; i++) {
        if (m_data.levels[i].id == 0) continue;
        if (!idList.empty()) idList += ",";
        idList += std::to_string(m_data.levels[i].id);
    }
    if (idList.empty()) return;

    auto searchObj = GJSearchObject::create(SearchType::Type19, gd::string(idList));
    GameLevelManager::get()->getOnlineLevels(searchObj);
}

void CustomGauntletLayer::loadLevelsFinished(CCArray* levels, char const*) {
    auto glm = GameLevelManager::get();
    if (glm->m_levelManagerDelegate == this)
        glm->m_levelManagerDelegate = nullptr;

    if (auto lc = getChildByIDRecursive("loading-circle")) lc->removeFromParent();

    CCArray* ordered = CCArray::createWithCapacity(5);
    for (int i = 0; i < 5; i++) {
        int wantId = m_data.levels[i].id;
        if (wantId == 0) { ordered->addObject(CCInteger::create(0)); continue; }
        GJGameLevel* found = nullptr;
        for (auto* obj : CCArrayExt<GJGameLevel*>(levels)) {
            if ((int)obj->m_levelID == wantId) { found = obj; break; }
        }
        if (found) {
            found->m_gauntletLevel2 = true;
            ordered->addObject(found);
        }
        else ordered->addObject(CCInteger::create(0));
    }

    m_loadedLevels = ordered;
    m_loadedLevels->retain();

    buildLevelButtons(ordered);
    loadIslandIcon();
}

void CustomGauntletLayer::loadLevelsFailed(char const*, int) {
    auto glm = GameLevelManager::get();
    if (glm->m_levelManagerDelegate == this)
        glm->m_levelManagerDelegate = nullptr;

    if (auto lc = getChildByIDRecursive("loading-circle")) lc->removeFromParent();

    Notification::create("Failed to load gauntlet levels.", NotificationIcon::Error)->show();
}

// Level buttons

void CustomGauntletLayer::buildLevelButtons(CCArray* levels) {
    auto winSize = CCDirector::get()->getWinSize();

    m_levelsMenu = CCMenu::create();
    m_levelsMenu->setID("level-array"_spr);
    m_levelsMenu->setPosition({0, -14});
    addChild(m_levelsMenu, 4);

    for (int i = 0; i < 5; i++) {
        auto& slotData = m_data.levels[i];
        auto obj = levels->objectAtIndex(i);
        auto level = typeinfo_cast<GJGameLevel*>(obj);
        bool hasCompleted = CustomGauntletManager::get()->isLevelRewardClaimed(slotData.id);
        bool isLocked = false;
        if (i > 0) {
            auto& prevSlotData = m_data.levels[i - 1];
            isLocked = !CustomGauntletManager::get()->isLevelRewardClaimed(prevSlotData.id);
        }
        m_lockedStates.push_back(isLocked);

        // Island Node
        auto levelSpr = CCNode::create();
        levelSpr->setContentSize({70, 80});
        levelSpr->setAnchorPoint({0.5, 0.5});
        levelSpr->setID(fmt::format("gauntlet-{}", i + 1).c_str());

        // Sprite PH
        auto islandSpr = CCSpriteWithHue::create("GR_unknownGauntlet_001.png"_spr);
        if (!islandSpr) continue; // bundled resource, should never actually fail to load
        islandSpr->setID(fmt::format("island-{}", i + 1).c_str());
        islandSpr->setPosition(levelSpr->getContentSize() / 2);
        if (isLocked) islandSpr->setColor({128, 128, 128});
        levelSpr->addChild(islandSpr);

        auto shadowSpr = CCSprite::create("GR_unknownGauntlet_001.png"_spr);
        if (!shadowSpr) shadowSpr = CCSprite::create();
        shadowSpr->setID(fmt::format("island-{}-shadow", i + 1).c_str());
        shadowSpr->setColor({0, 0, 0});
        shadowSpr->setOpacity(75);
        shadowSpr->setScaleY(1.2);
        shadowSpr->setPosition({islandSpr->getPositionX(), islandSpr->getPositionY() - 10});
        levelSpr->addChild(shadowSpr, -1);

        // Info stats
        std::string levelName    = level ? std::string(level->m_levelName)    : slotData.name;
        std::string creatorName  = level ? std::string(level->m_creatorName)  : slotData.creator;
        int         crystalCount = CustomGauntletManager::crystalsForLevel(level);

        auto nameLabel = CCLabelBMFont::create(levelName.c_str(), "bigFont.fnt");
        nameLabel->setID("level-name"_spr);
        limitLabel(nameLabel, 120, 0.4, 0.3);
        nameLabel->setPosition({islandSpr->getPositionX(), islandSpr->getPositionY() - 10});
        if (isLocked) nameLabel->setVisible(false);
        levelSpr->addChild(nameLabel);

        auto authorLabel = CCLabelBMFont::create(creatorName.c_str(), "goldFont.fnt");
        authorLabel->setID("creator-name"_spr);
        limitLabel(authorLabel, 120, 0.4, 0.25);
        authorLabel->setAlignment(kCCTextAlignmentCenter);
        authorLabel->setPosition({nameLabel->getPositionX(), nameLabel->getPositionY() - 10});
        if (isLocked) authorLabel->setVisible(false);
        levelSpr->addChild(authorLabel);

        // Crystals row
        auto crystalNode = CCNode::create();
        crystalNode->setID("crystal-node"_spr);
        crystalNode->setScale(0.5);
        crystalNode->setAnchorPoint({0.5, 0.5});
        crystalNode->setPosition({nameLabel->getPositionX(), nameLabel->getPositionY() - 27.5f});
        crystalNode->setLayout(RowLayout::create()
            ->setGap(5)->setAutoGrowAxis(true)
            ->setAxisAlignment(AxisAlignment::Center));

        auto crystalCountLabel = CCLabelBMFont::create(fmt::format("{}", crystalCount).c_str(), "bigFont.fnt");
        crystalCountLabel->setID("crystal-count"_spr);
        crystalCountLabel->setScale(0.65);
        if (hasCompleted) crystalCountLabel->setColor({255, 255, 50});

        auto crystalSpr = CCSprite::create("GR_crystal_001.png"_spr);
        crystalSpr->setID("crystal-icon"_spr);
        crystalSpr->setAnchorPoint({0.5, 0.5});
        crystalSpr->setScale(0.45);

        crystalNode->addChild(crystalCountLabel);
        crystalNode->addChild(crystalSpr);
        crystalNode->updateLayout();
        if (isLocked) crystalNode->setVisible(false);
        levelSpr->addChild(crystalNode);

        // Completion checkmark
        if (hasCompleted) {
            auto check = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
            check->setID("checkmark-icon"_spr);
            check->setAnchorPoint({0.5, 0.5});
            check->setPosition({islandSpr->getPositionX() + 15, islandSpr->getPositionY() + 15});
            levelSpr->addChild(check);
        }

        // Lock icon
        if (isLocked) {
            auto lockSpr = CCSprite::createWithSpriteFrameName("GJ_lock_001.png");
            lockSpr->setID("gauntlet-lock"_spr);
            lockSpr->setPosition({islandSpr->getPositionX(), islandSpr->getPositionY() - 15});
            levelSpr->addChild(lockSpr);
        }

        auto btn = CCMenuItemSpriteExtra::create(
            levelSpr, this,
            isLocked
                ? menu_selector(CustomGauntletLayer::onLocked)
                : menu_selector(CustomGauntletLayer::onLevel)
        );
        if (level) btn->setUserObject(level);
        btn->setTag(i);
        btn->setID(fmt::format("level-{}", i + 1).c_str());

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
    }

    // Vault
    auto dir = CCDirector::sharedDirector();

    auto vaultMenu = CCMenu::create();
    vaultMenu->setID("level-vault-menu"_spr);
    vaultMenu->setPosition({0, 0});
    this->addChild(vaultMenu, 5);

    auto vaultBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_safeBtn_001.png"),
        this, menu_selector(CustomGauntletLayer::gauntletVault)
    );
    vaultBtn->setPosition(ccp(dir->getScreenRight() - 31, dir->getScreenTop() - 30));
    vaultMenu->addChild(vaultBtn);

    if (Mod::get()->getSettingValue<bool>("vault-tooltip")) {
        auto vaultText = CCSprite::createWithSpriteFrameName("vaultText.png"_spr);
        vaultText->setPosition(ccp(vaultBtn->getPositionX() - 52.5, vaultBtn->getPositionY() - 37.5));
        vaultText->setScale(0.45);
        vaultText->setID("vault-text"_spr);
        vaultMenu->addChild(vaultText);
    }

    bool hover = Mod::get()->getSettingValue<bool>("level-hover");
	if (hover) {
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		for (int i = 0; i < 5; i++) {
			CCNode* island = m_levelsMenu->getChildByIDRecursive(fmt::format("gauntlet-{}", i + 1));
			if (!island) continue;

			float randomMoveUp = 2.0 + static_cast<float>(std::rand() % 15) / 10.0;
			float randomMoveDown = 2.0 + static_cast<float>(std::rand() % 15) / 10.0;

			CCMoveBy* moveUp = CCMoveBy::create(randomMoveUp, ccp(0, 5));
			CCMoveBy* moveDown = CCMoveBy::create(randomMoveDown, ccp(0, -5));

			CCEaseInOut* easeMoveUp = CCEaseInOut::create(moveUp, 2.0);
			CCEaseInOut* easeMoveDown = CCEaseInOut::create(moveDown, 2.0);

			CCSequence* hoverSequence = CCSequence::create(easeMoveUp, easeMoveDown, nullptr);

			CCRepeatForever* levelHover = CCRepeatForever::create(hoverSequence);
			island->runAction(levelHover);
		}
	}
}

void CustomGauntletLayer::onEnter() {
    CCLayer::onEnter();
    checkForUnlocks();
    checkPendingReward();
}

void CustomGauntletLayer::checkPendingReward() {
    int coins = 0;
    if (!CustomGauntletManager::get()->consumePendingGauntletReward(m_data.id, coins)) return;

    CCTexture2D* iconTex = nullptr;
    if (auto island = m_levelsMenu ? m_levelsMenu->getChildByIDRecursive("island-1") : nullptr) {
        iconTex = static_cast<CCSprite*>(island)->getTexture();
    }

    if (auto popup = CustomGauntletCompletionPopup::create(m_data, coins, iconTex)) {
        // Direct child, not ->show() - during a scene transition getRunningScene()
        // can still be the transition node, and anything attached to it gets
        // discarded once the transition finishes and swaps scenes.
        this->addChild(popup, 1000);
    }
}

void CustomGauntletLayer::onDebugReward(CCObject*) {
    // Fake coin count for previewing the animation - doesn't touch
    // CustomGauntletManager, so no real claim/reward state is affected.
    CCTexture2D* iconTex = nullptr;
    if (auto island = m_levelsMenu ? m_levelsMenu->getChildByIDRecursive("island-1") : nullptr) {
        iconTex = static_cast<CCSprite*>(island)->getTexture();
    }

    if (auto popup = CustomGauntletCompletionPopup::create(m_data, 250, iconTex)) {
        this->addChild(popup, 1000);
    }
}

void CustomGauntletLayer::checkForUnlocks() {
    if (!m_levelsMenu || m_lockedStates.empty()) return;

    for (int i = 0; i < 5 && i < static_cast<int>(m_lockedStates.size()); i++) {
        if (!m_lockedStates[i]) continue;

        auto& prevSlotData = m_data.levels[i - 1];
        bool stillLocked = !CustomGauntletManager::get()->isLevelRewardClaimed(prevSlotData.id);
        if (stillLocked) continue;

        m_lockedStates[i] = false;
        auto levelSpr = m_levelsMenu->getChildByIDRecursive(fmt::format("gauntlet-{}", i + 1));
        if (levelSpr) playUnlockAnimation(levelSpr, i);
    }
}

constexpr float kUnlockSfxPeakDelay = 1.5f;

void CustomGauntletLayer::playUnlockAnimation(CCNode* levelSpr, int index) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(levelSpr->getParent());
    if (btn) btn->setTarget(this, menu_selector(CustomGauntletLayer::onLevel));

    auto islandSpr = static_cast<CCSpriteWithHue*>(levelSpr->getChildByID(fmt::format("island-{}", index + 1)));
    auto lockSpr = levelSpr->getChildByID("gauntlet-lock"_spr);
    auto nameLabel = static_cast<CCLabelBMFont*>(levelSpr->getChildByID("level-name"_spr));
    auto authorLabel = static_cast<CCLabelBMFont*>(levelSpr->getChildByID("creator-name"_spr));
    auto crystalNode = levelSpr->getChildByID("crystal-node"_spr);

    FMODAudioEngine::sharedEngine()->playEffect("unlockGauntlet.ogg");

    if (lockSpr) {
        lockSpr->runAction(CCSequence::create(
            CCFadeOut::create(kUnlockSfxPeakDelay),
            CCRemoveSelf::create(),
            nullptr
        ));
    }

    playUnlockParticlesIn(levelSpr, index);
    if (islandSpr) islandShake(islandSpr, kUnlockSfxPeakDelay);

    // Name/author/crystal pop in instantly at the sfx peak, no fade.
    for (auto label : { nameLabel, authorLabel }) {
        if (!label) continue;
        label->runAction(CCSequence::create(
            CCDelayTime::create(kUnlockSfxPeakDelay),
            CCShow::create(),
            nullptr
        ));
    }

    if (crystalNode) {
        crystalNode->setScale(0.5);
        crystalNode->runAction(CCSequence::create(
            CCDelayTime::create(kUnlockSfxPeakDelay),
            CCShow::create(),
            nullptr
        ));
    }

    if (islandSpr) {
        islandSpr->setColor(ccc3(128, 128, 128));
        islandSpr->runAction(CCSequence::create(
            CCDelayTime::create(kUnlockSfxPeakDelay),
            CCCallFuncN::create(this, callfuncN_selector(CustomGauntletLayer::onUnlockPeak)),
            CCSpawn::create(
                CCTintTo::create(0.3, 255, 255, 255),
                HueLuminanceTo::create(0.6, 1, 0.),
                CCSequence::create(
                    CCEaseInOut::create(CCScaleTo::create(0.25, 1.35), 2.),
                    CCEaseBackOut::create(CCScaleTo::create(0.35, 1.)),
                    nullptr
                ),
                nullptr
            ),
            nullptr
        ));
    }
}

// Fires at the sfx's "unlock" peak (pSender is the island sprite mid-pulse).
void CustomGauntletLayer::onUnlockPeak(CCNode* sender) {
    auto levelSpr = sender->getParent();
    if (!levelSpr) return;
    auto btn = levelSpr->getParent();
    playUnlockParticlesOut(levelSpr, btn ? btn->getTag() : 0);
}

CCFiniteTimeAction* CustomGauntletLayer::generateShakeAction(CCPoint originalPos, float xyOffset, float duration) {
    float dx = CCRANDOM_MINUS1_1() * xyOffset;
    float dy = CCRANDOM_MINUS1_1() * xyOffset;
    return CCMoveTo::create(duration, {originalPos.x + dx, originalPos.y + dy});
}

void CustomGauntletLayer::islandShake(CCSprite* islandSpr, float duration) {
    constexpr float stepDuration = 0.05f;
    constexpr float xyOffset = 3;

    CCPoint originalPos = islandSpr->getPosition();
    int steps = static_cast<int>(duration / stepDuration);

    auto shakeSteps = CCArray::create();
    for (int i = 0; i < steps; i++) {
        shakeSteps->addObject(generateShakeAction(originalPos, xyOffset, stepDuration));
    }
    shakeSteps->addObject(CCMoveTo::create(stepDuration, originalPos));

    islandSpr->runAction(CCSequence::create(shakeSteps));
}

void CustomGauntletLayer::playUnlockParticlesIn(CCNode* levelSpr, int index) {
    auto particlesIn = GameToolbox::particleFromString(
        "150a1a2a0a75a-180a180a0a0a75a75a0a0a-2000a0a0a0a3a0a0a62a1a0a1a0a1a0a0.35a0.15a0a0a0a87a1a0a1a0a1a0a0.15a0.05a0.2a0a0.5a0.15a75a25a0a0a0a0a0a2a1a0a0a0a0a0a5a0a0a0a0a0a0a0a0a0a0a0a0",
        NULL,
        false
    );
    particlesIn->setPosition(levelSpr->getContentSize() / 2);
    particlesIn->setID(fmt::format("unlock-particles-in-{}", index + 1));
    particlesIn->setAutoRemoveOnFinish(true);
    levelSpr->addChild(particlesIn, -1);
}

void CustomGauntletLayer::playUnlockParticlesOut(CCNode* levelSpr, int index) {
    auto particlesOut = GameToolbox::particleFromString(
        "20a-1a0.75a0a-1a180a180a0a380a0a0a0a0a0a0a0a0a3a0a0a62a1a0a1a0a1a0a0.35a0.15a0a0a0a87a1a0a1a0a1a0a0.15a0.05a0a0a0.5a0.35a0a0a20a15a0a0a0a2a1a0a0a0a0a0a5a0a0a0a0a0a0a0a0a0a3.61a0a0",
        NULL,
        false
    );
    particlesOut->setPosition(levelSpr->getContentSize() / 2);
    particlesOut->setID(fmt::format("unlock-particles-out-{}", index + 1));
    particlesOut->setAutoRemoveOnFinish(true);
    levelSpr->addChild(particlesOut, -1);
}

void CustomGauntletLayer::onBack(CCObject*) {
    if (auto glm = GameLevelManager::get())
        if (glm->m_levelManagerDelegate == this)
            glm->m_levelManagerDelegate = nullptr;
    if (m_loadedLevels) { m_loadedLevels->release(); m_loadedLevels = nullptr; }
    auto scene = BetterGauntletSelectLayer::scene();
    if (scene) {
        CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, scene));
    }
}

void CustomGauntletLayer::onLevel(CCObject* sender) {
    auto btn   = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto level = static_cast<GJGameLevel*>(btn->getUserObject());
    if (!level) return;
    CustomGauntletManager::get()->markPendingGauntletAttempt(level->m_levelID.value());

    auto lil   = LevelInfoLayer::create(level, false);
    auto scene  = CCScene::create();
    scene->addChild(lil);
    CCDirector::get()->pushScene(CCTransitionFade::create(0.5, scene));
}

void CustomGauntletLayer::onLocked(CCObject* sender) {
    auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto levelSpr = static_cast<CCNode*>(button->getNormalImage());
    auto islandSpr = static_cast<CCSprite*>(levelSpr->getChildren()->objectAtIndex(1));
    if (!islandSpr) return;
	
    ccColor3B originalColor = ccc3(128, 128, 128);
    
    auto turnRed = CCTintTo::create(0, 192, 128, 128);
    auto resetColor = CCTintTo::create(0.25, originalColor.r, originalColor.g, originalColor.b);
    
    auto sequence = CCSequence::create(turnRed, resetColor, nullptr);
    islandSpr->runAction(sequence);
}

void CustomGauntletLayer::onInfo(CCObject*) {
    auto title = "The " + m_data.name + " Gauntlet";

    auto popup = MDPopup::create(
        title.c_str(),
        m_data.description.c_str(),
        "OK"
    );

    auto makeLabel = [](std::string const& text, float x, float y, float scale, CCTextAlignment align) {
        auto lbl = CCLabelBMFont::create(text.c_str(), "chatFont.fnt");
        lbl->setPosition(x, y);
        lbl->setScale(scale);
        lbl->setAnchorPoint(align == kCCTextAlignmentLeft
            ? ccp(0, 0) : ccp(1, 0));
        lbl->setOpacity(51);
        return lbl;
    };

    auto children = popup->getChildren();
    if (children && children->count() > 0) {
        auto child = static_cast<CCNode*>(children->objectAtIndex(0));
        if (!m_data.infoDate.empty())
            child->addChild(makeLabel(
            "Released on: " + m_data.infoDate, 
            383.5, 
            25, 
            0.5, 
            kCCTextAlignmentRight
        ));
        if (!m_data.infoVersion.empty())
            child->addChild(makeLabel(
            "Version: " + m_data.infoVersion, 
            383.5, 
            15, 
            0.5, 
            kCCTextAlignmentRight
        ));
        if (!m_data.infoSuggester.empty())
            child->addChild(makeLabel(
            "Suggested by: " + m_data.infoSuggester, 
            15.5, 
            15, 
            0.5, 
            kCCTextAlignmentLeft
        ));
    }

    popup->show();
}

void CustomGauntletLayer::gauntletVault(CCObject*) {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    std::string idList;
    for (auto const& slot : m_data.levels) {
        if (slot.id == 0) continue;
        if (!idList.empty()) idList += ",";
        idList += std::to_string(slot.id);
    }
    if (idList.empty()) return;

    auto searchObject = GJSearchObject::create(SearchType::Type19, gd::string(idList));
    auto browserLayer = LevelBrowserLayer::create(searchObject);

    auto browserBG = static_cast<CCSprite*>(browserLayer->getChildByIDRecursive("background"));
    auto browserBL = browserLayer->getChildByIDRecursive("left-corner");
    auto browserBR = browserLayer->getChildByIDRecursive("right-corner");

    auto grayscaleTL = CCSpriteGrayscale::createWithSpriteFrameName("GJ_sideArt_001.png");
    grayscaleTL->setPosition({director->getScreenLeft() - 1, director->getScreenTop() + 1});
    grayscaleTL->setAnchorPoint(ccp(0, 1));
    grayscaleTL->setColor(ccc3(67, 67, 67));
    grayscaleTL->setFlipY(true);
    grayscaleTL->setZOrder(-1);
    browserLayer->addChild(grayscaleTL);

    auto grayscaleTR = CCSpriteGrayscale::createWithSpriteFrameName("GJ_sideArt_001.png");
    grayscaleTR->setPosition({director->getScreenRight() + 1, director->getScreenTop() + 1});
    grayscaleTR->setAnchorPoint(ccp(1, 1));
    grayscaleTR->setColor(ccc3(67, 67, 67));
    grayscaleTR->setFlipX(true);
    grayscaleTR->setFlipY(true);
    grayscaleTR->setZOrder(-1);
    browserLayer->addChild(grayscaleTR);

    auto floor = CCSprite::createWithSpriteFrameName("gauntletGround_001.png"_spr);
    if (floor) {
        auto floorSize = floor->getContentSize();
        float scale = std::min(winSize.width / floorSize.width, winSize.height / floorSize.height);
        floor->setScale(scale);
        floor->setPosition(ccp(winSize.width / 2, floorSize.height * scale / 2));
        floor->setAnchorPoint(ccp(0.5, 0.5));
        floor->setZOrder(-2);
        floor->setColor(ccc3(175, 175, 175));
        browserLayer->addChild(floor);
    }

    if (browserBG) browserBG->setColor(ccc3(34, 34, 34));
    if (browserBL) browserBL->setVisible(false);
    if (browserBR) browserBR->setVisible(false);

    auto scene = CCScene::create();
    scene->addChild(browserLayer);
    director->pushScene(CCTransitionFade::create(0.5, scene));
}