#ifndef LWPP_INSTANCER_HANDLER
#define LWPP_INSTANCER_HANDLER
#include "lwpp/plugin_handler.h"
#include <lwinstancing.h>
#include <lwsurf.h>

namespace lwpp
{
//! Class for Instancers
/*!
 * @ingroup Handler
 */
	class InstancerHandler : public InstanceHandler, public ItemHandler, public RenderHandler
	{
	protected:
		LWItemID Context;
	public:
		InstancerHandler(void *g, void *context, LWError *err) : InstanceHandler(g, context, err, LWINSTANCER_HCLASS)
		{
			Context = static_cast<LWItemID>(context);
		}
		virtual ~InstancerHandler() {;}
		virtual void Evaluate(LWInstance inst, const LWInstancerAccess *ia) = 0;
	};

	//! Wrapper for an Instancer
	/*!
	* @ingroup Adaptor
	*/
	template <class T, int maxVersion, int minVersion> 
	class InstancerAdaptor : public InstanceAdaptor <T>, public ItemAdaptor <T>//, public RenderAdaptor <T>
	{
	public:
		InstancerAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWINSTANCER_HCLASS, name, Activate, tags);
		}
		//! Set static callbacks for a LightWave InstanceHandler
		static int Activate (int version, GlobalFunc *global, LWInstance inst, void *serverData)
		{
			UNUSED(serverData);
			try
			{
				if ( version > maxVersion ) return AFUNC_BADVERSION;
				if ( version < minVersion ) return AFUNC_BADVERSION;
				lwpp::SetSuperGlobal(global);
				LWInstancerHandler *plugin = static_cast<LWInstancerHandler *>(inst);

				InstanceAdaptor<T>::Activate(plugin->inst);
				ItemAdaptor<T>::Activate(plugin->item);
				//RenderAdaptor<T>::Activate(plugin->rend);
				plugin->evaluate = InstancerAdaptor::Evaluate;
				return AFUNC_OK;
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in InstancerAdaptor::Activate():", e.what());
				return AFUNC_BADAPP;
			}
		}
	private:
		static void Evaluate (LWInstance instance, const LWInstancerAccess *ia)
		{
			try
			{
				T *plugin = (T *) instance;
				plugin->Evaluate(instance, ia);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in InstancerAdaptor::Evaluate():", e.what());
			}
		}

	};

	//! Class for Procedural Textures with an XPanel
	//! @ingroup XPanelHandler
	IMPLEMENT_XPANELHANDLER(Instancer);

	IMPLEMENT_XPANELADAPTOR(Instancer, LWPP_INSTANCER_VERSION, LWPP_INSTANCER_VERSION );
}

#endif //  LWPP_INSTANCER_HANDLER