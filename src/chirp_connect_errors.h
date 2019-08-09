/**------------------------------------------------------------------------------
 *
 *  ASIO CONFIDENTIAL
 *
 *  @file chirp_connect_errors.h
 *
 *  @brief Error handling of the SDK.
 *
 *  All contents are strictly proprietary, and not for copying, resale,
 *  or use outside of the agreed license.
 *
 *  Copyright © 2011-2019, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

#ifndef CHIRP_CONNECT_ERRORS_H
#define CHIRP_CONNECT_ERRORS_H

#include "chirp_connect.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Various error codes the SDK can return. Note that some of the values don't
 * apply on this SDK.
 */
typedef enum {
    CHIRP_CONNECT_OK = 0, ///< No error.
    CHIRP_CONNECT_OUT_OF_MEMORY, ///< The SDK ran out of memory.
    CHIRP_CONNECT_NOT_INITIALISED, ///< The SDK hasn't been initialised, did you forget to set the config?
    CHIRP_CONNECT_INTERNAL_ERROR, ///< An internal error prevented the SDK from initialising correctly.
    CHIRP_CONNECT_MEMORY_LEAK, ///< Some memory hasn't been freed leading to some leaks.
    CHIRP_CONNECT_RECEIVING_NOT_AVAILABLE, ///< Receiving mode has been disabled and is not available.

    CHIRP_CONNECT_NOT_RUNNING, ///< The SDK is not running.
    CHIRP_CONNECT_ALREADY_RUNNING, ///< The SDK is already running.
    CHIRP_CONNECT_ALREADY_STOPPED, ///< The SDK has already stopped.
    CHIRP_CONNECT_ALREADY_SENDING, ///< The SDK is already sending.

    CHIRP_CONNECT_INVALID_SAMPLE_RATE = 20, ///< The sample rate is invalid (it must respect Nyquist law).
    CHIRP_CONNECT_NULL_BUFFER, ///< One of the parameters is a NULL buffer.
    CHIRP_CONNECT_NULL_POINTER, ///< One of the parameters is a NULL pointer.
    CHIRP_CONNECT_CHANNEL_NOT_SUPPORTED, ///< The channel asked is bigger than the maximum one authorised by the config being used.
    CHIRP_CONNECT_INVALID_FREQUENCY_CORRECTION, ///< Invalid frequency correction value.
    CHIRP_CONNECT_PROCESSING_ERROR, ///< An internal issue happened when processing.

    CHIRP_CONNECT_INVALID_KEY = 40, ///< Invalid application key.
    CHIRP_CONNECT_INVALID_SECRET, ///< Invalid application secret.
    CHIRP_CONNECT_INVALID_CREDENTIALS, ///< Invalid application credentials.
    CHIRP_CONNECT_MISSING_SIGNATURE, ///< Signature is missing from the config.
    CHIRP_CONNECT_INVALID_SIGNATURE, ///< Signature couldn't be verified.
    CHIRP_CONNECT_MISSING_CONFIG, ///< Config information is missing.
    CHIRP_CONNECT_INVALID_CONFIG, ///< Config information is invalid.
    CHIRP_CONNECT_EXPIRED_CONFIG, ///< This config has expired.
    CHIRP_CONNECT_INVALID_VERSION, ///< This config was generated for a different version. Please visit https://developers.chirp.io to upgrade your SDK.
    CHIRP_CONNECT_INVALID_PROJECT, ///< This config was generated for a different project.
    /*--------------------------------------------------------------------------
     * CHIRP_CONNECT_INVALID_CONFIG_CHARACTER needs to be kept at the end of
     * the credentials error code list as it deals with base64 but it is
     * implemented in chirp-connect.
     *------------------------------------------------------------------------*/
    CHIRP_CONNECT_INVALID_CONFIG_CHARACTER, ///< Your config contains one or many unknown character(s).

    CHIRP_CONNECT_PAYLOAD_EMPTY_MESSAGE = 80, ///< The payload is empty.
    CHIRP_CONNECT_PAYLOAD_INVALID_MESSAGE, ///< The payload is invalid.
    CHIRP_CONNECT_PAYLOAD_UNKNOWN_SYMBOLS, ///< The payload contains unknown symbols.
    CHIRP_CONNECT_PAYLOAD_DECODE_FAILED, ///< Couldn't decode the payload.
    CHIRP_CONNECT_PAYLOAD_TOO_LONG, ///< The payload's length is longer than the maximum one authorised by the config being used.
    CHIRP_CONNECT_PAYLOAD_TOO_SHORT, ///< The payload's length is shorter than the minimum one authorised by the config being used..

    CHIRP_CONNECT_INVALID_VOLUME = 99, ///< Volume value is incorrect.
    CHIRP_CONNECT_UNKNOWN_ERROR = 100, ///< The SDK has reported an unknown error.

    /*--------------------------------------------------------------------------
     * Reserved for the high level SDKs. Don't update it but rather add an issue
     * in the C-SDK.
     *------------------------------------------------------------------------*/
    CHIRP_CONNECT_NETWORK_ERROR = 200, ///< "Network error."
    CHIRP_CONNECT_NETWORK_NO_NETWORK, ///< "Couldn't reach the server, please check your network connection."
    CHIRP_CONNECT_NETWORK_PERMISSIONS_NOT_GRANTED, ///< "Network permissions were not granted by the application or user. Please add network permissions to your application, or contact sales@chirp.io to request completely offline operation."
    CHIRP_CONNECT_ACCOUNT_DISABLED, ///< "Your account has been disabled due to an unpaid license. Please contact sales@chirp.io."
    CHIRP_CONNECT_AUDIO_IO_ERROR, ///< "Audio IO error."
    CHIRP_CONNECT_SENDING_NOT_ENABLED, ///< "Send mode hasn't been enabled."
    CHIRP_CONNECT_RECEIVING_NOT_ENABLED, ///< "Receive mode hasn't been enabled."
    CHIRP_CONNECT_DEVICE_IS_MUTED, ///< "The device is muted. Cannot send data."
} chirp_connect_error_code_t;

/**
 * Convert a `chirp_connect_error_code_t` code to a string describing the error.
 *
 * @param err The error code which needs to be detailed.
 * @return    The string describing the error code.
 */
PUBLIC_SYM const char *chirp_connect_error_code_to_string(chirp_connect_error_code_t err);

#ifdef __cplusplus
}
#endif

#endif /* !CHIRP_CONNECT_ERRORS_H */
