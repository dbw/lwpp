#include <lwpp/file_request.h>
#include <fstream>
#include <lwpp/message.h>

#if !defined(S_ISDIR)
//#define S_ISDIR(m) (((m) & 0170000) == 0040000)
#define S_ISDIR(m) ((m) & _S_IFDIR)
#endif
#if !defined(S_ISREG)
#define S_ISREG(m) (((m) & 0170000) == 0100000)
#endif
#if !defined(S_IXUSR)
#define S_IXUSR 0000100/* execute/search permission, owner */
#endif

namespace lwpp
{

  typedef pathList::iterator pathIterator;
  typedef pathList::const_iterator pathCIterator;

	void trimTrail(std::string &path)
	{
		
		while (!path.empty()) 
        /*
         &&
         (
         path[path.size() - 1] == '\\' ||
         path[path.size() - 1] == '/' ||
         path[path.size() - 1] == ':'
         ))
         */
		{ 
			std::string::size_type i = path.find_last_of(fSeps);
			if (i != path.size() - 1) return;
			path.erase(path.size() - 1); // trailing slash
		}
	}
	
	const char *DirInfo::GetDirectory(const char* locType)
	{
		return_dir.clear();
		const char *temp = "";
		if (locType) 
		{ 
			temp = const_cast<char *>(globPtr(locType));
		}
		else
		{
			temp = const_cast<char *>(globPtr(dirType));
		}
        
		if (temp) 
		{
			return_dir += temp;
			// append file separator if needed
			if (return_dir.find_last_of (fSeps) != (return_dir.length() - 1))
			{
				return_dir += fSep;
			}
			return return_dir.c_str();
		}
		return "";
	}
    
	const char *DirInfo::stripPath(const char *fileName)
	{
		if (dirType == 0) return fileName;
		const char *baseDir = globPtr(dirType);
		size_t l = strlen(baseDir);
		if (strncmp(baseDir, fileName, strlen(baseDir))) return fileName;
		if (fileName[l] == fSep) return (fileName + l + 1); // remove trailing slash if present
		return fileName + l;
	}
    
	void FileName::buildFullName() 
	{
		//fullName.clear();
		splitFileName();
		fullName = "";
        
		if (useBase) 
		{
			DirInfo di(baseDir);
			fullName += di.GetDirectory();
			trimTrail(fullName);
      fullName += fSep;
			// fullName += fSep; di.Getdirectory always returns a proper file separator
		}
		if (!relativePath.empty())
		{
			//if(useBase) fullName += fSep;
			fullName += relativePath;
      fullName += fSep;
		}		
		fullName += fileName;
		valid = true;
	}
    
	std::string FileName::extractImageExtension(const char *source)
	{
		std::string src(source);
		std::string ext;
		std::string::size_type start = src.find_last_of("(");
		std::string::size_type end = src.find_last_of(")");
		if ((start != std::string::npos) && (end != std::string::npos))
		{
			// remove extension including the previous separator
			ext = src.substr(start + 1, end-start-1);
		}
		return ext;
	}
    
	std::string FileName::extractExtension(const char *source)
	{
		std::string src(source);
		//std::string::iterator iter = src.find_last_of(".");
		size_t pos = src.find_last_of(".");
		if (pos != std::string::npos)
		{
			return src.substr(pos);
		}
		return std::string("");
	}
    
	/*!
	 * Replace the file name extension
	 * @param *ext new extension, including the "." - may be an empty string to remove the extension.
	 */
	void FileName::setExtension(const char *ext)
	{
		// find last file separator
		std::string::size_type i = fileName.find_last_of(".");
		if (i != std::string::npos)
		{
			++i; // the first character behind fSep
			// remove extension including the previous separator
			fileName.erase(i - 1, fileName.size() + 1);
		}
		fileName.append(ext);
        
		i = fullName.find_last_of(".");
		if (i != std::string::npos)
		{
			++i; // the first character behind fSep
			// remove extension including the previous separator
			fullName.erase(i - 1, fullName.size() + 1);
		}
		fullName.append(ext);
		valid = false;
	}
    
	void FileName::stripNumberTrail()
	{
		std::string::size_type i = fileName.find_last_not_of("0123456789");
		if (i != std::string::npos)
		{
			// remove numbers
			fileName.erase(i + 1);
		}
	}
    
	std::string FileName::getFileName()
	{
		splitFileName();
#ifdef _DEBUG		
		lwpp::dostream dout;
		dout << "FileName::getFileName() - " << fileName << "\n";
#endif       
		return fileName;
	}
	std::string FileName::getPath()
	{
		splitFileName();
		return relativePath;
	}
	std::string FileName::getFullPath()
	{
		splitFileName();
		std::string fullPath = "";
        
		if (useBase) 
		{
			DirInfo di(baseDir);
			fullPath += di.GetDirectory();
			trimTrail(fullPath);
		}
		if (!relativePath.empty())
		{
			if (useBase) fullPath += fSep;
			fullPath += relativePath;
		}
		trimTrail(fullPath);
#ifdef _DEBUG		
		lwpp::dostream dout;
		dout << "FileName::getPath() - " << fullPath << "\n";
#endif         
		return fullPath;
	}
    
	void FileName::splitFileName()
	{
#ifdef _DEBUG		
		//lwpp::dostream dout;
		//dout << "FileName::splitFileName()" << "\n";
#endif
		DirInfo di(baseDir);
		std::string file(fullName);
		std::string::size_type i;
		// clear all paths
		fileName = "";
		relativePath = "";
        
		// find last file separator
		i = file.find_last_of(fSeps);
		// extract the file name
		if (i != std::string::npos)
		{
			//relativePath = file.substr(0, i - 1);
			++i; // the first character behind fSep
			// get the filename
			fileName = file.substr(i, file.size() - i);
			// remove filename including the previous separator
			file.erase(i - 1, fileName.size() + 1);
		}
		else // there is no path
		{
			fileName = file;
			file.erase();
		}
        
		// first check if the content path is contained at the start of the full name
#ifdef _DEBUG		
		//std::hex(dout);
		//dout << "file: " << file << "\n";
#endif
		
		i = file.find(di.GetDirectory());
		if ((i != std::string::npos) && (i == 0))		
		{
			// path found and it is at the beginning of the string
			useBase = true;
			file = file.erase(0, strlen(di.GetDirectory()));
#ifdef _DEBUG		
			//dout << "base dir: " << di.GetDirectory() << "\n";			
			//dout << "base dir found: " << file << " at:" << i << "\n";
#endif			
		}
		else
		{
			useBase = false;
		}
		//LWMessage::Info ("After content strip", file);
		// whatever is left now should be the relative path
		relativePath = file;
		// remove leading and trailing slashes
		if (useBase)
		{
			while (relativePath.find_first_of(fSep) == 0)
			{
				relativePath.erase(0, 1); // leading slash
			}
		}
		trimTrail(relativePath); // trailing file separator
        
#ifdef _DEBUG
		//dout << "FileNameSplit: " << fullName << "\n";
		//dout << "basePath: " << di.GetDirectory() << (useBase ? " (used)\n" : " (not used)\n");
		//dout << "relativePath: " << relativePath << "\n";
    //dout << "fileName: " << fileName << "\n";
#endif 
	}
    
	std::string FileName::getRelativeName()
	{
#ifdef _DEBUG
		lwpp::dostream dout;        
		dout << "getRelativeName (before): " << relName << "\n";
    dout << "getRelativeName (Path): " << relativePath << "\n";
		dout << "getRelativeName (filename): " << fileName << "\n";		
#endif
		relName.clear();
		if (!relativePath.empty())
		{
			relName += relativePath;
			relName += fSep;
		}
		relName += fileName;
        
#ifdef _DEBUG
		dout << "getRelativeName: " << relName << "\n";
#endif 
		return relName;
	}
    
	const char *FileName::getRelativeNameC()
	{
		getRelativeName();
		return relName.c_str();
	}
	  
	/*!
	 * This function will check if the file actually exists and build the full name accordingly.
	 * @param *relName a relative or absolute file name
	 * @return true if the file could be found
	 */
	bool FileName::setRelativeName(const char *rel)
	{
		// assume relName is absolute, attempt to open a file
		if (Exists(rel))
		{
			setFullName(rel);
			return true;
		}
        
		// build a file name assuming relName is relative
		DirInfo di(baseDir);
		std::string full;
		full = di.GetDirectory();
		full += fSep;
        full += rel;
		// attempt to open the file
		if (Exists(full))
		{
			setFullName(full.c_str());
			return true;
		}
		setFullName(rel);
		// all attempts failed
		return false;
	}
    
	//! Read Instance data
	LWError FileName::Load(const LoadState &ls )
	{
		LWError err = 0;
		char relName[1024];
		LWLOAD_STR(ls.getState(), relName, 1024);
#ifdef _DEBUG
		lwpp::dostream dout;
		dout << "FileName::Load: " << relName << "\n";
#endif 
		if (relName[0] != '\0') // we have a valid filename
		{
			if (!setRelativeName(relName))
			{
#ifdef _DEBUG
				dout << "FileName::Load: File not found" << getRelativeName() << "\n";
#endif 				
			}
		}
		return err;
	}
    
	//! Write Instance data
	LWError FileName::Save(const SaveState &ss )
	{
		LWError err = 0;
		LWSAVE_STR(ss.getState(), getRelativeName().c_str());
#ifdef _DEBUG
		lwpp::dostream dout;
		dout << "FileName::Save: " << getRelativeName() << "\n";
#endif 
		return err;
	}
    
	bool FileName::Exists(const std::string &fileName)
	{
		/*
         #ifdef _WIN32
         struct _stat stat_buf;
         return (_stat(name, &stat_buf) == 0);
         #else
         struct stat stat_buf;
         return (stat(name, &stat_buf) == 0);
         #endif
         */
		std::fstream fin;
		fin.open(fileName.c_str(),std::ios::in);
		bool rc = fin.is_open();
		fin.close();
		return rc;
	}
    
	/*
	 * Directory
	 */
    
	Directory::~Directory()
	{
		if (directory) closedir(directory);
		directory = 0;
	}
	Directory::Directory(const std::string name)
    : directory (0),
      entry(0)
	{
		directory = opendir(name.c_str());
#ifdef _DEBUG
		lwpp::dostream dout;
		dout << "Open directory: " << name << std::endl;
#endif 						
	}
	bool Directory::exists(void)
	{
		return (directory != 0);
	}
    
	bool isDirectory(const std::string name)
	{
		Stat st(name);
		return st.isDirectory();
	}
    
	const char *Directory::getEntry()
	{
		if (exists())
		{
			while (entry = readdir (directory))
			{
				if (strcmp(entry->d_name, "..") && strcmp(entry->d_name, "." ) )
				{
					return entry->d_name;
				}
			}
		}
		return 0;
	}
    
#if defined(LWPP_PLATFORM_OSX_CFM)
	void makeAllDirectories(const char *path)	
	{
		char DirName[1024];
		char* p = const_cast<char *>(path);
		char* q = DirName;
		bool root = true; 
		while(*p)
		{
			if (':' == *p)
			{
				if (root) 
				{
                    root = false; // skip root
				}
				else
				{
					_mkdir(DirName);
				}
			}
			*q++ = *p++;
			*q = '\0';
		}
        
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
	
    
    //! OS specific file requester
    
	OSFileRequest::OSFileRequest (const int reqType, char *title,
                                  const char *fileType, const char *ext, const char *baseName,
                                  const char *path,	const char *_baseDirType, int bufLen)
    :	type(reqType)
	{
		status = NavGetDefaultDialogCreationOptions(&dialogOptions);
		dialogOptions.modality = kWindowModalityAppModal;
		dialogOptions.windowTitle = CFStringCreateWithCString(NULL, title, kCFStringEncodingMacRoman);
		dialogOptions.saveFileName = CFStringCreateWithCString(NULL, baseName, kCFStringEncodingMacRoman);
		if (type == FREQ_LOAD)
		{
			status = NavCreateGetFileDialog(&dialogOptions, NULL, NULL, NULL, NULL, NULL, &dialog);
		}
		else if (type == FREQ_SAVE)
		{
			status = NavCreatePutFileDialog(&dialogOptions, kUnknownType, kUnknownType, NULL, NULL, &dialog);
		}			
		
	}
	OSFileRequest::~OSFileRequest()
	{
		NavDialogDispose(dialog);
	}
    
	bool OSFileRequest::Post(void)
	{
		status = NavDialogRun(dialog);
		NavUserAction action = NavDialogGetUserAction(dialog);
		if (( action != kNavUserActionOpen ) && (action != kNavUserActionSaveAs)) return false;
		status = NavDialogGetReply(dialog, &replyRecord);
		
		return true;
	}
    
	const char *OSFileRequest::getFullName(void)
	{
		temp_path.clear();
		FSRef fileAsFSRef;
		status = AEGetNthPtr(&(replyRecord.selection), 1, typeFSRef,  NULL, NULL, &fileAsFSRef, sizeof(FSRef), NULL);
        
		CFURLRef url = CFURLCreateFromFSRef(NULL, &fileAsFSRef);
		if (url == NULL) return 0;
		CFStringRef  hfs_path = NULL; 
		if (type == FREQ_LOAD)
		{
			hfs_path = CFURLCopyFileSystemPath(url, kCFURLHFSPathStyle);
		}
		else if (type == FREQ_SAVE)
		{
			CFURLRef file_url = CFURLCreateCopyAppendingPathComponent(NULL, url, replyRecord.saveFileName, false);
			hfs_path = CFURLCopyFileSystemPath(file_url, kCFURLHFSPathStyle);
			CFRelease(file_url);
		}
		char c_path[1024];
		CFStringGetCString(hfs_path, c_path, 1024-1, kCFStringEncodingUTF8);
		temp_path = c_path;			
		CFRelease(url);
		CFRelease(hfs_path);
		return temp_path.c_str();	
	}
    
#endif
    
	std::string stripExtension(const char *source)
	{
		std::string src(source);
		return src.substr(0, src.find_last_of("."));
	}
    
	std::string trim(const std::string& s, const char *ws)
	{
		if(s.length() == 0)
			return s;
		size_t b = s.find_first_not_of(ws);
		size_t e = s.find_last_not_of(ws);
		if(b == -1) // No non-spaces
			return "";
		return std::string(s, b, e - b + 1);
	}

  /*
   * Possible file paths:
   *
   * D:\classic\windows\path\and\file.ext
   * result:
   * - D:
   * - classic
   * - windows
   * - path
   * - and

   * \\UNC\based\path\and\file.ext
   * result:
   * - \
   * - UNC
   * - based
   * - path
   * - and

   * /Volume/On/OSX/and/file.ext
   * result:
   * - ""
   * - Volume
   * - On
   * - OSX
   * - and
   */
  pathList splitPath(std::string sourcePath)
  {
#ifdef _DEBUG		
		lwpp::dostream dout;
    dout << "splitPath (" << sourcePath << ");\n";
#endif 
    std::deque<std::string> elements;
    size_t pos = sourcePath.find_last_of(fSeps);
    while (pos != std::string::npos)
    {
      if (pos = sourcePath.size()-1)
      {
        pos = sourcePath.find_last_of(fSeps);
      }
      if (pos == 0)
      {

#ifdef LWPP_PLATFORM_OSX_UB
#ifdef _DEBUG		
        dout << "\"" << sourcePath.substr(1) << "\" ->" << sourcePath << "\n";
        dout << "\" \"\n";		
#endif 
        elements.push_front(sourcePath.substr(1));        
        elements.push_front("");
#else
#ifdef _DEBUG		
        dout << "\"" << sourcePath.substr(0,1) << "\" ->" << sourcePath << "\n";		
#endif 
        elements.push_front(sourcePath.substr(0,1));
#endif
        sourcePath.erase(0);
        pos = std::string::npos;
      }
      else if (pos != std::string::npos)
      {
#ifdef _DEBUG		
        dout << "\"" << sourcePath.substr(pos+1) << "\" ->" << sourcePath << "\n";
#endif 
        elements.push_front(sourcePath.substr(pos+1));
        sourcePath.erase(pos);
        pos = sourcePath.find_last_of(fSeps);
      }
    }
    if (!sourcePath.empty()) elements.push_front(sourcePath);
    return elements;
  }

  pathList splitFullPath(std::string sourcePath, std::string &fileName, std::string &ext)
  {
    size_t pos = sourcePath.find_last_of(fSeps);
    if (pos != std::string::npos) 
    {
      fileName = sourcePath.substr(pos+1);
    }
    else
    {
      fileName = sourcePath;
      sourcePath.clear();
    }
    // Strip file name and extension
    size_t ePos = fileName.find_last_of(".");
    if (ePos != std::string::npos) 
    {
      ext = fileName.substr(ePos+1);
      fileName.erase(ePos);
    }
    if (pos != std::string::npos) 
    {
      sourcePath.erase(pos);
    }
    return splitPath(sourcePath);
  }

  std::string makeFullPath(const pathList &path)
  {
#ifdef _DEBUG		
		lwpp::dostream dout;
    dout << "makeFullPath ();\n";
#endif     
    std::string fullName;
    for (pathCIterator iter = path.begin(); iter != path.end(); ++iter)
    {
      fullName += *iter;
      fullName += fSep;
#ifdef _DEBUG		
      dout << ": \"" << *iter << "\" " << fSep << "\n";
#endif        
    }
#ifdef _DEBUG		
    dout << "= \"" << fullName << "\"\n";
#endif  
    return fullName;
  }

  std::string makeFullFileName(const pathList &path, const std::string &file, const std::string &ext)
  {
    return makeFullPath(path) + file + "." + ext;
  }

  std::string makeRelativeName(pathList basePath, const std::string absFileName)
  {
    std::string name, ext;
    pathList abs = splitFullPath(absFileName, name, ext);

    // remove common elements
    while ((abs.begin() != abs.end()) &&
           (basePath.begin() != basePath.end()) &&
           (*basePath.begin() == *abs.begin()))
    {
      basePath.pop_front();
      abs.pop_front();
    }
    if (basePath.empty())
    {
      return makeFullFileName(abs, name, ext);
    }
    else
    {
      return absFileName;
    }
  }

  std::string makeRelativeName(const std::string basePath, const std::string absFileName)
  {
    pathList base = splitPath(basePath);
    return makeRelativeName(base, absFileName);
  }

  std::string makeAbsoluteName(pathList basePath, const std::string relFileName)
  {    
    std::string name, ext;
    pathList rel = splitFullPath(relFileName, name, ext);
    // append the relative path to the absolute path
    basePath.insert(basePath.end(), rel.begin(), rel.end());
    std::string absName = makeFullFileName(basePath, name, ext);
    if (FileName::Exists(absName)) return absName;
    return relFileName;
  }

  std::string makeAbsoluteName(const std::string basePath, const std::string relFileName)
  {
    pathList base = splitPath(basePath);
    return makeAbsoluteName(base, relFileName);
  }
	
	struct Stat::statData
	{
		struct _stat buf;
		int result;
	};

	Stat::Stat(const std::string fName)
		: d (new statData)
	{
		d->result = _stat(fName.c_str(), &d->buf);
	}

	Stat::~Stat()
	{
		delete d;
	}

	bool Stat::isDirectory() const
	{
		if( d->result == 0 )
		{
			return (S_ISDIR(d->buf.st_mode) != 0);
		}
		return false;
	}
	bool Stat::exists() const
	{
		return ( d->result == 0 );
	}

	time_t Stat::getCreateTimeStamp() const
	{
		return d->buf.st_ctime;
	}
	time_t Stat::getModifyTimeStamp() const
	{
		return d->buf.st_mtime;
	}

	size_t Stat::getFileSize() const
	{
		return d->buf.st_size;
	}

} // namespace lwpp
