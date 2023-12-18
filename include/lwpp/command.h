#ifndef LWPP_COMMAND_H
#define LWPP_COMMAND_H

#include <lwpp/global.h>
#include <string>

typedef int LWCommandFunc(const char*);

namespace lwpp
{
  //! A singleton that take a factory to create the singleton
  template <typename T, typename F>
  class Singleton
  {
  private:
    Singleton(const Singleton<T, F>&) {};
    Singleton& operator=(const Singleton<T, F>&) {};
  protected:
    Singleton() { ; }
  public:
    static T& getInstance()
    {
      static T* mInstance = (T*)F::Create();
      return *mInstance;
    }
  };

  struct LWCommandFactory
  {
    static void* Create()
    {
      return lwpp::SuperGlobal(LWPP_COMMANDFUNC_GLOBAL, GFUSE_TRANSIENT);
    }
  };

  typedef Singleton<LWCommandFunc, LWCommandFactory> LWCommand;

  //! Send a Command to layout during any context
  /*!
  * @warning This is based on an undocumented global and may fail
  */
  inline int SendCommand(const char* cmdname, const char* fmt, ...)
  {
    static char cmd[512], arg[1024];
    if (LWCommand::getInstance())
    {
      if (fmt)
      {
        auto loc = setlocale(LC_ALL, "C");
        //setlocale(LC_ALL, "C");
        va_list ap;
        va_start(ap, fmt);
        vsprintf(arg, fmt, ap);
        va_end(ap);
        sprintf(cmd, "%s %s", cmdname, arg);
        setlocale(LC_ALL, loc);
        return (LWCommand::getInstance())(cmd);
      }
      else
      {
        return (LWCommand::getInstance())(cmdname);
      }
    }
    return AFUNC_BADGLOBAL;
  }
  inline int SendCommand(const char* cmdname, ...)
  {

    static char cmd[1024], arg[1024];
    if (LWCommand::getInstance())
    {
      if (cmdname)
      {
        auto loc = setlocale(LC_ALL, "C");
        //setlocale(LC_ALL, "C");
        va_list ap;
        va_start(ap, cmdname);
        vsprintf(cmd, cmdname, ap);
        va_end(ap);
        setlocale(LC_ALL, loc);
        return (LWCommand::getInstance())(cmd);
      }
      else
      {
        return (LWCommand::getInstance())(cmdname);
      }
    }
    return AFUNC_BADGLOBAL;
  }

  inline void SendRedraw() { SendCommand("Redraw"); }
  inline void SendRedrawNow() { SendCommand("RedrawNow"); }

  inline std::string CommandString(const char* cmdname, ...)
  {
    static char cmd[1024], arg[1024];

    if (cmdname)
    {
      va_list ap;
      va_start(ap, cmdname);
      vsprintf(cmd, cmdname, ap);
      va_end(ap);
      return std::string(cmd);
    }
    return std::string();
  }

  inline int SendCommand(const std::string cmd)
  {
    if (LWCommand::getInstance())
    {
      LWCommand::getInstance()(cmd.c_str());
    }
    return AFUNC_BADGLOBAL;
  }
}

#endif // LWPP_COMMAND_H