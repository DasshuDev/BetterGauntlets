#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/CommentCell.hpp>
#include <Geode/utils/web.hpp>
#include <alphalaneous.badgify/include/Badgify.hpp>
#include "../../Managers/GauntletManagerCache.hpp"

using namespace geode::prelude;
using namespace alpha::badgify;

class $modify(GRProfilePage, ProfilePage) {
    bool init(int accountID, bool ownProfile) {
        if (!ProfilePage::init(accountID, ownProfile)) return false;

        // Re-fetch the manager list on every profile load/reload so newly
        // added (or removed) managers show up without restarting the game.
        GauntletManagerCache::get()->refresh();

        return true;
    }
};

$execute {
    alpha::badgify::registerBadge(
        "GR_badgeManager_001.png"_spr, 
        "Gauntlet Manager", 
        "This user oversees and moderates <cc>Better Gauntlets</c>' custom Gauntlets. They can <cg>add</c>, <cj>edit</c>, and <cr>delete</c> gauntlets added by the mod. This does not apply to the <cy>Lost Gauntlets</c>, which are hosted exclusively by <co>RobTop</c>.", 
        [] (const Badge& badge) {
            if (badge.modStatus == ModStatus::Regular) {
                showBadge(badge, CCSprite::createWithSpriteFrameName("modBadge_01_001.png"));
            }
        }
    );

    setProfileCallback("GR_badgeManager_001.png"_spr, [] (const Badge& badge) {
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