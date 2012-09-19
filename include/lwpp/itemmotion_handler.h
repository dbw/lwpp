#ifndef LWPP_ITEMMOTION_HANDLER_H
#define LWPP_ITEMMOTION_HANDLER_H
 
#include "lwpp/plugin_handler.h"
#include <lwmotion.h>
 
namespace lwpp
{
	//! @ingroup Handler
	class ItemMotionHandler : public InstanceHandler, public ItemHandler 
	{
	protected:
		LWItem Context;
	public:
		ItemMotionHandler(void* g, void* context, LWError* err)
			: InstanceHandler(g,context,err, LWITEMMOTION_HCLASS)
		{
			Context.SetID(LWItemID(context));
		}
		virtual ~ItemMotionHandler()
		{
			;
		}
		
		virtual void Evaluate(const LWItemMotionAccess *ma)
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
	class ItemMotionAdaptor : public InstanceAdaptor <T>, public ItemAdaptor <T> 
	{
	public:
		ItemMotionAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWITEMMOTION_HCLASS, name, Activate, tags);
		}
		static int Activate (int version , GlobalFunc* global, LWInstance inst, void* serverData)
		{
			if(version!=LWITEMMOTION_VERSION) return AFUNC_BADVERSION;
			lwpp::SetSuperGlobal(global);
			
			LWItemMotionHandler* plugin = static_cast<LWItemMotionHandler*>(inst);
			InstanceAdaptor<T>::Activate(plugin->inst);
			ItemAdaptor<T>::Activate(plugin->item);
 
			plugin->evaluate = ItemMotionAdaptor::Evaluate;
			plugin->flags    = ItemMotionAdaptor::Flags;
	
			UNUSED(serverData);
			return AFUNC_OK;
		}
	private:
		static void Evaluate (LWInstance inst, const LWItemMotionAccess* ma)
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
	IMPLEMENT_XPANELHANDLER(ItemMotion);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_XPANELADAPTOR(ItemMotion,min,max);
	//! @ingroup LWPanelHandler
	IMPLEMENT_LWPANELHANDLER(ItemMotion);
	//! @ingroup LWPanelAdaptor
	IMPLEMENT_LWPANELADAPTOR(ItemMotion,min,max);
}
#endif //ITEMMOTION_H