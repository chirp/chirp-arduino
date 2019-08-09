/**-----------------------------------------------------------------------------

    Example code to send data using the Chirp SDK, on the Arduino Nano 33 Sense
    board with Adafruit MAX98357A and a mini oval speaker

    @file Nano33SenseSend.ino

    @brief Create a developer account at https://developers.chirp.io,
    and copy and paste your key, secret and config string for the
    "16khz-mono-embedded" protocol into the credentials.h file.

    This example will send a single Chirp signal on startup, containing random
    data.

    *Important*: The example will not start until this Serial Monitor is opened.
    To disable this behaviour, comment out the while(!Serial) line.

    Circuit:
      - Arduino Nano 33 BLE board
      - Adafruit MAX98357A amplifier
      - Adafruit Mini Oval speaker

    Copyright © 2011-2019, Asio Ltd.
    All rights reserved.

  ----------------------------------------------------------------------------*/
#include "chirp_connect.h"
#include "credentials.h"

#define VOLUME                 0.3  // Between 0 and 1
#define BUFFER_SIZE            256
#define OUTPUT_SAMPLE_RATE     16667

#define I2S_DATA_PIN           23  // D7
#define I2S_SCK_PIN            21  // D8
#define I2S_LRCK_PIN           27  // D9

// Global variables ------------------------------------------------------------

static chirp_connect_t *chirp = NULL;
short *currentBuffer;
short buffer1[BUFFER_SIZE];
short buffer2[BUFFER_SIZE];

// Function definitions --------------------------------------------------------

void chirpErrorHandler(chirp_connect_error_code_t code);
void setupChirp(void);
void sendChirp(void);
void i2s_init(void);
void i2s_start(void);

// Main ------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  while(!Serial);  // Wait for Serial monitor before continuing

  setupChirp();
  sendChirp();

  i2s_init();
  i2s_start();
}

void loop()
{
  if (NRF_I2S->EVENTS_TXPTRUPD != 0)
  {
    currentBuffer = currentBuffer == buffer1 ? buffer2 : buffer1;
    chirp_connect_error_code_t err = chirp_connect_process_shorts_output(chirp, currentBuffer, BUFFER_SIZE);
    chirpErrorHandler(err);
    NRF_I2S->TXD.PTR = (uint32_t)(currentBuffer);
    NRF_I2S->EVENTS_TXPTRUPD = 0;
  }
}

// Chirp -----------------------------------------------------------------------

void onSendingCallback(void *chirp, uint8_t *payload, size_t length, uint8_t channel)
{
  Serial.println("Sending data...");
}

void onSentCallback(void *chirp, uint8_t *payload, size_t length, uint8_t channel)
{
  char *data = (char *)calloc(length + 1, sizeof(uint8_t));
  memcpy(data, payload, length);
  Serial.print("Sent data: ");
  Serial.println(data);
  free(data);
}

void chirpErrorHandler(chirp_connect_error_code_t code)
{
  if (code != CHIRP_CONNECT_OK)
  {
    const char *error_string = chirp_connect_error_code_to_string(code);
    Serial.println(error_string);
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

  chirp_connect_callback_set_t callback_set = {
    .on_state_changed = NULL,
    .on_sending = onSendingCallback,
    .on_sent = onSentCallback,
    .on_receiving = NULL,
    .on_received = NULL
  };

  err = chirp_connect_set_callbacks(chirp, callback_set);
  chirpErrorHandler(err);

  err = chirp_connect_set_output_sample_rate(chirp, OUTPUT_SAMPLE_RATE);
  chirpErrorHandler(err);

  err = chirp_connect_set_volume(chirp, VOLUME);
  chirpErrorHandler(err);

  err = chirp_connect_start(chirp);
  chirpErrorHandler(err);

  Serial.println("Chirp SDK initialised.");
  Serial.flush();
}

void sendChirp()
{
  chirp_connect_error_code_t err;

  char *payload = "hello";
  err = chirp_connect_send(chirp, (uint8_t *)payload, strlen(payload));
  chirpErrorHandler(err);

  Serial.print("Sending data: ");
  Serial.println(payload);
}

// I2S Audio -------------------------------------------------------------------

void
i2s_init()
{
  // Enable clocks
  NRF_I2S->CONFIG.TXEN = I2S_CONFIG_TXEN_TXEN_Enabled << I2S_CONFIG_TXEN_TXEN_Pos;
  NRF_I2S->CONFIG.MCKEN = I2S_CONFIG_MCKEN_MCKEN_Enabled << I2S_CONFIG_MCKEN_MCKEN_Pos;

  // MCKFREQ = 6.4 MHz
  NRF_I2S->CONFIG.MCKFREQ = I2S_CONFIG_MCKFREQ_MCKFREQ_32MDIV5 << I2S_CONFIG_MCKFREQ_MCKFREQ_Pos;

  // Ratio = 384
  // LCRK = 16667 Hz
  NRF_I2S->CONFIG.RATIO = I2S_CONFIG_RATIO_RATIO_384X << I2S_CONFIG_RATIO_RATIO_Pos;

  // Master mode
  NRF_I2S->CONFIG.MODE = I2S_CONFIG_MODE_MODE_MASTER << I2S_CONFIG_MODE_MODE_Pos;

   // 16 bits
  NRF_I2S->CONFIG.SWIDTH = I2S_CONFIG_SWIDTH_SWIDTH_16Bit << I2S_CONFIG_SWIDTH_SWIDTH_Pos;

  // Left alignment
  NRF_I2S->CONFIG.ALIGN = I2S_CONFIG_ALIGN_ALIGN_Left << I2S_CONFIG_ALIGN_ALIGN_Pos;

  // I2S format
  NRF_I2S->CONFIG.FORMAT = I2S_CONFIG_FORMAT_FORMAT_I2S << I2S_CONFIG_FORMAT_FORMAT_Pos;

  // Mono
  NRF_I2S->CONFIG.CHANNELS = I2S_CONFIG_CHANNELS_CHANNELS_Left << I2S_CONFIG_CHANNELS_CHANNELS_Pos;

  // Configure pins
  NRF_I2S->PSEL.SCK = I2S_SCK_PIN << I2S_PSEL_SCK_PIN_Pos;
  NRF_I2S->PSEL.LRCK = I2S_LRCK_PIN << I2S_PSEL_LRCK_PIN_Pos;
  NRF_I2S->PSEL.SDOUT = I2S_DATA_PIN << I2S_PSEL_SDOUT_PIN_Pos;
}

void
i2s_start()
{
  NRF_I2S->ENABLE = 1;

  // Configure DMA buffer
  NRF_I2S->TXD.PTR = (uint32_t)(buffer1);
  NRF_I2S->RXTXD.MAXCNT = BUFFER_SIZE / 2;

  NRF_I2S->TASKS_START = 1;
}
