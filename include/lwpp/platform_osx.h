
/*!
* @file
 * @brief Platform specific defines
 */
#ifndef LWPP_PLATFORM_OSX_H
#define LWPP_PLATFORM_OSX_H

#include <Carbon/Carbon.h>
#include <dirent.h>
#include <cstring>
#include <cmath>
#include <math.h>
#include <sstream>
#include <string>
#include <ios>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <stdexcept>
#include <sys/stat.h>

#ifndef MEMORY_BARRIER
#include <libkern/OSAtomic.h>
#define MEMORY_BARRIER OSMemoryBarrier()
#endif // MEMORY_BARRIER

// using namespace std;
#define _mkdir(x) mkdir(x, DEFFILEMODE)
#define _unlink(x) unlink(x)
#define _alloca(x) __alloca(x)
#define _stat stat

#ifdef _DEBUG
	#include <iostream>
	#include <fstream>	
	using std::cerr;
#endif

#define UNUSED(x) (x)

//used by OSX assert macros...
#ifdef check
#undef check
#endif // check

#endif // LWPP_PLATFORM_H