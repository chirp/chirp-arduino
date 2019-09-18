/**-----------------------------------------------------------------------------
 *
 *  ASIO CONFIDENTIAL
 *
 *  @file chirp_sdk.h
 *
 *  @brief Chirp C SDK implementation header.
 *
 *  All contents are strictly proprietary, and not for copying, resale,
 *  or use outside of the agreed license.
 *
 *  Copyright © 2011-2019, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

#ifndef CHIRP_SDK_H
#define CHIRP_SDK_H

#include <stdbool.h>
#include <stdint.h>

/**
* Mark the function as public. Any attempt to call a function without this
* marker will fail.
*/
#if defined(__WIN32) || defined(_WIN32) || defined(WIN32)
#define PUBLIC_SYM __declspec(dllexport)
#else
#define PUBLIC_SYM __attribute__ ((visibility ("default")))
#endif

#include "chirp_sdk_errors.h"
#include "chirp_sdk_events.h"
#include "chirp_sdk_version.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Typedef exposing the SDK structure to the API.
 */
typedef struct _chirp_sdk_t chirp_sdk_t;

/**
 * Allocate the memory and create the SDK structure. This function should be the
 * first one to be called among all the API functions.
 *
 * During the program life time, this function should be called only one time.
 *
 * @param key    The application key coming from your Chirp Account.
 * @param secret The application secret coming from your Chirp Account.
 * @return       A pointer to the newly allocated SDK structure.
 */
PUBLIC_SYM chirp_sdk_t *new_chirp_sdk(const char *key, const char *secret);

/**
 * Release the SDK. This function should be the last one to be called among all
 * the API functions.
 *
 * During the program life time, this function should be called only one time.
 *
 * @param sdk A pointer to the SDK structure that will be deleted.
 * @return    An error code resulting from the call. CHIRP_SDK_OK will
 *            be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t del_chirp_sdk(chirp_sdk_t **sdk);

/**
 * Free memory previously allocated and returned by the SDK.
 *
 * Note that this does not free the SDK itself. For this, use `del_chirp_sdk`
 * above.
 *
 * This function should be called to free the memory returned by the
 * following functions:
 *
 *   `chirp_sdk_get_info`
 *   `chirp_sdk_random_payload`
 *
 * As well as freeing the memory, this function tracks the ongoing heap
 * allocation which can be queried with `chirp_sdk_get_heap_usage`.
 *
 * @param ptr The pointer to the memory to be freed.
 */
PUBLIC_SYM void chirp_sdk_free(void *ptr);

/**
 * Set the SDK config string coming from your Chirp account. Your Chirp config
 * string configures your application's transmission settings (audio frequency,
 * data rate, payload sizes). To get your Chirp config, sign in to the Chirp
 * Admin Centre at https://developers.chirp.io.
 *
 * @param sdk    A pointer to the SDK structure which needs the config to be
 *               set.
 * @param config The config string which will be set.
 * @return       An error code resulting from the call. CHIRP_SDK_OK will
 *               be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_set_config(chirp_sdk_t *sdk, const char *config);

/**
 * Return a short description string of the config being used. An example of the
 * type of string can be : "Chirp SDK with "standard-2018" config v1
 * [max 32 bytes in 4.52s]"
 *
 * @param sdk A pointer to the SDK structure.
 * @return    The short config description string. The user has to free this
 *            string with `chirp_sdk_free` once it is not needed anymore.
 */
PUBLIC_SYM char *chirp_sdk_get_info(chirp_sdk_t *sdk);

/**
 * Set the callbacks to the SDK. Callbacks are functions which will be executed
 * once a key event happens. The list of the supported callbacks is explained in
 * the documentation of the `chirp_sdk_callback_set_t` structure in
 * `chirp_sdk_callbacks.h`.
 *
 * @param sdk          A pointer to the SDK structure.
 * @param callback_set A set of callbacks which will be set to the SDK.
 * @return             An error code resulting from the call. CHIRP_SDK_OK will
 *                     be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_set_callbacks(chirp_sdk_t *sdk, chirp_sdk_callback_set_t callback_set);

/**
 * Start the SDK and the audio processing. From this call, it is possible to
 * send and receive data.
 *
 * @param sdk A pointer to the SDK structure.
 * @return    An error code resulting from the call. CHIRP_SDK_OK will
 *            be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_start(chirp_sdk_t *sdk);

/**
 * Stop the SDK and the audio processing. Once this function is called, some
 * internal structures will be reset and any data being sent won't be
 * recoverable.
 *
 * @param sdk A pointer to the SDK structure.
 * @return    An error code resulting from the call. CHIRP_SDK_OK will
 *            be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_stop(chirp_sdk_t *sdk);

/**
 * Get the maximum payload length allowed by the current config set for the SDK.
 *
 *
 * @param sdk A pointer to the SDK structure.
 * @return    The maximum payload length that can be sent. A length of 0 is
 *            invalid. If the config hasn't been set yet when this function
 *            is called 0 is returned.
 */
PUBLIC_SYM size_t chirp_sdk_get_max_payload_length(chirp_sdk_t *sdk);

/**
 * Get the duration, in seconds, for a given payload length.
 *
 * @param sdk            A pointer to the SDK structure.
 * @param payload_length The length, in bytes, of the payload we want to know the
 *                       duration. You can get the maximum allowed length with
 *                       `chirp_sdk_get_max_payload_length`.
 * @return               The duration, in second, of the given length, -1 if the
 *                       payload is too short or -2 if the payload is too long.
 */
PUBLIC_SYM float chirp_sdk_get_duration_for_payload_length(chirp_sdk_t *sdk, size_t payload_length);

/**
 * Validate a payload. If uncertain, the user can call this function to confirm
 * this payload can be sent without issues.
 *
 * @param sdk    A pointer to the SDK structure.
 * @param bytes  A pointer to the payload to be validated.
 * @param length The length, in bytes, of the payload.
 * @return       An error code resulting from the call. CHIRP_SDK_OK will
 *               be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_is_valid(chirp_sdk_t *sdk, const uint8_t *bytes, size_t length);

/**
 * Generate a random payload by allocating a payload and randomising its content.
 *
 * @param sdk     A pointer to the SDK structure.
 * @param length  A pointer containing the length, in bytes, of the payload to be
 *                generated. If the length is 0, the SDK will randomise both the
 *                length of the payload and its content. The length pointer will
 *                then be updated with the random length. You can get the
 *                maximum allowed length with `chirp_sdk_get_max_payload_length`.
 * @return        A pointer to the newly created random data payload.The user
 *                has to free this pointer once they doesn't need it anymore using
 *                `chirp_sdk_free`.
 */
PUBLIC_SYM uint8_t *chirp_sdk_random_payload(chirp_sdk_t *sdk, size_t *length);

/**
 * Send a payload. A valid length is between 1 and the value returned by
 * `chirp_sdk_get_max_payload_length`.
 *
 * @param sdk    A pointer to the SDK structure.
 * @param bytes  A pointer to the payload that will be sent.
 * @param length The length, in bytes, of the payload which will be sent.
 * @return       An error code resulting from the call. CHIRP_SDK_OK will
 *               be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_send(chirp_sdk_t *sdk, uint8_t *bytes, size_t length);

/**
 * Float audio processing function handling both the encoding (output) and the
 * decoding (input).
 *
 * @param sdk    A pointer to the SDK structure.
 * @param in     A pointer to the float mono input buffer.
 * @param out    A pointer to the float mono output buffer.
 * @param length The length, in mono samples, of both the input and output buffers.
 * @return       An error code resulting from the call. CHIRP_SDK_OK will
 *               be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_process(chirp_sdk_t *sdk, float *in, float *out, size_t length);

/**
 * Float audio processing function for the decoding (input).
 *
 * @param sdk    A pointer to the SDK structure.
 * @param buffer The input buffer containing mono samples which will be decoded.
 * @param length The length, in mono samples, of the input buffer.
 * @return       An error code resulting from the call. CHIRP_SDK_OK will
 *               be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_process_input(chirp_sdk_t *sdk, float *buffer, size_t length);

/**
 * Float audio processing function for the encoding (output). Fill a buffer with
 * as many bytes as needed once the sending of a payload has been triggered.
 *
 * @param sdk    A pointer to the SDK structure.
 * @param buffer The output buffer which will be filled with new mono samples.
 * @param length The length, in mono samples, of the output buffer.
 * @return       An error code resulting from the call. CHIRP_SDK_OK will
 *               be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_process_output(chirp_sdk_t *sdk, float *buffer, size_t length);

/**
 * Short audio processing function handling both the encoding (output) and the
 * decoding (input).
 *
 * @param sdk    A pointer to the SDK structure.
 * @param in     A pointer to the short mono input buffer.
 * @param out    A pointer to the short mono output buffer.
 * @param length The length, in mono samples, of both the input and output buffers.
 * @return       An error code resulting from the call. CHIRP_SDK_OK will
 *               be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_process_shorts(chirp_sdk_t *sdk, short *in, short *out, size_t length);

/**
 * Short audio processing function for the decoding (input).
 *
 * @param sdk    A pointer to the SDK structure.
 * @param buffer The input buffer containing mono samples which will be decoded.
 * @param length The length, in mono samples, of the input buffer.
 * @return       An error code resulting from the call. CHIRP_SDK_OK will
 *               be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_process_shorts_input(chirp_sdk_t *sdk, const short *buffer, size_t length);

/**
 * Short audio processing function for the encoding (output). Fill a buffer,
 * as many times as needed, once the sending of a payload has been triggered.
 *
 * @param sdk    A pointer to the SDK structure.
 * @param buffer The output buffer which will be filled with new mono samples.
 * @param length The length, in mono samples, of the output buffer.
 * @return       An error code resulting from the call. CHIRP_SDK_OK will
 *               be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_process_shorts_output(chirp_sdk_t *sdk, short *buffer, size_t length);

/**
 * Get the SDK state for the given channel.
 *
 * @param sdk     A pointer to the SDK structure.
 * @param channel The channel we want to know the state of.
 * @return        The channel state of the SDK.
 */
PUBLIC_SYM chirp_sdk_state_t chirp_sdk_get_state_for_channel(chirp_sdk_t *sdk, uint8_t channel);

/**
 * Chirp listens for broadcasts on all channels simultaneously, but only
 * transmits on a single channel at a time. This function gets the channel on
 * which the data is sent.
 *
 * @param sdk A pointer to the SDK structure.
 * @return    The channel on which the data is sent (including 0) or -1 if
 *            the SDK hasn't been initialised.
 */
PUBLIC_SYM int8_t chirp_sdk_get_transmission_channel(chirp_sdk_t *sdk);

/**
 * Set the channel on which the data will be sent. Allowed values are between 0
 * and chirp_sdk_get_channel_count() - 1.
 *
 * @param sdk     A pointer to the SDK structure.
 * @param channel The channel number on which the data should be sent.
 * @return        An error code resulting from the call. CHIRP_SDK_OK will
 *                be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_set_transmission_channel(chirp_sdk_t *sdk, uint8_t channel);

/**
 * Get the number of channels supported by the protocol used. By default, most
 * protocols only support a single channel. To discuss support for multi-channel
 * transmission, please get in touch at developers@chirp.io.
 *
 * @param sdk A pointer to the SDK structure.
 * @return    The number of available channels.
 */
PUBLIC_SYM uint8_t chirp_sdk_get_channel_count(chirp_sdk_t *sdk);

/**
 * Get the state of the SDK. Possibles values are defined in chirp_sdk_states.h.
 *
 * @param sdk A pointer to the SDK structure.
 * @return    The state of the SDK.
 */
PUBLIC_SYM chirp_sdk_state_t chirp_sdk_get_state(chirp_sdk_t *sdk);

/**
 * Get the volume of the SDK, between 0 and 1. This volume only influences the
 * SDK's software output volume, and may be affected by the system's hardware
 * audio volume.
 *
 * @param sdk A pointer to the SDK structure.
 * @return    The volume of the output of the SDK or -1 if an error happened.
 */
PUBLIC_SYM float chirp_sdk_get_volume(chirp_sdk_t *sdk);

/**
 * Set the volume of the output of the SDK.
 *
 * @param sdk    A pointer to the SDK structure.
 * @param volume The volume of the output wanted, between 0 and 1. 1 being
 *               maximum volume and 0 being silent.
 * @return       An error code resulting from the call. CHIRP_SDK_OK will
 *               be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_set_volume(chirp_sdk_t *sdk, float volume);

/**
 * Get the sample rate at which the SDK is processing the input.
 *
 * @param sdk A pointer to the SDK structure.
 * @return    The actual sample rate used by the SDK to process the input.
 */
PUBLIC_SYM uint32_t chirp_sdk_get_input_sample_rate(chirp_sdk_t *sdk);

/**
 * Get the sample rate at which the SDK is processing the output.
 *
 * @param sdk A pointer to the SDK structure.
 * @return    The actual sample rate used by the SDK to process the output.
 */
PUBLIC_SYM uint32_t chirp_sdk_get_output_sample_rate(chirp_sdk_t *sdk);

/**
 * Set the input sample rate of the SDK. It must always be the same as the
 * system's audio I/O sample rate or the decoding will fail.
 *
 * @param sdk         A pointer to the SDK structure.
 * @param sample_rate The sample rate wanted for the input.
 * @return            An error code resulting from the call. CHIRP_SDK_OK will
 *                    be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_set_input_sample_rate(chirp_sdk_t *sdk, uint32_t sample_rate);

/**
 * Set the output sample rate of the SDK. It must always be the same as the
 * system's audio I/O sample rate or the encoding will be distorted.
 *
 * @param sdk         A pointer to the SDK structure.
 * @param sample_rate The sample rate wanted for the output.
 * @return            An error code resulting from the call. CHIRP_SDK_OK will
 *                    be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_set_output_sample_rate(chirp_sdk_t *sdk, uint32_t sample_rate);

/**
 * Get the SDK's listen to self state. This automatically mutes the decoder when
 * sending a chirp, to prevent the application from hearing its own chirps.
 * Defaults to false.
 *
 * Set to true if you want your application to be able to hear its own chirps.
 * This is typically only useful for testing and debugging - for example, when
 * passing the output of `process_output` directly into `process_input`
 *
 * @param sdk A pointer to the SDK structure.
 * @return    True : The SDK will attempt to decode the payloads it sends.
 *            False : The SDK will ignore the payloads it sends.
 */
PUBLIC_SYM bool chirp_sdk_get_listen_to_self(chirp_sdk_t *sdk);

/**
 * Set the listen to self status of the SDK.
 *
 * @param sdk            A pointer to the SDK structure.
 * @param listen_to_self True: The SDK will attempt to decode the payloads it sends.
*                        False: The SDK will ignore the payloads it sends.
 * @return               An error code resulting from the call. CHIRP_SDK_OK will
 *                       be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_set_listen_to_self(chirp_sdk_t *sdk, bool listen_to_self);

/**
 * Set the pointer which is accessible in the callbacks. This function doesn't
 * necessarily need to be to be called. In that case, the pointer passed the
 * callbacks will be NULL.
 *
 * @param sdk A pointer to the SDK structure.
 * @param ptr A pointer to any data you want to pass to the callbacks.
 * @return    An error code resulting from the call. CHIRP_SDK_OK will
 *            be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_set_callback_ptr(chirp_sdk_t *sdk, void *ptr);

/**
 * On some systems, the effective audio sample rate is not quite the same as the
 * expected sample rate - for example, if it is being driven by a clock whose
 * frequency is not an integer multiple of the required audio sample rate. This
 * setting rectifies the discrepancy between the two, by multiplying the detected
 * frequency by a fixed frequency correction coefficient.
 *
 * @param sdk        A pointer to the SDK structure.
 * @param correction A correction value between 0.5 and 1.5.
 * @return           An error code resulting from the call. CHIRP_SDK_OK will
 *                   be returned if everything went well.
 */
PUBLIC_SYM chirp_sdk_error_code_t chirp_sdk_set_frequency_correction(chirp_sdk_t *sdk, float correction);

/**
 * Return the current heap usage of the SDK, in bytes.
 *
 * @param sdk A pointer to the SDK structure.
 * @return    The heap usage in bytes of the SDK.
 */
PUBLIC_SYM int32_t chirp_sdk_get_heap_usage(chirp_sdk_t *sdk);

#ifdef __cplusplus
}
#endif

#endif /* !CHIRP_SDK_H */
