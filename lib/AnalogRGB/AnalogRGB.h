#ifndef _ANALOGRGB_H
#define _ANALOGRGB_H

#include <stdint.h>

typedef union {
    /*
    MEMORY LAYOUT (e.g. 0xff8000, orange)
    00000000 11111111 10000000 00000000
    [unused]      red    green     blue
    \_________________________________/-> rgb32
    */
    struct {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    };
    uint32_t rgb32;
} RGBColor;

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
