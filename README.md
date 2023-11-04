# rgblink
> control RGB LEDs/strips with Arduino

## Install instructions
Currently this project supports only Arduino Uno R3 and Arduino Nano.
1. Clone this git repo or [download ZIP](https://github.com/miskyy7507/rgblink/archive/refs/heads/master.zip) and unzip it
2. Download and install [Visual Studio Code](https://code.visualstudio.com/#alt-downloads)
3. (*optional, but recommended*) Create a [new profile](https://code.visualstudio.com/docs/editor/profiles#_create-a-profile) in VS Code and follow subsequent steps inside of it
3. Go to *Extensions* , search for `PlatformIO IDE` extension and install it by following steps on screen
4. Go to File -> Open Folder and open the root **rgblink** folder you've got in step 1 (the one that contains platformio.ini)
5. Connect your Arduino and hit the checkmark on the status bar (at the bottom) to build and flash the program.
6. If everyone goes according to plan, it should be working now!

Default pinout:
* red channel (`R_PIN`): **9**
* green channel (`G_PIN`): **11**
* blue channel (`B_PIN`): **10**
* IR receiver data pin (`IR_RECEIVE_PIN`): **2**

## ⚠️ Disclaimer
Lighting effects may potentially trigger seizures for people with photosensitive epilepsy.
As per MIT license, I (the author of this project) assume NO liability for any damage to any person or equipment.
