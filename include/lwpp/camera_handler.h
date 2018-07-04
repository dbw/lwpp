#ifndef LWPP_CAMERA_HANDLER_H
#define LWPP_CAMERA_HANDLER_H

#if (LW_SDK >= 95)

#include "lwpp/plugin_handler.h"
#include <lwcamera.h>

namespace lwpp
{
	//! @ingroup Handler
	class CameraHandler : public InstanceHandler, public ItemHandler, public RenderHandler
	{
	public:
		CameraHandler(void* g, void* context, LWError* err)
      : InstanceHandler (g, context, err, LWCAMERA_HCLASS)
		{
			;
		}
		virtual ~CameraHandler() {;}
		virtual bool Preview (double lpx, double lpy,LWDMatrix4 projection,const LWCameraAccess* camaccess)
		{
			return false; //Return 1, yes, we have returned a valid matrix
		}

		virtual LWError NewFrame (const LWFrameInfo* frameinfo, const LWCameraAccess* camaccess)
		{
			return 0;
		}

		virtual int Evaluate(double fpx, double fpy, double lpx, double lpy, double fractime, LWCameraRay* ray, const LWCameraAccess* camaccess)
		{
			return LWCAMTYPEEVAL_NO_RAY; //By default, no rays are cast
		}

		virtual int Flags()
		{
			return 0; //says This Plugin doesn't use DOF by default
		}
	};

	//! @ingroup Adaptor
	template <class T>
	class CameraAdaptor : public InstanceAdaptor <T>, public ItemAdaptor <T>, public RenderAdaptor <T>
	{
	public:
		CameraAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWCAMERA_HCLASS, name, Activate, tags);
		}
		static int Activate (int version , GlobalFunc* global, LWInstance inst, void* serverData)
		{
			if(version!=LWCAMERA_VERSION) return AFUNC_BADVERSION;
			lwpp::SetSuperGlobal(global);
			
			LWCameraHandler* plugin = static_cast<LWCameraHandler*>(inst);
			InstanceAdaptor<T>::Activate(plugin->inst);
			ItemAdaptor<T>::Activate(plugin->item);
			RenderAdaptor<T>::Activate(plugin->rend);

			plugin->preview  = CameraAdaptor::Preview;
			plugin->newFrame = CameraAdaptor::NewFrame;
			plugin->evaluate = CameraAdaptor::Evaluate;
			plugin->flags    = CameraAdaptor::Flags;
	
			UNUSED(serverData);
			return AFUNC_OK;
		}

	private:
		static int Preview (LWInstance inst, double lpx, double lpy, LWDMatrix4 projection, const LWCameraAccess* camaccess)
		{
			T* plugin = static_cast<T*>(inst);
			return plugin->Preview(lpx,lpy,projection,camaccess) ? 1 : 0;
		}
		static LWError NewFrame (LWInstance inst, const LWFrameInfo* frameinfo, const LWCameraAccess* camaccess)
		{
			T* plugin = static_cast<T*>(inst);
			return plugin->NewFrame(frameinfo, camaccess);
		}
		static int Evaluate (LWInstance inst, double fpx, double fpy, double lpx, double lpy, double fractime, LWCameraRay* ray, const LWCameraAccess* camaccess)
		{
			T* plugin = static_cast<T*>(inst);
			return plugin->Evaluate(fpx,fpy,lpx,lpy,fractime,ray,camaccess);
		}
		static unsigned int Flags (LWInstance inst)
		{
			T* plugin = static_cast<T*>(inst);
			return plugin->Flags();
		}
	};

	/* GUI Classes */
	//! @ingroup XPanelHandler
	IMPLEMENT_XPANELHANDLER(Camera);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_XPANELADAPTOR(Camera);


	class LWPanelCameraHandler : public CameraHandler, public LWPanelInterface {
	  public:
		LWPanelCameraHandler(void *g, void *context, LWError *err) : CameraHandler(g, context, err) 
		{
			;
		}
	};

	template <class T>
	class LWPanelCameraAdaptor : public CameraAdaptor<T>, public LWPanelAdaptor<T>
	{
		;
	};
}
#endif // version check
#endif //CAMERA_HANDLER_H