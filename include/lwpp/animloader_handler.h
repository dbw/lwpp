#ifndef ANIMLOADER_HANDLER_H
#define ANIMLOADER_HANDLER_H

#include "lwpp/plugin_handler.h"
#include <lwpp/lw_server.h>
#include <lwpp/monitor.h>
#include <lwpp/message.h>
#include <lwtypes.h>
#include <lwanimlod.h>

namespace lwpp
{

  //! @ingroup Handler
  class AnimLoaderHandler : public InstanceHandler
  {
  protected:
    LWAnimLoaderHandler *local;
    //SimpleMonitor monitor;
    int version;
    std::string fileName;
  public:
		AnimLoaderHandler(void* g, void* context, LWError* err)
			: InstanceHandler(g,context,err, LWANIMLOADER_HCLASS)
		{
			fileName = (const char*) context;
		}
    virtual ~AnimLoaderHandler() {;}
    bool virtual isValid() = 0;
    virtual int				GetFrameCount () = 0;
    virtual double		GetFrameRate  () = 0;
    virtual double		GetAspect     (int *w, int *h, double *pixAspect) = 0;
    virtual void			Evaluate      (double, LWAnimFrameAccess *) = 0;
  };

  //! @ingroup Adaptor
  template <class T>
  class AnimLoaderAdaptor : public InstanceAdaptor <T>
  {
  public:
    AnimLoaderAdaptor(const char *name, ServerTagInfo *tags)
    {
      LWServer::AddPlugin(LWANIMLOADER_HCLASS, name, Activate, tags);
    }
    //! Set static callbacks for the LightWave instance handler
    static int Activate( int version, GlobalFunc *global, void *l,  void *serverData )
    {
      if (version > LWANIMLOADER_VERSION) return AFUNC_BADVERSION;
      try
      {
        lwpp::SetSuperGlobal(global);
        LWAnimLoaderHandler *plugin = static_cast<LWAnimLoaderHandler *>(l);
        InstanceAdaptor<T>::Activate(plugin->inst);
        plugin->inst->create = AnimLoaderAdaptor::Create;
        plugin->frameCount = AnimLoaderAdaptor::FrameCount;
        plugin->frameRate = AnimLoaderAdaptor::FrameRate;
        plugin->aspect = AnimLoaderAdaptor::Aspect;
        plugin->evaluate = AnimLoaderAdaptor::Evaluate;

        return AFUNC_OK;
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured when attempting to load an Animation:", e.what());
        return AFUNC_BADAPP_SILENT;
      }
    }

  private:
			static LWInstance Create (void *priv, void *context, LWError *err)
			{
				try
				{
					LWError localErr = "\0";
					T *plugin = new T(priv, context, &localErr);
					if (*localErr)
					{
						delete plugin;
						plugin = 0;
						*err = localErr;
					}
          if (!plugin->isValid())
          {
            delete plugin;
						plugin = 0;
          }
					return static_cast<LWInstance> (plugin);
				}
				catch (std::exception &e)
				{
					static LWError error = e.what();
					err = &error;
					return 0;
				}
			}

    static int FrameCount (LWInstance instance)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->GetFrameCount();
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in AnimLoader::GetFrameCount():", e.what());
        return 0;
      }
    }

    static double FrameRate (LWInstance instance)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->GetFrameRate();
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in AnimLoader::GetFrameRate():", e.what());
        return 0.0;
      }
    }

    static double Aspect (LWInstance instance, int *w, int *h, double *pixAspect)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->GetAspect(w, h, pixAspect);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in AnimLoader::GetAspect():", e.what());
        return 0.0;
      }
    }

    static void Evaluate (LWInstance instance, double time, LWAnimFrameAccess *fa)
    {
      try
      {
        T *plugin = (T *) instance;
        plugin->Evaluate(time, fa);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in AnimLoader::Evaluate():", e.what());
      }
    }
  };
}

#endif // ANIMLOADER_HANDLER_H