#include "PagedPopup.hpp"

void PagedPopup::setupPageArrows(float arrowY) {
    if (arrowY < 0.f) arrowY = m_size.height / 2.f;

    auto arrowMenu = CCMenu::create();
    arrowMenu->setID("page-arrow-menu");
    arrowMenu->setPosition({0, 0});
    m_mainLayer->addChild(arrowMenu, 5);

    auto arrowLeft  = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    auto arrowRight = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    arrowRight->setFlipX(true);

    m_pageArrowLeft = CCMenuItemExt::createSpriteExtra(
        arrowLeft,
        [this](CCMenuItemSpriteExtra*) { turnPage(-1); }
    );
    m_pageArrowLeft->setPosition({-15, arrowY});
    m_pageArrowLeft->setID("page-arrow-left");
    arrowMenu->addChild(m_pageArrowLeft);

    m_pageArrowRight = CCMenuItemExt::createSpriteExtra(
        arrowRight,
        [this](CCMenuItemSpriteExtra*) { turnPage(1); }
    );
    m_pageArrowRight->setPosition({m_size.width + 15, arrowY});
    m_pageArrowRight->setID("page-arrow-right");
    arrowMenu->addChild(m_pageArrowRight);

    goToPage(0);
}

void PagedPopup::turnPage(int dir) {
    auto next = std::clamp(m_currentPage + dir, 0, getPageCount() - 1);
    if (next == m_currentPage) return;
    goToPage(next);
}

void PagedPopup::goToPage(int page) {
    m_currentPage = page;
    onPageChanged(page);

    m_pageArrowLeft->setVisible(m_currentPage > 0);
    m_pageArrowRight->setVisible(m_currentPage < getPageCount() - 1);
}
