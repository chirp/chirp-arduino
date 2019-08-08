/**-----------------------------------------------------------------------------

    Example code to send data using ESP32 and UDA1334 audio output

    @file ESP32Send.ino

    @brief Create a developer account at https://developers.chirp.io,
    and copy and paste your key, secret and config string for the
    "16khz-mono-embedded" protocol into the credentials.h file.

    When the EN switch is pressed on the board, a random chirp will be sent to
    the audio output.

    Note: this example can be used in conjunction with the receive example,
    to send and receive data in the same application.

    Copyright © 2011-2019, Asio Ltd.
    All rights reserved.

  ----------------------------------------------------------------------------*/
#include <driver/i2s.h>

#include "chirp_connect.h"
#include "credentials.h"

#define I2SO_DATA         23     // I2S DATA OUT on GPIO23
#define I2SO_BCK          18     // I2S BCLK on GPIO18
#define I2SO_WSEL         5      // I2S SELECT on GPIO5

#define LED_PIN           2      // LED
#define SWITCH_PIN        0      // Switch

#define VOLUME            0.5    // Between 0 and 1
#define BUFFER_SIZE       512
#define SAMPLE_RATE       16000

// Global variables ------------------------------------------------------------

static chirp_connect_t *chirp = NULL;
static chirp_connect_state_t currentState = CHIRP_CONNECT_STATE_NOT_CREATED;
static volatile bool buttonPressed = false;
static bool startTasks = false;

// Function definitions --------------------------------------------------------

void IRAM_ATTR handleInterrupt();
void setupChirp();
void chirpErrorHandler(chirp_connect_error_code_t code);
void setupAudioOutput(int sample_rate);

// Function declarations -------------------------------------------------------

void
setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), handleInterrupt, FALLING);

  Serial.begin(115200);
  Serial.printf("Heap size: %u\n", ESP.getFreeHeap());

  xTaskCreate(initTask, "initTask", 16384, NULL, 1, NULL);
}

void
loop()
{
  esp_err_t audioError;
  chirp_connect_error_code_t chirpError;

  if (startTasks) {
    xTaskCreate(processOutputTask, "processOutputTask", 16384, NULL, 3, NULL);
    startTasks = false;
  }

  if (buttonPressed) {
    char *payload = "hello";
    chirpError = chirp_connect_send(chirp, (uint8_t *)payload, strlen(payload));
    chirpErrorHandler(chirpError);
    Serial.print("Sending data: ");
    Serial.println(payload);
    buttonPressed = false;
  }
}

void
IRAM_ATTR handleInterrupt()
{
  buttonPressed = true;
}

// Tasks -----------------------------------------------------------------------

void
initTask(void *parameter)
{
  setupChirp();

  uint32_t output_sample_rate = chirp_connect_set_output_sample_rate(chirp, SAMPLE_RATE);
  setupAudioOutput(SAMPLE_RATE);

  Serial.printf("Heap size: %u\n", ESP.getFreeHeap());
  startTasks = true;
  vTaskDelete(NULL);
}

void
processOutputTask(void *parameter)
{
  esp_err_t audioError;
  chirp_connect_error_code_t chirpError;

  size_t bytesLength = 0;
  short buffer[BUFFER_SIZE] = {0};
  int32_t ibuffer[BUFFER_SIZE] = {0};

  while (currentState >= CHIRP_CONNECT_STATE_RUNNING) {
    chirpError = chirp_connect_process_shorts_output(chirp, buffer, BUFFER_SIZE);
    chirpErrorHandler(chirpError);

    for (int i = 0; i < BUFFER_SIZE; i++) {
      ibuffer[i] = (int32_t)buffer[i];
    }
    audioError = i2s_write(I2S_NUM_1, ibuffer, BUFFER_SIZE * 4, &bytesLength, portMAX_DELAY);
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
onSendingCallback(void *chirp, uint8_t *payload, size_t length, uint8_t channel)
{
  Serial.println("Sending data...");
  digitalWrite(LED_PIN, HIGH);
}

void
onSentCallback(void *chirp, uint8_t *payload, size_t length, uint8_t channel)
{
  char *data = chirp_connect_as_string((chirp_connect_t *)chirp, payload, length);
  Serial.printf("Send data: %s\n", data);
  digitalWrite(LED_PIN, LOW);
  chirp_connect_free(data);
}

void
setupChirp()
{
  chirp = new_chirp_connect(CHIRP_APP_KEY, CHIRP_APP_SECRET);
  if (chirp == NULL) {
    Serial.println("Chirp initialisation failed.");
    return;
  }

  chirp_connect_error_code_t err = chirp_connect_set_config(chirp, CHIRP_APP_CONFIG);
  chirpErrorHandler(err);

  chirp_connect_callback_set_t callbacks = {0};
  callbacks.on_sending = onSendingCallback;
  callbacks.on_sent = onSentCallback;
  callbacks.on_state_changed = onStateChangedCallback;

  err = chirp_connect_set_callbacks(chirp, callbacks);
  chirpErrorHandler(err);

  err = chirp_connect_set_callback_ptr(chirp, chirp);
  chirpErrorHandler(err);

  err = chirp_connect_start(chirp);
  chirpErrorHandler(err);

  err = chirp_connect_set_volume(chirp, VOLUME);
  chirpErrorHandler(err);

  Serial.println("Chirp SDK initialised.");
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
setupAudioOutput(int sample_rate)
{
  /*
     Set up I2S audio for UDA1334 DAC output
  */
  esp_err_t err;
  Serial.println("Initialising audio output driver..");

  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = sample_rate,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_LSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = true
  };

  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2SO_BCK,
    .ws_io_num = I2SO_WSEL,
    .data_out_num = I2SO_DATA,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  err = i2s_driver_install(I2S_NUM_1, &i2s_config, 0, NULL);
  if (err != ESP_OK) {
    Serial.printf("Failed installing driver: %d\n", err);
    while (true);
  }

  err = i2s_set_pin(I2S_NUM_1, &pin_config);
  if (err != ESP_OK) {
    Serial.printf("Failed setting pin: %d\n", err);
    while (true);
  }

  err = i2s_set_sample_rates(I2S_NUM_1, sample_rate);
  if (err != ESP_OK) {
    Serial.printf("Failed to set sample rates: %d\n", err);
    while (true);
  }

  Serial.println("Audio output driver initalised.");
}
