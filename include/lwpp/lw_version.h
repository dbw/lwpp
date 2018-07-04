/*!
 * @file
 * Compatibility layer for various version of LightWave. Currently static and determined during compile time.
 */

#include <lwsdkver.h>

#ifndef LW_VERSION
#define LW_VERSION

// Disable "unreferenced formal parameter" warning in MSVC
#pragma warning(disable: 4100)

#define LWPP_COMMANDFUNC_GLOBAL "LW Command Interface"

// As of LW 2018, the SDK versions will be used, compatibility across major version of LW is not guaranteed


#endif // LW_VERSION