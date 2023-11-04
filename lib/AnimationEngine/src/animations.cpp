#include "animations.h"
#include "Adafruit_NeoPixel.h"

#define COLOR_RED       0xff0000
#define COLOR_GREEN     0x00ff00
#define COLOR_BLUE      0x0000ff
#define COLOR_YELLOW    0xffff00
#define COLOR_CYAN      0x00ffff
#define COLOR_MAGENTA   0xff00ff
#define COLOR_WHITE     0xffffff
#define COLOR_BLACK     0x000000

RGBColor jump3Animation(uint32_t timeframe) {
    RGBColor colorFrame;

    timeframe %= 2250;
    // cycle between red, green and blue every 750ms
    if (timeframe < 750) {
        colorFrame.rgb32 = COLOR_RED;
    } else if (timeframe >= 750 && timeframe < 1500) {
        colorFrame.rgb32 = COLOR_GREEN;
    } else {
        colorFrame.rgb32 = COLOR_BLUE;
    }

    return colorFrame;
}

RGBColor jump7Animation(uint32_t timeframe) {
    RGBColor colorFrames[7];
    colorFrames[0].rgb32 = COLOR_RED;
    colorFrames[1].rgb32 = COLOR_GREEN;
    colorFrames[2].rgb32 = COLOR_BLUE;
    colorFrames[3].rgb32 = COLOR_YELLOW;
    colorFrames[4].rgb32 = COLOR_CYAN;
    colorFrames[5].rgb32 = COLOR_MAGENTA;
    colorFrames[6].rgb32 = COLOR_WHITE;

    timeframe = timeframe % (750 * 7) / 750;

    return colorFrames[timeframe];
}

RGBColor fade3Animation(uint32_t timeframe) {
    RGBColor colorFrame;
    colorFrame.rgb32 = 0x0;

    timeframe = (timeframe / 3) % 768;

    if (timeframe < 256) {
        colorFrame.red = Adafruit_NeoPixel::sine8(timeframe + 192);
    } else if (timeframe < 512) {
        colorFrame.green = Adafruit_NeoPixel::sine8(timeframe + 192);
    } else {
        colorFrame.blue = Adafruit_NeoPixel::sine8(timeframe + 192);
    }

    return colorFrame;
}

RGBColor fade7Animation(uint32_t timeframe) {
    RGBColor colorFrame;

    colorFrame.rgb32 = Adafruit_NeoPixel::gamma32(Adafruit_NeoPixel::ColorHSV(timeframe * 25));

    return colorFrame;
}

RGBColor flashAnimation(uint32_t timeframe) {
    RGBColor colorFrame;

    timeframe %= 1500;
    if (timeframe < 750) {
        colorFrame.rgb32 = COLOR_WHITE;
    } else {
        colorFrame.rgb32 = COLOR_BLACK;
    }

    return colorFrame;
}
