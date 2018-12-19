/*!
 * @brief contain Cocoa specific code for the OSX XCode version 
 *  platform_cocoa.cpp
 *  lwpp
 *
 *  Created by Michael Wolf on 7/5/09.
 *  Copyright 2009 db&w Bornemann und Wolf GbR. All rights reserved.
 *
 */

#include "lwpp/file_request.h"
#include "lwpp/platform_cocoa.h"

namespace lwpp
{

	const char *fSeps = "\\/";
	
	void makeAllDirectories(const char *path)
	{

		mode_t oldMode = umask( 0 );

		char DirName[1024];
		char* p = const_cast<char *>(path);
		char* q = DirName;
		bool root = true;

		while(*p)
		{
			if ('/' == *p)
			{
				if (root) 
				{
					root = false; // skip root
				}
				else
				{
					mkdir(DirName, ALLPERMS);
					//LWMessage::Info("Created directory",DirName);
	#ifdef _DEBUG
					lwpp::dostream dout;
					dout << "Creating: " << DirName << "\n";
	#endif // _DEBUG
				}
			}

			*q++ = *p++;
			*q = '\0';
		}
		//_mkdir(DirName);
		umask (oldMode);
	}

	std::string getTempPath()
	{
		return cocGetTempPath();
	}

	struct OSFileRequest::Data : public cocFileRequest
	{
		Data(const int reqType, const char *title,
					const char *fileType, const char *ext, const char *baseName,
					const char *path,	const char *_baseDirType, int bufLen)
		: cocFileRequest(reqType, title, fileType, ext, baseName, path, _baseDirType,bufLen)
		{
			;
		}
        ~Data() {}
		
	};

	OSFileRequest::OSFileRequest (const int reqType, const char *title,
																const char *fileType, const char *ext, const char *baseName,
																const char *path,	const char *_baseDirType, int bufLen)
	:	type(reqType),
		m_data(new Data(reqType, title, fileType, ext, baseName, path, _baseDirType,bufLen))
	{
	}

	OSFileRequest::~OSFileRequest()
	{
		delete m_data;
	}

	bool OSFileRequest::Post()
	{
		return m_data->Post();		
	}

	const char *OSFileRequest::getFullName()
	{
		return m_data->getFullName();	
	}
		
		//! Open a URL in the default application, usually a browser
	void OpenURL(const std::string urlString)
	{
		cocOpenURL(urlString);
	}
	
	bool isDoubleClick(unsigned int milliseconds)
	{
		//return (milliseconds <= (GetDblTime() * 17)); // return ticks
		return milliseconds < 800; // 0.8 seconds as a constant
	}
    
    bool getMousePosition(int &x, int &y)
    {
        cocMouseLocation(x,y);
        return true;
    }
    
} // end namespace lwpp




