/**------------------------------------------------------------------------------
 *
 *  ASIO CONFIDENTIAL
 *
 *  @file chirp_connect_states.h
 *
 *  All contents are strictly proprietary, and not for copying, resale,
 *  or use outside of the agreed license.
 *
 *  Copyright © 2011-2018, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

#ifndef __CHIRP_CONNECT_STATE_H__
#define __CHIRP_CONNECT_STATE_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Various states the SDK can return.
 */
typedef enum {
    // The audio processing has not been initialised yet
    CHIRP_CONNECT_STATE_NOT_CREATED,
    // The SDK is not processing audio
    CHIRP_CONNECT_STATE_STOPPED,
    // The audio processing is paused
    CHIRP_CONNECT_STATE_PAUSED,
    // The audio processing is running
    CHIRP_CONNECT_STATE_RUNNING,
    // The SDK is sending data
    CHIRP_CONNECT_STATE_SENDING,
    // The SDK is receiving data
    CHIRP_CONNECT_STATE_RECEIVING,
} chirp_connect_state_t;

#ifdef __cplusplus
}
#endif
#endif // __CHIRP_CONNECT_STATE_H__
