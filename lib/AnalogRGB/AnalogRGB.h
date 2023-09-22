#ifndef _ANALOGRGB_H
#define _ANALOGRGB_H

#include <stdint.h>

#include "RGBColor.h"

class AnalogRGB {
public:
    AnalogRGB(uint8_t r_pin, uint8_t g_pin, uint8_t b_pin);
    void begin();

    uint8_t getRed();
    uint8_t getGreen();
    uint8_t getBlue();
    uint32_t getRGB();
    RGBColor getRGBColor();
    uint8_t getBrightness();
    bool isOn();

    void setRed(uint8_t value);
    void setGreen(uint8_t value);
    void setBlue(uint8_t value);
    void setRGB(uint32_t value);
    void setRGBColor(RGBColor value);
    void setBrightness(uint8_t value);
    void powerToggle();

    void updateColor();

    void swapColors();

private:
    uint8_t _r_pin;
    uint8_t _g_pin;
    uint8_t _b_pin;

    RGBColor color;

    uint8_t brightness;
    uint8_t _color_swap_counter;
    bool powerState;
};

#endif // _ANALOGRGB_H
