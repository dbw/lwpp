#ifndef LWPP_CHANNEL_HANDLER_H
#define LWPP_CHANNEL_HANDLER_H
 
#include "lwpp/plugin_handler.h"
#include <lwchannel.h>

namespace lwpp
{
	class ChannelHandler : public lwpp::InstanceHandler
	{
	protected:
		LWItem Context;
	public:
		ChannelHandler(void* g, void* context, LWError* err)
			: InstanceHandler(g,context,err, LWCHANNEL_HCLASS)
		{
			Context.SetID(LWItemID(context));
		}
		virtual ~ChannelHandler()
		{
			;
		}
		
		virtual void Evaluate(const LWChannelAccess *ca)
		{
			;
		}
		virtual unsigned int Flags()
		{
			return 0; //says This Plugin doesn't get applied after IK by default.
		}
	};

	//! @ingroup Adaptor
	template <class T, int min, int max>
	class ChannelAdaptor : public InstanceAdaptor <T>
	{
	public:
		ChannelAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWITEMMOTION_HCLASS, name, Activate, tags);
		}
		static int Activate (int version , GlobalFunc* global, LWInstance inst, void* serverData)
		{
			if(version!=LWCHANNEL_VERSION) return AFUNC_BADVERSION;
			lwpp::SetSuperGlobal(global);
			
			LWChannelHandler* plugin = static_cast<LWChannelHandler*>(inst);
			InstanceAdaptor<T>::Activate(plugin->inst);
			ItemAdaptor<T>::Activate(plugin->item);
 
			plugin->evaluate = ItemMotionAdaptor::Evaluate;
			plugin->flags    = ItemMotionAdaptor::Flags;
	
			UNUSED(serverData);
			return AFUNC_OK;
		}
	private:
		static void Evaluate (LWInstance inst, const LWChannelAccess* ma)
		{
			T* plugin = static_cast<T*>(inst);
			plugin->Evaluate(ma);
		}
		static unsigned int Flags (LWInstance inst)
		{
			T* plugin = static_cast<T*>(inst);
			return plugin->Flags();
		}
	};
		 
	/* GUI Classes */
	//! @ingroup XPanelHandler
	IMPLEMENT_XPANELHANDLER(Channel);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_XPANELADAPTOR(Channel,min,max);
	//! @ingroup LWPanelHandler
	IMPLEMENT_LWPANELHANDLER(Channel);
	//! @ingroup LWPanelAdaptor
	IMPLEMENT_LWPANELADAPTOR(Channel,min,max);
}

#endif //LWPP_CHANNEL_HANDLER_H