#include "GauntletLayer.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

void RedesignedGauntletLayer::editGauntletLayer(std::string title, cocos2d::ccColor3B bgColor, cocos2d::ccColor3B titleColor, cocos2d::ccColor3B highlightColor) {

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCDirector* director = CCDirector::sharedDirector();

    CCNode* corner = CCNode::create();
    corner->setID("corners"_spr);
    corner->setZOrder(-1);
    this->addChild(corner);

    CCLabelBMFont* titleText = static_cast<CCLabelBMFont*>(this->getChildByID("title-shadow"));
    CCLabelBMFont* shadowText = static_cast<CCLabelBMFont*>(this->getChildByID("title"));
    CCLabelBMFont* outlineText = CCLabelBMFont::create(std::string("The " + title + " Gauntlet").c_str(), "GR_OxygeneFontOutline.fnt"_spr);
    CCLabelBMFont* highlightText = CCLabelBMFont::create(std::string("The " + title + " Gauntlet").c_str(), "GR_OxygeneFontHighlight.fnt"_spr);
    
    titleText->setString(std::string("The " + title + " Gauntlet").c_str());
    shadowText->setString(std::string("The " + title + " Gauntlet").c_str());

    highlightText->setID("title-highlight"_spr);
    highlightText->setFntFile("GR_OxygeneFontHighlight.fnt"_spr);
    highlightText->setScale(0.75);
    highlightText->setPosition(winSize.width / 2 + 2, winSize.height - 30);
    highlightText->setZOrder(37);
    
    outlineText->setID("title-outline"_spr);
    outlineText->setFntFile("GR_OxygeneFontOutline.fnt"_spr);
    outlineText->setScale(0.75);
    outlineText->setPosition(winSize.width / 2 - 1.5, winSize.height - 30);

    if (!titleText || !shadowText) {
        return;
    }

    this->addChild(outlineText);    
    this->addChild(highlightText);

    // ------------------------------------------------------------------------------------ //

    // Left Corner
    CCNode* TLParent = CCNode::create();
    TLParent->setID("top-left"_spr);
    TLParent->setPosition(director->getScreenLeft() - 1, director->getScreenTop() + 1);
    corner->addChild(TLParent);

    CCSprite* TLCorner = CCSprite::createWithSpriteFrameName("GR_gauntletCorner_001.png"_spr);
    TLCorner->setAnchorPoint(ccp(0, 0));
    TLCorner->setScaleY(TLCorner->getScaleY() * -1);
    TLParent->addChild(TLCorner);

    CCSprite* TLColor = CCSprite::createWithSpriteFrameName("GR_gauntletCornerColor_001.png"_spr);
    TLColor->setAnchorPoint(ccp(0, 0));
    TLColor->setZOrder(1);
    TLColor->setScaleY(TLColor->getScaleY() * -1);
    TLParent->addChild(TLColor);

    CCSprite* TLHighlight = CCSprite::createWithSpriteFrameName("GR_gauntletCornerHighlight_001.png"_spr);
    TLHighlight->setAnchorPoint(ccp(0, 0));
    TLHighlight->setZOrder(1);
    TLHighlight->setScaleY(TLHighlight->getScaleY() * -1);
    TLParent->addChild(TLHighlight);

    // ------------------------------------------------------------------------------------ //

    // Right Corner 
    CCNode* TRParent = CCNode::create();
    TRParent->setID("top-right"_spr);
    TRParent->setPosition(director->getScreenRight() + 1, director->getScreenTop() + 1);
    corner->addChild(TRParent);

    CCSprite* TRCorner = CCSprite::createWithSpriteFrameName("GR_gauntletCorner_001.png"_spr);
    TRCorner->setAnchorPoint(ccp(0, 0));
    TRCorner->setScale(TRCorner->getScale() * -1);
    TRParent->addChild(TRCorner);

    CCSprite* TRColor = CCSprite::createWithSpriteFrameName("GR_gauntletCornerColor_001.png"_spr);
    TRColor->setAnchorPoint(ccp(0, 0));
    TRColor->setZOrder(1);
    TRColor->setScale(TRColor->getScale() * -1);
    TRParent->addChild(TRColor);

    CCSprite* TRHighlight = CCSprite::createWithSpriteFrameName("GR_gauntletCornerHighlight_001.png"_spr);
    TRHighlight->setAnchorPoint(ccp(0, 0));
    TRHighlight->setZOrder(1);
    TRHighlight->setScale(TRHighlight->getScale() * -1);
    TRParent->addChild(TRHighlight);

    // ------------------------------------------------------------------------------------ //    

    // Colors
    m_backgroundSprite->setColor(bgColor);
    titleText->setColor(titleColor);
    highlightText->setColor(highlightColor);
    TLColor->setColor(titleColor);
    TRColor->setColor(titleColor);
    TLHighlight->setColor(highlightColor);
    TRHighlight->setColor(highlightColor);
}