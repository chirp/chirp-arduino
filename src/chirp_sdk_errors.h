/**-----------------------------------------------------------------------------
 *
 *  ASIO CONFIDENTIAL
 *
 *  @file chirp_sdk_errors.h
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

#ifndef CHIRP_SDK_ERRORS_H
#define CHIRP_SDK_ERRORS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Various error codes the SDK can return. Note that some of the values don't
 * apply on this SDK.
 */
typedef enum {
    CHIRP_SDK_OK = 0, ///< No error.
    CHIRP_SDK_OUT_OF_MEMORY, ///< The SDK ran out of memory.
    CHIRP_SDK_NOT_INITIALISED, ///< The SDK hasn't been initialised, did you forget to set the config?
    CHIRP_SDK_INTERNAL_ERROR, ///< An internal error prevented the SDK from initialising correctly.
    CHIRP_SDK_MEMORY_LEAK, ///< Some memory hasn't been freed leading to some leaks.
    CHIRP_SDK_RECEIVING_NOT_AVAILABLE, ///< Receiving mode has been disabled and is not available.

    CHIRP_SDK_NOT_RUNNING, ///< The SDK is not running.
    CHIRP_SDK_ALREADY_RUNNING, ///< The SDK is already running.
    CHIRP_SDK_ALREADY_STOPPED, ///< The SDK has already stopped.
    CHIRP_SDK_ALREADY_SENDING, ///< The SDK is already sending.

    CHIRP_SDK_INVALID_SAMPLE_RATE = 20, ///< The sample rate is invalid (it must respect Nyquist law).
    CHIRP_SDK_NULL_BUFFER, ///< One of the parameters is a NULL buffer.
    CHIRP_SDK_NULL_POINTER, ///< One of the parameters is a NULL pointer.
    CHIRP_SDK_CHANNEL_NOT_SUPPORTED, ///< The channel asked is bigger than the maximum one authorised by the config being used.
    CHIRP_SDK_INVALID_FREQUENCY_CORRECTION, ///< Invalid frequency correction value.
    CHIRP_SDK_PROCESSING_ERROR, ///< An internal issue happened when processing.

    CHIRP_SDK_INVALID_KEY = 40, ///< Invalid application key.
    CHIRP_SDK_INVALID_SECRET, ///< Invalid application secret.
    CHIRP_SDK_INVALID_CREDENTIALS, ///< Invalid application credentials.
    CHIRP_SDK_MISSING_SIGNATURE, ///< Signature is missing from the config.
    CHIRP_SDK_INVALID_SIGNATURE, ///< Signature couldn't be verified.
    CHIRP_SDK_MISSING_CONFIG, ///< Config information is missing.
    CHIRP_SDK_INVALID_CONFIG, ///< Config information is invalid.
    CHIRP_SDK_EXPIRED_CONFIG, ///< This config has expired.
    CHIRP_SDK_INVALID_VERSION, ///< This config was generated for a different version. Please visit https://developers.chirp.io to upgrade your SDK.
    CHIRP_SDK_INVALID_PROJECT, ///< This config was generated for a different project.
    /*--------------------------------------------------------------------------
     * CHIRP_SDK_INVALID_CONFIG_CHARACTER needs to be kept at the end of
     * the credentials error code list as it deals with base64 but it is
     * implemented in chirp-sdk.
     *------------------------------------------------------------------------*/
    CHIRP_SDK_INVALID_CONFIG_CHARACTER, ///< Your config contains one or many unknown character(s).

    CHIRP_SDK_PAYLOAD_EMPTY_MESSAGE = 80, ///< The payload is empty.
    CHIRP_SDK_PAYLOAD_INVALID_MESSAGE, ///< The payload is invalid.
    CHIRP_SDK_PAYLOAD_UNKNOWN_SYMBOLS, ///< The payload contains unknown symbols.
    CHIRP_SDK_PAYLOAD_DECODE_FAILED, ///< Couldn't decode the payload.
    CHIRP_SDK_PAYLOAD_TOO_LONG, ///< The payload's length is longer than the maximum one authorised by the config being used.
    CHIRP_SDK_PAYLOAD_TOO_SHORT, ///< The payload's length is shorter than the minimum one authorised by the config being used..

    CHIRP_SDK_INVALID_VOLUME = 99, ///< Volume value is incorrect.
    CHIRP_SDK_UNKNOWN_ERROR = 100, ///< The SDK has reported an unknown error.

    /*--------------------------------------------------------------------------
     * Reserved for the high level SDKs. Don't update it but rather add an issue
     * in the C-SDK.
     *------------------------------------------------------------------------*/
    CHIRP_SDK_NETWORK_ERROR = 200, ///< "Network error."
    CHIRP_SDK_NETWORK_NO_NETWORK, ///< "Couldn't reach the server, please check your network connection."
    CHIRP_SDK_NETWORK_PERMISSIONS_NOT_GRANTED, ///< "Network permissions were not granted by the application or user. Please add network permissions to your application, or contact sales@chirp.io to request completely offline operation."
    CHIRP_SDK_ACCOUNT_DISABLED, ///< "Your account has been disabled due to an unpaid license. Please contact sales@chirp.io."
    CHIRP_SDK_AUDIO_IO_ERROR, ///< "Audio IO error."
    CHIRP_SDK_SENDING_NOT_ENABLED, ///< "Send mode hasn't been enabled."
    CHIRP_SDK_RECEIVING_NOT_ENABLED, ///< "Receive mode hasn't been enabled."
    CHIRP_SDK_DEVICE_IS_MUTED, ///< "The device is muted. Cannot send data."
} chirp_sdk_error_code_t;

#include "chirp_sdk.h"

/**
 * Convert a `chirp_sdk_error_code_t` code to a string describing the error.
 *
 * @param err The error code which needs to be detailed.
 * @return    The string describing the error code.
 */
PUBLIC_SYM const char *chirp_sdk_error_code_to_string(chirp_sdk_error_code_t err);

#ifdef __cplusplus
}
#endif

#endif /* !CHIRP_SDK_ERRORS_H */
