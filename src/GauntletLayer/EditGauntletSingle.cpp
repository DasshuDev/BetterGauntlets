#include "GauntletLayer.hpp"

using namespace geode::prelude;

void RedesignedGauntletLayer::editGauntletSingle(std::string title, cocos2d::ccColor3B bgColor, cocos2d::ccColor3B titleColor, cocos2d::ccColor3B highlightColor) {

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto director = CCDirector::sharedDirector();

    auto gauntletOutlineText = CCLabelBMFont::create(std::string(title + " Gauntlet").c_str(), "GR_OxygeneFontOutline.fnt"_spr);
    auto gauntletHighlightText = CCLabelBMFont::create(std::string(title + " Gauntlet").c_str(), "GR_OxygeneFontHighlight.fnt"_spr);

    auto gauntletTitleText = static_cast<CCLabelBMFont*>(this->getChildByID("title-shadow"));
    if (!gauntletTitleText) return;
    auto gauntletShadowText = static_cast<CCLabelBMFont*>(this->getChildByID("title"));
    if (!gauntletShadowText) return;

    auto cornerParent = CCNode::create();
    cornerParent->setID("corners");
    cornerParent->setZOrder(-1);
    this->addChild(cornerParent);

    auto TLCornerParent = CCNode::create();
    TLCornerParent->setID("top-left");
    TLCornerParent->setPositionY(director->getScreenTop());
    cornerParent->addChild(TLCornerParent);

    auto TRCornerParent = CCNode::create();
    TRCornerParent->setID("top-right");
    TRCornerParent->setPositionY(director->getScreenTop());
    cornerParent->addChild(TRCornerParent);

    // Left Corner
    auto TLCorner = CCSprite::createWithSpriteFrameName("GR_gauntletCorner_001.png"_spr);
    TLCorner->setID("TL-corner");
    TLCorner->setAnchorPoint(ccp(0, 0));
    TLCorner->setPosition(ccp(director->getScreenLeft(), director->getScreenBottom()));
    TLCorner->setScaleY(TLCorner->getScaleY() * -1);
    TLCornerParent->addChild(TLCorner);

    auto TLCornerColor = CCSprite::createWithSpriteFrameName("GR_gauntletCornerColor_002.png"_spr);
    TLCornerColor->setID("TL-color");
    TLCornerColor->setAnchorPoint(ccp(0, 0));
    TLCornerColor->setZOrder(1);
    TLCornerColor->setPosition(ccp(director->getScreenLeft(), director->getScreenBottom()));
    TLCornerColor->setScaleY(TLCornerColor->getScaleY() * -1);
    TLCornerParent->addChild(TLCornerColor);

    auto TLCornerHighlight = CCSprite::createWithSpriteFrameName("GR_gauntletCornerHighlight_001.png"_spr);
    TLCornerHighlight->setID("TL-highlight");
    TLCornerHighlight->setAnchorPoint(ccp(0, 0));
    TLCornerHighlight->setZOrder(1);
    TLCornerHighlight->setPosition(ccp(director->getScreenLeft(), director->getScreenBottom()));
    TLCornerHighlight->setScaleY(TLCornerHighlight->getScaleY() * -1);
    TLCornerParent->addChild(TLCornerHighlight);

    // Right Corner
    auto TRCorner = CCSprite::createWithSpriteFrameName("GR_gauntletCorner_001.png"_spr);
    TRCorner->setID("TR-corner");
    TRCorner->setAnchorPoint(ccp(0, 0));
    TRCorner->setPosition(ccp(director->getScreenRight(), director->getScreenBottom()));
    TRCorner->setScaleY(TRCorner->getScaleY() * -1);
    TRCorner->setScaleX(TRCorner->getScaleX() * -1);
    TRCornerParent->addChild(TRCorner);

    auto TRCornerColor = CCSprite::createWithSpriteFrameName("GR_gauntletCornerColor_002.png"_spr);
    TRCornerColor->setID("TR-color");
    TRCornerColor->setAnchorPoint(ccp(0, 0));
    TRCornerColor->setZOrder(1);
    TRCornerColor->setPosition(ccp(director->getScreenRight(), director->getScreenBottom()));
    TRCornerColor->setScaleY(TRCornerColor->getScaleY() * -1);
    TRCornerColor->setScaleX(TRCornerColor->getScaleX() * -1);
    TRCornerParent->addChild(TRCornerColor);

    auto TRCornerHighlight = CCSprite::createWithSpriteFrameName("GR_gauntletCornerHighlight_001.png"_spr);
    TRCornerHighlight->setID("TR-highlight");
    TRCornerHighlight->setAnchorPoint(ccp(0, 0));
    TRCornerHighlight->setZOrder(1);
    TRCornerHighlight->setPosition(ccp(director->getScreenRight(), director->getScreenBottom()));
    TRCornerHighlight->setScaleY(TRCornerHighlight->getScaleY() * -1);
    TRCornerHighlight->setScaleX(TRCornerHighlight->getScaleX() * -1);
    TRCornerParent->addChild(TRCornerHighlight);

    // Text Highlights
    gauntletHighlightText->setID("title-highlight");
    gauntletHighlightText->setFntFile("GR_OxygeneFontHighlight.fnt"_spr);
    gauntletHighlightText->setScale(0.75);
    gauntletHighlightText->setPositionX(winSize.width / 2 + 2);
    gauntletHighlightText->setPositionY(winSize.height - 30);
    this->addChild(gauntletHighlightText);

    // Text Outlines
    gauntletOutlineText->setID("title-outline");
    gauntletOutlineText->setFntFile("GR_OxygeneFontOutline.fnt"_spr);
    gauntletOutlineText->setScale(0.75);
    gauntletOutlineText->setPositionX(winSize.width / 2 - 1.5);
    gauntletOutlineText->setPositionY(winSize.height - 30);
    this->addChild(gauntletOutlineText);

    // Shadow
    gauntletShadowText->setString(std::string(title + " Gauntlet").c_str());

    // Colors
    m_backgroundSprite->setColor(bgColor);
    gauntletTitleText->setColor(titleColor);
    gauntletHighlightText->setColor(highlightColor);
    TLCornerColor->setColor(titleColor);
    TRCornerColor->setColor(titleColor);
    TLCornerHighlight->setColor(highlightColor);
    TRCornerHighlight->setColor(highlightColor);

    gauntletHighlightText->setZOrder(37);
}