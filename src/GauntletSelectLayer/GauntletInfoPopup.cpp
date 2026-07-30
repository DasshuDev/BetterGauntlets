#include "GauntletInfoPopup.hpp"
#include <algorithm>

GauntletInfoPopup* GauntletInfoPopup::create(std::vector<InfoPage> pages) {
    auto ret = new GauntletInfoPopup();
    if (ret && ret->init(380, 200, "GJ_square05.png", pages)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GauntletInfoPopup::init(
    float width, float height, char const* bg, std::vector<InfoPage> pages
) {
    if (!Popup::init(width, height, bg)) return false;
    if (pages.empty()) return false;

    m_pages = std::move(pages);

    auto arrowMenu = CCMenu::create();
    arrowMenu->setID("page-arrow-menu");
    arrowMenu->setPosition({0, 0});
    m_mainLayer->addChild(arrowMenu, 5);

    auto arrowLeft = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    auto arrowRight = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    arrowRight->setFlipX(true);

    m_pageArrowLeft = CCMenuItemExt::createSpriteExtra(
        arrowLeft,
        [this](CCMenuItemSpriteExtra*) { turnPage(-1); }
    );
    m_pageArrowLeft->setPosition({-5, m_size.height / 2.f});
    m_pageArrowLeft->setID("page-arrow-left");
    arrowMenu->addChild(m_pageArrowLeft);

    m_pageArrowRight = CCMenuItemExt::createSpriteExtra(
        arrowRight,
        [this](CCMenuItemSpriteExtra*) { turnPage(1); }
    );
    m_pageArrowRight->setPosition({m_size.width + 5, m_size.height / 2.f});
    m_pageArrowRight->setID("page-arrow-right");
    arrowMenu->addChild(m_pageArrowRight);

    updatePage();

    return true;
}

void GauntletInfoPopup::turnPage(int dir) {
    auto next = std::clamp(m_currentPage + dir, 0, (int)m_pages.size() - 1);
    if (next == m_currentPage) return;

    m_currentPage = next;
    updatePage();
}

void GauntletInfoPopup::updatePage() {
    cleanUpPage();

    auto const& page = m_pages.at(m_currentPage);

    float titleY = m_size.height - 20.f;
    float contentWidth = m_size.width - 40.f;
    float contentHeight = titleY - 35.f;

    m_pageTitleLabel = CCLabelBMFont::create(page.title.c_str(), "goldFont.fnt");
    m_pageTitleLabel->setScale(0.7f);
    m_pageTitleLabel->setPosition({m_size.width / 2.f, titleY});
    m_pageTitleLabel->setID("page-title-label");
    m_mainLayer->addChild(m_pageTitleLabel);

    m_pageDescArea = MDTextArea::create(page.description, {contentWidth, contentHeight});
    m_pageDescArea->setPosition({m_size.width / 2.f, 15.f + contentHeight / 2.f});
    m_pageDescArea->setID("page-desc-area");
    m_mainLayer->addChild(m_pageDescArea);

    m_pageArrowLeft->setVisible(m_currentPage > 0);
    m_pageArrowRight->setVisible(m_currentPage < m_pages.size() - 1);
}

void GauntletInfoPopup::cleanUpPage() {
    if (m_pageTitleLabel) {
        m_pageTitleLabel->removeFromParent();
        m_pageTitleLabel = nullptr;
    }
    if (m_pageDescArea) {
        m_pageDescArea->removeFromParent();
        m_pageDescArea = nullptr;
    }
}
