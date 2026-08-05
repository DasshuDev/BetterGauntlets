#include "GauntletInfoPopup.hpp"

GauntletInfoPopup *GauntletInfoPopup::create(std::vector<InfoPage> pages) {
  auto ret = new GauntletInfoPopup();
  if (ret && ret->init(380, 200, "square01_001.png", pages)) {
    ret->autorelease();
    return ret;
  }
  delete ret;
  return nullptr;
}

bool GauntletInfoPopup::init(float width, float height, char const *bg, std::vector<InfoPage> pages) {
  if (!Popup::init(width, height, bg))
    return false;
  if (pages.empty())
    return false;

  m_pages = std::move(pages);

  setupPageArrows();

  return true;
}

int GauntletInfoPopup::getPageCount() const { return (int)m_pages.size(); }

void GauntletInfoPopup::onPageChanged(int page) {
  if (m_pageTitleLabel) {
    m_pageTitleLabel->removeFromParent();
    m_pageTitleLabel = nullptr;
  }
  if (m_pageDescArea) {
    m_pageDescArea->removeFromParent();
    m_pageDescArea = nullptr;
  }

  auto const &p = m_pages.at(page);

  float titleY = m_size.height - 20;
  float contentWidth = m_size.width - 40;
  float contentHeight = titleY - 45;

  m_pageTitleLabel = CCLabelBMFont::create(p.title.c_str(), "goldFont.fnt");
  m_pageTitleLabel->setScale(0.7f);
  m_pageTitleLabel->setPosition({m_size.width / 2, titleY - 2.5f});
  m_pageTitleLabel->setID("page-title-label");
  m_mainLayer->addChild(m_pageTitleLabel);

  if (m_pageIndicatorLabel) {
    m_pageIndicatorLabel->setPositionY(m_pageTitleLabel->getPositionY() - 12.5f);
  }

  m_pageDescArea =
      MDTextArea::create(p.description, {contentWidth, contentHeight});
  m_pageDescArea->setPosition({m_size.width / 2, 20 + contentHeight / 2});
  m_pageDescArea->setID("page-desc-area");
  m_mainLayer->addChild(m_pageDescArea);
}
