#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/MDTextArea.hpp>

using namespace geode::prelude;

class GauntletInfoPopup : public Popup {
public:
    struct InfoPage {
        std::string title;
        std::string description;
    };

    static GauntletInfoPopup* create(std::vector<InfoPage> pages);

protected:
    bool init(float width, float height, char const* bg, std::vector<InfoPage> pages);

    std::vector<InfoPage> m_pages;
    int m_currentPage = 0;

    CCMenuItemSpriteExtra* m_pageArrowLeft = nullptr;
    CCMenuItemSpriteExtra* m_pageArrowRight = nullptr;
    CCLabelBMFont* m_pageTitleLabel = nullptr;
    MDTextArea* m_pageDescArea = nullptr;

    void turnPage(int dir);
    void updatePage();
    void cleanUpPage();
};
