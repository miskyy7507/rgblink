#include <Arduino.h>

#define NO_LED_FEEDBACK_CODE
#define IR_RECEIVE_PIN 2
#include "TinyIRReceiver.hpp"

#include "AnalogRGB.h"

#include "remoteButtonsCodes.h"
#include "predefinedColors.h"

#include "AnimationEngine.h"
#include "animations.h"

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

AnimationEngine animate(rgbStrip);

void setPredefinedColor(uint32_t newColor) {
    animate.stop();
    diyColorIndex = -1;
    rgbStrip.setRGB(newColor);
    rgbStrip.updateColor();
}

void setDiyColor(int8_t index) {
    animate.stop();
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

void setup() {
    // turn off built-in led
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    rgbStrip.begin();

    initPCIInterruptForTinyReceiver();
}

void loop() {
    animate.loop(millis());

    if (lastReceivedCommandDone) {
        return;
    }

    if (!rgbStrip.isOn()) {
        if (lastReceivedCommandRepeatCount == 0) {
            switch (lastReceivedCommand) {
            case REMOTE_POWER:
                rgbStrip.powerToggle();
                animate.play();
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

    case REMOTE_POWER:
        if (lastReceivedCommandRepeatCount == 0) {
            rgbStrip.powerToggle();
            animate.pause();
        }
        break;
    case REMOTE_PLAY_PAUSE:
        if (lastReceivedCommandRepeatCount == 0) {
            animate.toggle();
        }
        break;
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
        diyColorIndex = -1;
        animate.start(jump3Animation);
        break;
    case REMOTE_JUMP7:
        diyColorIndex = -1;
        animate.start(jump7Animation);
        break;
    case REMOTE_FADE3:
        diyColorIndex = -1;
        animate.start(fade3Animation);
        break;
    case REMOTE_FADE7:
        diyColorIndex = -1;
        animate.start(fade7Animation);
        break;
    case REMOTE_FLASH:
        diyColorIndex = -1;
        animate.start(flashAnimation);
        break;
    // case REMOTE_AUTO: // TODO

    case REMOTE_QUICK:
        animate.speedUp();
        break;
    case REMOTE_SLOW:
        animate.slowDown();
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
