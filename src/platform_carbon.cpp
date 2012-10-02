/*!
 * @brief contain Carbon specific code for the OSX XCode version 
 *  platform_carbon.cpp
 *  lwpp
 *
 *  Created by Michael Wolf on 7/5/09.
 *  Copyright 2009 db&w Bornemann und Wolf GbR. All rights reserved.
 *
 */

#include "lwpp/file_request.h"

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
		std::string result;
		FSRef fs;
		OSErr err = FSFindFolder(kUserDomain, kTemporaryFolderType, true, &fs);
		if (err != noErr)
		{
			//qWarning() << "error doing FSFindFolder:" << err;
			return result;
		}
		
		CFURLRef dirURL = CFURLCreateFromFSRef(kCFAllocatorSystemDefault, &fs);
		char fsRepr[1024];
		if (!CFURLGetFileSystemRepresentation(dirURL, true, (UInt8*) fsRepr, 1024)) {
			//qWarning() << "error invoking CFURLGetFileSystemRepresentation";
			return result;
		}
		result = fsRepr;
		//result.setPath(QString::fromUtf8(fsRepr));
		CFRelease(dirURL);
		return result;
	}

		struct OSFileRequest::Data
	{
		NavDialogCreationOptions dialogOptions;
		NavDialogRef dialog;
		OSStatus status;
		NavReplyRecord replyRecord;
		int type;
		std::string temp_path;
	};

	OSFileRequest::OSFileRequest (const int reqType, const char *title,
																const char *fileType, const char *ext, const char *baseName,
																const char *path,	const char *_baseDirType, int bufLen)
	:	type(reqType),
		m_data(new Data)
	{
		m_data->status = NavGetDefaultDialogCreationOptions(&m_data->dialogOptions);
		m_data->dialogOptions.modality = kWindowModalityAppModal;
		m_data->dialogOptions.windowTitle = CFStringCreateWithCString(NULL, title, kCFStringEncodingMacRoman);
		m_data->dialogOptions.saveFileName = CFStringCreateWithCString(NULL, baseName, kCFStringEncodingMacRoman);
		if (type == FREQ_LOAD)
		{
			m_data->status = NavCreateGetFileDialog(&m_data->dialogOptions, NULL, NULL, NULL, NULL, NULL, &m_data->dialog);
		}
		else if (type == FREQ_SAVE)
		{
			m_data->status = NavCreatePutFileDialog(&m_data->dialogOptions, kUnknownType, kUnknownType, NULL, NULL, &m_data->dialog);
		}
	}

	OSFileRequest::~OSFileRequest()
	{
		NavDialogDispose(m_data->dialog);
		delete m_data;
	}

	bool OSFileRequest::Post(void)
	{
		m_data->status = NavDialogRun(m_data->dialog);
		NavUserAction action = NavDialogGetUserAction(m_data->dialog);
		if (( action != kNavUserActionOpen ) && (action != kNavUserActionSaveAs)) return false;
		m_data->status = NavDialogGetReply(m_data->dialog, &m_data->replyRecord);
		
		return true;
	}

	const char *OSFileRequest::getFullName(void)
	{
		m_data->temp_path.clear();
		FSRef fileAsFSRef;
		m_data->status = AEGetNthPtr(&(m_data->replyRecord.selection), 1, typeFSRef,  NULL, NULL, &fileAsFSRef, sizeof(FSRef), NULL);
		
		CFURLRef url = CFURLCreateFromFSRef(NULL, &fileAsFSRef);
		if (url == NULL) return 0;
		CFStringRef  hfs_path = NULL; 
		if (type == FREQ_LOAD)
		{
			hfs_path = CFURLCopyFileSystemPath(url,  kCFURLPOSIXPathStyle);
		}
		else if (type == FREQ_SAVE)
		{
			CFURLRef file_url = CFURLCreateCopyAppendingPathComponent(NULL, url, m_data->replyRecord.saveFileName, false);
			hfs_path = CFURLCopyFileSystemPath(file_url,  kCFURLPOSIXPathStyle);
			CFRelease(file_url);
		}
		char c_path[1024];
		CFStringGetCString(hfs_path, c_path, 1024-1, kCFStringEncodingUTF8);
		m_data->temp_path = c_path;			
		CFRelease(url);
		CFRelease(hfs_path);
		return m_data->temp_path.c_str();	
	}
		
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




