#ifndef LWPP_ENVIRONMENT_HANDLER_H
#define LWPP_ENVIRONMENT_HANDLER_H
 
#include "lwpp/plugin_handler.h"
#include <lwenviron.h>
 
namespace lwpp
{
	//! @ingroup Handler
	class EnvironmentHandler : public InstanceHandler, public ItemHandler, public RenderHandler 
	{
	public:
		EnvironmentHandler(void* g, void* context, LWError* err) : InstanceHandler(g,context,err){};
		virtual ~EnvironmentHandler(){};
		
		virtual LWError Evaluate (LWEnvironmentAccess *enva)
		{
			double x,y,z;
			x = enva->dir[0];
			y = enva->dir[1];
			z = enva->dir[2];
			enva->color[0]=x/2.0+0.5;
			enva->color[1]=y/2.0+0.5;
			enva->color[2]=z/2.0+0.5;
			return NULL;
		};
		virtual unsigned int Flags()
		{
			return 0; 
		}
	};
 
	//! @ingroup Adaptor
	template <class T>
	class EnvironmentAdaptor : public InstanceAdaptor <T>, public ItemAdaptor <T> , public RenderAdaptor<T>
	{
	public:
		EnvironmentAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWENVIRONMENT_HCLASS, name, Activate, tags);
		}
		static int Activate (int version , GlobalFunc* global, LWInstance inst, void* serverData)
		{
			if(version!=LWENVIRONMENT_VERSION) return AFUNC_BADVERSION;
			lwpp::SetSuperGlobal(global);
			
			LWEnvironmentHandler* plugin = static_cast<LWEnvironmentHandler*>(inst);
			InstanceAdaptor<T>::Activate(plugin->inst);
			ItemAdaptor<T>::Activate(plugin->item);
			RenderAdaptor<T>::Activate(plugin->rend);
 
			plugin->evaluate = EnvironmentAdaptor::Evaluate;
			plugin->flags    = EnvironmentAdaptor::Flags;
	
			UNUSED(serverData);
			return AFUNC_OK;
		}
	private:
		static LWError Evaluate (LWInstance inst, LWEnvironmentAccess* ma)
		{
			T* plugin = static_cast<T*>(inst);
			plugin->Evaluate(ma);
			return NULL;
		}
		static unsigned int Flags (LWInstance inst)
		{
			T* plugin = static_cast<T*>(inst);
			return plugin->Flags();
		}
	};
 
	/* GUI Classes */
	//! @ingroup XPanelHandler
	IMPLEMENT_XPANELHANDLER(Environment);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_XPANELADAPTOR(Environment);
	//! @ingroup LWPanelHandler
	IMPLEMENT_LWPANELHANDLER(Environment);
	//! @ingroup LWPanelAdaptor
	IMPLEMENT_LWPANELADAPTOR(Environment);
}
#endif //LWPP_ENVIRONMENT_HANDLER_H