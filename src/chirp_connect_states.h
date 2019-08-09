/**-----------------------------------------------------------------------------
 *
 *  ASIO CONFIDENTIAL
 *
 *  @file chirp_connect_states.h
 *
 *  @brief Enumeration of the possible states of the SDK.
 *
 *  All contents are strictly proprietary, and not for copying, resale,
 *  or use outside of the agreed license.
 *
 *  Copyright © 2011-2019, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

#ifndef CHIRP_CONNECT_STATES_H
#define CHIRP_CONNECT_STATES_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Various states the SDK can return.
 */
typedef enum {
    CHIRP_CONNECT_STATE_NOT_CREATED, ///< The audio processing has not been initialised yet.
    CHIRP_CONNECT_STATE_STOPPED, ///< The SDK is not processing audio.
    CHIRP_CONNECT_STATE_RUNNING, ///< The audio processing is running.
    CHIRP_CONNECT_STATE_SENDING, ///< The SDK is sending data.
    CHIRP_CONNECT_STATE_RECEIVING, ///< The SDK is receiving data.
} chirp_connect_state_t;

#ifdef __cplusplus
}
#endif

#endif /* !CHIRP_CONNECT_STATES_H */
