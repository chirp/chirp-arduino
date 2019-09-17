# Chirp Arduino - Platforms

Chirp is a library enabling Arduino-based devices to send and receive data using sound. You'll need:

 * A compatible Arduino board
 * A digital I2S MEMS microphone (if your board does not contain a microphone)
 * A digital I2S amplifier and compatible speaker

For receiving data, you will need a digital MEMS microphone. Some boards (for example, the Nano 33 Sense and Microsoft MXChip) already include a MEMS mic so you are good to go. For others, you will need an external mic such as the [SPH0645](https://www.adafruit.com/product/3421) or [ICS-43434](https://www.mouser.co.uk/ProductDetail/TDK-InvenSense/ICS-43434?qs=u4fy%2FsgLU9PAgmWRI7%252BqXA%3D%3D).

For sending data, we recommend using a digital I2S audio output such as the [UDA1334A](https://www.adafruit.com/product/3678) or [MAX98357A](https://www.adafruit.com/product/3006), connected to a compatible speaker.

You can quickly test that your device is receiving chirps by playing some random test signals from the [Developer Hub](https://developers.chirp.io).

To test whether your device is sending chirps OK, we recommend setting up the [Python command-line tools](https://developers.chirp.io/docs/tutorials/command-line) to receive data from the Arduino.