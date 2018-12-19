/*
 *  platform_cocoa.h
 *  lwpp
 *
 *  Created by Michael Wolf on 9/30/09.
 *  Copyright 2009 db&w Bornemann und Wolf GbR. All rights reserved.
 *
 */

#ifndef LWPP_PLATFORM_COCOA
#define LWPP_PLATFORM_COCOA

#include <string>

void cocOpenURL(const std::string &s);

void cocMouseLocation(int &x, int &y);

std::string cocGetTempPath();

class cocFileRequest
{
    std::string retValue;
    const char *title;
	int reqType;
    const char *extension;
    const char *savePath;
    const char *fName;
public:
	cocFileRequest(const int _reqType, const char *_title,
								 const char *fileType, const char *ext, const char *baseName,
								 const char *path,	const char *_baseDirType, int bufLen);
	~cocFileRequest() {;}
	bool Post();
	const char *getFullName();
};

#endif
