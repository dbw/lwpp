/*!
 * @brief contain Carbon specific code for the OSX XCode version 
 *  platform_cfm.cpp
 *  lwpp
 *
 *  Created by Michael Wolf on 7/5/09.
 *  Copyright 2009 db&w Bornemann und Wolf GbR. All rights reserved.
 *
 */

#include "lwpp/file_request.h"

namespace lwpp
{

  const char *fSeps=":/\\"; 

		//! Open a URL in the default application, usually a browser
	void OpenURL(const std::string urlString)
	{
		// Thanks to Willard Myers, on the qt-interest mailing list.
		ICInstance icInstance;
		if (ICStart ( &icInstance, '????') == noErr)  
		{
			ConstStr255Param hint (0x0);
			long l = urlString.length();
			long s = 0;
			ICLaunchURL (icInstance, hint, urlString.c_str(), l, &s, &l);
			ICStop (icInstance);
		}
	}
	
	bool isDoubleClick(unsigned int milliseconds)
	{
		return (milliseconds <= GetDblTime());
	}

} // end namespace lwpp




