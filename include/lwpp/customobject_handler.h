#ifndef CUSTOMOBJECT_HANDLER_H
#define CUSTOMOBJECT_HANDLER_H

#include <lwpp/plugin_handler.h>
#include <lwpp/customobject_access.h>

namespace lwpp
{	
	//! Base class for CustomObject plugins
	/*!
	* @note Work in progress
	* @ingroup Handler
	*/
	class CustomObjHandler : public InstanceHandler, public ItemHandler, public RenderHandler
	{
	protected:
		//! Stores the mID of the object wich the plugin is applied to
		LWItemID Context;
	public:
		CustomObjHandler(void *g, void *context, LWError *err) : InstanceHandler(g, context, err, LWCUSTOMOBJ_HCLASS)
		{
			Context = LWItemID(context);
		}
		virtual ~CustomObjHandler() {;}
		virtual void Evaluate(CustomObjAccess &coa)
		{
			UNUSED(coa);
		}
		virtual unsigned int Flags() {return 0;}
	};

	//! Wrapper for an CustomObjectHandler
	/*!
	* @ingroup Adaptor
	*/
	template <class T, int minVersion>
	class CustomObjAdaptor : public InstanceAdaptor <T>, public ItemAdaptor <T>, public RenderAdaptor <T>
	{
	public:
		CustomObjAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWCUSTOMOBJ_HCLASS, name, Activate, tags);
		}
		//! Set static callbacks for a LightWave CustomObject
		static int Activate (int version, GlobalFunc *global, LWInstance inst, void *serverData)
		{
			if ( version < minVersion ) return AFUNC_BADVERSION;

			try
			{
				UNUSED(serverData);
				lwpp::SetSuperGlobal(global);

				LWCustomObjHandler *plugin = static_cast<LWCustomObjHandler *>(inst);

				InstanceAdaptor<T>::Activate(plugin->inst);
				ItemAdaptor<T>::Activate(plugin->item);
				RenderAdaptor<T>::Activate(plugin->rend);
				plugin->evaluate = CustomObjAdaptor::Evaluate;
				plugin->flags   = CustomObjAdaptor::Flags;

				return AFUNC_OK;
			} 
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in CustomObject::Activate():", e.what());
				return AFUNC_BADAPP;
			}
		}
	private:

		static void Evaluate (LWInstance instance, const LWCustomObjAccess *lw_coa)
		{
			try
			{
				T *plugin = (T *) instance;
				plugin->Evaluate(CustomObjAccess(lw_coa));
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in CustomObject::Evaluate():", e.what());
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
				lwpp::LWMessage::Error("An exception occured in CustomObject::Flags():", e.what());
				return 0;
			}
		}
	};

	//! Class for CustomObject Handler with an XPanel
	//! @ingroup XPanelHandler
	IMPLEMENT_XPANELHANDLER(CustomObj);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_XPANELADAPTOR(CustomObj, LWCUSTOMOBJ_VERSION);

	//! @ingroup LWPanelHandler
	IMPLEMENT_LWPANELHANDLER(CustomObj);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_LWPANELADAPTOR(CustomObj, LWCUSTOMOBJ_VERSION);

} // end namespace

#endif // CUSTOMOBJECT_HANDLER_H