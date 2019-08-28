# Chirp for Arduino

*Version 3.3.1, August 2019*

## Overview

Chirp is a library enabling Arduino-based devices to send and receive data using sound. You'll need:

 * A compatible Arduino board
 * A digital I2S MEMS microphone (if your board does not contain a microphone)
 * A digital I2S amplifier and compatible speaker

For receiving data, you will need a digital MEMS microphone. Some boards (for example, the Nano 33 Sense and Microsoft MXChip) already include a MEMS mic so you are good to go. For others, you will need an external mic such as the [SPH0645](https://www.adafruit.com/product/3421) or [ICS-43434](https://www.mouser.co.uk/ProductDetail/TDK-InvenSense/ICS-43434?qs=u4fy%2FsgLU9PAgmWRI7%252BqXA%3D%3D).

For sending data, we recommend using a digital I2S audio output such as the [UDA1334A](https://www.adafruit.com/product/3678) or [MAX98357A](https://www.adafruit.com/product/3006), connected to a compatible speaker.

You can quickly test that your device is receiving chirps by playing some random test signals from the [Developer Hub](https://developers.chirp.io).

To test whether your device is sending chirps OK, we recommend setting up the [Python command-line tools](https://developers.chirp.io/docs/tutorials/command-line) to receive data from the Arduino.

## Supported hardware

The following Arduino-compatible boards are able to both send and receive chirps:

 * Arduino Nano 33 Sense
 * Microsoft MXChip
 * ESP32

The following Arduino-compatible boards are only able to send chirps, as they are not able to do on-chip DSP:

 * Arduino MKRZero
 * Arduino Vidor 4000
 * Genuino Zero
 * MKR Fox 1200
 * MKR1000 WiFi

## Installation

Chirp is written for the Arduino IDE versions 1.8.6 and above.

Install ChirpSDK as a library using "Manage Libraries". For instructions, see

[http://arduino.cc/en/Guide/Libraries](http://arduino.cc/en/Guide/Libraries)

Once installed, you can access the example programs from the menu:

```
File > Examples > ChirpSDK > Example
```

and you can include the headers to use Chirp in your own code by adding:

```
#include "chirp_sdk.h"
```

## Usage

To set up the Chirp SDK, initialise and configure with your app key,
secret and config from the [Developer Hub](https://developers.chirp.io).

*Note* You must select the `16khz-mono-embedded` protocol from the dropdown menu, when
selecting your chirp configuration.

    chirp = new_chirp_sdk(APP_KEY, APP_SECRET);
    if (chirp == NULL) {
        Serial.println("Chirp initialisation failed.");
        return;
    }

Then set any required callbacks and start the SDK running.

    chirp_sdk_error_code_t err = chirp_sdk_set_config(chirp, APP_CONFIG);
    if (err != CHIRP_SDK_OK)
        return;

    chirp_sdk_callback_set_t callbacks = {0};
    callbacks.on_received = onReceivedCallback;
    err = chirp_sdk_set_callbacks(chirp, callbacks);
    if (err != CHIRP_SDK_OK)
        return;

    err = chirp_sdk_set_callback_ptr(chirp, chirp);
    if (err != CHIRP_SDK_OK)
        return;

    err = chirp_sdk_set_input_sample_rate(chirp, input_sample_rate);
    if (err != CHIRP_SDK_OK)
        return;
    err = chirp_sdk_set_output_sample_rate(chirp, output_sample_rate);
    if (err != CHIRP_SDK_OK)
        return;

    err = chirp_sdk_start(chirp);
    if (err != CHIRP_SDK_OK)
        return;

## Callbacks

The received data is passed back to the `onReceivedCallback` function. If the payload pointer is null then there has been an error decoding the data.

    void
    onReceivedCallback(void *chirp, uint8_t *payload, size_t length, uint8_t channel) {
        if (payload) {
            char *hexString = chirp_sdk_as_string(chirp, payload, length);
            Serial.printf("Received data = %s\n", hexString);
            chirp_sdk_free(hexString);
        } else {
            Serial.println("Decode failed.");
        }
    }

A complete list of callbacks is shown below.

    void onStateChangedCallback(void *ptr, chirp_sdk_state_t old_state, chirp_sdk_state_t new_state) {
        // Put here what you want to do when the SDK's state is changing.
    }

    void onSendingCallback(void *ptr, uint8_t *bytes, size_t length, uint8_t channel) {
        // Put here what you want to do when the SDK starts to send some data.
    }

    void onSentCallback(void *ptr, uint8_t *bytes, size_t length, uint8_t channel) {
        // Put here what you want to do when the SDK has sent some data.
    }

    void onReceivingCallback(void *ptr, uint8_t *bytes, size_t length, uint8_t channel) {
        // Put here what you want to do when the SDK starts receiving some data.
    }

    void onReceivedCallback(void *ptr, uint8_t *bytes, size_t length, uint8_t channel) {
        // Put here what you want to do when the SDK has received some data.
    }

    // If you don't set all the callbacks, make sure the unused callbacks are set to NULL.
    chirp_sdk_callback_set_t callbacks_set = {
        .on_state_changed = on_state_changed_callback,
        .on_sending = on_sending_callback,
        .on_sent = on_sent_callback,
        .on_receiving = on_receiving_callback,
        .on_received = on_received_callback
    };
    err = chirp_sdk_set_callbacks(chirp, callbacks_set);
    if (err != CHIRP_SDK_OK)
    {
        const char *error_string = chirp_sdk_error_code_to_string(err);
        printf("%s\n", error_string);
    }


## Payloads

A Chirp payload is simply an array of bytes. You can send a random data payload to the speakers like so.

    size_t payload_length = chirp_sdk_get_max_payload_length(chirp);
    uint8_t *payload = chirp_sdk_random_payload(chirp, &payload_length);

    err = chirp_sdk_send(chirp, payload, payload_length);
    if (err != CHIRP_SDK_OK) {
        const char *error_string = chirp_sdk_error_code_to_string(err);
        printf("%s\n", error_string);
    }

Or you can easily send an ASCII string

    char *identifier = "hello";
    err = chirp_sdk_send(chirp, (uint8_t *)identifier, strlen(identifier));
    if (err != CHIRP_SDK_OK) {
        const char *error_string = chirp_sdk_error_code_to_string(err);
        printf("%s\n", error_string);
    }

## Processing

To process audio data from the microphone, and fill the output buffer with audio data, call the following functions with data periodically.

    err = chirp_sdk_process_input(chirp, input_buffer, input_buffer_length);

    err = chirp_sdk_process_output(chirp, output_buffer, output_buffer_length);

***

All content copyright &copy; Asio Ltd, 2013-2019. All rights reserved.
