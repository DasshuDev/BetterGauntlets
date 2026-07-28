#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/CommentCell.hpp>
#include <Geode/utils/web.hpp>
#include <alphalaneous.badgify/include/Badgify.hpp>
#include "../../Managers/GauntletManagerCache.hpp"
#include "../../Managers/StatsSyncManager.hpp"
#include "../../Data/CustomGauntletManager.hpp"

using namespace geode::prelude;
using namespace alpha::badgify;

class $modify(GRProfilePage, ProfilePage) {
    bool init(int accountID, bool ownProfile) {
        if (!ProfilePage::init(accountID, ownProfile)) return false;
        GauntletManagerCache::get()->refresh();

        if (ownProfile) {
            auto resetMenu = CCMenu::create();
            resetMenu->setID("gr-reset-crystals-menu"_spr);
            resetMenu->setAnchorPoint({1, 1});
            resetMenu->setPosition({this->getContentSize().width - 4, this->getContentSize().height - 4});
            this->addChild(resetMenu, 20);

            auto resetSpr = ButtonSprite::create("Reset Crystals", "bigFont.fnt", "GJ_button_06.png");
            resetSpr->setScale(0.4f);
            auto resetBtn = CCMenuItemExt::createSpriteExtra(resetSpr, [](CCMenuItemSpriteExtra*) {
                createQuickPopup(
                    "Reset Crystals",
                    "Reset your <cr>crystal total</c> to <cy>0</c> and sync that to the server "
                    "immediately? This <cr>cannot be undone</c>.",
                    "Cancel", "Reset",
                    [](FLAlertLayer*, bool confirmed) {
                        if (!confirmed) return;
                        CustomGauntletManager::get()->resetCrystals();
                        StatsSyncManager::get()->sync(0, 0, [](bool success, std::string const& error) {
                            if (!success) {
                                Notification::create(
                                    fmt::format("Reset locally, but sync failed: {}", error),
                                    NotificationIcon::Error
                                )->show();
                                return;
                            }
                            Notification::create("Crystal count reset to 0.", NotificationIcon::Success)->show();
                        });
                    }
                );
            });
            resetBtn->setID("reset-crystals-button"_spr);
            resetMenu->addChild(resetBtn);
        }

        return true;
    }
};

$execute {
    registerBadge(
        "GRManager"_spr, 
        "Gauntlet Manager", 
        "This user oversees and moderates <cc>Better Gauntlets</c>' custom Gauntlets. They can <cg>add</c>, <cj>edit</c>, and <cr>delete</c> gauntlets added by the mod. This does not apply to the <cy>Lost Gauntlets</c>, which are hosted exclusively by <co>RobTop</c>.", 
        [] (const Badge& badge) {
            if (badge.modStatus == ModStatus::Regular) {
                showBadge(badge, CCSprite::createWithSpriteFrameName("modBadge_01_001.png"));
            }
        }
    );
    setCommentColor("GRManager"_spr, { 241, 125, 179 });
    setPriority("GRManager"_spr, 999999999);

    setProfileCallback("GRManager"_spr, [] (const Badge& badge) {
        GauntletManagerCache::get()->isManager(badge.user->m_accountID, [badge](bool isManager) {
            if (!isManager) return;

            if (badge.location == Location::Profile || badge.location == Location::Comment) {
                showBadge(badge, CCSprite::createWithSpriteFrameName("GR_badgeManager_001.png"_spr));
            }
            if (badge.location == Location::InfoPopup) {
                showBadge(badge, CCSprite::createWithSpriteFrameName("GR_badgeManager_xlarge_001.png"_spr));
            }
        });
    });
}