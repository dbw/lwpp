/*!
* @file
 * @brief Platform specific defines
 */
#ifndef LWPP_PLATFORM_WIN_H
#define LWPP_PLATFORM_WIN_H

#pragma warning (push,1)
#ifdef _DEBUG
  #define _CRTDBG_MAP_ALLOC
  #include <stdlib.h>
  #include <crtdbg.h>
  #define DEBUG_NEW new(_CLIENT_BLOCK, __FILE__, __LINE__)
/*
  #ifndef _CRTDBG_MAP_ALLOC
    #define _CRTDBG_MAP_ALLOC
  #endif
*/
#else
  #include <stdlib.h>
  #define DEBUG_CLIENTBLOCK
#endif
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#include <direct.h>
#include <cmath>
#include <cstring>
#include <sstream>
#include <string>
#include <ios>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <malloc.h> // _alloca
#pragma warning (pop)
#pragma warning(disable : 4996)
#pragma warning(disable : 4530)
#pragma warning(disable : 4291) // warning no matching delete operator found

#ifdef _DEBUG
  #define new DEBUG_NEW
#endif

/******************************************************************************
* MODULE     : dirent.h
* DESCRIPTION: Windows version of POSIX dirent.h
* COPYRIGHT  : (C) 2003 Dan Martens dan_martens@lycos.com
*******************************************************************************
* This software falls under the GNU general public license and comes WITHOUT
* ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for more details.
* If you don't have this file, write to the Free Software Foundation, Inc.,
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
******************************************************************************/
struct dirent
{
  long d_ino; /* unique file number */
  off_t d_off; /*offset of this file within the director*/
  unsigned short d_reclen; /*length of this entry*/
  char d_name[MAX_PATH]; /*name of the entry*/
};

typedef struct DIR_
{
  bool isOpen;
  bool findDataValid;
  HANDLE findHandle;
  int position;
  WIN32_FIND_DATA findData;
  char d_name[MAX_PATH];
}DIR;

int            closedir(DIR *);
DIR           *opendir(const char *);
struct dirent *readdir(DIR *);
int            readdir_r(DIR *, struct dirent *, struct dirent **);
void           rewinddir(DIR *);
void           seekdir(DIR *, long int);
long int       telldir(DIR *);

extern const char *strptime(const char *p, const char *fmt, struct tm *dt);

#endif // LWPP_PLATFORM_WIN_H