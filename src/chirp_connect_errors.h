/**------------------------------------------------------------------------------
 *
 *  ASIO CONFIDENTIAL
 *
 *  @file chirp_connect_errors.h
 *
 *  All contents are strictly proprietary, and not for copying, resale,
 *  or use outside of the agreed license.
 *
 *  Copyright © 2011-2018, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

#ifndef __CHIRP_CONNECT_ERROR_H__
#define __CHIRP_CONNECT_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "chirp_sdk_defines.h"

/**
 * Various error codes the SDK can return.
 */
typedef enum {
    CHIRP_CONNECT_OK = 0,
    CHIRP_CONNECT_OUT_OF_MEMORY,
    CHIRP_CONNECT_NOT_INITIALISED,

    CHIRP_CONNECT_NOT_STARTED = 10,
    CHIRP_CONNECT_NOT_STOPPED,
    CHIRP_CONNECT_NOT_RUNNING,
    CHIRP_CONNECT_ALREADY_RUNNING,
    CHIRP_CONNECT_ALREADY_STOPPED,

    CHIRP_CONNECT_INVALID_SAMPLE_RATE = 20,
    CHIRP_CONNECT_NULL_BUFFER,
    CHIRP_CONNECT_NULL_POINTER,
    CHIRP_CONNECT_INVALID_LENGTH,
    CHIRP_CONNECT_INVALID_CHANNEL,
    CHIRP_CONNECT_INVALID_FREQUENCY_CORRECTION,

    CHIRP_CONNECT_INVALID_KEY = 40,
    CHIRP_CONNECT_INVALID_SECRET,
    CHIRP_CONNECT_INVALID_CREDENTIALS,
    CHIRP_CONNECT_MISSING_SIGNATURE,
    CHIRP_CONNECT_INVALID_SIGNATURE,
    CHIRP_CONNECT_MISSING_CONFIG,
    CHIRP_CONNECT_INVALID_CONFIG,
    CHIRP_CONNECT_EXPIRED_CONFIG,
    CHIRP_CONNECT_INVALID_VERSION,
    CHIRP_CONNECT_INVALID_PROJECT,
    /*--------------------------------------------------------------------------
     * CHIRP_CONNECT_INVALID_CONFIG_CHARACTER needs to be kept at the end of
     * the credentials error code list as it deals with base64 but it is
     * implemented in chirp-connect.
     *------------------------------------------------------------------------*/
    CHIRP_CONNECT_INVALID_CONFIG_CHARACTER,

    CHIRP_CONNECT_PAYLOAD_EMPTY_MESSAGE = 80,
    CHIRP_CONNECT_PAYLOAD_NO_ALPHABET,
    CHIRP_CONNECT_PAYLOAD_INVALID_MESSAGE,
    CHIRP_CONNECT_PAYLOAD_INVALID_MESSAGE_LENGTH,
    CHIRP_CONNECT_PAYLOAD_INVALID_ENCODED_LENGTH,
    CHIRP_CONNECT_PAYLOAD_UNKNOWN_SYMBOLS,
    CHIRP_CONNECT_PAYLOAD_DECODE_FAILED,
    CHIRP_CONNECT_PAYLOAD_TOO_LONG,

    CHIRP_CONNECT_INVALID_VOLUME = 99,
    CHIRP_CONNECT_UNKNOWN_ERROR = 100,

    CHIRP_CONNECT_NETWORK_ERROR = 105,
    CHIRP_CONNECT_NETWORK_NO_NETWORK,
    CHIRP_CONNECT_NETWORK_PERMISSIONS_NOT_GRANTED,

    CHIRP_CONNECT_ACCOUNT_DISABLED = 110,

    CHIRP_CONNECT_AUDIO_IO = 120,
} chirp_connect_error_code_t;

/**
 * Convert a `chirp_connect_error_code_t` code to a string describe the error.
 *
 * @param err The error code which needs to be detailed.
 * @return    The string describing the error code.
 */
PUBLIC_SYM const char *chirp_connect_error_code_to_string(chirp_connect_error_code_t err);

#ifdef __cplusplus
}
#endif
#endif // __CHIRP_CONNECT_ERROR_H__
