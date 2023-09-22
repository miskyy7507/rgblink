#ifndef _ANIMATIONENGINE_H
#define _ANIMATIONENGINE_H

#include "AnalogRGB.h"
#include "RGBColor.h"

class AnimationEngine {
public:
    /// @brief Constructor for animation engine object.
    /// @param light Reference to analog RGB light object that will be controlled by the engine.
    AnimationEngine(AnalogRGB &light);

    /// @brief Starts over animation loop.
    /// @param animFunc Animation to be played.
    void start(RGBColor (*animFunc)(uint32_t timeframe));

    /// @brief Play/resume animation.
    void play();

    /// @brief Pause animation.
    void pause();

    /// @brief Toggle animation. If it's currently paused, it will be resumed, and the other way around.
    void toggle();

    /// @brief Request next animation frame.
    /// @param timestamp Current timestamp in milliseconds.
    void loop(uint32_t timestampMs);

    /// @brief Speeds up animation by 10%.
    void speedUp();

    /// @brief Slows down animation by 10%.
    void slowDown();

    /// @brief Stop animation. Afterwards it cannot be resumed and it needs to be `start()`'ed again.
    void stop();

private:
    AnalogRGB &_light;
    RGBColor (*currentAnimFunc)(uint32_t timeframe);
    bool isPlaying;
    bool isStopped;
    uint32_t animationTimeframe;
    float animationSpeedFactor;
    uint32_t lastRealTimeframeMs;
    uint8_t animationFramerateMs;
};

#endif // _ANIMATIONENGINE_H
