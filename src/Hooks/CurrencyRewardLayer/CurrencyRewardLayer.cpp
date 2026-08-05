#include "CurrencyRewardLayer.hpp"
#include "../../Data/CustomGauntletManager.hpp"

using namespace geode::prelude;

namespace {
Ref<CurrencyRewardLayer> s_activeCrystalLayer;
}

void GRCurrencyRewardLayer::queueCrystalReward(int amount) {
  s_pendingCrystals = amount;
}

int GRCurrencyRewardLayer::consumePendingCrystalReward() {
  int amount = s_pendingCrystals;
  s_pendingCrystals = 0;
  return amount;
}

void GRCurrencyRewardLayer::showCrystalReward(CCNode *parent,
                                              int rewardAmount) {
  if (rewardAmount <= 0 || !parent)
    return;

  auto winSize = CCDirector::sharedDirector()->getWinSize();

  auto rewardLayer = CurrencyRewardLayer::create(
      0, 0, 0, rewardAmount, CurrencySpriteType::Star, 0,
      CurrencySpriteType::Star, 0, winSize / 2, CurrencyRewardType::Default, 0,
      1);
  if (!rewardLayer)
    return;

  s_activeCrystalLayer = rewardLayer;

  int totalAfter = CustomGauntletManager::get()->getCrystalTotal();
  rewardLayer->m_particlesAdded = false;
  rewardLayer->m_diamonds = 0;
  rewardLayer->incrementDiamondsCount(totalAfter - rewardAmount);

  std::string frameName = "GR_crystal_001.png"_spr;
  CCTexture2D *texture = nullptr;
  CCSpriteFrame *displayFrame = nullptr;
  if (auto crystalSprite = CCSprite::create(frameName.c_str())) {
    crystalSprite->setScale(4.0);
    texture = crystalSprite->getTexture();
    if (texture) {
      displayFrame = CCSpriteFrame::createWithTexture(
          texture, {{0, 0}, texture->getContentSize()});
    }
  }

  if (rewardLayer->m_diamondsSprite && displayFrame)
    rewardLayer->m_diamondsSprite->setDisplayFrame(displayFrame);

  if (rewardLayer->m_currencyBatchNode && texture)
    rewardLayer->m_currencyBatchNode->setTexture(texture);

  for (auto sprite : CCArrayExt<CurrencySprite *>(rewardLayer->m_objects)) {
    if (!sprite)
      continue;
    if (sprite->m_burstSprite)
      sprite->m_burstSprite->setVisible(false);
    if (auto child = sprite->getChildByIndex(0))
      child->setVisible(false);
    if (sprite->m_spriteType == CurrencySpriteType::Diamond && displayFrame) {
      sprite->setDisplayFrame(displayFrame);
    }
  }

  auto tintFrom = ccColor3B({255, 255, 128});
  auto tintTo = ccColor3B({255, 255, 255});

  if (rewardLayer->m_diamondsLabel) {
    rewardLayer->m_diamondsLabel->runAction(
        CCRepeatForever::create(CCSequence::create(
            CCTintTo::create(0.5, tintFrom.r, tintFrom.g, tintFrom.b),
            CCTintTo::create(0.5, tintTo.r, tintTo.g, tintTo.b), nullptr)));
  }

  if (rewardLayer->m_mainNode)
    rewardLayer->m_mainNode->updateLayout();

  parent->addChild(rewardLayer, 100);
}

void GRCurrencyRewardLayer::update(float dt) {
  CurrencyRewardLayer::update(dt);
  if (s_activeCrystalLayer == this && m_mainNode) {
    m_mainNode->updateLayout();
  }
}
