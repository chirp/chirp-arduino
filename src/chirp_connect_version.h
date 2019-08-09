/**-----------------------------------------------------------------------------
 *
 *  @file chirp_connect_version.h
 *
 *  @brief Provides getter to the lib name, version and build number of the
 *         library
 *
 *  ASIO CONFIDENTIAL
 *
 *  All contents are strictly proprietary, and not for copying, resale,
 *  or use outside of the agreed license.
 *
 *  Copyright © 2011-2019, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

#ifndef CHIRP_CONNECT_VERSION_H
#define CHIRP_CONNECT_VERSION_H

#include "chirp_connect.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get the name of SDK : "chirp-connect". This function doesn't rely at all on
 * the SDK creation and can be called at any time.
 *
 * @return The name of the library
 */
PUBLIC_SYM const char *chirp_connect_get_library_name(void);

/**
 * Get the version number of the SDK. This function doesn't rely at all on the
 * SDK creation and can be called at any time.
 *
 * @return The version number of the SDK in the MAJOR.MINOR.PATCH string
 *         representation.
 */
PUBLIC_SYM const char *chirp_connect_get_version(void);

/**
 * Get the build number of the SDK. This function doesn't rely at all on the
 * SDK creation and can be called at any time.
 *
 * @return The build number of the SDK as a string.
 */
PUBLIC_SYM const char *chirp_connect_get_build_number(void);

#ifdef __cplusplus
}
#endif

#endif /* !CHIRP_CONNECT_VERSION_H */
