#pragma once

#include <cocos2d.h>
#include <Geode/binding/CCSpriteWithHue.hpp>

using namespace geode::prelude;

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
    float m_from = 0;
    float m_to = 0;
};
