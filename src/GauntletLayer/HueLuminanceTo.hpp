#pragma once

#include <cocos2d.h>
#include <Geode/binding/CCSpriteWithHue.hpp>

using namespace geode::prelude;

// Animates a CCSpriteWithHue's luminance from `from` to `to` over `duration`,
// used to flash an island sprite white then fade back to its true colors.
class HueLuminanceTo : public CCActionInterval {
public:
    static HueLuminanceTo* create(float duration, float from, float to) {
        auto ret = new HueLuminanceTo();
        if (ret->initWithDuration(duration)) {
            ret->m_from = from;
            ret->m_to = to;
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    void update(float time) override {
        static_cast<CCSpriteWithHue*>(m_pTarget)->setEvenLuminance(m_from + (m_to - m_from) * time);
    }

private:
    float m_from = 0.f;
    float m_to = 0.f;
};
