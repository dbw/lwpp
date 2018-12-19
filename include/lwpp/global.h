#ifndef LWPP_GLOBAL_H
#define LWPP_GLOBAL_H
#include <lwpp/debug.h>
#include <lwrender.h>
#include <lwglobsrv.h>
#include <lwpp/lw_version.h>
#include <lwpp/lw_server.h>
#include <lwpp/exception.h>
#include <lwhost.h>

#ifndef NULL
#define NULL 0
#endif // 0

#if defined(_MSC_VER)
// C4661: '...' : no suitable definition provided for explicit template instantiation request
#pragma warning(disable: 4661)
#endif

#define TOKENIZE(x) x

#define IMPLEMENT_GLOBAL(base, name) \
template<> base * GlobalBase< base >::globPtr = 0; \
template<> size_t	GlobalBase< base >::usage_count = 0; \
template<> const char * GlobalBase< base >::m_globalName = name;

// llvm needs this (and apparently it is a C++ requirement) but VC stumbles over it
#ifdef __llvm__
#define DEFINE_GLOBAL(base) \
template<> base * GlobalBase< base >::globPtr; \
template<> size_t	GlobalBase< base >::usage_count; \
template<> const char * GlobalBase< base >::m_globalName;
#else
#define DEFINE_GLOBAL(base)
#endif

#define IMPLEMENT_NAMED_GLOBAL(base, name) \
template<> base * nGlobalBase< base, name>::globPtr = 0; \
template<> size_t	nGlobalBase< base, name>::usage_count = 0;


// Added for 2015
#ifndef LWINF_MODELYEAR
#define LWINF_MODELYEAR         0x3F000000
#define LWINF_GETMODELYEAR(x)   ((((x) & LWINF_MODELYEAR) >> 24) + 2015)
#endif

//! LightWrap++ related classes and functions
namespace lwpp
{
  // LW version information
  //! Wrap functions related to the LightWave system
  class LightWave
  {
    static unsigned int systemID;
    static unsigned int productInfo;
  public:
    void static Init();
    //! Get the contex (Layout, Modeler, or LWSN)
    unsigned int static Context() 
    {
      return (systemID & LWSYS_TYPEBITS);
    }
    //! Get the DongleID
    unsigned int static DongleID()
    {
      Init();
      #define LWSYS_NEW_SERIALBITS 0x0000FFFF // redefine, this seems to be a bug in the SDK ... or modeler
      return (systemID & LWSYS_NEW_SERIALBITS);
      //return (systemID & LWSYS_SERIALBITS);
    }
    //! Check if we're running in Layout
    bool static isLayout() 
    {
      return (Context() == LWSYS_LAYOUT);
    }
    //! Check if we're running in Modeler
    bool static isModeler() 
    {
      return (Context() == LWSYS_MODELER);
    }
    //! Check if we're running on Screamernet
    bool static isScreamernet() 
    {
      return (Context() == LWSYS_SCREAMERNET);
    }
    //! Get the major version of LightWave
    unsigned int static Major() 
    {
      return LWINF_GETMAJOR(productInfo);
    }
    //! Get the minor version of LightWave
    unsigned int static Minor() 
    {
      return LWINF_GETMINOR(productInfo);
    }
    unsigned int static ModelYear() 
    {
      return LWINF_GETMODELYEAR(productInfo);
    }
    bool static isModelYear() 
    {
      return (Major() == 15);
    }
    //! Get the build of LightWave.
    /*! @note The build number is different for different programs of the same release */
    unsigned int static Build() 
    {
      return LWINF_GETBUILD(productInfo);
    }
    //! Returns true if the current version of LightWave is at least the version designated by major and minor
    static bool isAtLeast(const unsigned int major, const unsigned int minor) 
    {
      if ( isModelYear() ) return true;
      if (major < Major()) return true;
      if (major > Major()) return false;
      if (minor <= Minor()) return true;
      return false;
    }
    //! Returns true if the current version of LightWave is at least the version designated by the year and minor
    static bool isAtLeastYear(const unsigned int year, const unsigned int minor)
    {
      if ( !isModelYear() ) return false;
      if ( year < ModelYear() ) return true;
      if ( year > ModelYear() ) return false;
      if ( minor <= Minor() ) return true;
      return false;
    }
    static bool is(const unsigned int major, const unsigned int minor) 
    {
      return (major == Major()) && (minor == Minor());
    }
    //! Returns true if the current version of LightWave is at least the version designated by major and minor and the build
    static bool isAtLeast(const unsigned int major, const unsigned int minor, const unsigned build) 
    {
      if ( isModelYear() ) return true;
      if (isAtLeast(major, minor))
      {
        return (build >= Build());
      }
      return false;
    }
    int static GetNumCores();
  };

  //! Superglobal storage for GlobalFunc
  /*!
   * This is shared by all plugins within the DLL, and should be initialized as soon as possible
   */
  extern GlobalFunc *SuperGlobal;
  //! Set Superglobal
  /*!
   * Initialize the SuperGlobal, this is just to keep it clean
   */
  void SetSuperGlobal(GlobalFunc *g);


  //! Template class base for any kind of global that will be AQUIREd/RELEASEd,
  template<class G>
  class GlobalBase
  {
  private:
    static const char *m_globalName; //!< Name of the global, should probably be static too
    static size_t	usage_count; //!< Usage count for the global
  protected:
    static G	 *globPtr; //!< Pointer to the global functions
    bool _acquireGlobal()
    {
      return (globPtr = reinterpret_cast<G *> (SuperGlobal(m_globalName, GFUSE_ACQUIRE))) != 0;
    }
  public:
    //! Constructor, acquire the global and increase the usage counter
    GlobalBase()
    {
      if (usage_count < 1)
      {
        if (!_acquireGlobal())
        {
          //throw MissingGlobal(name);
        }
				usage_count++;
      }
    }
    //! Destructor, RELEASE the global if usage counter drops to 0
    virtual ~GlobalBase()
    {
#ifdef _DEBUG
    //lwpp::dostream dout;
    //dout << "Releasing Global: " << m_name << " (Post)\n";
#endif
      usage_count--;
      if (usage_count == 0)
      {
        SuperGlobal(m_globalName, GFUSE_RELEASE); // release global
      }
    }

    //! Check if the global is available
    virtual bool available() const
    {
      return (globPtr != 0);
    }

		const char *getGlobalName() const { return m_globalName; }

    //! Get the pointer to the global
    G* operator->() {return globPtr;}

    //! Get the pointer to the global
    virtual G *getGlobal() {return globPtr;}
  };

  class genericGlobal
  {
  public:
    genericGlobal() {}
    virtual ~genericGlobal() {}
  };

  template<typename G, const char *globalName>
  class nGlobalBase : public genericGlobal
  {
  private:
    static size_t	usage_count; //!< Usage count for the global
  protected:
    static G	 *globPtr; //!< Pointer to the global functions
  public:
    //! Constructor, acquire the global and increase the usage counter
    explicit nGlobalBase()
    {
      if ((globPtr = reinterpret_cast<G *> (SuperGlobal(globalName, GFUSE_ACQUIRE))) != 0)
      {
        usage_count++;
      }
      else
      {
        //throw MissingGlobal(name);
      }
      //! @todo throw exception if the global could not be aquired
    }
    //! Destructor, RELEASE the global if usage counter drops to 0
    virtual ~nGlobalBase()
    {
#ifdef _DEBUG
    //lwpp::dostream dout;
    //dout << "Releasing Global: " << m_name << " (Post)\n";
#endif
      usage_count--;
      if (usage_count == 0)
      {
        SuperGlobal(globalName, GFUSE_RELEASE); // release global
      }
    }

    //! Check if the global is available
    virtual bool available()
    {
      return (globPtr != 0);
    }

    //! Get the pointer to the global
    G* operator->() {return globPtr;}

    //! Get the pointer to the global
    virtual G *getGlobal() {return globPtr;}
  };

  //! Template class base for any TRANSIENT global
  template<class T>
  class	TransientGlobalBase
  {
  protected:
    static T	*globPtr;
  public:
    //! Constructor
    explicit TransientGlobalBase(const char *global_name)
    {
      globPtr = static_cast<T *> (SuperGlobal(global_name, GFUSE_TRANSIENT));
    }
  };

  template <class T>
  T *getTransGlobal(const char *globalName)
  {
    return static_cast<T *> (SuperGlobal(globalName, GFUSE_TRANSIENT));
  }

  DEFINE_GLOBAL(LWGlobalPool)

  class GlobalMemory : protected GlobalBase<LWGlobalPool>
  {
  public:
    GlobalMemory() {}
    LWMemChunk first(void) {return globPtr->first();}
    LWMemChunk next(LWMemChunk chunk) {return globPtr->next(chunk);}
    const char *ID(LWMemChunk chunk) {return globPtr->ID(chunk);}
    int size(LWMemChunk chunk) {return globPtr->size(chunk);}
    LWMemChunk find(const char *ID) {return globPtr->find(ID);}
    LWMemChunk create(const char *ID, int size) {return globPtr->create(ID, size);}

    template <class T>
    T *Find (const char *ID)
    {
      return static_cast<T *>(globPtr->find(ID));
    }
    template <class T>
    T* Create(const char *ID)
    {
      return static_cast<T *>(globPtr->create(ID, sizeof(T)));
    }
    /*!
     * @return true if the memory chunk existed already
     */
    /*
    template <class T>
    bool Get(const char *ID, T*& type)
    {
      type = Find<T>(ID);
      if (type) return true;
      type = Create<T>(ID);
      return false;
    }
    */
    //! Find and return a named chukc of memory, or create a new one if it doesn't exist.
    /*!
     * @param ID ID of the memory block
     * @param type Point to a block of type T, this will contain the found memory block
     * @param defaultVal Default value for situations where there is no memory block (i.e. Modeler)
     * @return true of a block could be found or the defaults are being used
     */
    template <class T>
    bool Get(const char *ID, T*& type, T& defaultVal)
    {
      if (available())
      {
        type = Find<T>(ID);
        if (type) return true;
        type = Create<T>(ID);
        return false;
      }
      else // if the global is not available, return a default value
      {
        type = &defaultVal;
        return true;
      }
    }
  };

  class CustomGlobal
  {
    public:
      CustomGlobal(const char *name, ActivateFunc *activate, ServerTagInfo *tags)
      {
        LWServer::AddPlugin(LWGLOBALSERVICE_CLASS, name, activate, tags);
      }
  };

  DEFINE_GLOBAL(LWServerInfo)
    
  class ServerInfo : public GlobalBase<LWServerInfo>
  {
  public:
    const char *getPath(std::string server_class, std::string server_name)
    {
      if (available()) return globPtr->path(server_class.c_str(), server_name.c_str());
      return 0;
    }
    std::vector<std::string> getList (std::string server_class)
    {
      std::vector<std::string> pList;
      const char **plugList = globPtr->list(server_class.c_str());
      if (plugList)
      {
        int i = 0;
        while (plugList[i])
        {
          pList.push_back(plugList[i]);
          ++i;
        }
      }
      return pList;
    }
  };

} // end namespace lwpp

constexpr unsigned int lwID(const char id[4])
{
	return LWID_(id[0], id[1], id[2], id[3]);
}

constexpr unsigned int operator"" _lw(const char *id, size_t)
{
	return LWID_(id[0], id[1], id[2], id[3]);
}

#endif // LWPP_GLOBAL
