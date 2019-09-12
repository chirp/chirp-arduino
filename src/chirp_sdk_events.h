/**-----------------------------------------------------------------------------
 *
 *  ASIO CONFIDENTIAL
 *
 *  @file chirp_sdk_events.h
 *
 *  @brief Events implementations of the C SDK (callbacks and states).
 *
 *  All contents are strictly proprietary, and not for copying, resale,
 *  or use outside of the agreed license.
 *
 *  Copyright © 2011-2019, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

#ifndef CHIRP_SDK_EVENTS_H
#define CHIRP_SDK_EVENTS_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Various states the SDK can return.
 */
typedef enum {
    CHIRP_SDK_STATE_NOT_CREATED, ///< The audio processing has not been initialised yet.
    CHIRP_SDK_STATE_STOPPED, ///< The SDK is not processing audio.
    CHIRP_SDK_STATE_RUNNING, ///< The audio processing is running.
    CHIRP_SDK_STATE_SENDING, ///< The SDK is sending data.
    CHIRP_SDK_STATE_RECEIVING, ///< The SDK is receiving data.
} chirp_sdk_state_t;

/**
 * On_sending, on_sent, on_receiving and on_received callback prototype
 * definitions. These callbacks are called if set and respectively if the SDK is
 * sending, sent, is receiving or received some data.
 *
 * @param ptr     Pointer of data. It's either the pointer set when calling
 *                `chirp_sdk_set_callback_ptr` or NULL.
 * @param bytes   on_sending : The data being sent.
 *                on_sent : The data sent.
 *                on_receiving : NULL.
 *                on_received : NULL if the decode failed or the data received.
 * @param length  on_sending : The length, in bytes, of the data being sent.
 *                on_sent : The length, in bytes, of the data sent.
 *                on_receiving : 0.
 *                on_received : 0 if the decode failed or the length, in bytes,
 *                of the data received.
 * @param channel Channel on which the data has been received.
 */
typedef void (*chirp_sdk_callback_t)(void *ptr, uint8_t *bytes, size_t length, uint8_t channel);

/**
 * On_state_changed callback prototype definition. This is called if the
 * callback has been set and when the SDK's state is changing.
 * The possibles values are the one of the `chirp_sdk_state_t` enum
 * located in `chirp_sdk_states.h`.
 *
 * @param ptr       Pointer of data. It's either the pointer set when calling
 *                  `chirp_sdk_set_callback_ptr` or NULL.
 * @param old_state The old state of the SDK before reaching this callback.
 * @param new_state The new state of the SDK when leaving this callback.
 */
typedef void (*chirp_sdk_state_callback_t)(void *ptr, chirp_sdk_state_t old_state, chirp_sdk_state_t new_state);

/**
 * Structure containing the callbacks pointers. It is not necessary to set all
 * the fields. Not setting a callback will only result in not being notified of
 * the event.
 */
typedef struct {
    chirp_sdk_state_callback_t on_state_changed; ///< Triggered when the SDK's state is changing.
    chirp_sdk_callback_t on_sending; ///< Triggered when the SDK starts sending some data.
    chirp_sdk_callback_t on_sent; ///< Triggered when the SDK has sent the data.
    chirp_sdk_callback_t on_receiving; ///< Triggered when the SDK starts receiving some data.
    chirp_sdk_callback_t on_received; ///< Triggered when the SDK has received the data.
} chirp_sdk_callback_set_t;

#ifdef __cplusplus
}
#endif

#endif /* !CHIRP_SDK_EVENTS_H */
