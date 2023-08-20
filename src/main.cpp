#include <Arduino.h>

#include "Adafruit_NeoPixel.h"

#define NO_LED_FEEDBACK_CODE
#define IR_RECEIVE_PIN 2
#include "TinyIRReceiver.hpp"

#include "AnalogRGB.h"

#include "remoteButtonsCodes.h"
#include "predefinedColors.h"

#define R_PIN 9
#define G_PIN 11
#define B_PIN 10
AnalogRGB rgbStrip(R_PIN, G_PIN, B_PIN);

uint8_t lastReceivedCommand;
bool lastReceivedCommandDone = true;
uint16_t lastReceivedCommandRepeatCount;
uint32_t lastReceivedCommandTimestamp;
int8_t diyColorIndex = -1;
RGBColor diyColors[6] = {};
const int8_t animationMillisDur = 5; // 200 FPS
enum AnimationMode {jump3, jump7, fade3, fade7, flash, automatic, none = -1};
AnimationMode currentAnimationMode = none;
uint32_t lastAnimationFrameTimestamp;
uint16_t lastAnimationFrameIndex;
uint16_t animationSpeed = 128;
uint16_t fade7Hue = 0;
bool isAnimationPaused = false;

void setPredefinedColor(uint32_t newColor) {
    currentAnimationMode = none;
    diyColorIndex = -1;
    rgbStrip.setRGB(newColor);
    rgbStrip.updateColor();
}

void setDiyColor(int8_t index) {
    currentAnimationMode = none;
    if (diyColorIndex == index) { // save the diy color
        diyColors[diyColorIndex] = rgbStrip.getRGBColor();
    } else {
        diyColorIndex = index;
        rgbStrip.setRGBColor(diyColors[diyColorIndex]);
        rgbStrip.updateColor();
    }
}

void controlDiyColor(uint8_t command, uint16_t commandRepeatCount) {
    if (diyColorIndex == -1) {
        return;
    }
    switch (command) {
    int16_t newColorValue;
    case REMOTE_R_UP:
        newColorValue = (rgbStrip.getRed() + ((commandRepeatCount > 5) ? 8 : 1));
        rgbStrip.setRed(newColorValue > 255 ? 255 : newColorValue);
        rgbStrip.updateColor();
        break;
    case REMOTE_G_UP:
        newColorValue = (rgbStrip.getGreen() + ((commandRepeatCount > 5) ? 8 : 1));
        rgbStrip.setGreen(newColorValue > 255 ? 255 : newColorValue);
        rgbStrip.updateColor();
        break;
    case REMOTE_B_UP:
        newColorValue = (rgbStrip.getBlue() + ((commandRepeatCount > 5) ? 8 : 1));
        rgbStrip.setBlue(newColorValue > 255 ? 255 : newColorValue);
        rgbStrip.updateColor();
        break;
    case REMOTE_R_DOWN:
        newColorValue = (rgbStrip.getRed() - ((commandRepeatCount > 5) ? 8 : 1));
        rgbStrip.setRed(newColorValue < 0 ? 0 : newColorValue);
        rgbStrip.updateColor();
        break;
    case REMOTE_G_DOWN:
        newColorValue = (rgbStrip.getGreen() - ((commandRepeatCount > 5) ? 8 : 1));
        rgbStrip.setGreen(newColorValue < 0 ? 0 : newColorValue);
        rgbStrip.updateColor();
        break;
    case REMOTE_B_DOWN:
        newColorValue = (rgbStrip.getBlue() - ((commandRepeatCount > 5) ? 8 : 1));
        rgbStrip.setBlue(newColorValue < 0 ? 0 : newColorValue);
        rgbStrip.updateColor();
        break;
    }
}

void jump3Animation() {
    currentAnimationMode = jump3;
    uint32_t animationColors[] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE};
    while (lastReceivedCommandDone) {
        if (((millis() - lastAnimationFrameTimestamp) < animationMillisDur * animationSpeed) || isAnimationPaused) {
            continue;
        }
        rgbStrip.setRGB(animationColors[lastAnimationFrameIndex]);
        rgbStrip.updateColor();
        lastAnimationFrameTimestamp = millis();
        lastAnimationFrameIndex = (lastAnimationFrameIndex + 1) % 3;
    }
}

void jump7Animation() {
    currentAnimationMode = jump7;
    uint32_t animationColors[] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_YELLOW, COLOR_PURPLE, COLOR_CYAN, COLOR_WHITE};

    while (lastReceivedCommandDone) {
        if (((millis() - lastAnimationFrameTimestamp) < animationMillisDur * animationSpeed) || isAnimationPaused) {
            continue;
        }
        rgbStrip.setRGB(animationColors[lastAnimationFrameIndex]);
        rgbStrip.updateColor();
        lastAnimationFrameTimestamp = millis();
        lastAnimationFrameIndex = (lastAnimationFrameIndex + 1) % 7;
    }
}

void fade3Animation() {
    currentAnimationMode = fade3;

    while (lastReceivedCommandDone) {
        if (((millis() - lastAnimationFrameTimestamp) < animationMillisDur * animationSpeed) || isAnimationPaused) {
            continue;
        }
        rgbStrip.setRGB(COLOR_BLACK);
        if (lastAnimationFrameIndex < 256) {
            rgbStrip.setRed(Adafruit_NeoPixel::sine8(lastAnimationFrameIndex + 192));
        } else if (lastAnimationFrameIndex < 512) {
            rgbStrip.setGreen(Adafruit_NeoPixel::sine8(lastAnimationFrameIndex + 192));
        } else {
            rgbStrip.setBlue(Adafruit_NeoPixel::sine8(lastAnimationFrameIndex + 192));
        }
        rgbStrip.updateColor();
        lastAnimationFrameTimestamp = millis();
        lastAnimationFrameIndex = (lastAnimationFrameIndex + 1) % 768;
    }
}

void fade7Animation() {
    currentAnimationMode = fade7;
    while (lastReceivedCommandDone) {
        if (((millis() - lastAnimationFrameTimestamp) < animationMillisDur) || isAnimationPaused) {
            continue;
        }
        rgbStrip.setRGB(Adafruit_NeoPixel::gamma32(Adafruit_NeoPixel::ColorHSV(fade7Hue)));
        rgbStrip.updateColor();
        lastAnimationFrameTimestamp = millis();
        fade7Hue += animationSpeed;
    }
}

void flashAnimation() {
    currentAnimationMode = flash;
    uint32_t animationColors[] = {COLOR_WHITE, COLOR_BLACK};

    while (lastReceivedCommandDone) {
        if (((millis() - lastAnimationFrameTimestamp) < animationMillisDur * animationSpeed) || isAnimationPaused) {
            continue;
        }
        rgbStrip.setRGB(animationColors[lastAnimationFrameIndex]);
        rgbStrip.updateColor();
        lastAnimationFrameTimestamp = millis();
        lastAnimationFrameIndex = (lastAnimationFrameIndex + 1) % 2;
    }
}

void setup() {
    // turn off built-in led
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    rgbStrip.begin();

    initPCIInterruptForTinyReceiver();
}

void loop() {
    if (rgbStrip.isOn()) {
        switch (currentAnimationMode) {
        case jump3:
            jump3Animation();
            break;
        case jump7:
            jump7Animation();
            break;
        case flash:
            flashAnimation();
            break;
        case fade3:
            fade3Animation();
            break;
        case fade7:
            fade7Animation();
            break;
        case none:
            break;
        }
    }

    if (lastReceivedCommandDone) {
        return;
    }

    if (!(rgbStrip.isOn())) {
        if (lastReceivedCommandRepeatCount == 0) {
            switch (lastReceivedCommand) {
            case REMOTE_POWER:
                rgbStrip.powerToggle();
                break;
            case REMOTE_FADE7: // swap color channels with FADE7 button when off
                rgbStrip.swapColors();
                break;
            }
        }
        lastReceivedCommandDone = true;
        return;
    }

    lastReceivedCommandDone = true;

    switch (lastReceivedCommand) {
    int16_t newBrightness;
    int16_t newAnimationSpeed;

    case REMOTE_POWER:
        if (lastReceivedCommandRepeatCount == 0) {
            rgbStrip.powerToggle();
        }
        break;
    case REMOTE_PLAY_PAUSE:
        if (currentAnimationMode != none) {
            isAnimationPaused = !isAnimationPaused;
        }

    case REMOTE_BRIGHTNESS_UP:
        newBrightness = (rgbStrip.getBrightness() + ((lastReceivedCommandRepeatCount > 5) ? 8 : 1));
        rgbStrip.setBrightness(newBrightness > 255 ? 255 : newBrightness);
        rgbStrip.updateColor();
        break;
    case REMOTE_BRIGHTNESS_DOWN:
        newBrightness = (rgbStrip.getBrightness() - ((lastReceivedCommandRepeatCount > 5) ? 8 : 1));
        rgbStrip.setBrightness(newBrightness < 0 ? 0 : newBrightness);
        rgbStrip.updateColor();
        break;

    case REMOTE_RED:
        setPredefinedColor(COLOR_RED);
        break;
    case REMOTE_GREEN:
        setPredefinedColor(COLOR_GREEN);
        break;
    case REMOTE_BLUE:
        setPredefinedColor(COLOR_BLUE);
        break;
    case REMOTE_WHITE:
        setPredefinedColor(COLOR_WHITE);
        break;

    case REMOTE_RED_ORANGE:
        setPredefinedColor(COLOR_RED_ORANGE);
        break;
    case REMOTE_LIME:
        setPredefinedColor(COLOR_LIME);
        break;
    case REMOTE_LIGHT_BLUE:
        setPredefinedColor(COLOR_LIGHT_BLUE);
        break;
    case REMOTE_WARM_WHITE:
        setPredefinedColor(COLOR_WARM_WHITE);
        break;

    case REMOTE_ORANGE:
        setPredefinedColor(COLOR_ORANGE);
        break;
    case REMOTE_CYAN:
        setPredefinedColor(COLOR_CYAN);
        break;
    case REMOTE_SLATE_BLUE:
        setPredefinedColor(COLOR_SLATE_BLUE);
        break;
    case REMOTE_WARMER_WHITE:
        setPredefinedColor(COLOR_WARMER_WHITE);
        break;

    case REMOTE_LIGHT_ORANGE:
        setPredefinedColor(COLOR_LIGHT_ORANGE);
        break;
    case REMOTE_AQUA:
        setPredefinedColor(COLOR_AQUA);
        break;
    case REMOTE_DARK_PURPLE:
        setPredefinedColor(COLOR_DARK_PURPLE);
        break;
    case REMOTE_COLD_WHITE:
        setPredefinedColor(COLOR_COLD_WHITE);
        break;

    case REMOTE_YELLOW:
        setPredefinedColor(COLOR_YELLOW);
        break;
    case REMOTE_TEAL:
        setPredefinedColor(COLOR_TEAL);
        break;
    case REMOTE_PURPLE:
        setPredefinedColor(COLOR_PURPLE);
        break;
    case REMOTE_COLDER_WHITE:
        setPredefinedColor(COLOR_COLDER_WHITE);
        break;


    case REMOTE_DIY1:
        if (lastReceivedCommandRepeatCount == 0) {
            setDiyColor(0);
        }
        break;
    case REMOTE_DIY2:
        if (lastReceivedCommandRepeatCount == 0) {
            setDiyColor(1);
        }
        break;
    case REMOTE_DIY3:
        if (lastReceivedCommandRepeatCount == 0) {
            setDiyColor(2);
        }
        break;
    case REMOTE_DIY4:
        if (lastReceivedCommandRepeatCount == 0) {
            setDiyColor(3);
        }
        break;
    case REMOTE_DIY5:
        if (lastReceivedCommandRepeatCount == 0) {
            setDiyColor(4);
        }
        break;
    case REMOTE_DIY6:
        if (lastReceivedCommandRepeatCount == 0) {
            setDiyColor(5);
        }
        break;
    case REMOTE_R_UP:
    case REMOTE_G_UP:
    case REMOTE_B_UP:
    case REMOTE_R_DOWN:
    case REMOTE_G_DOWN:
    case REMOTE_B_DOWN:
        controlDiyColor(lastReceivedCommand, lastReceivedCommandRepeatCount);
        break;

    case REMOTE_JUMP3:
        currentAnimationMode = jump3;
        isAnimationPaused = false;
        lastAnimationFrameTimestamp = 0;
        lastAnimationFrameIndex = 0;
        jump3Animation();
        break;
    case REMOTE_JUMP7:
        currentAnimationMode = jump7;
        isAnimationPaused = false;
        lastAnimationFrameTimestamp = 0;
        lastAnimationFrameIndex = 0;
        jump7Animation();
        break;
    case REMOTE_FADE3:
        currentAnimationMode = fade3;
        isAnimationPaused = false;
        lastAnimationFrameTimestamp = 0;
        lastAnimationFrameIndex = 0;
        fade3Animation();
        break;
    case REMOTE_FADE7:
        currentAnimationMode = fade7;
        isAnimationPaused = false;
        lastAnimationFrameTimestamp = 0;
        fade7Hue = 0;
        fade7Animation();
        break;
    case REMOTE_FLASH:
        currentAnimationMode = flash;
        isAnimationPaused = false;
        lastAnimationFrameTimestamp = 0;
        lastAnimationFrameIndex = 0;
        flashAnimation();
        break;
    // case REMOTE_AUTO: // TODO

    case REMOTE_QUICK:
        if (currentAnimationMode != none) {
            newAnimationSpeed = (animationSpeed + ((lastReceivedCommandRepeatCount > 5) ? 8 : 1));
            animationSpeed = (newAnimationSpeed > 1024 ? 1024 : newAnimationSpeed);
        }
        break;
    case REMOTE_SLOW:
        if (currentAnimationMode != none) {
            newAnimationSpeed = (animationSpeed - ((lastReceivedCommandRepeatCount > 5) ? 8 : 1));
            animationSpeed = (newAnimationSpeed < 1 ? 1 : newAnimationSpeed);
        }
        break;
    }
}

void handleReceivedTinyIRData(uint8_t aAddress, uint8_t aCommand, uint8_t aFlags) {
    // ignore commands with parity failed or addresses other than REMOTE_ADDRESS
    if ((aFlags & 0x4) || (aAddress != REMOTE_ADDRESS)) {
        return;
    }
    // ignore repeat commands that were fired 200ms after the last valid command or don't match the last command
    if ((aFlags & 0x1) && (((millis() - lastReceivedCommandTimestamp) > 200) || (aCommand != lastReceivedCommand))) {
        return;
    }

    lastReceivedCommand = aCommand;
    lastReceivedCommandDone = false;
    lastReceivedCommandTimestamp = millis();

    if (aFlags & 0x1) {
        lastReceivedCommandRepeatCount += 1;
    }
    else {
        lastReceivedCommandRepeatCount = 0;
    }
}
