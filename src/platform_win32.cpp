/*!
 * Contains all platform specific code for windows
 */
#include "lwpp/file_request.h"
#include "lwpp/utility.h"
#include "lwpp/platform_win.h"

namespace lwpp
{
	const char *fSeps = "\\/";

	void makeAllDirectories(const char *path)
	{
#ifdef _DEBUG
		dout << "makeAllDirectories: " << path << std::endl;
#endif
		char DirName[1024];
		char* p = const_cast<char *>(path);
		char* q = DirName;
		
		while(*p)
		{
			*q = '\0';
			if (('\\' == *p) || ('/' == *p))
			{
				if (':' != *(p-1) && '\\' != *(p-1) )
				{
#ifdef _DEBUG
					dout << "Creating directory: " << DirName << std::endl;
#endif
					_mkdir(DirName);
				}
			}
			*q++ = *p++;
		}
	}

	std::string getTempPath()
	{
		DWORD result = ::GetTempPath(0, "");
		if(result == 0) 
		{
			throw std::runtime_error("Could not get system temp path");
		}

		std::vector<char> tempPath(result + 1);
		result = ::GetTempPath(static_cast<DWORD>(tempPath.size()), &tempPath[0]);
		if((result == 0) || (result >= tempPath.size()))
		{
			throw std::runtime_error("Could not get system temp path");
		}

		return std::string(tempPath.begin(), tempPath.begin() + static_cast<std::size_t>(result));
	}

	struct OSFileRequest::Data
	{
		OPENFILENAME ofn;
	};

//! OS specific file requester
	OSFileRequest::OSFileRequest (const int reqType, const char *title,	const char *fileType, const char *ext,
																const char *baseName,	const char *path,	const char *_baseDirType, int bufLen)
			:	type(reqType), m_data (new Data)
	{
		_baseDirType;
		// Initialize OPENFILENAME
		ZeroMemory(&m_data->ofn, sizeof(m_data->ofn)); // Clear to 0
		m_data->ofn.lStructSize = sizeof(m_data->ofn);
		m_data->ofn.hwndOwner = NULL;
		m_data->ofn.lpstrFile = new CHAR[bufLen];
		strcpy(m_data->ofn.lpstrFile, baseName);
		m_data->ofn.nMaxFile = bufLen;
		m_data->ofn.lpstrFilter = fileType;
		m_data->ofn.nFilterIndex = 1;
		m_data->ofn.lpstrFileTitle = 0;
		m_data->ofn.nMaxFileTitle = 0;
		m_data->ofn.lpstrInitialDir = path;
		m_data->ofn.lpstrDefExt = ext;
		m_data->ofn.lpstrTitle = title;
	}

	OSFileRequest::~OSFileRequest()
	{
		delete[] m_data->ofn.lpstrFile;
		delete m_data;
	}

	bool OSFileRequest::Post(void)
	{
		if (type == FREQ_LOAD)
		{
			m_data->ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			return (GetOpenFileName(&m_data->ofn) == TRUE);
		}
		else if (type == FREQ_SAVE)
		{
			m_data->ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
			return (GetSaveFileName(&m_data->ofn) == TRUE);
		}
		else 
			return false;
	}

	const char *OSFileRequest::getFullName(void)
	{
		return m_data->ofn.lpstrFile;
	}

	void OpenURL(const std::string urlString)
	{
		ShellExecuteA( NULL, "open", urlString.c_str(), NULL, "c:\\", SW_SHOW );
	};

	void OpenExplorer(const std::string urlString)
	{
		ShellExecuteA(NULL, "explore", urlString.c_str(), NULL, "c:\\", SW_SHOW);
	};

	bool isDoubleClick(unsigned int milliseconds)
	{
		return (milliseconds < GetDoubleClickTime());
	}

	bool getMousePosition(int &x, int &y)
	{
		POINT lpPoint;
		if (GetCursorPos(&lpPoint))
		{
			x = lpPoint.x;
			y = lpPoint.y;
			return true;
		}
		return false;
	}

}