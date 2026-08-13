#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class PagedPopup : public Popup {
protected:
  void setupPageArrows(float arrowY = -1);

  void turnPage(int dir);
  void goToPage(int page);

  int m_currentPage = 0;

  CCMenuItemSpriteExtra *m_pageArrowLeft = nullptr;
  CCMenuItemSpriteExtra *m_pageArrowRight = nullptr;
  CCLabelBMFont *m_pageIndicatorLabel = nullptr;

  virtual int getPageCount() const = 0;
  virtual void onPageChanged(int page) = 0;
};
