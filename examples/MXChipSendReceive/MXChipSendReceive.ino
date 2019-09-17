/**------------------------------------------------------------------------------
 *
 *  Simple example using the Chirp SDK on the Microsoft MXChip IoT DevKit.
 *
 *  @file MXChipSendReceive.ino
 *
 *  @brief After creating a developer account on https://developers.chirp.io, get
 *  your key, secret and config string from your account using the "16kHz-mono-embedded"
 *  protocol, and set them in this file (in CHIRP_APP_KEY, CHIRP_APP_SECRET, CHIRP_APP_CONFIG).
 *
 *  This example will start in listening mode. The listening and playing modes
 *  can alternate by pressing the button A (on the left).
 *
 *  Each time the SDK starts receiving some data, the LED will turn blue.
 *  If the data has been successfully decoded then the LED will turn green and the
 *  hexadecimal representation of the data as well as the length of the message,
 *  in bytes, will be displayed. If the decode fails, the LED will turn red.
 *
 *  In playing mode, each press on button B (on the right) will start sending a
 *  random payload of random length, and turn the LED yellow. Once the payload
 *  has been sent, the LED will turn cyan and the hexadecimal representation of the
 *  data sent as well as the length of the payload, in bytes, will be displayed.
 *  The audio data is sent via the 3.5mm jack output.
 *
 *  See README.md for further information and known issues.
 *
 *  Copyright © 2011-2018, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

/*
 * MXChip and Arduino native headers.
 */
#include "Arduino.h"
#include "AudioClassV2.h"
#include "OledDisplay.h"
#include "RGB_LED.h"

/*
 * Main Chirp SDK header. This header and the ones it depends on must be in
 * the same folder.
 */
#include "chirp_sdk.h"
#include "credentials.h"

/*
 * The audio sample rate used by the microphone. At present, only 16kHz is
 * supported by the SDK on the MXChip board.
 */
#define SAMPLE_RATE 16000
#define AUDIO_SAMPLE_SIZE 16
#define SHORT_BUFFER_SIZE (AUDIO_CHUNK_SIZE / 2)
#define FLOAT_BUFFER_SIZE (SHORT_BUFFER_SIZE / 2)

/*
 * To keep track of audio buffer states.
 */
typedef enum {
  BUFFER_STATE_NONE,
  BUFFER_STATE_EMPTY,
  BUFFER_STATE_FULL,
} bufferState;

/*
 * Class handling the audio on the board. The state is recording by default.
 */
static AudioClass& Audio = AudioClass::getInstance();
AUDIO_STATE_TypeDef audioState = AUDIO_STATE_RECORDING;

/*
 * Buffers containnig the audio to play and record data.
 */
static int16_t shortRecordBuffer[SHORT_BUFFER_SIZE] = {0};
static float floatRecordBuffer[FLOAT_BUFFER_SIZE] = {0};
bufferState recordBufferState = BUFFER_STATE_EMPTY;

static int16_t shortPlayBuffer[SHORT_BUFFER_SIZE] = {0};
bufferState playBufferState = BUFFER_STATE_EMPTY;

int lastButtonAState;
int buttonAState;

int buttonBState;
int lastButtonBState;

RGB_LED rgbLed;

/*
 * Convert a payload to an hexadecimal identifer terminating with '\0'
 */
void payload_to_hex(char *payload, int payload_length, char *hex_str)
{
    for (int i = 0; i < payload_length; i++)
    {
        sprintf(hex_str + i * 2, "%02x", payload[i]);
    }
    hex_str[payload_length * 2] = '\\0';
}

/*
 * Global pointer to the SDK structure. This is global as this pointer is
 * needed when processing the audio in the loop() function.
 */
chirp_sdk_t *chirp = NULL;

/*
 * Simple error handler which display an error message and loop indefinitely.
 */
void errorHandler(chirp_sdk_error_code_t errorCode)
{
    if (errorCode != CHIRP_SDK_OK)
    {
        Serial.printf("Error handler : %s\n", chirp_sdk_error_code_to_string(errorCode));
        while(true);
    }
}

/*
 * Audio recording callback called by the Audio Class instance when a new
 * buffer of samples is available with new recorded samples.
 */
void recordCallback(void)
{
    Audio.readFromRecordBuffer((char *) shortRecordBuffer, SHORT_BUFFER_SIZE * 2);
    recordBufferState = BUFFER_STATE_FULL;
}

/*
 * Audio playing callback called by the Audio Class instance when a new
 * buffer of samples is available with new samples to be played.
 */
void playCallback(void)
{
    if(playBufferState == BUFFER_STATE_FULL)
    {
        Audio.writeToPlayBuffer((char *) shortPlayBuffer, SHORT_BUFFER_SIZE * 2);
        playBufferState = BUFFER_STATE_EMPTY;
    }
}

/*
 * Callback reached when the SDK starts sending data.
 */
void on_sending_callback(void *data, uint8_t *payload, size_t length, uint8_t channel)
{
    Screen.clean();
    rgbLed.setColor(255, 255, 0);
}

/*
 * Callback reached when the SDK has sent the data.
 */
void on_sent_callback(void *data, uint8_t *payload, size_t length, uint8_t channel)
{
    char identifier[length * 2 + 1];
    payload_to_hex(payload, length, identifier);
    char strLength[8] = {0};
    itoa(length, strLength, 10);
    Screen.clean();
    Screen.print(0, "Sent !");
    Screen.print(1, (const char *) identifier, true);
    Screen.print(3, strLength);
    rgbLed.setColor(0, 255, 255);
}

/*
 * Callback reached when the SDK starts receiving some data.
 */
void on_receiving_callback(void *data, uint8_t *payload, size_t length, uint8_t channel)
{
    rgbLed.setColor(0, 0, 255);
}

/*
 * Callback reached when the SDK has received some data.
 */
void on_received_callback(void *data, uint8_t *payload, size_t length, uint8_t channel)
{
    // A pointer not null with a length different than 0 means the decode has succedeed.
    if (payload && length != 0)
    {
        char identifier[length * 2 + 1];
        payload_to_hex(payload, length, identifier);
        char strLength[8] = {0};
        itoa(length, strLength, 10);
        Screen.clean();
        Screen.print(0, "Received !");
        Screen.print(1, (const char *) identifier, true);
        Screen.print(3, strLength);
        rgbLed.setColor(0, 255, 0);
    }
    // A null pointer with a length of 0 means the decode has failed.
    else
    {
        Screen.clean();
        Screen.print(0, "Decode failed =(");
        rgbLed.setColor(255, 0, 0);
    }
}

/*
 * Setup function where the SDK is initialised.
 */
void setup()
{
    Serial.begin(115200);
    delay(1000);

    pinMode(USER_BUTTON_A, INPUT);
    lastButtonAState = digitalRead(USER_BUTTON_A);

    chirp = new_chirp_sdk(CHIRP_APP_KEY, CHIRP_APP_SECRET);
    if (chirp)
    {
        printf("Initialisation is OK\n");
    }
    else
    {
        printf("Initialisation failed\nAre your key and secret correct ?\n");
        exit(1);
    }

    chirp_sdk_error_code_t errorCode = chirp_sdk_set_config(chirp, CHIRP_APP_CONFIG);
    errorHandler(errorCode);

    printf("Config set correctly\n");

    char *info = chirp_sdk_get_info(chirp);
    printf("%s - V%s\n", info, chirp_sdk_get_version());
    chirp_sdk_free(info);

    errorCode = chirp_sdk_set_input_sample_rate(chirp, SAMPLE_RATE);
    errorHandler(errorCode);
    printf("Input sample rate is : %u\n", chirp_sdk_get_input_sample_rate(chirp));

    errorCode = chirp_sdk_set_output_sample_rate(chirp, SAMPLE_RATE);
    errorHandler(errorCode);
    printf("Output sample rate is : %u\n", chirp_sdk_get_output_sample_rate(chirp));

    // The static structure is set to 0. This is needed because we are not setting
    // the `on_state_changed` callback.
    chirp_sdk_callback_set_t callbacks = {0};
    callbacks.on_sending = on_sending_callback;
    callbacks.on_sent = on_sent_callback;
    callbacks.on_received = on_received_callback;
    callbacks.on_receiving = on_receiving_callback;

    errorCode = chirp_sdk_set_callbacks(chirp, callbacks);
    errorHandler(errorCode);

    printf("Callbacks set\n");

    // MXChip specific : A software adjustment of the sample rate is needed.
    errorCode = chirp_sdk_set_frequency_correction(chirp, 0.9950933459f);
    errorHandler(errorCode);

    errorCode = chirp_sdk_start(chirp);
    errorHandler(errorCode);

    printf("SDK started\n");

    Screen.clean();
    Screen.print(0, "Chirp C SDK");
    Screen.print(1, "Listening ...");

    // Setup the audio class and start recording.
    Audio.format(SAMPLE_RATE, AUDIO_SAMPLE_SIZE);
    int res = Audio.startRecord(recordCallback);
    if (res != 0)
    {
        Serial.printf("Error when starting audio\n");
        while(true);
    }
}

void loop()
{
    buttonAState = digitalRead(USER_BUTTON_A);
    buttonBState = digitalRead(USER_BUTTON_B);
    chirp_sdk_error_code_t errorCode;

    //  If we've pressed the button A, alternate the audio state between Recording and Playing.
    if (buttonAState == LOW && lastButtonAState == HIGH)
    {
        if (audioState == AUDIO_STATE_RECORDING)
        {
            audioState = AUDIO_STATE_PLAYING;
            Audio.stop();
            Audio.startPlay(playCallback);
            Screen.clean();
            Screen.print(0, "Playing");
        }
        else if (audioState == AUDIO_STATE_PLAYING)
        {
            audioState = AUDIO_STATE_RECORDING;
            Audio.stop();
            Audio.startRecord(recordCallback);
            Screen.clean();
            Screen.print(0, "Recording");
        }

        rgbLed.turnOff();
    }

    if (audioState == AUDIO_STATE_RECORDING)
    {
        // Once the recording buffer is full, we process it.
        if (recordBufferState == BUFFER_STATE_FULL)
        {
            // Convert the stereo audio samples into mono by taking every other sample and convert them
            // from int16_t to float.
            for (int i = 0; i < FLOAT_BUFFER_SIZE; i++)
            {
                floatRecordBuffer[i] = (float) shortRecordBuffer[i * 2] / 32767.0f;
            }
            errorCode = chirp_sdk_process_input(chirp, floatRecordBuffer, FLOAT_BUFFER_SIZE);
            errorHandler(errorCode);
            recordBufferState = BUFFER_STATE_EMPTY;
        }
    }
    else if (audioState == AUDIO_STATE_PLAYING)
    {
        // If the button B is pressed a chirp is sent.
        if (buttonBState == LOW && lastButtonBState == HIGH)
        {
            size_t randomPayloadLength = 0;
            uint8_t *randomPayload =  chirp_sdk_random_payload(chirp, &randomPayloadLength);
            errorCode = chirp_sdk_send(chirp, randomPayload, randomPayloadLength);
            errorHandler(errorCode);
            chirp_sdk_free(randomPayload);
        }

        chirp_sdk_state_t state = chirp_sdk_get_state(chirp);
        if (state == CHIRP_SDK_STATE_SENDING && playBufferState == BUFFER_STATE_EMPTY)
        {
            float tmpBuffer[SHORT_BUFFER_SIZE / 2] = {0};
            errorCode = chirp_sdk_process_output(chirp, tmpBuffer, SHORT_BUFFER_SIZE / 2);
            errorHandler(errorCode);
            // On the contrary of the recording part, we duplicate the data produced by the SDK
            // to create a stereo audio stream that is converted from float to int16_t samples.
            for (uint16_t i = 0; i < SHORT_BUFFER_SIZE / 2; i++)
            {
                shortPlayBuffer[i * 2] = tmpBuffer[i] * 32767.0f;
                shortPlayBuffer[i * 2 + 1] = tmpBuffer[i] * 32767.0f;
            }
            playBufferState = BUFFER_STATE_FULL;
        }
    }

    lastButtonAState = buttonAState;
    lastButtonBState = buttonBState;
}
