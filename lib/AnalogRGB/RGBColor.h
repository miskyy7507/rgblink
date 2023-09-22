#ifndef _RGBCOLOR_H
#define _RGBCOLOR_H

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

#endif // _RGBCOLOR_H
