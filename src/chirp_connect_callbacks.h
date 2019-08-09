/**------------------------------------------------------------------------------
 *
 *  ASIO CONFIDENTIAL
 *
 *  @file chirp_connect_callbacks.h
 *
 *  @brief Callback implementations of the C SDK.
 *
 *  All contents are strictly proprietary, and not for copying, resale,
 *  or use outside of the agreed license.
 *
 *  Copyright © 2011-2019, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

#ifndef CHIRP_CONNECT_CALLBACKS_H
#define CHIRP_CONNECT_CALLBACKS_H

#include <stdint.h>
#include <stddef.h>

#include "chirp_connect_states.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * On_sending, on_sent, on_receiving and on_received callback prototype
 * definitions. These callbacks are called if set and respectively if the SDK is
 * sending, sent, is receiving or received some data.
 *
 * @param ptr     Pointer of data. It's either the pointer set when calling
 *                `chirp_connect_set_callback_ptr` or NULL.
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
typedef void (*chirp_connect_callback_t)(void *ptr, uint8_t *bytes, size_t length, uint8_t channel);

/**
 * On_state_changed callback prototype definition. This is called if the
 * callback has been set and when the SDK's state is changing.
 * The possibles values are the one of the `chirp_connect_state_t` enum
 * located in `chirp_connect_states.h`.
 *
 * @param ptr       Pointer of data. It's either the pointer set when calling
 *                  `chirp_connect_set_callback_ptr` or NULL.
 * @param old_state The old state of the SDK before reaching this callback.
 * @param new_state The new state of the SDK when leaving this callback.
 */
typedef void (*chirp_connect_state_callback_t)(void *ptr, chirp_connect_state_t old_state, chirp_connect_state_t new_state);

/**
 * Structure containing the callbacks pointers. It is not necessary to set all
 * the fields. Not setting a callback will only result in not being notified of
 * the event.
 */
typedef struct {
    chirp_connect_state_callback_t on_state_changed; ///< Triggered when the SDK's state is changing.
    chirp_connect_callback_t on_sending; ///< Triggered when the SDK starts sending some data.
    chirp_connect_callback_t on_sent; ///< Triggered when the SDK has sent the data.
    chirp_connect_callback_t on_receiving; ///< Triggered when the SDK starts receiving some data.
    chirp_connect_callback_t on_received; ///< Triggered when the SDK has received the data.
} chirp_connect_callback_set_t;

#ifdef __cplusplus
}
#endif

#endif /* !CHIRP_CONNECT_CALLBACKS_H */
