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
	/*
	Also add
	static bool canLoad(const char *file);
	doe the derived class
	*/
  class AnimLoaderHandler : public InstanceHandler
  {
  protected:
    LWAnimLoaderHandler *local;
    int version;
    std::string mAnimFileName;
		int mWidth = 0, mHeight = 0, mFrameCount = 0;
		double mPixelAspect = 1.0, mFrameRate = 24;
  public:
		AnimLoaderHandler(void* g, void* context, LWError* err)
			: InstanceHandler(g,context,err, LWANIMLOADER_HCLASS)
		{
			mAnimFileName = (const char*) context;
		}
    virtual ~AnimLoaderHandler() {;}
    //virtual bool isValid() = 0;
		//virtual void Cleanup() = 0; // only called if isValid() == false
		virtual int				GetFrameCount()
		{
			return mFrameCount;
		}
		virtual double		GetFrameRate()
		{
			return mFrameRate;
		}
		virtual double		GetAspect(int *w, int *h, double *pixAspect)
		{
			*w = mWidth;
			*h = mHeight;
			*pixAspect = mPixelAspect;
			if (mHeight) return mWidth * mPixelAspect / mHeight;
			return 1.0;
		}
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
        plugin->inst->create = AnimLoaderAdaptor::Create; // create acts a bit different in this case
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

#ifdef _DEBUG
					dout << "--> AnimLoaderAdaptor context = " << (const char*)context << " ) ?\n";
#endif					
					T *plugin = T::Create(priv, context, err);
					if (*err && (plugin == nullptr))
					{
						delete plugin;
						plugin = 0;
					}
#ifdef _DEBUG
					dout << "<-- AnimLoaderAdaptor context = " << (const char*)context << " ) ?\n";
#endif					
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