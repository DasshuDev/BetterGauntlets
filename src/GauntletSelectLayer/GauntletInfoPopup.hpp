#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include "../Managers/PagedPopup.hpp"

using namespace geode::prelude;

class GauntletInfoPopup : public PagedPopup {
public:
    struct InfoPage {
        std::string title;
        std::string description;
    };

    static GauntletInfoPopup* create(std::vector<InfoPage> pages);

protected:
    bool init(float width, float height, char const* bg, std::vector<InfoPage> pages);

    std::vector<InfoPage> m_pages;

    CCLabelBMFont* m_pageTitleLabel = nullptr;
    MDTextArea* m_pageDescArea = nullptr;

    int  getPageCount() const override;
    void onPageChanged(int page) override;
};
