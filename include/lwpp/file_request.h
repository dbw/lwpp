#ifndef FILE_REQUEST_H
#define FILE_REQUEST_H

#include <lwpp/utility.h>
#include <lwpp/storeable.h>
#include <lwpp/message.h>
#include <deque>

#ifndef LWFTYPE_INSTALL
#define LWFTYPE_INSTALL					"Install" // installation directory, not defined by LW
#endif

namespace lwpp
{
#ifdef LWPP_PLATFORM_WIN
	const char fSep = '\\'; //! File separator
#elif defined(LWPP_PLATFORM_OSX_CFM)
	const char fSep = ':';
	//extern const char *fSeps = ":";
#elif defined(LWPP_PLATFORM_OSX_UB)
	const char fSep = '/';
#else
#error no valid Platform
#endif
  extern const char *fSeps;
//! Wrapper for LWDirInfoFunc
//! @ingroup Entities
//! @ingroup Globals
class DirInfo	: protected GlobalBase<LWDirInfoFunc>
	{
	private:
		const char *dirType;
		std::string return_dir;
	public:
		/*!
		 * @param *dirtype As defined by LW as a LWFTYPE_*
		 */
		DirInfo (const char *dirtype = 0) :  dirType(dirtype)
		{
			if ((dirtype == 0) || (*dirtype == 0)) dirType = LWFTYPE_CONTENT;
		}
		//! Return the path to the define type of directory
		/*!
		 * @param *locType Directory type string or 0 for the default type
		 * @return Path of the directory
		 */
		const char *GetDirectory(const char* locType = 0);

		const std::string GetDirectoryStr(const char* locType = 0)
		{
			return std::string(GetDirectory(locType));
		}
		//! Remove a DirInfo directory from a full filename/path if possible
		/*!
		 * @param *fileName Fully qualified file name that may include the current DirInfo path
		 * @return stripped/relative file name if the original contained the DirInfo directory.
		 */
		const char *stripPath(const char *fileName);
	};

	// Directory scanning
	class Directory
	{
		DIR *directory;
		struct dirent *entry;
		public:
			Directory(const std::string name);
			~Directory();
			bool exists();
			const char *getEntry(void);
	};

	bool isDirectory(const std::string name);

	void makeAllDirectories(const char *path);

	class FileType	: protected GlobalBase<LWFileTypeFunc>
	{
	public:
		FileType (void){;}
		const char *GetType(const char *filetype) const
		{
			return globPtr(filetype);
		}
	};

	//! returns the current temp path, including a trailing separator
	std::string getTempPath();

	//! virtual base class for all file requesters
	class BaseFileRequest
	{
	public:
		virtual bool Post(void) = 0;
		virtual const char *getFullName(void) = 0;
	};

	//! OS specific file requester
	class OSFileRequest : public BaseFileRequest
	{
		struct Data;
		Data *m_data;
	public:
		OSFileRequest (const int reqType = FREQ_LOAD, const char *title = "",
									 const char *fileType = "", const char *ext = "", const char *baseName = "",
									const char *path = "",	const char *_baseDirType = 0, int bufLen = 1024);

		virtual ~OSFileRequest();

		virtual bool Post(void);

		virtual const char *getFullName(void);
	private:
		int type;
#ifdef LWPP_PLATFORM_WIN
#elif defined(LWPP_PLATFORM_OSX_CFM)
		NavDialogCreationOptions dialogOptions;
		NavDialogRef dialog;
		OSStatus status;
		NavReplyRecord replyRecord;
		std::string temp_path;
#elif defined(LWPP_PLATFORM_OSX_UB)
#else
 #error No OS specifice file requester!
#endif
	};

	//! A general file requester
	//! @ingroup Globals
	/*!
	 * The file path return is content directory aware and will (optionally) return a relative path.
	 * @note FREQ_MULTILOAD is not tested and probably needs a slightly different concept.
	 */
	class FileRequest2 : public BaseFileRequest, protected GlobalBase<LWFileActivateFunc>, protected DirInfo
	{
		private:
			static LWFileReqLocal frloc;
		public:
			/*!
			 * @param reqType Type of file requester, FREQ_LOAD, FREQ_SAVE, FREQ_DIRECTORY or FREQ_MULTILOAD
			 * @param *title Title for the file requester
			 * @param *fileType A Filter string
			 * @param *baseName The initial file name
			 * @param *path The initial path for the file requester
			 * @param *_baseDirType Base directory as defined as LWFTYPE_* in lwhost.h or 0
			 * @param *pickName PickName callback for a FREQ_MULTILOAD style requester
			 * @param bufLen Length for the string buffers
			 */
			FileRequest2 (const int reqType = FREQ_LOAD, const char *title = "",
										const char *fileType = "", const char *baseName = "",
										const char *path = "",	const char *_baseDirType = 0,
										int (*pickName) (void) = 0, int bufLen = 1024)
			: DirInfo(_baseDirType)
			{
				frloc.reqType = reqType;
				frloc.bufLen = bufLen;
				frloc.result = 0;
				frloc.title = title;
				frloc.fileType = fileType;
				frloc.path = new char[bufLen];
				frloc.baseName = new char[bufLen];
				frloc.fullName = new char[bufLen];
				if (_baseDirType)
				{
					strcpy(frloc.path, DirInfo::GetDirectory());
				}
				else strcpy (frloc.path, path);
				strcpy (frloc.baseName, baseName);
				frloc.pickName = pickName;
			}
			~FileRequest2 (void)
			{
				delete[] frloc.path;
				delete[] frloc.baseName;
				delete[] frloc.fullName;
			}
			//! Post file request and return the result
			/*!
			 * @return True if the file selection is valid
			 */
			bool Post()
			{
				return ((GlobalBase<LWFileActivateFunc>::globPtr( LWFILEREQ_VERSION, &frloc ) == AFUNC_OK) && (frloc.result == 1));
			}

			static std::vector<std::string> ret;
			static int pick() 
			{
				ret.push_back(frloc.fullName);
				return 0;
			}

			std::vector<std::string> PostMulti()
			{				
				ret.clear();
				frloc.reqType = FREQ_MULTILOAD;
				frloc.pickName = pick;

				if ((GlobalBase<LWFileActivateFunc>::globPtr(LWFILEREQ_VERSION, &frloc) != AFUNC_OK) || (frloc.result != 1))
					ret.clear();
				return ret;
			}

			//! Return the file name of the file selected in the requester
			const char *getFullName()
			{
				return DirInfo::stripPath(frloc.fullName);
			}
			const char *getFullPath() const
			{
				return frloc.fullName;
			}
			const char *getPath() const
			{
				return frloc.path;
			}
			LWFileReqLocal *getFileReqLocal()
			{
				return &frloc;
			}
			void setBaseName(const std::string name)
			{
				strcpy (frloc.baseName, name.c_str());
			}
	};

	// remove trailing file separator
	void trimTrail(std::string &path);

	//! Manages file names that are content directory aware
	/*!
	 * A filename consists of three components:
	 * - a basePath as defined by LWs content directory system
	 * - a relativePath that is appended to the basePath
	 * - a fileName that is the actual name of the file
	 *
	 * The fullPath is the basePath + relativePath.
	 *
	 * The fully qualified name of the file is fullPath + fileName.
	 *
	 * @note The basePath may be void, in which case the relative path is the fully qualified path already.
	 *
	 * @todo Rework this for other operating systems.
	 * @todo Change this into a more generic disk asset manager
	 */
	class FileName : public Storeable
	{
		private:
			const char *baseDir; //! Base directory as defined as LWFTYPE_* in lwhost.h
			std::string fileName; //! Name of the file
			std::string relativePath; //! Path to the file in relation to the base directory
			std::string fullName; //! Fully qualified name to load from
			std::string relName;
			bool valid; //! The current fullName is valid
			bool useBase; //! use the baseDir for building a qualified name
			//! Build a fully qualified file name using the components available
			void buildFullName();
			//! Split the full file name
			void splitFileName();
			//! Set the relative file name
			bool setRelativeName(const char *relName);
			void setExtension(const char *ext);
			static std::string extractExtension(const char *source);
		public:
			//!
			/*!
			 * @param *_baseDir LWFTYPE_* style content directory description
			 * @param base Set to true if the base name should be taken into account
			 */
			FileName (const char *_baseDir = LWFTYPE_CONTENT, bool base = false) : baseDir(_baseDir), valid(false), useBase(base)
			{
				;
			}
			FileName (const std::string name, bool base = false) : baseDir(LWFTYPE_CONTENT), valid(false), useBase(base)
			{
				setFullName(name);
			}
			//! Get the file name relative to the base dir
			std::string getRelativeName();
			const char *getRelativeNameC();
			//std::string getRelativeName(const char *_baseDir);
			//! return the file name only
			std::string getFileName(void);
			//! return the relative Path only
			std::string getPath(void);
			//! return full Path
			std::string getFullPath(void);
			//! create the full path if needed
			//void makePath(void);

			const char *getFullName()
			{
				if (!valid) buildFullName();
#ifdef _DEBUG
        dout << "FileName::getFullName() -  " << fullName << std::endl;
#endif
				return fullName.c_str();
			}
			std::string getAbsoluteName()
			{
				if (!valid) buildFullName();
				if (!useBase)
				{
					DirInfo di(baseDir);
					return std::string(di.GetDirectory()) + fullName;
				}
				return fullName;
			}
			FileName &operator=(const char *name)
			{
				setFullName(name);
				return *this;
			}
			FileName &operator=(const std::string name)
			{
				setFullName(name);
				return *this;
			}
			//! Set to a full name. This splits the components and stores them
			void setFullName(const std::string name)
			{
				fullName = name;
				splitFileName();
			}
			//! Set the file name only
			void setFileName(const std::string name)
			{
				fileName = name;
        valid = false;
			}
			//! Set the extension of the filename
			void setFileExtension(const char *extension = "")
			{
				setExtension(extension);
			}
			std::string getFileExtension()
			{
				return std::string(extractExtension(fileName.c_str()));
			}
			// remove trailing numbers
			void stripNumberTrail(void);
			//! Extract the extension from a string that includes "(.ext)" - such as the names of image savers.
			void setExtractedExtension(const char *source)
			{
				std::string ext(extractImageExtension(source));
				setExtension(ext.c_str());
			}
			static std::string extractImageExtension(const char *source);

			//! Check if a file exists
			//static bool Exists(const char *name);
			static bool Exists(const std::string &name); //{return Exists(name.c_str());}
			bool Exists() {return Exists(getFullName());}

			LWError Load(const LoadState &ls );
			LWError Save(const SaveState &ss );
	};

	std::string stripExtension(const char *source);
	inline void stripExtension(std::string &source)
	{
		source = stripExtension(source.c_str());
	}

	std::string trim(const std::string& s, const char *ws=" \t\r");

  //! Return the file name in relation to base
  std::string makeRelativeName(const std::string basePath, const std::string absFileName);
  
  std::string makeAbsoluteName(const std::string basePath, const std::string relFileName);

  typedef std::deque<std::string> pathList;
  std::string makeRelativeName(pathList basePath, const std::string absFileName);
  pathList splitPath(std::string sourcePath);
  pathList splitFullPath(std::string sourcePath, std::string &fileName, std::string &ext);
  std::string makeFullFileName(const pathList &path, const std::string &file, const std::string &ext);
  std::string makeFullPath(const pathList &path);

  std::string makeAbsoluteName(pathList basePath, const std::string relFileName);
  std::string makeAbsoluteName(const std::string basePath, const std::string relFileName);

  //! Wrapper to extract stats from a file
	class Stat
	{
		struct statData;
		statData *d;
	public:
    //! Construct the class with the full file name
		Stat(const std::string fName);
		~Stat();
    //! Check if the file is a directory
		bool isDirectory() const;
    //! Check if the file exists
		bool exists() const;
    //! Return the creation timestamp of the file
		time_t getCreateTimeStamp() const;
    //! Return the timestamp of the last modification of the file
    time_t getModifyTimeStamp() const;
    //! Return the size of the file
		size_t getFileSize() const;
	};
} // end namespace lwpp

#endif // FILE_REQUEST_H 
