#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

// Shared base for popups that flip between a fixed set of pages using
// left/right arrow buttons anchored just outside the popup's edges. This only
// owns the arrows, the current page index, and turning/clamping between pages -
// subclasses are responsible for building and tearing down their own page
// content in onPageChanged().
class PagedPopup : public Popup {
protected:
  // Call once, after all pages' content nodes exist, to create the arrow
  // buttons and jump to page 0. Defaults to vertical center of the popup.
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
