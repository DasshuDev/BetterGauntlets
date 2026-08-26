#include "CurrencyRewardLayer.hpp"
#include "../../Data/CustomGauntletManager.hpp"
#include "Geode/cocos/actions/CCActionInterval.h"
#include <Geode/binding/RewardUnlockLayer.hpp>

using namespace geode::prelude;

namespace {
Ref<CurrencyRewardLayer> m_activeCrystalLayer;
Ref<CurrencyRewardLayer> m_activeGauntletRewardLayer;

// derive scale from each sprite's actual content size, since the coin and crystal art have very different native pixel dimensions
constexpr float kGauntletCurrencySpriteTargetSize = 24.f;

float gauntletCurrencyTargetScale(CCSprite *sprite) {
  if (!sprite) return 1.f;
  float maxDim = std::max(sprite->getContentSize().width, sprite->getContentSize().height);
  return maxDim > 0.f ? kGauntletCurrencySpriteTargetSize / maxDim : 1.f;
}
}

void GRCurrencyRewardLayer::queueCrystalReward(int amount) {
  m_pendingCrystals = amount;
}

int GRCurrencyRewardLayer::consumePendingCrystalReward() {
  int amount = m_pendingCrystals;
  m_pendingCrystals = 0;
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

  m_activeCrystalLayer = rewardLayer;

  int totalAfter = CustomGauntletManager::get()->getCrystalTotal();
  rewardLayer->m_particlesAdded = false;
  rewardLayer->m_diamonds = 0;
  rewardLayer->incrementDiamondsCount(totalAfter - rewardAmount);

  std::string frameName = "GR_crystal_001.png"_spr;
  CCTexture2D *texture = nullptr;
  CCSpriteFrame *displayFrame = nullptr;
  if (auto crystalSprite = CCSprite::create(frameName.c_str())) {
    texture = crystalSprite->getTexture();
    if (texture) {
      displayFrame = CCSpriteFrame::createWithTexture(texture, {{0, 0}, texture->getContentSize()});
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
  if (m_activeCrystalLayer == this && m_mainNode) {
    m_mainNode->updateLayout();
  }
  if (m_activeGauntletRewardLayer == this) {

    for (auto sprite : CCArrayExt<CurrencySprite *>(m_objects)) {
      if (!sprite) continue;

      float targetScale = gauntletCurrencyTargetScale(sprite);
      sprite->setScale(targetScale);
      if (auto follower = sprite->getFollower()) {
        follower->setScale(targetScale);
      }
      if (sprite->m_particleSystem) {
        sprite->m_particleSystem->setScale(targetScale);
      }
    }

  }
}

void GRCurrencyRewardLayer::setGauntletRewardStylingActive(bool active) {
  m_gauntletRewardStylingActive = active;
  if (!active) m_activeGauntletRewardLayer = nullptr;
}

void GRCurrencyRewardLayer::setGauntletRewardCoins(int coins) {
  m_gauntletRewardCoins = coins;
}

void GRCurrencyRewardLayer::applyGauntletCurrencyLabels(CurrencyRewardLayer *layer) {
  if (!layer) return;

  if (layer->m_diamondsLabel) {
    int coinTotal = CustomGauntletManager::get()->getCoinTotal();
    layer->m_orbs = 0;
    layer->incrementCount(coinTotal);
  }

  if (layer->m_orbsLabel) {
    int crystalTotal = CustomGauntletManager::get()->getCrystalTotal();
    layer->m_diamonds = 0;
    layer->incrementDiamondsCount(crystalTotal);
  }
}

namespace {
CCSpriteFrame *frameFromFile(char const *frameName) {
  auto spr = CCSprite::create(frameName);
  if (!spr) return nullptr;
  auto tex = spr->getTexture();
  if (!tex) return nullptr;
  return CCSpriteFrame::createWithTexture(tex, {{0, 0}, tex->getContentSize()});
}

void removeNonCurrencySprites(CCSpriteBatchNode *batchNode) {
  if (!batchNode) return;
  std::vector<CCNode *> toRemove;
  if (auto children = batchNode->getChildren()) {
    for (auto obj : CCArrayExt<CCNode *>(children)) {
      if (!typeinfo_cast<CurrencySprite *>(obj)) toRemove.push_back(obj);
    }
  }
  for (auto child : toRemove) batchNode->removeChild(child, true);
}

RewardUnlockLayer *findAncestorRewardUnlockLayer(CCNode *node) {
  while (node) {
    if (auto rul = typeinfo_cast<RewardUnlockLayer *>(node)) return rul;
    node = node->getParent();
  }
  return nullptr;
}
} // namespace

void GRCurrencyRewardLayer::restyleForGauntletReward(CurrencyRewardLayer *layer) {
  if (!layer) return;

  m_activeGauntletRewardLayer = layer;

  auto coinFrame = frameFromFile("GR_gauntletCoin_001.png"_spr);
  auto crystalFrame = frameFromFile("GR_crystal_001.png"_spr);

  auto a = ccColor3B({255, 215, 134});
  auto b = ccColor3B({255, 255, 255});
  auto c = ccColor3B({251, 255, 143});

  auto colorHueA = CCRepeatForever::create(CCSequence::create(
      CCTintTo::create(0.35, a.r, a.g, a.b), CCDelayTime::create(0.15),
      CCTintTo::create(0.35, b.r, b.g, b.b), CCDelayTime::create(0.15), nullptr));

  auto colorHueB = CCRepeatForever::create(CCSequence::create(
      CCTintTo::create(0.35, c.r, c.g, c.b), CCDelayTime::create(0.15),
      CCTintTo::create(0.35, b.r, b.g, b.b), CCDelayTime::create(0.15), nullptr));

  if (layer->m_orbsSprite && crystalFrame) {
    layer->m_orbsSprite->setDisplayFrame(crystalFrame);
    layer->m_orbsSprite->setScale(0.4);
  }
  if (layer->m_diamondsSprite && coinFrame) {
    layer->m_diamondsSprite->setDisplayFrame(coinFrame);
  }

  if (layer->m_orbBatchNode && coinFrame) {
    layer->m_orbBatchNode->setTexture(coinFrame->getTexture());
    removeNonCurrencySprites(layer->m_orbBatchNode);
  }
  if (layer->m_currencyBatchNode && crystalFrame) {
    layer->m_currencyBatchNode->setTexture(crystalFrame->getTexture());
    removeNonCurrencySprites(layer->m_currencyBatchNode);
  }

  if (layer->m_orbsLabel && layer->m_diamondsLabel) {
    layer->m_orbsLabel->stopAllActions();
    layer->m_orbsLabel->setColor({b.r, b.g, b.b});
    layer->m_orbsLabel->runAction(colorHueA);
    layer->m_diamondsLabel->stopAllActions();
    layer->m_diamondsLabel->setColor({b.r, b.g, b.b});
    layer->m_diamondsLabel->runAction(colorHueB);
  }
  applyGauntletCurrencyLabels(layer);

  for (auto sprite : CCArrayExt<CurrencySprite *>(layer->m_objects)) {
    if (!sprite) continue;
    if (sprite->m_spriteType == CurrencySpriteType::Diamond) {
      if (crystalFrame) sprite->setDisplayFrame(crystalFrame);
    } else {
      if (coinFrame) sprite->setDisplayFrame(coinFrame);
    }
    sprite->stopAllActions();
    float targetScale = gauntletCurrencyTargetScale(sprite);
    sprite->setScale(targetScale);

    if (auto follower = sprite->getFollower()) {
      follower->stopAllActions();
      follower->setScale(targetScale);
    }
    if (sprite->m_particleSystem) {
      sprite->m_particleSystem->stopAllActions();
      sprite->m_particleSystem->setScale(targetScale);
    }
  }

  if (layer->m_mainNode) layer->m_mainNode->updateLayout();
}

void GRCurrencyRewardLayer::restyleRewardUnlockLayerIcons(CurrencyRewardLayer *layer) {
  if (!layer) {
    log::warn("GRCurrencyRewardLayer::restyleRewardUnlockLayerIcons - layer is null");
    return;
  }

  auto rewardUnlockLayer = findAncestorRewardUnlockLayer(layer);
  if (!rewardUnlockLayer) {
    auto parent = layer->getParent();
    return;
  }

  auto innerLayer = rewardUnlockLayer->getChildByIndex(0);
  if (!innerLayer) {
    log::warn("GRCurrencyRewardLayer::restyleRewardUnlockLayerIcons - RewardUnlockLayer has no child 0 (innerLayer)");
    return;
  }
  log::debug(
      "GRCurrencyRewardLayer::restyleRewardUnlockLayerIcons - found innerLayer with {} children",
      innerLayer->getChildrenCount()
  );

  auto coinFrame = frameFromFile("GR_gauntletCoin_001.png"_spr);
  auto crystalFrame = frameFromFile("GR_crystal_001.png"_spr);
  if (!coinFrame) log::error("GRCurrencyRewardLayer::restyleRewardUnlockLayerIcons - failed to load GR_gauntletCoin_001.png");
  if (!crystalFrame) log::error("GRCurrencyRewardLayer::restyleRewardUnlockLayerIcons - failed to load GR_crystal_001.png");
  
  if (auto diamondWrapper = innerLayer->getChildByIndex(13)) {
      if (auto diamondIcon = typeinfo_cast<CCSprite *>(diamondWrapper->getChildByIndex(0))) {
          if (crystalFrame) {
              diamondIcon->setDisplayFrame(crystalFrame);
              diamondIcon->stopAllActions();
              diamondIcon->runAction(CCSequence::create(
                CCDelayTime::create(0.45),
                CCFadeTo::create(0.25, 255),
                nullptr
            ));
              diamondIcon->setScale(gauntletCurrencyTargetScale(diamondIcon));
            }
        }
    }
    
  if (auto orbWrapper = innerLayer->getChildByIndex(14)) {
    if (auto orbIcon = typeinfo_cast<CCSprite *>(orbWrapper->getChildByIndex(0))) {
        if (coinFrame) {
        orbIcon->setDisplayFrame(coinFrame);
        orbIcon->stopAllActions();
        orbIcon->runAction(CCSequence::create(
            CCDelayTime::create(0.65),
            CCFadeTo::create(0.25, 255),
            nullptr
        ));
        orbIcon->setScale(gauntletCurrencyTargetScale(orbIcon));
        }
    }
  }
}

bool GRCurrencyRewardLayer::init(
    int orbs, int stars, int moons, int diamonds,
    CurrencySpriteType demonKey, int keyCount,
    CurrencySpriteType shardType, int shardsCount,
    CCPoint position, CurrencyRewardType rewardType,
    float yOffset, float time
) {
  log::debug(
      "[X] GRCurrencyRewardLayer::init - entered, orbs={} diamonds={} stylingActive={}",
      orbs, diamonds, m_gauntletRewardStylingActive
  );

  if (!CurrencyRewardLayer::init(
          orbs, stars, moons, diamonds, demonKey, keyCount, shardType,
          shardsCount, position, rewardType, yOffset, time
      ))
    return false;
  log::debug("[X] GRCurrencyRewardLayer::init - base CurrencyRewardLayer::init succeeded");

  if (m_gauntletRewardStylingActive) {
    restyleForGauntletReward(this);
    log::debug("[X] GRCurrencyRewardLayer::init - restyleForGauntletReward returned, scheduling delayed icon fix");

    this->runAction(CCSequence::create(
        CCDelayTime::create(0.05f),
        CallFuncExt::create([this] {
          log::debug("[X] GRCurrencyRewardLayer::init - delayed action fired, calling restyleRewardUnlockLayerIcons");
          restyleRewardUnlockLayerIcons(this);
        }),
        nullptr
    ));
  } else {
    log::debug("[X] GRCurrencyRewardLayer::init - gauntlet reward styling not active, skipping restyle");
  }

  return true;
}