#include <Arduino.h>
#include "AnalogRGB.h"

AnalogRGB::AnalogRGB(uint8_t r_pin, uint8_t g_pin, uint8_t b_pin):
    _r_pin(r_pin),
    _g_pin(g_pin),
    _b_pin(b_pin) {
    this->color.rgb32 = 0x101010;
    this->brightness = 255;
    this->_color_swap_counter = 0;
    this->powerState = false;
};

void AnalogRGB::begin() {
    pinMode(this->_r_pin, OUTPUT);
    pinMode(this->_g_pin, OUTPUT);
    pinMode(this->_b_pin, OUTPUT);
    digitalWrite(this->_r_pin, LOW);
    digitalWrite(this->_g_pin, LOW);
    digitalWrite(this->_b_pin, LOW);
    // this->powerToggle();
    // this->updateColor();
}

uint8_t AnalogRGB::getRed() {
    return this->color.red;
}
uint8_t AnalogRGB::getGreen() {
    return this->color.green;
}
uint8_t AnalogRGB::getBlue() {
    return this->color.blue;
}
uint32_t AnalogRGB::getRGB() {
    return this->color.rgb32;
}
RGBColor AnalogRGB::getRGBColor() {
    return this->color;
}
uint8_t AnalogRGB::getBrightness() {
    return this->brightness;
}
bool AnalogRGB::isOn() {
    return this->powerState;
}

void AnalogRGB::setRed(uint8_t value) {
    this->color.red = value;
}
void AnalogRGB::setGreen(uint8_t value) {
    this->color.green = value;
}
void AnalogRGB::setBlue(uint8_t value) {
    this->color.blue = value;
}
void AnalogRGB::setRGB(uint32_t value) {
    this->color.rgb32 = value;
}
void AnalogRGB::setRGBColor(RGBColor value) {
    this->color = value;
}
void AnalogRGB::setBrightness(uint8_t value) {
    this->brightness = value;
}

void AnalogRGB::updateColor() {
    if (this->powerState == false) {
        return;
    }
    int16_t b1 = this->brightness + 1;
    // range 1-256, allows to use right bitshift instead of slow division
    analogWrite(_r_pin, (this->color.red * b1) >> 8);
    analogWrite(_g_pin, (this->color.green * b1) >> 8);
    analogWrite(_b_pin, (this->color.blue * b1) >> 8);
}

void AnalogRGB::powerToggle() {
    this->powerState = !this->powerState;
    if (this->powerState == true) {
        this->updateColor();
    } else {
        digitalWrite(_r_pin, LOW);
        digitalWrite(_g_pin, LOW);
        digitalWrite(_b_pin, LOW);
    }
}

void AnalogRGB::swapColors() {
    this->_color_swap_counter = (this->_color_swap_counter + 1) % 6;
    uint8_t temp;
    switch (_color_swap_counter) {
        case 0:
        case 3:
        // swap 1st (initially red) and 2nd (init. green) pin
            temp = this->_r_pin;
            this->_r_pin = this->_g_pin;
            this->_g_pin = temp;
            break;
        case 1:
        case 4:
        // swap 1st (init. red) and 3rd (init. blue) pin
            temp = this->_r_pin;
            this->_r_pin = this->_b_pin;
            this->_b_pin = temp;
            break;
        case 2:
        case 5:
        // swap 2nd (init. green) and 3rd (init. blue) pin
            temp = this->_g_pin;
            this->_g_pin = this->_b_pin;
            this->_b_pin = temp;
            break;
    }
}
