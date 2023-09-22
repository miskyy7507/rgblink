#include "AnimationEngine.h"
#include <Arduino.h>

AnimationEngine::AnimationEngine(AnalogRGB& light):
    _light(light)
{
    this->currentAnimFunc = nullptr;
    this->isPlaying = false;
    this->isStopped = true;
    this->animationSpeedFactor = 1.0f;
    this->animationFramerateMs = 8;
}

void AnimationEngine::start(RGBColor(*animFunc)(uint32_t timeframe)) {
    this->currentAnimFunc = animFunc;
    this->animationTimeframe = 0;
    this->isStopped = false;
    this->play();
}

void AnimationEngine::play() {
    if (this->isStopped) {
        return;
    }
    this->lastRealTimeframeMs = millis();
    this->isPlaying = true;
}

void AnimationEngine::pause() {
    if (this->isStopped) {
        return;
    }
    this->isPlaying = false;
}

void AnimationEngine::toggle() {
    if (this->isStopped) {
        return;
    }
    if (this->isPlaying) {
        this->pause();
    } else {
        this->play();
    }
}

void AnimationEngine::speedUp() {
    if (this->isStopped) {
        return;
    }
    float newAnimationSpeedFactor = this->animationSpeedFactor *= 1.1;
    this->animationSpeedFactor = (newAnimationSpeedFactor > 30 ? 30 : newAnimationSpeedFactor);
}

void AnimationEngine::slowDown() {
    if (this->isStopped) {
        return;
    }
    float newAnimationSpeedFactor = this->animationSpeedFactor /= 1.1;
    this->animationSpeedFactor = (newAnimationSpeedFactor < 0.125 ? 0.125 : newAnimationSpeedFactor);
}

void AnimationEngine::loop(uint32_t timestampMs) {
    if (this->isStopped) {
        return;
    }
    if (((timestampMs - this->lastRealTimeframeMs) < this->animationFramerateMs) || !this->isPlaying) {
        return;
    }
    this->animationTimeframe += (timestampMs - this->lastRealTimeframeMs) * this->animationSpeedFactor;
    this->lastRealTimeframeMs = timestampMs;
    this->_light.setRGBColor(this->currentAnimFunc(this->animationTimeframe));
    this->_light.updateColor();
}

void AnimationEngine::stop() {
    this->isStopped = true;
}
