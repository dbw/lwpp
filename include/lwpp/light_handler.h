#ifndef LWPP_LIGHT_HANDLER
#define LWPP_LIGHT_HANDLER

#if (LW_SDK >= 95)

#ifdef LW11_COMPAT
#include "lwpp/LW11Compat.h"
#endif

#include "lwpp/plugin_handler.h"
#include <lwlight.h>

namespace lwpp
{
  //! Class for Lights
  /*!
  * @ingroup Handler
  */
  class LightHandler : public InstanceHandler, public ItemHandler, public RenderHandler
  {
  protected:
    LWItemID Context;
  public:
    LightHandler(void *g, void *context, LWError *err) : InstanceHandler(g, context, err, LWLIGHT_HCLASS)
    {
      Context = static_cast<LWItemID>(context);
    }
    virtual ~LightHandler() {;}
    virtual unsigned int Flags      ()
    {
      return (LWLIGHTTYPEF_NO_PHOTONS | LWLIGHTTYPEF_NO_RAYILLUM);
    }
    virtual LWError      NewFrame   (const LWFrameInfo* frameinfo, unsigned int* maxilluminations, const LWLightAccess* lightaccess) = 0;
    virtual int          Evaluate   (const LWDVector spot, double fractime, LWIllumination illumination[], const LWLightAccess* lightaccess) = 0;
    virtual unsigned int GetPhotons (unsigned int maxphotons, LWPhoton photons[], const LWLightAccess* lightaccess) {return 0;}
    virtual unsigned int GetRayIlluminations (LWDVector raystart, LWDVector raydir, double farclip, unsigned int maxrayilluminations, LWRayIllumination rayilluminations[], const LWLightAccess* lightaccess) {return 0;}
#ifdef LW11_COMPAT
    virtual LWError      NewFrame11   (const LWFrameInfo* frameinfo, unsigned int* maxilluminations, const LW11::LWLightAccess* lightaccess) = 0;
    virtual int          Evaluate11   (const LWDVector spot, double fractime, LWIllumination illumination[], const LW11::LWLightAccess* lightaccess) = 0;
    virtual unsigned int GetPhotons11 (unsigned int maxphotons, LWPhoton photons[], const LW11::LWLightAccess* lightaccess) {return 0;}
    virtual unsigned int GetRayIlluminations11 (LWDVector raystart, LWDVector raydir, double farclip, unsigned int maxrayilluminations, LWRayIllumination rayilluminations[], const LW11::LWLightAccess* lightaccess) {return 0;}
#endif
    virtual const LWPreviewLight* Preview() {return 0;}
  };

  //! Wrapper for an LightHandler
  /*!
  * @ingroup Adaptor
  */
  template <class T, int minVersion, int maxVersion> 
  class LightAdaptor : public InstanceAdaptor <T>, public ItemAdaptor <T>, public RenderAdaptor <T>
  {
  public:
    LightAdaptor(const char *name, ServerTagInfo *tags)
    {
      LWServer::AddPlugin(LWLIGHT_HCLASS, name, Activate, tags);
    }
    //! Set static callbacks for a LightWave ImageFilter
    static int Activate (int version, GlobalFunc *global, LWInstance inst, void *serverData)
    {
      UNUSED(serverData);
      try
      {
        if ( version > maxVersion ) return AFUNC_BADVERSION;
        if ( version < minVersion ) return AFUNC_BADVERSION;
        lwpp::SetSuperGlobal(global);
        LWLightHandler *plugin = static_cast<LWLightHandler *>(inst);

        InstanceAdaptor<T>::Activate(plugin->inst);
        ItemAdaptor<T>::Activate(plugin->item);
        RenderAdaptor<T>::Activate(plugin->rend);

        if (version < LWLIGHT11_VERSION)
        {
          plugin->evaluate = LightAdaptor::Evaluate;
          plugin->flags   = LightAdaptor::Flags;
          plugin->newFrame = LightAdaptor::NewFrame;
          plugin->getPhotons = LightAdaptor::GetPhotons;
          plugin->getRayIlluminations = LightAdaptor::GetRayIlluminations;
          plugin->preview = LightAdaptor::Preview;
        }
        else
        {
          LW11::LWLightHandler *plug11 = static_cast<LW11::LWLightHandler *>(inst);
          plug11->evaluate = LightAdaptor::Evaluate11;
          plug11->flags   = LightAdaptor::Flags;
          plug11->newFrame = LightAdaptor::NewFrame11;
          plug11->getPhotons = LightAdaptor::GetPhotons11;
          plug11->getRayIlluminations = LightAdaptor::GetRayIlluminations11;
          plug11->preview = LightAdaptor::Preview;
        }
        return AFUNC_OK;
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in LightAdaptor::Activate():", e.what());
        return AFUNC_BADAPP;
      }
    }
  private:
    static LWError NewFrame (LWInstance instance, const LWFrameInfo* frameinfo, unsigned int* maxilluminations, const LWLightAccess* lightaccess)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->NewFrame(frameinfo, maxilluminations, lightaccess);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in LightHandler::NewFrame():", e.what());
        return 0;
      }
    }
    static int Evaluate (LWInstance instance, const LWDVector spot, double fractime, LWIllumination illumination[], const LWLightAccess* lightaccess)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->Evaluate(spot, fractime, illumination, lightaccess);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in LightHandler::Evaluate():", e.what());
        return 0;
      }
    }
    static unsigned int GetPhotons (LWInstance instance, unsigned int maxphotons, LWPhoton photons[], const LWLightAccess* lightaccess)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->GetPhotons(maxphotons, photons, lightaccess);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in LightHandler::GetPhotons():", e.what());
        return 0;
      }
    }
    static unsigned int GetRayIlluminations (LWInstance instance, LWDVector raystart, LWDVector raydir, double farclip, unsigned int maxrayilluminations, LWRayIllumination rayilluminations[], const LWLightAccess* lightaccess)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->GetRayIlluminations(raystart, raydir, farclip, maxrayilluminations, rayilluminations, lightaccess);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in LightHandler::GetRayIlluminations():", e.what());
        return 0;
      }
    }
#ifdef LW11_COMPAT
    static LWError NewFrame11 (LWInstance instance, const LWFrameInfo* frameinfo, unsigned int* maxilluminations, const LW11::LWLightAccess* lightaccess)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->NewFrame11(frameinfo, maxilluminations, lightaccess);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in LightHandler::NewFrame():", e.what());
        return 0;
      }
    }
    static int Evaluate11 (LWInstance instance, const LWDVector spot, double fractime, LWIllumination illumination[], const LW11::LWLightAccess* lightaccess)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->Evaluate11(spot, fractime, illumination, lightaccess);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in LightHandler::Evaluate():", e.what());
        return 0;
      }
    }
    static unsigned int GetPhotons11 (LWInstance instance, unsigned int maxphotons, LWPhoton photons[], const LW11::LWLightAccess* lightaccess)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->GetPhotons11(maxphotons, photons, lightaccess);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in LightHandler::GetPhotons():", e.what());
        return 0;
      }
    }
    static unsigned int GetRayIlluminations11 (LWInstance instance, LWDVector raystart, LWDVector raydir, double farclip, unsigned int maxrayilluminations, LWRayIllumination rayilluminations[], const LW11::LWLightAccess* lightaccess)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->GetRayIlluminations11(raystart, raydir, farclip, maxrayilluminations, rayilluminations, lightaccess);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in LightHandler::GetRayIlluminations():", e.what());
        return 0;
      }
    }
#endif //LW11_COMPAT

    static const LWPreviewLight* Preview (LWInstance instance)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->Preview();
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in LightHandler::Preview():", e.what());
        return 0;
      }
    }

    static unsigned int Flags (LWInstance instance)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->Flags();
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in LightHandler::Evaluate():", e.what());
        return 0;
      }
    }
  };

  //! Class for Procedural Textures with an XPanel
  //! @ingroup XPanelHandler
  IMPLEMENT_XPANELHANDLER(Light);
  //! @ingroup XPanelAdaptor
  IMPLEMENT_XPANELADAPTOR(Light, LWPP_LIGHT_VERSION, LWPP_LIGHT_VERSION);	
}

#endif // LWSDK version check
#endif // LWPP_LIGHT_HANDLER