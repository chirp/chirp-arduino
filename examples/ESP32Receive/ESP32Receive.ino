/**-----------------------------------------------------------------------------

    Example code using the Chirp SDK to receive data using ESP32 and
    SPH0645 microphone

    @file ESP32Receive.ino

    @brief Create a developer account at https://developers.chirp.io,
    and copy and paste your key, secret and config string for the
    "16khz-mono-embedded" protocol into the credentials.h file.

    This example will start listening for chirps and print to the terminal
    when anything is received.

    Note: this example can be used in conjunction with the send example,
    to send and receive data in the same application.

    Copyright © 2011-2019, Asio Ltd.
    All rights reserved.

  ----------------------------------------------------------------------------*/
#include <driver/i2s.h>

#include "chirp_connect.h"
#include "credentials.h"

#define I2SI_DATA         12     // I2S DATA IN on GPIO32
#define I2SI_BCK          14     // I2S BCLK on GPIO14
#define I2SI_LRCL         15     // I2S SELECT on GPIO15

#define LED_PIN           2      // Pin number for on-board LED
#define SWITCH_PIN        0      // Pin number for on-board switch

#define BUFFER_SIZE       512    // Audio buffer size
#define SAMPLE_RATE       16000  // Audio sample rate

/**
   Convert I2S input data.
   Data is 18 bit signed, MSBit first, two's complement.
   The MIC_CALIBRATION value is determined using the Serial
   Plotter to centre the audio about zero. The below value
   should be correct for most ESP32 boards.
*/
#define MIC_CALIBRATION   13125
#define CONVERT_INPUT(sample) (((int32_t)(sample) >> 14) + MIC_CALIBRATION)

// Global variables ------------------------------------------------------------

static chirp_connect_t *chirp = NULL;
static chirp_connect_state_t currentState = CHIRP_CONNECT_STATE_NOT_CREATED;
static bool startTasks = false;

// Function definitions --------------------------------------------------------

void setupChirp();
void chirpErrorHandler(chirp_connect_error_code_t code);
void setupAudioInput(int sample_rate);

// Function declarations -------------------------------------------------------

void
setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);
  Serial.printf("Heap size: %u\n", ESP.getFreeHeap());

  xTaskCreate(initTask, "initTask", 16384, NULL, 1, NULL);
}

void
loop()
{
  esp_err_t audioError;
  chirp_connect_error_code_t chirpError;

  if (startTasks)
  {
    xTaskCreate(processInputTask, "processInputTask", 16384, NULL, 5, NULL);
    startTasks = false;
  }
}

// RTOS Tasks ------------------------------------------------------------------

void
initTask(void *parameter)
{
  setupChirp();

  chirp_connect_error_code_t chirpError = chirp_connect_set_input_sample_rate(chirp, SAMPLE_RATE);
  chirpErrorHandler(chirpError);
  setupAudioInput(SAMPLE_RATE);

  Serial.printf("Heap size: %u\n", ESP.getFreeHeap());
  startTasks = true;
  vTaskDelete(NULL);
}

void
processInputTask(void *parameter)
{
  esp_err_t audioError;
  chirp_connect_error_code_t chirpError;

  size_t bytesLength = 0;
  float buffer[BUFFER_SIZE] = {0};
  int32_t ibuffer[BUFFER_SIZE] = {0};

  while (currentState >= CHIRP_CONNECT_STATE_RUNNING)
  {
    audioError = i2s_read(I2S_NUM_0, ibuffer, BUFFER_SIZE * 4, &bytesLength, portMAX_DELAY);
    if (bytesLength)
    {
      for (int i = 0; i < bytesLength / 4; i++)
      {
        buffer[i] = (float) CONVERT_INPUT(ibuffer[i]);
      }

      chirpError = chirp_connect_process_input(chirp, buffer, bytesLength / 4);
      chirpErrorHandler(chirpError);
    }
  }
  vTaskDelete(NULL);
}

// Chirp -----------------------------------------------------------------------

void
onStateChangedCallback(void *chirp, chirp_connect_state_t previous, chirp_connect_state_t current)
{
  currentState = current;
  Serial.printf("State changed from %d to %d\n", previous, current);
}

void
onReceivingCallback(void *chirp, uint8_t *payload, size_t length, uint8_t channel)
{
  Serial.println("Receiving data...");
  digitalWrite(LED_PIN, HIGH);
}

void
onReceivedCallback(void *chirp, uint8_t *payload, size_t length, uint8_t channel)
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

void
setupChirp()
{
  chirp = new_chirp_connect(CHIRP_APP_KEY, CHIRP_APP_SECRET);
  if (chirp == NULL)
  {
    Serial.println("Chirp initialisation failed.");
    return;
  }

  chirp_connect_error_code_t err = chirp_connect_set_config(chirp, CHIRP_APP_CONFIG);
  chirpErrorHandler(err);

  chirp_connect_callback_set_t callbacks = {0};
  callbacks.on_state_changed = onStateChangedCallback;
  callbacks.on_receiving = onReceivingCallback;
  callbacks.on_received = onReceivedCallback;

  err = chirp_connect_set_callbacks(chirp, callbacks);
  chirpErrorHandler(err);

  err = chirp_connect_set_callback_ptr(chirp, chirp);
  chirpErrorHandler(err);

  err = chirp_connect_start(chirp);
  chirpErrorHandler(err);

  Serial.println("Chirp Connect initialised.");
}

void
chirpErrorHandler(chirp_connect_error_code_t code)
{
  if (code != CHIRP_CONNECT_OK)
  {
    const char *error_string = chirp_connect_error_code_to_string(code);
    Serial.println(error_string);
    exit(42);
  }
}

// I2S Audio -------------------------------------------------------------------

void
setupAudioInput(int sample_rate)
{
  /*
     Set up I2S audio for SPH0645 microphone
  */
  esp_err_t err;
  Serial.println("Initialising audio input driver..");

  const i2s_config_t i2s_config =
  {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = sample_rate,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = true
  };

  const i2s_pin_config_t pin_config =
  {
    .bck_io_num = I2SI_BCK,
    .ws_io_num = I2SI_LRCL,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2SI_DATA
  };

  err = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  if (err != ESP_OK)
  {
    Serial.printf("Failed installing driver: %d\n", err);
    while (true);
  }

  err = i2s_set_pin(I2S_NUM_0, &pin_config);
  if (err != ESP_OK)
  {
    Serial.printf("Failed setting pin: %d\n", err);
    while (true);
  }

  err = i2s_set_sample_rates(I2S_NUM_0, sample_rate);
  if (err != ESP_OK)
  {
    Serial.printf("Failed to set sample rates: %d\n", err);
    while (true);
  }

  Serial.println("Audio input driver initalised.");
}
