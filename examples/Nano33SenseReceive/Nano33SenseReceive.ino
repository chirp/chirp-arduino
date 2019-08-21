/**-----------------------------------------------------------------------------

    Example code using the Chirp SDK to receive data using the
    Arduino Nano 33 Sense board.

    @file Nano33SenseReceive.ino

    @brief Create a developer account at https://developers.chirp.io,
    and copy and paste your key, secret and config string for the
    "16khz-mono-embedded" protocol into the credentials.h file.

    This example will start listening for chirps and print to the terminal
    when anything is received.

    *Note*: this example can be used in conjunction with the send example,
    to send and receive data in the same application.

    *Important*: The example will not start until this Serial Monitor is opened.
    To disable this behaviour, comment out the while(!Serial) line.

    Circuit:
      - Arduino Nano 33 BLE board

    Copyright © 2011-2019, Asio Ltd.
    All rights reserved.

  ----------------------------------------------------------------------------*/
#include <PDM.h>

#include "chirp_connect.h"
#include "credentials.h"

#define SAMPLE_RATE       16000   // Audio sample rate
#define BUFFER_SIZE       256     // Audio buffer size

// Global variables ------------------------------------------------------------

static chirp_connect_t *chirp = NULL;
short sampleBuffer[BUFFER_SIZE];
volatile int samplesRead;

// Function definitions --------------------------------------------------------

void setupChirp(void);
void chirpErrorHandler(chirp_connect_error_code_t code);
void onPDMdata(void);

// Main ------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  // Enable high frequency oscillator
  NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_HFCLKSTART    = 1;
  while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);

  setupChirp();

  PDM.onReceive(onPDMdata);
  PDM.setGain(30);

  if (!PDM.begin(1, SAMPLE_RATE))
  {
    Serial.println("Failed to start PDM!");
    while (1);
  }
}

void loop()
{
  if (samplesRead)
  {
    chirp_connect_error_code_t err = chirp_connect_process_shorts_input(chirp, sampleBuffer, samplesRead);
    chirpErrorHandler(err);
    samplesRead = 0;
  }
}

void onPDMdata()
{
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / sizeof(short);
}

// Chirp -----------------------------------------------------------------------

void onReceivingCallback(void *chirp, uint8_t *payload, size_t length, uint8_t channel)
{
  Serial.println("Receiving data...");
}

void onReceivedCallback(void *chirp, uint8_t *payload, size_t length, uint8_t channel)
{
  if (payload)
  {
    char *data = (char *)calloc(length + 1, sizeof(uint8_t));
    memcpy(data, payload, length * sizeof(uint8_t));
    Serial.print("Received data: ");
    Serial.println(data);
    free(data);
  }
  else
  {
    Serial.println("Decode failed.");
  }
}

void chirpErrorHandler(chirp_connect_error_code_t code)
{
  if (code != CHIRP_CONNECT_OK)
  {
    const char *errorString = chirp_connect_error_code_to_string(code);
    Serial.println(errorString);
    exit(42);
  }
}

void setupChirp(void)
{
  chirp = new_chirp_connect(CHIRP_APP_KEY, CHIRP_APP_SECRET);
  if (chirp == NULL)
  {
    Serial.println("Chirp initialisation failed.");
    return;
  }

  chirp_connect_error_code_t err = chirp_connect_set_config(chirp, CHIRP_APP_CONFIG);
  chirpErrorHandler(err);

  chirp_connect_callback_set_t callback_set =
  {
    .on_state_changed = NULL,
    .on_sending = NULL,
    .on_sent = NULL,
    .on_receiving = onReceivingCallback,
    .on_received = onReceivedCallback
  };

  err = chirp_connect_set_callbacks(chirp, callback_set);
  chirpErrorHandler(err);

  err = chirp_connect_set_input_sample_rate(chirp, SAMPLE_RATE);
  chirpErrorHandler(err);

  // A fixed frequency correction coefficient is needed to correct a clock
  // mismatch between the 16000Hz requested sample rate and the Nano's actual
  // audio sample rate.
  err = chirp_connect_set_frequency_correction(chirp, 1.00812);
  chirpErrorHandler(err);

  err = chirp_connect_start(chirp);
  chirpErrorHandler(err);

  Serial.println("Chirp SDK initialised.");
  Serial.flush();
}
