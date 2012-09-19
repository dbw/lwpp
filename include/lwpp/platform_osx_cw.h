/*!
* @file
 * @brief Platform specific defines
 */
#ifndef LWPP_PLATFORM_OSX_CW_H
#define LWPP_PLATFORM_OSX_CW_H

#include <carbonrel.h>
#include <ansi_prefix.mac.h>
//#include <Carbon.h>
#include <Navigation.h>
#include <InternetConfig.h>
//#include <MacHeadersCarbon.h>
#include <stat.h>
#include <dirent.h>
#include <cstring>
#include <cmath>
#include <sstream>
#include <string>
#include <ios>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <TextUtils.h>
#include <Folders.h>
#include <alloca.h>
#define _mkdir(x) mkdir(x)
#define _unlink(x) unlink(x)
#define _stat stat
// MW places all standard C includes in std:: which is correct
#define _alloca(x) alloca(x)
using _CSTD::size_t;
using _CSTD::strlen;
using _CSTD::strncmp;
using _CSTD::strcpy;
using _CSTD::strcmp;
using _CSTD::atan2;
using _CSTD::sqrt;
using _CSTD::abs;
using _CSTD::sin;
using _CSTD::cos;
using _CSTD::sprintf;
using _CSTD::fopen;
using _CSTD::FILE;
using _CSTD::vsprintf;
using _CSTD::powf;
using _CSTD::pow;
using _CSTD::atan;
using _CSTD::fabs;
using _CSTD::floor;
using _CSTD::size_t;
using _CSTD::toupper;
using _CSTD::tolower;
using _CSTD::atoi;
using _CSTD::isdigit;

#ifdef _DEBUG
#include <iostream>
#include <fstream>	
using std::cerr;
#endif

#endif // LWPP_PLATFORM_OSX_CW_H