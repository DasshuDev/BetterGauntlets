// all of this was stolen from Rated Layouts by ArcticWoof ( hope you dont mind :3 )

#include "DialogIcons.hpp"
#include <Geode/modify/DialogLayer.hpp>
#include <unordered_map>

namespace {
    std::unordered_map<DialogObject*, std::string> g_customIconFrames;
}

namespace DialogIcon {

    void setDialogObjectIcon(DialogLayer* dialog, int characterFrame) {
        if (!dialog || !dialog->m_mainLayer || !dialog->m_characterSprite) {
            return;
        }

        dialog->m_characterSprite->setVisible(false);
        int iconFrame = characterFrame - 1;
        if (characterFrame <= 0) {
            iconFrame = 0;
        }
        if (iconFrame < 0 || iconFrame >= DialogIconCount) {
            iconFrame = 0;
        }

        for (int frame = 0; frame < DialogIconCount; frame++) {
            int tag = DialogIconTagBase + frame;
            int bgTag = tag + 1000;
            
            auto icon = typeinfo_cast<CCSprite*>(dialog->m_mainLayer->getChildByTag(tag));
            auto iconBG = typeinfo_cast<CCSprite*>(dialog->m_mainLayer->getChildByTag(bgTag));
            
            if (!icon) {
                auto frameName = fmt::format("dialogIcon_{:02}.png"_spr, frame);
                icon = CCSprite::createWithSpriteFrameName(frameName.c_str());
                if (!icon) {
                    continue;
                }
                icon->setPosition(dialog->m_characterSprite->getPosition());
                icon->setTag(tag);
                dialog->m_mainLayer->addChild(icon, 3);
            }
            
            if (!iconBG) {
                iconBG = CCSprite::create("dialogIcon_blank.png"_spr);
                iconBG->setPosition(dialog->m_characterSprite->getPosition());
                iconBG->setTag(bgTag);
                dialog->m_mainLayer->addChild(iconBG, 2);
            }
            
            icon->setPosition(dialog->m_characterSprite->getPosition());
            iconBG->setPosition(dialog->m_characterSprite->getPosition());
            
            icon->setVisible(frame == iconFrame);
            iconBG->setVisible(frame == iconFrame);
        }
    }

    void setDialogObjectCustomIcon(DialogLayer* dialog, const std::string& frameName) {
        if (!dialog || !dialog->m_mainLayer || !dialog->m_characterSprite) {
            return;
        }

        dialog->m_characterSprite->setVisible(false);

        for (int frame = 0; frame < DialogIconCount; frame++) {
            int tag = DialogIconTagBase + frame;
            int bgTag = tag + 1000;
            
            if (auto existing = typeinfo_cast<CCSprite*>(dialog->m_mainLayer->getChildByTag(tag))) {
                existing->setVisible(false);
            }
            if (auto existingBG = typeinfo_cast<CCSprite*>(dialog->m_mainLayer->getChildByTag(bgTag))) {
                existingBG->setVisible(false);
            }
        }

        static constexpr int CustomDialogIconTag = 0xD1A200;
        static constexpr int CustomDialogIconBGTag = 0xD1A201;
        
        auto iconBG = typeinfo_cast<CCSprite*>(dialog->m_mainLayer->getChildByTag(CustomDialogIconBGTag));
        if (!iconBG) {
            iconBG = CCSprite::createWithSpriteFrameName("dialogIcon_blank.png"_spr);
            iconBG->setTag(CustomDialogIconBGTag);
            dialog->m_mainLayer->addChild(iconBG, 2);
        }
        
        auto icon = typeinfo_cast<CCSprite*>(dialog->m_mainLayer->getChildByTag(CustomDialogIconTag));
        if (!icon) {
            icon = CCSprite::createWithSpriteFrameName(frameName.c_str());
            if (!icon) {
                return;
            }
            icon->setTag(CustomDialogIconTag);
            dialog->m_mainLayer->addChild(icon, 3);
        } else if (auto newFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName.c_str())) {
            icon->setDisplayFrame(newFrame);
        }

        iconBG->setPosition(dialog->m_characterSprite->getPosition());
        iconBG->setVisible(true);

        icon->setPosition(dialog->m_characterSprite->getPosition());
        icon->setVisible(true);
    }

    void setDialogSequenceCustomIcons(const std::vector<DialogObject*>& objects, const std::vector<std::string>& frameNames) {
        for (size_t i = 0; i < objects.size() && i < frameNames.size(); i++) {
            if (objects[i]) {
                g_customIconFrames[objects[i]] = frameNames[i];
            }
        }
    }
}

class $modify(DialogIconDisplayHook, DialogLayer) {
    void displayDialogObject(DialogObject* object) {
        DialogLayer::displayDialogObject(object);

        if (!object) {
            return;
        }

        auto it = g_customIconFrames.find(object);
        if (it != g_customIconFrames.end()) {
            DialogIcon::setDialogObjectCustomIcon(this, it->second);
        }
    }

    void onClose() {
        if (m_dialogObjects) {
            for (auto* obj : CCArrayExt<DialogObject*>(m_dialogObjects)) {
                g_customIconFrames.erase(obj);
            }
        }
        DialogLayer::onClose();
    }
};