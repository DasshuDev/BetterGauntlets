#include "GauntletLayer.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

void RedesignedGauntletLayer::editGauntletSingle(std::string title, cocos2d::ccColor3B bgColor, cocos2d::ccColor3B titleColor, cocos2d::ccColor3B highlightColor) {

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCDirector* director = CCDirector::sharedDirector();

    CCNode* corner = CCNode::create();
    corner->setID("corners"_spr);
    corner->setZOrder(-1);
    this->addChild(corner);

    CCLabelBMFont* gauntletTitleText = static_cast<CCLabelBMFont*>(this->getChildByID("title-shadow"));
    CCLabelBMFont* gauntletShadowText = static_cast<CCLabelBMFont*>(this->getChildByID("title"));
    CCLabelBMFont* gauntletOutlineText = CCLabelBMFont::create(std::string("The " + title + " Gauntlet").c_str(), "GR_OxygeneFontOutline.fnt"_spr);
    CCLabelBMFont* gauntletHighlightText = CCLabelBMFont::create(std::string("The " + title + " Gauntlet").c_str(), "GR_OxygeneFontHighlight.fnt"_spr);
    
    gauntletTitleText->setString(std::string("The " + title + " Gauntlet").c_str());
    gauntletShadowText->setString(std::string("The " + title + " Gauntlet").c_str());

    gauntletHighlightText->setID("title-highlight"_spr);
    gauntletHighlightText->setFntFile("GR_OxygeneFontHighlight.fnt"_spr);
    gauntletHighlightText->setScale(0.75);
    gauntletHighlightText->setPositionX(winSize.width / 2 + 2);
    gauntletHighlightText->setPositionY(winSize.height - 30);
    gauntletHighlightText->setZOrder(37);
    
    gauntletOutlineText->setID("title-outline"_spr);
    gauntletOutlineText->setFntFile("GR_OxygeneFontOutline.fnt"_spr);
    gauntletOutlineText->setScale(0.75);
    gauntletOutlineText->setPositionX(winSize.width / 2 - 1.5);
    gauntletOutlineText->setPositionY(winSize.height - 30);

    if (!gauntletTitleText || !gauntletShadowText) {
        return;
    }

    this->addChild(gauntletOutlineText);    
    this->addChild(gauntletHighlightText);

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
    gauntletTitleText->setColor(titleColor);
    gauntletHighlightText->setColor(highlightColor);
    TLColor->setColor(titleColor);
    TRColor->setColor(titleColor);
    TLHighlight->setColor(highlightColor);
    TRHighlight->setColor(highlightColor);
}