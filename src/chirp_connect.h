/**------------------------------------------------------------------------------
 *
 *  ASIO CONFIDENTIAL
 *
 *  @file chirp_connect.h
 *
 *  All contents are strictly proprietary, and not for copying, resale,
 *  or use outside of the agreed license.
 *
 *  Copyright © 2011-2018, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

#ifndef __CHIRP_CONNECT_H__
#define __CHIRP_CONNECT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "chirp_connect_callbacks.h"
#include "chirp_sdk_defines.h"
#include "chirp_connect_errors.h"
#include "chirp_connect_states.h"

/**
 * Typedef exposing the SDK structure to the API.
 */
typedef struct _chirp_connect_t chirp_connect_t;

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
PUBLIC_SYM chirp_connect_t *new_chirp_connect(const char *key, const char *secret);

/**
 * Free the memory of the SDK. This function should be the last one to be called
 * among all the API functions.
 *
 * During the program life time, this function should be called only one time.
 *
 * @param connect A pointer to the SDK structure that will be deleted.
 * @return        An error code resulting from the call. CHIRP_CONNECT_OK will
 *                be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t del_chirp_connect(chirp_connect_t **connect);

/**
 * Free some memory previously allocated and returned by the SDK.
 * @param ptr The pointer to the memory to be freed.
 */
PUBLIC_SYM void chirp_connect_free(void *ptr);

/**
 * Set the SDK config string coming from your Chirp account. Your Chirp config
 * string configures your application's transmission settings (audio frequency,
 * data rate, payload sizes). To get your Chirp config, sign in to the Chirp
 * Admin Centre at https://developers.chirp.io.
 *
 * @param connect A pointer to the SDK structure which needs the config to be
 *                set.
 * @param config The config string which will be set.
 * @return        An error code resulting from the call. CHIRP_CONNECT_OK will
 *                be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_set_config(chirp_connect_t *connect, const char *config);

/**
 * Return a short description string of the config being used. An example of the
 * type of string can be : "Chirp Connect with "standard-2018" config v1
 * [max 32 bytes in 4.52s]"
 *
 * @param connect A pointer to the SDK structure.
 * @return        The short config description string. The user has to free this
 *                string with `chirp_connect_free` once it is not needed anymore.
 */
PUBLIC_SYM char *chirp_connect_get_info(chirp_connect_t *connect);

/**
 * Set the callbacks to the SDK. Callbacks are functions which will be executed
 * once a key event happens. The list of the supported callbacks is explained in
 * the documentation of the `chirp_connect_callback_set_t` structure in
 * `chirp_connect_callbacks.h`.
 *
 * @param connect   A pointer to the SDK structure.
 * @param callbacks A set of callbacks which will be set to the SDK.
 * @return          An error code resulting from the call. CHIRP_CONNECT_OK will
 *                  be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_set_callbacks(chirp_connect_t *connect, chirp_connect_callback_set_t callbacks);

/**
 * Start the SDK and the audio processing. From this call, it is possible to
 * send and receive data.
 *
 * @param connect A pointer to the SDK structure.
 * @return        An error code resulting from the call. CHIRP_CONNECT_OK will
 *                be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_start(chirp_connect_t *connect);

/**
 * Pause the SDK and the audio processing. No more data can be sent or received.
 * If this is called when sending data, the rest of the audio will resume when
 * leaving the pause state. On the contrary of `chirp_connect_stop`, this function
 * doesn't free any internal memory.
 *
 * @param connect A pointer to the SDK structure.
 * @param pause   A boolean indicating the SDK's `pause` status. If true, the
 *                SDK will pause the audio processing. If false, the SDK will
 *                resume the audio processing.
 * @return        An error code resulting from the call. CHIRP_CONNECT_OK will
 *                be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_pause(chirp_connect_t *connect, bool pause);

/**
 * Stop the SDK and the audio processing. Once this function is called, some
 * internal structures will be reset and any data being sent won't be
 * recoverable.
 *
 * @param connect A pointer to the SDK structure.
 * @return        An error code resulting from the call. CHIRP_CONNECT_OK will
 *                be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_stop(chirp_connect_t *connect);

/**
 * Allocate a new empty payload which will later be filled with the data to send.
 * To release the memory, call `chirp_connect_free` on the data pointer.
 *
 * @param connect A pointer to the SDK structure.
 * @param length  The length, in bytes, of the new payload.
 * @return        A pointer to the newly created data payload.
 */
PUBLIC_SYM uint8_t *chirp_connect_new_payload(chirp_connect_t *connect, size_t length);

/**
 * Get the maximum payload length allowed by the current config set for the SDK.
 *
 * @param connect A pointer to the SDK structure.
 * @return        The maximum payload length that can be sent. A length of 0 is
 *                invalid.
 */
PUBLIC_SYM size_t chirp_connect_get_max_payload_length(chirp_connect_t *connect);

/**
 * Get the duration, in seconds, for a given payload length.
 *
 * @param connect A pointer to the SDK structure.
 * @param length  The length, in bytes, of the payload we want to know the duration.
 * @return        The duration, in second, of the given length.
 */
PUBLIC_SYM float chirp_connect_get_duration_for_payload_length(chirp_connect_t *connect, size_t length);

/**
 * Validate a payload. If uncertain, the user can call this function to confirm
 * this payload can be sent without issues.
 *
 * @param connect A pointer to the SDK structure.
 * @param bytes   A pointer to the payload to be validated.
 * @param length  The length, in bytes, of the payload.
 * @return        An error code resulting from the call. CHIRP_CONNECT_OK will
 *                be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_is_valid(chirp_connect_t *connect, const uint8_t *bytes, size_t length);

/**
 * Generate a random payload by allocating a payload and randomising its content.
 *
 * @param connect A pointer to the SDK structure.
 * @param length  A pointer containing the length, in bytes, of the payload to be
 *                generated. If the length is 0, the SDK will randomise both the
 *                length of the payload and its content. The length pointer will
 *                then be updated with the random length.
 * @return        A pointer to the newly created random data payload.The user
 *                has to free this pointer once they doesn't need it anymore using
 *                `chirp_connect_free`.
 */
PUBLIC_SYM uint8_t *chirp_connect_random_payload(chirp_connect_t *connect, size_t *length);

/**
 * Convert the payload to an hexadecimal string representation to offer a quick
 * and easy human readable way to represent the data.
 *
 * @param connect A pointer to the SDK structure.
 * @param bytes   A pointer to the payload to be validated.
 * @param length  The length, in bytes, of the data payload.
 * @return        The string representation of the payload. The user has to free
 *                this pointer once they doesn't need it anymore using
 *                `chirp_connect_free`.
 */
PUBLIC_SYM char *chirp_connect_as_string(chirp_connect_t *connect, uint8_t *bytes, size_t length);

/**
 * Send a payload.
 *
 * @param connect A pointer to the SDK structure.
 * @param bytes   A pointer to the payload that will be sent.
 * @param length  The length, in bytes, of the payload which will be sent.
 * @return        An error code resulting from the call. CHIRP_CONNECT_OK will
 *                be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_send(chirp_connect_t *connect, uint8_t *bytes, size_t length);

/**
 * Float audio processing function handling both the encoding (output) and the
 * decoding (input).
 *
 * @param connect A pointer to the SDK structure.
 * @param in      A pointer to the float mono input buffer.
 * @param out     A pointer to the float mono output buffer.
 * @param length  The length, in mono samples, of both the input and output buffers.
 * @return        An error code resulting from the call. CHIRP_CONNECT_OK will
 *                be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_process(chirp_connect_t *connect, float *in, float *out, size_t length);

/**
 * Float audio processing function for the decoding (input).
 *
 * @param connect A pointer to the SDK structure.
 * @param buffer  The input buffer containing mono samples which will be decoded.
 * @param length  The length, in mono samples, of the input buffer.
 * @return        An error code resulting from the call. CHIRP_CONNECT_OK will
 *                be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_process_input(chirp_connect_t *connect, float *buffer, size_t length);

/**
 * Float audio processing function for the encoding (output). Fill a buffer with
 * as many bytes as needed once the sending of a payload has been triggered.
 *
 * @param connect A pointer to the SDK structure.
 * @param buffer  The output buffer which will be filled with new mono samples.
 * @param length  The length, in mono samples, of the output buffer.
 * @return        An error code resulting from the call. CHIRP_CONNECT_OK will
 *                be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_process_output(chirp_connect_t *connect, float *buffer, size_t length);

/**
 * Short audio processing function handling both the encoding (output) and the
 * decoding (input).
 *
 * @param connect A pointer to the SDK structure.
 * @param in      A pointer to the short mono input buffer.
 * @param out     A pointer to the short mono output buffer.
 * @param length  The length, in mono samples, of both the input and output buffers.
 * @return        An error code resulting from the call. CHIRP_CONNECT_OK will
 *                be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_process_shorts(chirp_connect_t *connect, short *in, short *out, size_t length);

/**
 * Short audio processing function for the decoding (input).
 *
 * @param connect A pointer to the SDK structure.
 * @param buffer  The input buffer containing mono samples which will be decoded.
 * @param length  The length, in mono samples, of the input buffer.
 * @return        An error code resulting from the call. CHIRP_CONNECT_OK will
 *                be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_process_shorts_input(chirp_connect_t *connect, const short *buffer, size_t length);

/**
 * Short audio processing function for the encoding (output). Fill a buffer,
 * as many times as needed, once the sending of a payload has been triggered.
 *
 * @param connect A pointer to the SDK structure.
 * @param buffer  The output buffer which will be filled with new mono samples.
 * @param length  The length, in mono samples, of the output buffer.
 * @return        An error code resulting from the call. CHIRP_CONNECT_OK will
 *                be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_process_shorts_output(chirp_connect_t *connect, short *buffer, size_t length);

/**
 * Get the SDK state for the given channel.
 *
 * @param connect A pointer to the SDK structure.
 * @param channel The channel we want to know the state of.
 * @return        The channel state of the SDK.
 */
PUBLIC_SYM chirp_connect_state_t chirp_connect_get_state_for_channel(chirp_connect_t *connect, uint8_t channel);

/**
 * Chirp listens for broadcasts on all channels simultaneously, but only
 * transmits on a single channel at a time. This function gets the channel on
 * which the data is sent.
 *
 * @param connect A pointer to the SDK structure.
 * @return        The channel on which the data is sent.
 */
PUBLIC_SYM uint8_t chirp_connect_get_transmission_channel(chirp_connect_t *connect);

/**
 * Set the channel on which the data will be sent. Allowed values are between 0
 * and chirp_connect_get_channel_count() - 1.
 *
 * @param connect A pointer to the SDK structure.
 * @param channel The channel number on which the data should be sent.
 * @return        An error code resulting from the call. CHIRP_CONNECT_OK will
 *                be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_set_transmission_channel(chirp_connect_t *connect, uint8_t channel);

/**
 * Get the number of channels supported by the protocol used. By default, most
 * protocols only support a single channel. To discuss support for multi-channel
 * transmission, please get in touch at developers@chirp.io.
 *
 * @param connect A pointer to the SDK structure.
 * @return        The number of available channels.
 */
PUBLIC_SYM uint8_t chirp_connect_get_channel_count(chirp_connect_t *connect);

/**
 * Get the state of the SDK. Possibles values are defined in chirp_connect_states.h.
 *
 * @param connect A pointer to the SDK structure.
 * @return        The state of the SDK.
 */
PUBLIC_SYM chirp_connect_state_t chirp_connect_get_state(chirp_connect_t *connect);

/**
 * Get the volume of the SDK, between 0 and 1. This volume only influences the
 * SDK's software output volume, and may be affected by the system's hardware
 * audio volume.
 *
 * @param connect A pointer to the SDK structure.
 * @return        The volume of the output of SDK.
 */
PUBLIC_SYM float chirp_connect_get_volume(chirp_connect_t *connect);

/**
 * Set the volume of the output of the SDK.
 *
 * @param connect A pointer to the SDK structure.
 * @param volume  The volume of the output wanted, between 0 and 1. 1 being
 *                maximum volume and 0 being silent.
 * @return        An error code resulting from the call. CHIRP_CONNECT_OK will
 *                be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_set_volume(chirp_connect_t *connect, float volume);

/**
 * Get the sample rate at which the SDK is processing the input.
 *
 * @param connect A pointer to the SDK structure.
 * @return        The actual sample rate used by the SDK to process the input.
 */
PUBLIC_SYM uint32_t chirp_connect_get_input_sample_rate(chirp_connect_t *connect);

/**
 * Get the sample rate at which the SDK is processing the output.
 *
 * @param connect A pointer to the SDK structure.
 * @return        The actual sample rate used by the SDK to process the output.
 */
PUBLIC_SYM uint32_t chirp_connect_get_output_sample_rate(chirp_connect_t *connect);

/**
 * Set the input sample rate of the SDK. It must always be the same as the
 * system's audio I/O sample rate or the decoding will fail.
 *
 * @param connect     A pointer to the SDK structure.
 * @param sample_rate The sample rate wanted for the input.
 * @return            An error code resulting from the call. CHIRP_CONNECT_OK will
 *                    be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_set_input_sample_rate(chirp_connect_t *connect, uint32_t sample_rate);

/**
 * Set the output sample rate of the SDK. It must always be the same as the
 * system's audio I/O sample rate or the encoding will be distorted.
 *
 * @param connect     A pointer to the SDK structure.
 * @param sample_rate The sample rate wanted for the output.
 * @return            An error code resulting from the call. CHIRP_CONNECT_OK will
 *                    be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_set_output_sample_rate(chirp_connect_t *connect, uint32_t sample_rate);

/**
 * Get the SDK's auto-mute state. This automatically mutes the decoder when
 * sending a chirp, to prevent the application from hearing its own chirps.
 * Defaults to true.
 *
 * Set to false if you want your application to be able to hear its own chirps.
 * This is typically only useful for testing and debugging - for example, when
 * passing the output of `process_output` directly into `process_input`
 *
 * @param connect A pointer to the SDK structure.
 * @return        False: The SDK will attempt to decode the payloads it sends.
 *                True: The SDK will ignore the payloads it sends.
 */
PUBLIC_SYM bool chirp_connect_get_auto_mute(chirp_connect_t *connect);

/**
 * Set the auto mute status of the SDK.
 *
 * @param connect   A pointer to the SDK structure.
 * @param auto_mute False: The SDK will attempt to decode the payloads it sends.
 *                  True: The SDK will ignore the payloads it sends.
 * @return          An error code resulting from the call. CHIRP_CONNECT_OK will
 *                  be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_set_auto_mute(chirp_connect_t *connect, bool auto_mute);

/**
 * Set the pointer which is accessible in the callbacks. This function doesn't
 * necessarily need to be to be called. In that case, the pointer passed the
 * callbacks will be NULL.
 *
 * @param connect A pointer to the SDK structure.
 * @param ptr     A pointer to any data you want to pass to the callbacks.
 *                If NULL, the SDK will automatically set it to the `connect`
 *                parameter
 * @return        An error code resulting from the call. CHIRP_CONNECT_OK will
 *                be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_set_callback_ptr(chirp_connect_t *connect, void *ptr);

/**
 * On some systems, the effective audio sample rate is not quite the same as the
 * expected sample rate - for example, if it is being driven by a clock whose
 * frequency is not an integer multiple of the required audio sample rate. This
 * setting rectifies the discrepancy between the two, by multiplying the detected
 * frequency by a fixed frequency correction coefficient.
 *
 * @param connect    A pointer to the SDK structure.
 * @param correction A correction value between 0.5 and 1.5.
 * @return           An error code resulting from the call. CHIRP_CONNECT_OK will
 *                   be returned if everything went well.
 */
PUBLIC_SYM chirp_connect_error_code_t chirp_connect_set_frequency_correction(chirp_connect_t *connect, float correction);

/**
 * Get the version number of the SDK. This function doesn't rely at all on the
 * SDK creation and can be called at any time.
 *
 * @return The version number of the SDK in the MAJOR.MINOR.PATCH string
 *         representation.
 */
PUBLIC_SYM const char *chirp_connect_get_version(void);

#ifdef __cplusplus
}
#endif
#endif // __CHIRP_CONNECT_H__
