#include <ctime>
#include <cue/ListNode.hpp>
#include "GauntletEditPopup.hpp"

using namespace geode::prelude;

// Helpers

static std::string getBgIconSpriteName(int index) {
    return fmt::format("bgIcon_{:02d}_001.png", std::clamp(index, 1, 59));
}

// create / init

GauntletEditPopup* GauntletEditPopup::create(
    GauntletEditData const& existing,
    EditSavedCallback onSaved
) {
    auto ret = new GauntletEditPopup();
    if (ret && ret->init(440, 270, "GJ_square05.png", existing, onSaved)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GauntletEditPopup::init(
    float width, float height, char const* bg,
    GauntletEditData const& existing, EditSavedCallback onSaved
) {
    if (!Popup::init(width, height, bg)) return false;

    m_data = existing;
    m_onSaved = onSaved;

    // Layer swap menus

    m_GSLPreview = CCMenu::create();
    m_GSLPreview->setContentSize(m_mainLayer->getContentSize());
    m_GSLPreview->setPosition({0, 0});
    m_GSLPreview->setID("preview-gsl");
    m_mainLayer->addChild(m_GSLPreview, 5);

    m_GLPreview = CCMenu::create();
    m_GLPreview->setContentSize(m_mainLayer->getContentSize());
    m_GLPreview->setPosition({0, 0});
    m_GLPreview->setID("preview-gl");
    m_GLPreview->setVisible(false);
    m_mainLayer->addChild(m_GLPreview, 5);

    // Toggle button

    auto toggleMenu = CCMenu::create();
    toggleMenu->setPosition({m_mainLayer->getContentSize() - 3});
    m_mainLayer->addChild(toggleMenu);

    auto icon = CCSprite::createWithSpriteFrameName("GJ_sortIcon_001.png");
    icon->setRotation(90);
    icon->setFlipY(true);

    auto toggleLayer = CCMenuItemToggler::create(
        IconButtonSprite::create("GJ_button_01.png", icon, nullptr, nullptr),
        IconButtonSprite::create("GJ_button_02.png", icon, nullptr, nullptr),
        this,
        menu_selector(GauntletEditPopup::onSwapLayer)
    );
    toggleMenu->addChild(toggleLayer);

    // Title label

    auto titleStr = existing.id == 0 ? "Create Gauntlet" : "Edit Gauntlet";
    auto titleLabel = CCLabelBMFont::create(titleStr, "goldFont.fnt");
    titleLabel->setPosition(m_size.width / 2, m_size.height - 20);
    titleLabel->setScale(0.7f);
    titleLabel->setID("title-label");
    m_mainLayer->addChild(titleLabel);

    // Background panel

    auto mainLayerBG = NineSlice::create("square02_001.png");
    mainLayerBG->setContentSize({m_mainLayer->getContentWidth() - 30, m_mainLayer->getContentHeight() - 55});
    mainLayerBG->setPosition({m_mainLayer->getContentWidth() / 2, m_mainLayer->getContentHeight() / 2 - 11.5f});
    mainLayerBG->setOpacity(80);
    m_mainLayer->addChild(mainLayerBG);

    // Name input

    auto nameContainer = CCMenu::create();
    nameContainer->setLayout(RowLayout::create()
        ->setGap(10)
        ->setAxisAlignment(AxisAlignment::Center)
        ->setAxisReverse(true)
    );
    nameContainer->setPosition({75, m_mainLayer->getContentHeight() - 72});
    nameContainer->setContentSize({100, 50});
    nameContainer->setID("name-container");

    m_nameInput = TextInput::create(100, "Name");

    auto nameLabel = CCLabelBMFont::create("Name", "goldFont.fnt");
    nameLabel->setPosition(nameContainer->getPositionX(), nameContainer->getPositionY() + 20);
    nameLabel->setScale(0.52);

    auto applyNameBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GR_pushBtn_001.png"_spr),
        this,
        menu_selector(GauntletEditPopup::updatePreviewName)
    );
    applyNameBtn->setScale(0.6);
    applyNameBtn->m_baseScale = 0.6;
    applyNameBtn->m_scaleMultiplier = 1.15;

    nameContainer->addChild(applyNameBtn);
    nameContainer->addChild(m_nameInput);
    nameContainer->updateLayout();

    m_GSLPreview->addChild(nameLabel);
    m_GSLPreview->addChild(nameContainer);

    // Description input

    auto descContainer = CCMenu::create();
    descContainer->setLayout(RowLayout::create()
        ->setGap(10)
        ->setAxisAlignment(AxisAlignment::Start)
    );
    descContainer->setPosition({165, nameContainer->getPositionY() - 46.5f});
    descContainer->setContentSize({280, 50});
    descContainer->setID("description-container");

    m_descInput = TextInput::create(350, "Description", "chatFont.fnt");
    m_descInput->setTextAlign(TextInputAlign::Left);
    m_descInput->setCommonFilter(CommonFilter::Any);

    auto descLabel = CCLabelBMFont::create("Description", "goldFont.fnt");
    descLabel->setPosition(descContainer->getPositionX(), descContainer->getPositionY() + 20);
    descLabel->setScale(0.52);

    auto applyDescBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GR_pushBtn_001.png"_spr),
        this,
        menu_selector(GauntletEditPopup::updateDescription)
    );
    applyDescBtn->setScale(0.6);
    applyDescBtn->m_baseScale = 0.6;
    applyDescBtn->m_scaleMultiplier = 1.15;

    descContainer->addChild(m_descInput);
    descContainer->addChild(applyDescBtn);
    descContainer->updateLayout();

    m_GSLPreview->addChild(descLabel);
    m_GSLPreview->addChild(descContainer);

    // Sprite / icon picker

    auto spriteContainer = CCMenu::create();
    spriteContainer->setPosition({nameContainer->getPositionX() + 110, nameContainer->getPositionY()});
    spriteContainer->setContentSize({100, 50});
    spriteContainer->setID("sprite-container");

    auto spriteLabel = CCLabelBMFont::create("Sprite", "goldFont.fnt");
    spriteLabel->setPosition(spriteContainer->getPositionX(), spriteContainer->getPositionY() + 20);
    spriteLabel->setScale(0.52f);

    auto addSpriteBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Upload Sprite",
            100,
            80,
            1,
            true,
            "goldFont.fnt",
            "GJ_button_05.png"
        ),
        this,
        menu_selector(GauntletEditPopup::onPickIcon)
    );
    addSpriteBtn->setScale(0.75);
    addSpriteBtn->m_baseScale = 0.75;
    addSpriteBtn->m_scaleMultiplier = 1.15;

    spriteContainer->addChild(addSpriteBtn);

    m_GSLPreview->addChild(spriteLabel);
    m_GSLPreview->addChild(spriteContainer);

    // Node color pickers

    auto colorMenu = CCMenu::create();
    colorMenu->setLayout(RowLayout::create()
        ->setGap(-10)
    );
    colorMenu->setPosition(spriteContainer->getPositionX() + 92.5, spriteContainer->getPositionY() + 8);
    colorMenu->setContentWidth(82);

    // Name color column
    auto nameColNode = CCMenu::create();
    nameColNode->setContentSize({40, 80});
    nameColNode->setLayout(ColumnLayout::create()
        ->setGap(-3)
        ->setAxisReverse(true)
    );

    auto colorNameLabel = CCLabelBMFont::create("Col1", "goldFont.fnt");

    m_colorSprName = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    auto colorNameBtn = CCMenuItemSpriteExtra::create(
        m_colorSprName, this,
        menu_selector(GauntletEditPopup::onPickNameColor)
    );
    colorNameBtn->setScale(0.75);
    colorNameBtn->m_baseScale = 0.75;
    colorNameBtn->m_scaleMultiplier = 1.15;

    nameColNode->addChild(colorNameLabel);
    nameColNode->addChild(colorNameBtn);
    nameColNode->updateLayout();

    // Node color column
    auto nodeColNode = CCMenu::create();
    nodeColNode->setContentSize({40, 80});
    nodeColNode->setLayout(ColumnLayout::create()
        ->setGap(-3)
        ->setAxisReverse(true)
    );

    auto colorNodeLabel = CCLabelBMFont::create("Col2", "goldFont.fnt");

    m_colorSprNode = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    auto colorNodeBtn = CCMenuItemSpriteExtra::create(
        m_colorSprNode,
        this,
        menu_selector(GauntletEditPopup::onPickNodeColor)
    );
    colorNodeBtn->setScale(0.75);
    colorNodeBtn->m_baseScale = 0.75;
    colorNodeBtn->m_scaleMultiplier = 1.15;

    nodeColNode->addChild(colorNodeLabel);
    nodeColNode->addChild(colorNodeBtn);
    nodeColNode->updateLayout();

    colorMenu->addChild(nameColNode);
    colorMenu->addChild(nodeColNode);
    colorMenu->updateLayout();

    colorNameLabel->setScale(0.75);
    colorNodeLabel->setScale(0.75);

    m_GSLPreview->addChild(colorMenu);

    // Background picker

    auto bgContainer = CCMenu::create();
    bgContainer->setLayout(RowLayout::create()
        ->setGap(10)
        ->setAxisAlignment(AxisAlignment::Center)
    );
    bgContainer->setPosition({185, descContainer->getPositionY() - 71.5f});
    bgContainer->setContentSize({100, 50});
    bgContainer->setID("bg-container");

    auto bgLabel = CCLabelBMFont::create("Background", "goldFont.fnt");
    bgLabel->setPosition(bgContainer->getPositionX(), bgContainer->getPositionY() + 45);
    bgLabel->setScale(0.52);

    auto bgIcon = CCSprite::createWithSpriteFrameName(getBgIconSpriteName(m_bgIndex).c_str());
    bgIcon->setColor(m_selectedBGColor);
    bgIcon->setScale(1.7);

    auto bgBtn = CCMenuItemSpriteExtra::create(
        bgIcon,
        this,
        menu_selector(GauntletEditPopup::onPickBackground)
    );
    bgBtn->m_scaleMultiplier = 1.15;

    m_bgIndexButton = bgBtn;
    m_bgIconSpr = bgIcon;

    bgContainer->addChild(bgBtn);
    bgContainer->updateLayout();

    m_GSLPreview->addChild(bgLabel);
    m_GSLPreview->addChild(bgContainer);

    // BG Color

    auto bgColorMenu = CCMenu::create();
    bgColorMenu->setLayout(RowLayout::create()
        ->setGap(-10)
    );
    bgColorMenu->setPosition(spriteContainer->getPositionX() + 92.5, 112);
    bgColorMenu->setContentWidth(82);

    // Name color column
    auto bgColorNode = CCMenu::create();
    bgColorNode->setContentSize({40, 80});
    bgColorNode->setLayout(ColumnLayout::create()
        ->setGap(-3)
        ->setAxisReverse(true)
    );

    auto bgColorLabel = CCLabelBMFont::create("BG Color", "goldFont.fnt");

    m_colorSprBG = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    auto bgColorBtn = CCMenuItemSpriteExtra::create(
        m_colorSprBG,
        this,
        menu_selector(GauntletEditPopup::onPickBGColor)
    );
    bgColorBtn->setScale(0.75);
    bgColorBtn->m_baseScale = 0.75;
    bgColorBtn->m_scaleMultiplier = 1.15;

    bgColorNode->addChild(bgColorLabel);
    bgColorNode->addChild(bgColorBtn);
    bgColorNode->updateLayout();

    bgColorMenu->addChild(bgColorNode);
    bgColorMenu->updateLayout();

    bgColorLabel->setScale(0.75);

    m_GSLPreview->addChild(bgColorMenu);

    // Accent1 / Accent2

    auto accentMenu = CCMenu::create();
    accentMenu->setLayout(RowLayout::create()
        ->setGap(-10)
    );
    accentMenu->setPosition(spriteContainer->getPositionX() + 92.5, 65);
    accentMenu->setContentWidth(82);

    // Name color column
    auto accentColNode1 = CCMenu::create();
    accentColNode1->setContentSize({40, 80});
    accentColNode1->setLayout(ColumnLayout::create()
        ->setGap(-3)
        ->setAxisReverse(true)
    );

    auto accentColLabel1 = CCLabelBMFont::create("Acc1", "goldFont.fnt");

    m_colorAccent1 = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    auto colorAccentBtn1 = CCMenuItemSpriteExtra::create(
        m_colorAccent1,
        this,
        menu_selector(GauntletEditPopup::onPickAcc1Color)
    );
    colorAccentBtn1->setScale(0.75);
    colorAccentBtn1->m_baseScale = 0.75;
    colorAccentBtn1->m_scaleMultiplier = 1.15;

    accentColNode1->addChild(accentColLabel1);
    accentColNode1->addChild(colorAccentBtn1);
    accentColNode1->updateLayout();

    // Node color column
    auto accentColNode2 = CCMenu::create();
    accentColNode2->setContentSize({40, 80});
    accentColNode2->setLayout(ColumnLayout::create()
        ->setGap(-3)
        ->setAxisReverse(true)
    );

    auto accentColLabel2 = CCLabelBMFont::create("Acc2", "goldFont.fnt");

    m_colorAccent2 = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    auto colorAccentBtn2 = CCMenuItemSpriteExtra::create(
        m_colorAccent2,
        this,
        menu_selector(GauntletEditPopup::onPickAcc2Color)
    );
    colorAccentBtn2->setScale(0.75);
    colorAccentBtn2->m_baseScale = 0.75;
    colorAccentBtn2->m_scaleMultiplier = 1.15;

    accentColNode2->addChild(accentColLabel2);
    accentColNode2->addChild(colorAccentBtn2);
    accentColNode2->updateLayout();

    accentMenu->addChild(accentColNode1);
    accentMenu->addChild(accentColNode2);
    accentMenu->updateLayout();

    accentColLabel1->setScale(0.75);
    accentColLabel2->setScale(0.75);

    m_GSLPreview->addChild(accentMenu);

    // Preview panel

    auto previewContainer = CCMenu::create();
    previewContainer->setPosition({m_mainLayer->getContentWidth() - 70, 116});
    previewContainer->setID("preview-container");

    m_previewBG = NineSlice::create("GJ_squareB_01.png");
    m_previewBG->setID("preview-background");
    m_previewBG->setContentSize({110, 220});
    m_previewBG->setColor(m_selectedNodeColor);
    m_previewBG->setScale(0.85f);

    auto previewLabel = CCLabelBMFont::create("Preview", "goldFont.fnt");
    previewLabel->setPosition(m_previewBG->getPositionX(), m_previewBG->getContentHeight() / 2 - 7);
    previewLabel->setScale(0.52f);

    m_previewTitle = CCLabelBMFont::create("", "bigFont.fnt");
    m_previewTitle->setPosition({m_previewBG->getContentWidth() / 2, m_previewBG->getContentHeight() / 2 + 84});
    m_previewTitle->setScale(0.7);

    m_previewTitleShadow = CCLabelBMFont::create("", "bigFont.fnt");
    m_previewTitleShadow->setPosition(m_previewTitle->getPositionX() + 2, m_previewTitle->getPositionY() - 2);
    m_previewTitleShadow->setScale(0.7);
    m_previewTitleShadow->setColor({0, 0, 0});
    m_previewTitleShadow->setOpacity(60);

    m_gauntletText = CCLabelBMFont::create("Gauntlet", "bigFont.fnt");
    m_gauntletText->setPosition({m_previewBG->getContentWidth() / 2, m_previewBG->getContentHeight() / 2 + 69});
    m_gauntletText->setScale(0.45);

    m_gauntletTextShadow = CCLabelBMFont::create("Gauntlet", "bigFont.fnt");
    m_gauntletTextShadow->setPosition(m_gauntletText->getPositionX() + 2, m_gauntletText->getPositionY() - 2);
    m_gauntletTextShadow->setScale(0.45);
    m_gauntletTextShadow->setColor({0, 0, 0});
    m_gauntletTextShadow->setOpacity(60);

    auto rewardLabel = CCLabelBMFont::create("Reward", "goldFont.fnt");
    rewardLabel->setPosition(m_previewBG->getContentWidth() / 2, 27);
    rewardLabel->setScale(0.5);

    auto rewardLabelShadow = CCLabelBMFont::create("Reward", "goldFont.fnt");
    rewardLabelShadow->setPosition(rewardLabel->getPositionX() + 2, rewardLabel->getPositionY() - 2);
    rewardLabelShadow->setScale(0.5);
    rewardLabelShadow->setColor({0, 0, 0});
    rewardLabelShadow->setOpacity(60);

    auto chestSpr = CCSprite::createWithSpriteFrameName("GR_chest01_01_001.png"_spr);
    chestSpr->setPosition({rewardLabel->getPositionX(), rewardLabel->getPositionY() + 13.5f});
    chestSpr->setScale(0.3);

    auto chestSprShadow = CCSprite::createWithSpriteFrameName("GR_chest01_01_001.png"_spr);
    chestSprShadow->setPosition({chestSpr->getPositionX() + 2, chestSpr->getPositionY() - 2});
    chestSprShadow->setScale(0.3);
    chestSprShadow->setColor({0, 0, 0});
    chestSprShadow->setOpacity(60);

    // info button (shows description + metadata)
    auto previewDescMenu = CCMenu::create();
    previewDescMenu->setPosition(m_previewBG->getContentSize() - 10);

    auto previewDescBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
        this,
        menu_selector(GauntletEditPopup::onPreviewInfo)
    );
    previewDescBtn->setScale(0.75);
    previewDescBtn->m_baseScale = 0.75;
    previewDescBtn->m_scaleMultiplier = 1.15;
    previewDescMenu->addChild(previewDescBtn);

    m_previewBG->addChild(chestSprShadow);
    m_previewBG->addChild(chestSpr, 1);
    m_previewBG->addChild(rewardLabelShadow);
    m_previewBG->addChild(rewardLabel, 1);
    m_previewBG->addChild(m_gauntletTextShadow);
    m_previewBG->addChild(m_gauntletText, 1);
    m_previewBG->addChild(m_previewTitleShadow);
    m_previewBG->addChild(m_previewTitle, 1);
    m_previewBG->addChild(previewDescMenu);

    previewContainer->addChild(previewLabel);
    previewContainer->addChild(m_previewBG);

    m_mainLayer->addChild(previewContainer);

    // Info inputs (date / version / suggester) 

    m_infoDateInput = TextInput::create(100, "Release Date", "chatFont.fnt");
    m_infoDateInput->setCommonFilter(CommonFilter::Any);

    m_infoVersionInput = TextInput::create(100, "Release Version", "chatFont.fnt");
    m_infoVersionInput->setCommonFilter(CommonFilter::Float);

    m_infoAccIDInput = TextInput::create(100, "Suggester accID", "chatFont.fnt");
    m_infoAccIDInput->setCommonFilter(CommonFilter::Int);

    // inputs column
    auto infoMenu = CCMenu::create();
    infoMenu->setContentHeight(70);
    infoMenu->setPosition({59.75, 80});
    infoMenu->setLayout(ColumnLayout::create()
        ->setGap(5)
        ->setAxisAlignment(AxisAlignment::Start)
    );
    infoMenu->addChild(m_infoDateInput);
    infoMenu->addChild(m_infoVersionInput);
    infoMenu->addChild(m_infoAccIDInput);
    infoMenu->updateLayout();

    auto infoLabel = CCLabelBMFont::create("Info", "goldFont.fnt");
    infoLabel->setPosition({59.75, infoMenu->getPositionY() + 45});
    infoLabel->setScale(0.52);

    // push buttons column
    auto makePushBtn = [&](SEL_MenuHandler sel) {
        auto btn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GR_pushBtn_001.png"_spr), this, sel);
        btn->setScale(0.6);
        btn->m_baseScale = 0.6f;
        btn->m_scaleMultiplier = 1.15;
        return btn;
    };

    auto pushInfoMenu = CCMenu::create();
    pushInfoMenu->setContentHeight(70);
    pushInfoMenu->setPosition({113.75, infoMenu->getPositionY()});
    pushInfoMenu->setLayout(ColumnLayout::create()
        ->setGap(5)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setAxisReverse(true)
    );
    pushInfoMenu->addChild(makePushBtn(menu_selector(GauntletEditPopup::updateInfoAccID)));
    pushInfoMenu->addChild(makePushBtn(menu_selector(GauntletEditPopup::updateInfoVersion)));
    pushInfoMenu->addChild(makePushBtn(menu_selector(GauntletEditPopup::updateInfoDate)));
    pushInfoMenu->updateLayout();

    m_GSLPreview->addChild(infoLabel);
    m_GSLPreview->addChild(infoMenu);
    m_GSLPreview->addChild(pushInfoMenu);

    // GL Preview — Level Search

    auto stencil = NineSlice::create("square02b_001.png");
    stencil->setContentSize({290, 181});
    stencil->setPosition({170, 116.5});

    m_clippingNode = CCClippingNode::create(stencil);
    m_clippingNode->setAlphaThreshold(0);
    m_clippingNode->setContentSize(stencil->getContentSize());
    m_clippingNode->setID("preview-clipping-node");
    m_GLPreview->addChild(m_clippingNode);

    auto outline = NineSlice::create("GJ_square07.png");
    outline->setContentSize(stencil->getContentSize());
    outline->setPosition(stencil->getPosition());
    m_GLPreview->addChild(outline, 1);

    auto leftCornerNode = CCNode::create();
    leftCornerNode->setPosition({26, 206});
    leftCornerNode->setScale(0.75);
    leftCornerNode->setAnchorPoint({0, 1});
    m_clippingNode->addChild(leftCornerNode, 1);

    auto leftCornerBase = CCSprite::createWithSpriteFrameName("GR_gauntletCorner_001.png"_spr);
    leftCornerBase->setAnchorPoint({0, 1});
    leftCornerBase->setFlipY(true);
    leftCornerNode->addChild(leftCornerBase);

    m_leftCornerAccent1 = CCSprite::createWithSpriteFrameName("GR_gauntletCornerColor_001.png"_spr);
    m_leftCornerAccent1->setAnchorPoint({0, 1});
    m_leftCornerAccent1->setFlipY(true);
    m_leftCornerAccent1->setColor(m_selectedAccentColor1);
    leftCornerNode->addChild(m_leftCornerAccent1);

    m_leftCornerAccent2 = CCSprite::createWithSpriteFrameName("GR_gauntletCornerHighlight_001.png"_spr);
    m_leftCornerAccent2->setAnchorPoint({0, 1});
    m_leftCornerAccent2->setFlipY(true);
    m_leftCornerAccent2->setColor(m_selectedAccentColor2);
    leftCornerNode->addChild(m_leftCornerAccent2);

    auto rightCornerNode = CCNode::create();
    rightCornerNode->setPosition({314, 206});
    rightCornerNode->setScale(0.75);
    rightCornerNode->setAnchorPoint({1, 1});
    m_clippingNode->addChild(rightCornerNode, 1);

    auto rightCornerBase = CCSprite::createWithSpriteFrameName("GR_gauntletCorner_001.png"_spr);
    rightCornerBase->setAnchorPoint({1, 1});
    rightCornerBase->setFlipY(true);
    rightCornerBase->setFlipX(true);
    rightCornerNode->addChild(rightCornerBase);

    m_rightCornerAccent1 = CCSprite::createWithSpriteFrameName("GR_gauntletCornerColor_001.png"_spr);
    m_rightCornerAccent1->setAnchorPoint({1, 1});
    m_rightCornerAccent1->setFlipY(true);
    m_rightCornerAccent1->setFlipX(true);
    m_rightCornerAccent1->setColor(m_selectedAccentColor1);
    rightCornerNode->addChild(m_rightCornerAccent1);

    m_rightCornerAccent2 = CCSprite::createWithSpriteFrameName("GR_gauntletCornerHighlight_001.png"_spr);
    m_rightCornerAccent2->setAnchorPoint({1, 1});
    m_rightCornerAccent2->setFlipY(true);
    m_rightCornerAccent2->setFlipX(true);
    m_rightCornerAccent2->setColor(m_selectedAccentColor2);
    rightCornerNode->addChild(m_rightCornerAccent2);

    m_GLBackground = CCSprite::create(fmt::format("game_bg_{:02}_001.png", m_bgIndex).c_str());
    m_GLBackground->setPosition({0, 0});
    m_GLBackground->setAnchorPoint({0, 0});
    m_GLBackground->setScale(0.66);
    m_GLBackground->setColor(m_selectedBGColor);
    m_GLBackground->setID("preview-gauntletlayer-bg");

    m_clippingNode->addChild(m_GLBackground);

    // Levels

    auto levelSearchNode = CCMenu::create();
    levelSearchNode->setPosition(170, 185);
    levelSearchNode->setLayout(RowLayout::create()
        ->setGap(10)
        ->setAxisAlignment(AxisAlignment::Center)
    );
    levelSearchNode->setScale(0.7);
    m_GLPreview->addChild(levelSearchNode);

    m_levelSearchInput = TextInput::create(200, "Level ID", "bigFont.fnt");
    m_levelSearchInput->setCommonFilter(CommonFilter::Int);
    
    auto searchBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GR_addBtn_001.png"_spr),
        this,
        menu_selector(GauntletEditPopup::onAddLevel)
    );
    
    levelSearchNode->addChild(m_levelSearchInput);
    levelSearchNode->addChild(searchBtn);
    levelSearchNode->updateLayout();

    auto previewLevelLabel = CCLabelBMFont::create("Levels", "goldFont.fnt");
    previewLevelLabel->setPosition({outline->getPositionX(), outline->getPositionY() + outline->getContentSize().height / 2 + 12});
    previewLevelLabel->setScale(0.52);
    m_GLPreview->addChild(previewLevelLabel);

    auto listSize = CCSize(290, 181);
    auto listEvenColor = ccc4(255, 255, 255, 50);
    auto listOddColor = ccc4(255, 255, 255, 30);

    auto levelListMenu = CCMenu::create();
    levelListMenu->setID("level-list-menu");
    levelListMenu->setContentSize({255, 135});
    levelListMenu->setPosition({170, 100});
    levelListMenu->ignoreAnchorPointForPosition(false);
    m_GLPreview->addChild(levelListMenu);

    for (int i = 0; i < 5; i++) {
        auto row = CCNode::create();
        row->setID(fmt::format("level-node-{}", i).c_str());
        row->setContentSize({75, 75});
        row->setAnchorPoint({0.5, 0.5});

        if (!m_data.iconURL.empty()) {
            m_slotBgHolders[i].spawn(
                web::WebRequest().get(m_data.iconURL),
                [row](web::WebResponse res) {
                    if (!res.ok() || !row) return;
                    auto bytes = res.data();
                    queueInMainThread([row, bytes]() {
                        if (!row) return;
                        auto img = new CCImage();
                        if (!img->initWithImageData(const_cast<unsigned char*>(bytes.data()), bytes.size())) {
                            delete img;
                            return;
                        }
                        auto tex = new CCTexture2D();
                        tex->initWithImage(img);
                        delete img;
                        auto icon = CCSprite::createWithTexture(tex);
                        tex->release();
                        icon->setScale(0.75);
                        icon->setPosition({row->getContentWidth() / 2, row->getContentHeight() / 2});
                        icon->setID("gauntlet-icon-bg");
                        if (auto old = row->getChildByID("row-bg")) old->removeFromParent();
                        row->addChild(icon, 1);
                    });
                }
            );
        }

        // Level info — populated by refreshLevels()
        auto nameLabel = CCLabelBMFont::create(fmt::format("Level {}", i + 1).c_str(), "bigFont.fnt");
        nameLabel->setID("level-name");
        nameLabel->setScale(0.35);
        nameLabel->setAnchorPoint({0.5, 0.5});
        nameLabel->limitLabelWidth(110, 0.3, 0.05);
        nameLabel->setPosition({row->getContentWidth() / 2, row->getContentHeight() / 2 - 5});
        row->addChild(nameLabel, 2);

        auto creatorLabel = CCLabelBMFont::create("Creator", "goldFont.fnt");
        creatorLabel->setID("level-creator");
        creatorLabel->setScale(0.25);
        creatorLabel->setAnchorPoint({0.5, 0.5});
        creatorLabel->setPosition({row->getContentWidth() / 2, row->getContentHeight() / 2 - 12.5f});
        row->addChild(creatorLabel, 2);

        auto starsLabel = CCLabelBMFont::create("", "bigFont.fnt");
        starsLabel->setID("level-stars");
        starsLabel->setScale(0.25);
        starsLabel->setAnchorPoint({0.5, 0.5});
        starsLabel->setPosition({row->getContentWidth() / 2, row->getContentHeight() / 2 - 22});
        row->addChild(starsLabel, 2);

        // Remove button
        auto removeMenu = CCMenu::create();
        removeMenu->setPosition(row->getContentSize() - 15);
        row->addChild(removeMenu, 2);

        auto removeSpr = CCSprite::createWithSpriteFrameName("GR_deleteBtn_001.png"_spr);
        removeSpr->setScale(0.4);
        auto removeBtn = CCMenuItemExt::createSpriteExtra(removeSpr,
            [this, i](CCMenuItemSpriteExtra*) {
                if (i < (int)m_levels.size()) {
                    m_levels.erase(m_levels.begin() + i);
                    refreshLevels();
                }
            }
        );
        removeBtn->m_scaleMultiplier = 1.15;
        removeMenu->addChild(removeBtn);

        levelListMenu->addChild(row);

        if (i == 0) row->setPosition({25, 35});
        else if (i == 1) row->setPosition({75, 100});
        else if (i == 2) row->setPosition({125, 35});
        else if (i == 3) row->setPosition({175, 100});
        else if (i == 4) row->setPosition({225, 35});
    }

    // Save button

    auto saveMenu = CCMenu::create();
    saveMenu->setPosition(m_mainLayer->getContentWidth() / 2, 3);

    auto saveBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Save"),
        this,
        menu_selector(GauntletEditPopup::onSave)
    );
    saveBtn->setScale(0.75);
    saveBtn->m_baseScale = 0.75;
    saveBtn->m_scaleMultiplier = 1.15;

    saveMenu->addChild(saveBtn);

    m_mainLayer->addChild(saveMenu);

    // Restore state from existing data
    if (!existing.name.empty()) {
        m_nameInput->setString(existing.name);
        updatePreviewName(nullptr);
    }
    if (!existing.description.empty()) {
        m_descInput->setString(existing.description);
        updateDescription(nullptr);
    }

    // Restore info strings
    m_infoDate      = existing.infoDate;
    m_infoVersion   = existing.infoVersion;
    m_infoSuggester = existing.infoSuggester;
    m_infoAccID     = existing.infoAccID;
    if (!existing.infoDate.empty())      m_infoDateInput->setString(existing.infoDate);
    if (!existing.infoVersion.empty())   m_infoVersionInput->setString(existing.infoVersion);
    if (existing.infoAccID != 0)
        m_infoAccIDInput->setString(std::to_string(existing.infoAccID));

    // Restore colors
    m_selectedColor = existing.nameColor;
    if (m_colorSprName) m_colorSprName->setColor(m_selectedColor);
    if (m_previewTitle) m_previewTitle->setColor(m_selectedColor);
    if (m_gauntletText) m_gauntletText->setColor(m_selectedColor);

    m_selectedNodeColor = existing.nodeColor;
    if (m_colorSprNode) m_colorSprNode->setColor(m_selectedNodeColor);
    if (m_previewBG)    m_previewBG->setColor(m_selectedNodeColor);

    m_selectedBGColor = existing.bgColor;
    if (m_colorSprBG) m_colorSprBG->setColor(m_selectedBGColor);
    if (m_bgIconSpr)  m_bgIconSpr->setColor(m_selectedBGColor);
    if (m_GLBackground) m_GLBackground->setColor(m_selectedBGColor);

    m_selectedAccentColor1 = existing.accentColor1;
    if (m_colorAccent1) m_colorAccent1->setColor(m_selectedAccentColor1);
    if (m_leftCornerAccent1) m_leftCornerAccent1->setColor(m_selectedAccentColor1);
    if (m_rightCornerAccent1) m_rightCornerAccent1->setColor(m_selectedAccentColor1);

    m_selectedAccentColor2 = existing.accentColor2;
    if (m_colorAccent2) m_colorAccent2->setColor(m_selectedAccentColor2);
    if (m_leftCornerAccent2) m_leftCornerAccent2->setColor(m_selectedAccentColor2);
    if (m_rightCornerAccent2) m_rightCornerAccent2->setColor(m_selectedAccentColor2);

    // Restore background index
    m_bgIndex = existing.bgIndex > 0 ? existing.bgIndex : 1;
    updateBgIcon();

    // Restore levels
    m_levels.clear();
    log::debug("Restoring levels for the {} Gauntlet...", existing.name);
    for (int i = 0; i < 5; i++) {
        log::debug("  slot {}: id={} name='{}' creator='{}' stars={}", i, existing.levels[i].id, existing.levels[i].name, existing.levels[i].creator, existing.levels[i].stars);
        if (existing.levels[i].id != 0) {
            LevelRewardEntry e;
            e.levelId     = existing.levels[i].id;
            e.levelName   = existing.levels[i].name;
            e.creatorName = existing.levels[i].creator;
            e.reward      = existing.levels[i].stars;
            m_levels.push_back(e);
        }
    }
    log::debug("  loaded {}/5 levels", m_levels.size());
    if (!m_levels.empty()) refreshLevels();

    // Restore icon from URL
    if (!existing.iconURL.empty() && !m_pendingIconPath.has_value()) {
        m_data.iconURL = existing.iconURL;
        m_webIconHolder.spawn(
            web::WebRequest().get(existing.iconURL),
            [this](web::WebResponse res) {
                if (!res.ok()) return;
                auto bytes = res.data();
                queueInMainThread([this, bytes]() {
                    auto container = m_mainLayer->getChildByIDRecursive("preview-background");
                    if (!container) return;
                    auto img = new CCImage();
                    if (!img->initWithImageData(
                            const_cast<unsigned char*>(bytes.data()), bytes.size())) {
                        delete img;
                        return;
                    }
                    auto tex = new CCTexture2D();
                    tex->initWithImage(img);
                    delete img;
                    if (auto old = container->getChildByID("preview-icon"))
                        old->removeFromParent();
                    if (auto old = container->getChildByID("preview-icon-shadow"))
                        old->removeFromParent();
                    auto icon = CCSprite::createWithTexture(tex);
                    icon->setID("preview-icon");
                    icon->setPosition({container->getContentWidth() / 2, (container->getContentHeight() / 2) + 10});
                    icon->setScale(1.05);
                    auto shadow = CCSprite::createWithTexture(tex);
                    shadow->setID("preview-icon-shadow");
                    shadow->setColor({0, 0, 0});
                    shadow->setOpacity(50);
                    shadow->setPosition({
                        icon->getPositionX(),
                        icon->getPositionY() - 10
                    });
                    shadow->setScaleX(icon->getScaleX());
                    shadow->setScaleY(icon->getScaleY() * 1.2f);
                    container->addChild(shadow);
                    container->addChild(icon);
                    tex->release();
                });
            }
        );
    }

    return true;
}

// onClose 

void GauntletEditPopup::onClose(CCObject* sender) {
    auto alert = FLAlertLayer::create(
        this,
        "Hold on!",
        "Are you sure you want to <cy>close the Gauntlet Creator</c>? All progress will be lost and <cr>cannot be undone</c>!",
        "Cancel",
        "Yes"
    );
    alert->m_button1->updateBGImage("GJ_button_05.png");
    alert->m_button2->updateBGImage("GJ_button_06.png");
    alert->show();

    auto glm = GameLevelManager::get();
    if (glm->m_levelManagerDelegate == this) glm->m_levelManagerDelegate = nullptr;
}

void GauntletEditPopup::FLAlert_Clicked(FLAlertLayer* alert, bool btn2) {
    if (!btn2) return;

    auto glm = GameLevelManager::get();
    if (glm->m_levelManagerDelegate == this)
        glm->m_levelManagerDelegate = nullptr;
    if (glm->m_userInfoDelegate == this)
        glm->m_userInfoDelegate = nullptr;

    Popup::onClose(nullptr);
}

// Layer swap

void GauntletEditPopup::onSwapLayer(CCObject* sender) {
    if (!m_GSLPreview || !m_GLPreview) return;
    m_GSLPreview->setVisible(!m_GSLPreview->isVisible());
    m_GLPreview->setVisible(!m_GLPreview->isVisible());
}

// Preview name

void GauntletEditPopup::updatePreviewName(CCObject* sender) {
    if (!m_previewTitle || !m_previewTitleShadow) return;
    auto str = std::string(m_nameInput->getString());
    m_previewTitle->setString(str.c_str());
    m_previewTitle->updateLabel();
    m_previewTitle->limitLabelWidth(80, 0.7, 0.00001);
    m_previewTitleShadow->setString(str.c_str());
    m_previewTitleShadow->updateLabel();
    m_previewTitleShadow->limitLabelWidth(80, 0.7, 0.00001);
}

// Description

void GauntletEditPopup::updateDescription(CCObject* sender) {
    if (!m_descInput || !m_previewBG) return;
}

void GauntletEditPopup::onPreviewInfo(CCObject* sender) {
    auto text = m_descInput ? std::string(m_descInput->getString()) : "";

    auto alert = MDPopup::create(
        fmt::format("The {} Gauntlet", m_previewTitle->getString()),
        text.c_str(),
        "Ok", nullptr
    );

    // append metadata labels to the first child of the alert
    auto releaseDate = CCLabelBMFont::create(m_infoDate.c_str(), "chatFont.fnt");
    releaseDate->setPosition({383.5f, 25});
    releaseDate->setScale(0.5f);
    releaseDate->setAnchorPoint({1, 0});
    releaseDate->setOpacity(51);

    auto releaseVer = CCLabelBMFont::create(m_infoVersion.c_str(), "chatFont.fnt");
    releaseVer->setPosition({383.5f, 15});
    releaseVer->setScale(0.5f);
    releaseVer->setAnchorPoint({1, 0});
    releaseVer->setOpacity(51);

    auto credit = CCLabelBMFont::create(m_infoSuggester.c_str(), "chatFont.fnt");
    credit->setPosition({15.5f, 15});
    credit->setScale(0.5f);
    credit->setAnchorPoint({0, 0});
    credit->setOpacity(51);

    auto children = alert->getChildren();
    if (children && children->count() > 0) {
        auto child = static_cast<CCNode*>(children->objectAtIndex(0));
        child->addChild(releaseDate);
        child->addChild(releaseVer);
        child->addChild(credit);
    }

    alert->show();
}

// Info push buttons

void GauntletEditPopup::updateInfoDate(CCObject* sender) {
    auto input = std::string(m_infoDateInput->getString());
    if (input.empty()) {
        auto t = std::time(nullptr);
        std::tm tm{};
        #ifdef _WIN32
        localtime_s(&tm, &t);
        #else
        localtime_r(&t, &tm);
        #endif
        char buf[32];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d", &tm);
        input = buf;
        m_infoDateInput->setString(input);
    }
    m_infoDate = fmt::format("Released on: {}", input);
    Notification::create("Release date set", NotificationIcon::Success)->show();
}

void GauntletEditPopup::updateInfoVersion(CCObject* sender) {
    auto input = std::string(m_infoVersionInput->getString());
    if (input.empty()) {
        input = Mod::get()->getVersion().toVString();
        m_infoVersionInput->setString(input);
    }
    m_infoVersion = fmt::format("Version: {}", input);
    Notification::create("Version set", NotificationIcon::Success)->show();
}

void GauntletEditPopup::updateInfoAccID(CCObject* sender) {
    auto idStr = std::string(m_infoAccIDInput->getString());

    int accID = idStr.empty()
        ? GJAccountManager::get()->m_accountID
        : geode::utils::numFromString<int>(idStr).unwrapOr(0);

    if (accID == 0) {
        Notification::create("Invalid account ID", NotificationIcon::Error)->show();
        return;
    }

    m_infoAccID = accID;

    // Check cache first
    auto glm   = GameLevelManager::get();
    auto score = glm->userInfoForAccountID(accID);
    if (score) {
        m_infoSuggester = fmt::format("Suggested by: {}", std::string(score->m_userName));
        Notification::create(m_infoSuggester, NotificationIcon::Success)->show();
        return;
    }

    // Not cached — fetch from server, GD will cache it automatically via storeUserInfo
    if (m_searchingUser) return;
    m_searchingUser = true;
    glm->m_userInfoDelegate = this;
    glm->getGJUserInfo(accID);
}

void GauntletEditPopup::getUserInfoFinished(GJUserScore* score) {
    m_searchingUser = false;
    auto glm = GameLevelManager::get();
    if (glm->m_userInfoDelegate == this)
        glm->m_userInfoDelegate = nullptr;

    if (!score) {
        Notification::create("User not found", NotificationIcon::Error)->show();
        return;
    }

    // GD has already called storeUserInfo internally, so next time it'll be cached
    m_infoSuggester = fmt::format("Suggested by: {}", std::string(score->m_userName));
    Notification::create(m_infoSuggester, NotificationIcon::Success)->show();
}

void GauntletEditPopup::getUserInfoFailed(int) {
    m_searchingUser = false;
    auto glm = GameLevelManager::get();
    if (glm->m_userInfoDelegate == this)
        glm->m_userInfoDelegate = nullptr;

    Notification::create("Failed to fetch user", NotificationIcon::Error)->show();
}

// Icon picker

void GauntletEditPopup::onPickIcon(CCObject*) {
    m_pickHolder.spawn(
        file::pick(file::PickMode::OpenFile, file::FilePickOptions{
            dirs::getGameDir(),
            {{ "PNG Images", { "*.png" } }}
        }),
        [this](geode::Result<std::optional<std::filesystem::path>> result) {
            if (!result || !result.unwrap().has_value()) return;
            auto path = result.unwrap().value();
            m_pendingIconPath = path;

            queueInMainThread([this, path]() {
                Notification::create(
                    fmt::format("Icon selected: {}", path.filename().string()),
                    NotificationIcon::Success
                )->show();

                auto container = m_mainLayer->getChildByIDRecursive("preview-background");
                if (!container) return;
                // auto listMenu = m_GLPreview->getChildByIDRecursive("level-list-menu");
                // if (!listMenu) return;

                auto previewIcon = CCSprite::create(path.string().c_str());
                auto previewIconShadow = CCSprite::create(path.string().c_str());
                if (!previewIcon || !previewIconShadow) return;

                if (auto old = container->getChildByID("preview-icon"))
                    old->removeFromParent();
                if (auto old = container->getChildByID("preview-icon-shadow"))
                    old->removeFromParent();

                previewIcon->setID("preview-icon");
                previewIcon->setPosition({container->getContentWidth() / 2, (container->getContentHeight() / 2) + 10});
                previewIcon->setScale(1.05);

                previewIconShadow->setID("preview-icon-shadow");
                previewIconShadow->setPosition({previewIcon->getPositionX(), previewIcon->getPositionY() - 10});
                previewIconShadow->setColor({0, 0, 0});
                previewIconShadow->setScaleX(previewIcon->getScaleX());
                previewIconShadow->setScaleY(previewIcon->getScaleY() * 1.2f);
                previewIconShadow->setOpacity(50);

                container->addChild(previewIconShadow);
                container->addChild(previewIcon);

                // Update icon on every level slot
                auto listMenu = m_GLPreview
                    ? static_cast<CCNode*>(m_GLPreview->getChildByID("level-list-menu"))
                    : nullptr;
                if (listMenu) {
                    auto rows = listMenu->getChildren();
                    if (rows) {
                        for (unsigned int j = 0; j < rows->count(); j++) {
                            auto rowNode = static_cast<CCNode*>(rows->objectAtIndex(j));
                            if (!rowNode) continue;
                            if (auto old = rowNode->getChildByID("gauntlet-icon-bg"))
                                old->removeFromParent();
                            auto slotIcon = CCSprite::create(path.string().c_str());
                            if (!slotIcon) continue;
                            slotIcon->setID("gauntlet-icon-bg");
                            slotIcon->setScale(0.75f);
                            slotIcon->setPosition({rowNode->getContentWidth() / 2,
                                                   rowNode->getContentHeight() / 2});
                            rowNode->addChild(slotIcon, 1);
                        }
                    }
                }

            });
        }
    );
}

// Background picker

void GauntletEditPopup::onPickBackground(CCObject* sender) {
    auto layer = SelectArtLayer::create(SelectArtType::Background, m_bgIndex);
    if (!layer) return;
    layer->m_delegate = this;
    layer->show();
}

void GauntletEditPopup::selectArtClosed(SelectArtLayer* layer) {
    if (!layer) return;
    m_bgIndex = layer->m_art;
    updateBgIcon();
}

void GauntletEditPopup::updateBgIcon() {
    // Update the small icon button in the fields layer
    if (m_bgIconSpr) {
        auto frame = CCSpriteFrameCache::sharedSpriteFrameCache()
            ->spriteFrameByName(getBgIconSpriteName(m_bgIndex).c_str());
        if (frame) m_bgIconSpr->setDisplayFrame(frame);
    }

    // Update the GL preview background — replace the sprite entirely
    if (!m_GLPreview) return;
    if (auto old = m_GLPreview->getChildByIDRecursive("preview-gauntletlayer-bg"))
        old->removeFromParent();

    m_GLBackground = CCSprite::create(
        fmt::format("game_bg_{:02d}_001.png", m_bgIndex).c_str()
    );
    if (!m_GLBackground) return;

    m_GLBackground->setID("preview-gauntletlayer-bg");
    m_GLBackground->setPosition({0, 0});
    m_GLBackground->setColor(m_selectedBGColor);
    m_GLBackground->setScale(0.66);
    m_GLBackground->setAnchorPoint({0, 0});
    m_clippingNode->addChild(m_GLBackground, -1);
}

// Color pickers

void GauntletEditPopup::onPickNameColor(CCObject* sender) {
    m_colorPopup = ColorPickPopup::create(m_selectedColor);
    if (!m_colorPopup) return;
    m_colorPopup->setCallback([this](ccColor4B const& color) {
        m_selectedColor = {color.r, color.g, color.b};
        if (m_colorSprName) m_colorSprName->setColor(m_selectedColor);
        if (m_previewTitle) m_previewTitle->setColor(m_selectedColor);
        if (m_gauntletText) m_gauntletText->setColor(m_selectedColor);
    });
    m_colorPopup->show();
}

void GauntletEditPopup::onPickNodeColor(CCObject* sender) {
    m_colorPopup = ColorPickPopup::create(m_selectedNodeColor);
    if (!m_colorPopup) return;
    m_colorPopup->setCallback([this](ccColor4B const& color) {
        m_selectedNodeColor = {color.r, color.g, color.b};
        if (m_colorSprNode) m_colorSprNode->setColor(m_selectedNodeColor);
        if (m_previewBG) m_previewBG->setColor(m_selectedNodeColor);
    });
    m_colorPopup->show();
}

void GauntletEditPopup::onPickBGColor(CCObject* sender) {
    m_colorPopup = ColorPickPopup::create(m_selectedBGColor);
    if (!m_colorPopup) return;
    m_colorPopup->setCallback([this](ccColor4B const& color) {
        m_selectedBGColor = {color.r, color.g, color.b};
        if (m_colorSprBG) m_colorSprBG->setColor(m_selectedBGColor);
        if (m_bgIconSpr) m_bgIconSpr->setColor(m_selectedBGColor);
        if (m_GLBackground) m_GLBackground->setColor(m_selectedBGColor);
    });
    m_colorPopup->show();
}

void GauntletEditPopup::onPickAcc1Color(CCObject* sender) {
    m_colorPopup = ColorPickPopup::create(m_selectedAccentColor1);
    if (!m_colorPopup) return;
    m_colorPopup->setCallback([this](ccColor4B const& color) {
        m_selectedAccentColor1 = {color.r, color.g, color.b};
        if (m_colorAccent1) m_colorAccent1->setColor(m_selectedAccentColor1);
        if (m_leftCornerAccent1) m_leftCornerAccent1->setColor(m_selectedAccentColor1);
        if (m_rightCornerAccent1) m_rightCornerAccent1->setColor(m_selectedAccentColor1);
    });
    m_colorPopup->show();
}

void GauntletEditPopup::onPickAcc2Color(CCObject* sender) {
    m_colorPopup = ColorPickPopup::create(m_selectedAccentColor2);
    if (!m_colorPopup) return;
    m_colorPopup->setCallback([this](ccColor4B const& color) {
        m_selectedAccentColor2 = {color.r, color.g, color.b};
        if (m_colorAccent2) m_colorAccent2->setColor(m_selectedAccentColor2);
        if (m_leftCornerAccent2) m_leftCornerAccent2->setColor(m_selectedAccentColor2);
        if (m_rightCornerAccent2) m_rightCornerAccent2->setColor(m_selectedAccentColor2);
    });
    m_colorPopup->show();
}

void GauntletEditPopup::onAddLevel(CCObject*) {
    auto idStr = std::string(m_levelSearchInput->getString());
    if (idStr.empty()) {
        Notification::create("Enter a level ID", NotificationIcon::Error)->show();
        return;
    }
    if (m_searchingLevel) return;

    // Max 5 levels
    if (m_levels.size() >= 5) {
        Notification::create("Maximum of 5 levels reached!", NotificationIcon::Warning)->show();
        return;
    }

    m_searchingLevel = true;

    auto glm = GameLevelManager::get();
    glm->m_levelManagerDelegate = this;

    auto searchObj = GJSearchObject::create(SearchType::Type19, gd::string(idStr));
    m_pendingSearchKey = searchObj->getKey();
    glm->getOnlineLevels(searchObj);
}

void GauntletEditPopup::loadLevelsFinished(CCArray* levels, char const* key) {
    if (!m_searchingLevel || m_pendingSearchKey != key) return;

    m_searchingLevel = false;
    auto glm = GameLevelManager::get();
    if (glm->m_levelManagerDelegate == this)
        glm->m_levelManagerDelegate = nullptr;

    if (!levels || levels->count() == 0) {
        Notification::create("Level not found.", NotificationIcon::Error)->show();
        return;
    }

    auto level = static_cast<GJGameLevel*>(levels->objectAtIndex(0));
    if (!level) return;

    // Prevent duplicates
    for (auto const& e : m_levels) {
        if (e.levelId == (int)level->m_levelID) {
            Notification::create("Level already added.", NotificationIcon::Warning)->show();
            return;
        }
    }

    LevelRewardEntry entry;
    entry.levelId    = level->m_levelID;
    entry.levelName  = level->m_levelName;
    entry.creatorName = level->m_creatorName;
    entry.reward     = level->m_stars;

    m_levels.push_back(entry);
    m_levelSearchInput->setString("");
    refreshLevels();

    Notification::create(
        fmt::format("Added \"{}\"", entry.levelName),
        NotificationIcon::Success
    )->show();
}

void GauntletEditPopup::loadLevelsFailed(char const* key, int) {
    if (!m_searchingLevel || m_pendingSearchKey != key) return;

    m_searchingLevel = false;
    auto glm = GameLevelManager::get();
    if (glm->m_levelManagerDelegate == this)
        glm->m_levelManagerDelegate = nullptr;

    Notification::create("Failed to fetch level.", NotificationIcon::Error)->show();
}

void GauntletEditPopup::onRemoveLevel(CCObject* sender) {
    log::debug("delete");
}

void GauntletEditPopup::refreshLevels() {
    auto listMenu = m_GLPreview
        ? m_GLPreview->getChildByID("level-list-menu")
        : nullptr;
    if (!listMenu) return;

    auto children = listMenu->getChildren();
    if (!children) return;

    for (int i = 0; i < 5; i++) {
        auto row = static_cast<CCNode*>(children->objectAtIndex(i));
        if (!row) continue;

        bool hasLevel = i < (int)m_levels.size();
        auto entry = hasLevel ? m_levels[i] : LevelRewardEntry{};

        if (auto lbl = dynamic_cast<CCLabelBMFont*>(row->getChildByID("level-name")))
            lbl->setString(hasLevel
                ? entry.levelName.c_str()
                : fmt::format("Level {}", i + 1).c_str());

        if (auto lbl = dynamic_cast<CCLabelBMFont*>(row->getChildByID("level-creator")))
            lbl->setString(hasLevel
                ? fmt::format("by {}", entry.creatorName).c_str() : "Creator");

        if (auto lbl = dynamic_cast<CCLabelBMFont*>(row->getChildByID("level-stars")))
            lbl->setString(hasLevel
                ? fmt::format("{} \x02", entry.reward).c_str() : "");

        if (auto bg = dynamic_cast<CCScale9Sprite*>(row->getChildByID("row-bg")))
            bg->setColor(hasLevel ? ccColor3B{40, 80, 40} : ccColor3B{20, 20, 20});
    }

    if (m_previewLevelCount)
        m_previewLevelCount->setString(
            fmt::format("{}/5", m_levels.size()).c_str());
}

// Save

void GauntletEditPopup::onSave(CCObject* sender) {
    if (m_nameInput->getString().empty() || m_descInput->getString().empty() || (!m_pendingIconPath.has_value() && m_data.iconURL.empty() || m_levels.size() != 5)) {
            Notification::create("Not all fields are completed.", NotificationIcon::Warning)->show();
        return;
    }

    // Pack all state into m_data
    m_data.name          = m_nameInput->getString();
    m_data.description   = m_descInput->getString();
    m_data.nameColor     = m_selectedColor;
    m_data.nodeColor     = m_selectedNodeColor;
    m_data.bgColor       = m_selectedBGColor;
    m_data.bgIndex       = m_bgIndex;
    m_data.accentColor1  = m_selectedAccentColor1;
    m_data.accentColor2  = m_selectedAccentColor2;
    m_data.infoDate      = m_infoDate;
    m_data.infoVersion   = m_infoVersion;
    m_data.infoSuggester = m_infoSuggester;
    m_data.infoAccID     = m_infoAccID;

    // Pack level slots
    for (int i = 0; i < 5; i++) {
        if (i < (int)m_levels.size()) {
            m_data.levels[i].id      = m_levels[i].levelId;
            m_data.levels[i].name    = m_levels[i].levelName;
            m_data.levels[i].creator = m_levels[i].creatorName;
            m_data.levels[i].stars   = m_levels[i].reward;
        } else {
            m_data.levels[i] = {};
        }
    }

    if (m_pendingIconPath.has_value()) {
        m_uploadHolder.spawn(
            GauntletManagerAPI::get()->uploadIcon(m_pendingIconPath.value()),
            [this](web::WebResponse res) {
                if (!res.ok()) {
                    Notification::create("Icon upload failed.", NotificationIcon::Error)->show();
                    return;
                }
                m_data.iconURL = res.json().unwrapOr(matjson::Value())["url"].asString().unwrapOr("");
                m_pendingIconPath.reset();
                doSave();
            }
        );
    } else {
        // iconURL already set from existing data — skip upload
        doSave();
    }
}

void GauntletEditPopup::doSave() {
    if (m_onSaved) m_onSaved(m_data);
    Popup::onClose(nullptr);
}