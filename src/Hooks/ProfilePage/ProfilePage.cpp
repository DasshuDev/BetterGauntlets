#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/CommentCell.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

static const int MANAGER_ACCOUNT_ID = 1975253; // placeholder until isManager is added by Supernova backend

static void addManagerBadge(CCMenu* usernameMenu) {

    if (!usernameMenu) return;
    if (usernameMenu->getChildByID("gauntlet-manager-badge"_spr)) return;

    auto sprite = CCSprite::createWithSpriteFrameName("GR_badgeManager_001.png"_spr);
    if (!sprite) return;

    auto badge = CCMenuItemExt::createSpriteExtra(sprite, [](CCMenuItemSpriteExtra*) {
        FLAlertLayer::create(
            nullptr,
            "Gauntlet Manager",
            "This user oversees and moderates <cc>Better Gauntlets</c>' custom Gauntlets. They can <cg>add</c>, <cj>edit</c>, and <cr>delete</c> gauntlets added by the mod. This does not apply to the <cy>Lost Gauntlets</c>, which are hosted exclusively by <co>RobTop</c>.",   
            "OK", nullptr, 300.f
        )->show();
    });

    // ID must contain "-badge" for BadgesAPI to detect and position it correctly
    badge->setID("gauntlet-manager-badge:999"_spr);
    usernameMenu->addChild(badge);
    usernameMenu->updateLayout();
}

class $modify(MyProfilePage, ProfilePage) {
    struct Fields {
        async::TaskHolder<web::WebResponse> m_checkHolder;
    };

    void loadPageFromUserInfo(GJUserScore* a2) {
        ProfilePage::loadPageFromUserInfo(a2);

        if (a2->m_accountID != MANAGER_ACCOUNT_ID) return;

        auto layer = m_mainLayer;
        if (!layer) return;

        auto usernameMenu = static_cast<CCMenu*>(layer->getChildByIDRecursive("username-menu"));
        addManagerBadge(usernameMenu);
    }
};

class $modify(MyCommentCell, CommentCell) {
    void loadFromComment(GJComment* comment) {
        CommentCell::loadFromComment(comment);

        if (comment->m_accountID != MANAGER_ACCOUNT_ID) return;

        auto layer = m_mainLayer;
        if (!layer) return;

        auto usernameMenu = static_cast<CCMenu*>(layer->getChildByIDRecursive("username-menu"));
        addManagerBadge(usernameMenu);

        auto badge = static_cast<CCMenu*>(layer->getChildByIDRecursive("gauntlet-manager-badge:999"_spr));
        if (!badge) return;
        badge->setScale(0.7);
    }
};