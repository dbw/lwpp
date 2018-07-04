/*!
 * @file
 * @brief Platform specific defines
 */
#ifndef LWPP_PLATFORM_H
#define LWPP_PLATFORM_H

/*
__BIG_ENDIAN__ __LITTLE_ENDIAN__
*/

// supported platforms
#define LWPP_WIN 1
#define LWPP_OSX_CW 2
#define LWPP_OSX_UB 3

#ifdef _WIN32
	#ifndef _MSWIN
		#define _MSWIN
	#endif
  #define LWPP_PLATFORM LWPP_WIN
  #define LWPP_PLATFORM_WIN
  #ifdef _WIN64
		#define LWPP_PLATFORM_NAME "Win64"
	#else
		#define LWPP_PLATFORM_NAME "Win32"
	#endif
  #include "platform_win.h"
#endif

#ifdef __APPLE__ 
	#ifndef _MACOS
		#define _MACOS
	#endif

	#ifdef __GNUC__
		#define LWPP_PLATFORM LWPP_OSX_UB
		#define LWPP_PLATFORM_OSX_UB  
		#if __ppc64__
			#define LWPP_PLATFORM_NAME "MacOS-PPC-64"
		#elif __x86_64__
			#define LWPP_PLATFORM_NAME "MacOS-x86_64"
		#elif __ppc__
			#define LWPP_PLATFORM_NAME "MacOS-PPC"
		#elif __i386__
			#define LWPP_PLATFORM_NAME "OSX-x86"
		#else
			#error No valid OSX platform 
		#endif
		#if __LP64__
			#include "platform_osx64.h"
		#else
			#include "platform_osx.h"
		#endif
	#endif
#endif
#ifdef __MWERKS__
		#ifndef _MACOS
			#define _MACOS
		#endif
		#define LWPP_PLATFORM LWPP_OSX_CFM
		#define LWPP_PLATFORM_OSX_CFM
		#define LWPP_PLATFORM_NAME "OSX-CFM"
		#include "platform_osx_cw.h"
#endif

#define UNUSED(x) (x)

#if defined(_MACOS) && defined(_MSWIN)
  #error Both Windows and MacOS platform defined!
#endif

#if !defined(LWPP_PLATFORM_OSX_CFM) && !defined(LWPP_PLATFORM_OSX_UB) && !defined(LWPP_PLATFORM_WIN)
  #error No supported platform defined!
#endif

#endif // LWPP_PLATFORM_H
