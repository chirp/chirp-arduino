/**------------------------------------------------------------------------------
 *
 *  ASIO CONFIDENTIAL
 *
 *  @file chirp_sdk_defines.h
 *
 *  @brief Defines for the chirp SDK interfaces.
 *
 *  All contents are strictly proprietary, and not for copying, resale,
 *  or use outside of the agreed license.
 *
 *  Copyright © 2011-2018, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

#ifndef CHIRP_SDK_DEFINES_H
#define CHIRP_SDK_DEFINES_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__WIN32) || defined(_WIN32) || defined(WIN32)
#define PUBLIC_SYM __declspec(dllexport)
#else
#define PUBLIC_SYM __attribute__ ((visibility ("default")))
#endif

/**
 * Define of a possible, but quite big for embedded systems, buffer size.
 */
#define CHIRP_SDK_BUFFER_SIZE 4096

#ifdef __cplusplus
}
#endif

#endif /* !CHIRP_SDK_DEFINES_H */
