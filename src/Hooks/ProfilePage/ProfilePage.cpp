#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/utils/web.hpp>
#include <dasshu.badgified/include/Badgified.hpp>
#include "../../APIs/GauntletManagerCache.hpp"


using namespace geode::prelude;
using namespace dasshu::badgified;

class $modify(GRProfilePage, ProfilePage) {
    bool init(int accountID, bool ownProfile) {
        if (!ProfilePage::init(accountID, ownProfile)) return false;
        GauntletManagerCache::get()->refresh();

        return true;
    }
};

$execute {

    // Managers
    registerBadge(
        "manager"_spr, 
        "Gauntlet Manager", 
        "This user oversees and moderates <cc>Better Gauntlets</c>' custom Gauntlets. "
        "They can <cg>add</c>, <cj>edit</c>, and <cr>delete</c> gauntlets added by the mod. "
        "This does not apply to the <cy>Lost Gauntlets</c>, which are hosted exclusively by <co>RobTop</c>.", 
        [] (const Badge& badge) {
            if (badge.modStatus == ModStatus::Regular) showBadge(badge, CCSprite::createWithSpriteFrameName("modBadge_01_001.png"));
        }
    );

    setProfileCallback(
        "manager"_spr,
        [] (const Badge& badge) {
        GauntletManagerCache::get()->isManager(badge.user->m_accountID, [badge](bool isManager) {
            if (!isManager) return;
            if (badge.location == Location::Profile || badge.location == Location::Comment) showBadge(badge, CCSprite::createWithSpriteFrameName("GR_badgeManager_001.png"_spr));
            if (badge.location == Location::InfoPopup) showBadge(badge, CCSprite::createWithSpriteFrameName("GR_badgeManager_xlarge_001.png"_spr));
        });
    });

    // Helpers
    registerBadge(
        "helper"_spr,
        "Gauntlet Helper",
        "This user helps out with <cc>Better Gauntlets</c>' custom Gauntlets. "
        "They assist the <cj>Gauntlet Managers</c> but do not have full edit access themselves.",
        [] (const Badge& badge) {
            if (badge.modStatus == ModStatus::Regular) showBadge(badge, CCSprite::createWithSpriteFrameName("modBadge_01_001.png"));
        }
    );

    setProfileCallback(
        "helper"_spr,
        [] (const Badge& badge) {
        GauntletManagerCache::get()->isHelper(badge.user->m_accountID, [badge](bool isHelper) {
            if (!isHelper) return;
            if (badge.location == Location::Profile || badge.location == Location::Comment) showBadge(badge, CCSprite::createWithSpriteFrameName("GR_badgeHelper_001.png"_spr));
            if (badge.location == Location::InfoPopup) showBadge(badge, CCSprite::createWithSpriteFrameName("GR_badgeHelper_xlarge_001.png"_spr));
        });
    });

    // Comment colors
    setCommentColor("manager"_spr, ccc3(255, 185, 225));
    setCommentColor("helper"_spr, ccc3(150, 210, 225));
}