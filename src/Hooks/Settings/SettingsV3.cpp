#include <Geode/loader/SettingV3.hpp>
#include "../../Managers/StatsSyncManager.hpp"
#include "../../Data/CustomGauntletManager.hpp"

using namespace geode::prelude;

class ResetStatsSettingV3 : public SettingV3 {
public:
    static Result<std::shared_ptr<SettingV3>> parse(
        std::string const& key, std::string const& modID, matjson::Value const& json
    ) {
        auto res = std::make_shared<ResetStatsSettingV3>();
        auto root = checkJson(json, "ResetStatsSettingV3");

        res->init(key, modID, root);
        res->parseNameAndDescription(root);
        res->parseEnableIf(root);

        root.checkUnknownKeys();
        return root.ok(std::static_pointer_cast<SettingV3>(res));
    }

    // No data to save/load - this setting has no value.
    bool load(matjson::Value const& json) override { return true; }
    bool save(matjson::Value& json) const override { return true; }
    bool isDefaultValue() const override { return true; }
    void reset() override {}

    SettingNodeV3* createNode(float width) override;
};

class ResetStatsSettingNodeV3 : public SettingNodeV3 {
protected:
    ButtonSprite* m_buttonSprite;
    CCMenuItemSpriteExtra* m_button;

    bool init(std::shared_ptr<ResetStatsSettingV3> setting, float width) {
        if (!SettingNodeV3::init(setting, width))
            return false;

        m_buttonSprite = ButtonSprite::create("Reset", "goldFont.fnt", "GJ_button_06.png", .8f);
        m_buttonSprite->setScale(.5f);
        m_button = CCMenuItemSpriteExtra::create(
            m_buttonSprite, this, menu_selector(ResetStatsSettingNodeV3::onButton)
        );
        this->getButtonMenu()->addChildAtPosition(m_button, Anchor::Center);
        this->getButtonMenu()->setPositionX(this->getButtonMenu()->getPositionX() + 10);
        this->getButtonMenu()->setContentWidth(60);
        this->getButtonMenu()->updateLayout();

        this->updateState(nullptr);

        return true;
    }

    void updateState(CCNode* invoker) override {
        SettingNodeV3::updateState(invoker);

        auto shouldEnable = this->getSetting()->shouldEnable();
        m_button->setEnabled(shouldEnable);
        m_buttonSprite->setCascadeColorEnabled(true);
        m_buttonSprite->setCascadeOpacityEnabled(true);
        m_buttonSprite->setOpacity(shouldEnable ? 255 : 155);
        m_buttonSprite->setColor(shouldEnable ? ccWHITE : ccGRAY);
    }

    void onButton(CCObject*) {
        createQuickPopup(
            "Hold up!",
            "Resetting your <cy>Crystal</c> and <cg>Coin</c> totals and <cc>custom Gauntlet completions</c> "
            "to <cy>default (0)</c> is <cr>IRREVERSIBLE</c>! Are you sure you want to continue?",
            "Cancel", "Reset",
            [](FLAlertLayer*, bool confirmed) {
                if (!confirmed) return;
                CustomGauntletManager::get()->resetCrystals();
                CustomGauntletManager::get()->resetClaimedRewards();
                StatsSyncManager::get()->resetSelf([](bool success, std::string const& error) {
                    if (!success) {
                        Notification::create(
                            fmt::format("Reset locally, but sync failed. Error {}", error),
                            NotificationIcon::Error
                        )->show();
                        return;
                    }
                    Notification::create("Stats reset successful!", NotificationIcon::Success)->show();
                });
            }
        );
    }

    // No-ops - neither makes sense for a setting with no committable value.
    void onCommit() override {}
    void onResetToDefault() override {}

public:
    static ResetStatsSettingNodeV3* create(std::shared_ptr<ResetStatsSettingV3> setting, float width) {
        auto ret = new ResetStatsSettingNodeV3();
        if (ret->init(setting, width)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    bool hasUncommittedChanges() const override { return false; }
    bool hasNonDefaultValue() const override { return false; }

    std::shared_ptr<ResetStatsSettingV3> getSetting() const {
        return std::static_pointer_cast<ResetStatsSettingV3>(SettingNodeV3::getSetting());
    }
};

SettingNodeV3* ResetStatsSettingV3::createNode(float width) {
    return ResetStatsSettingNodeV3::create(
        std::static_pointer_cast<ResetStatsSettingV3>(shared_from_this()),
        width
    );
}

$on_mod(Loaded) {
    (void)Mod::get()->registerCustomSettingType("reset-stats", &ResetStatsSettingV3::parse);
}