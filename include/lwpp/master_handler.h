#ifndef LWPP_MASTER_HANDLER
#define LWPP_MASTER_HANDLER

#include "lwpp/plugin_handler.h"
#include <lwmaster.h>
#include <lwpp/lw_server.h>

namespace lwpp
{
	//! @ingroup Handler
	class MasterHandler : public InstanceHandler, public ItemHandler
	{
	public:
		MasterHandler(void *g, void *context, LWError *err) : InstanceHandler(g, context, err, LWMASTER_HCLASS)
		{
			;
		}
		virtual ~MasterHandler()
		{
			;
		}
		virtual double Event (const LWMasterAccess *ma) {UNUSED(ma);return 0.0;}
		virtual unsigned int Flags() {return 0;}
	};

//! Wrapper for an MasterHandler
/*!
 * @ingroup Adaptor
 */
template <class T, int version, int plugType, bool noIO = false>
class MasterAdaptor : public InstanceAdaptor <T>, public ItemAdaptor <T>
{
	public:
		MasterAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWMASTER_HCLASS, name, Activate, tags);
		}
		//! Set static callbacks for a LightWave Master
		static int Activate (int lw_version, GlobalFunc *global, LWInstance inst, void *serverData)
		{
			if ( lw_version != version ) return AFUNC_BADVERSION;
			try
			{
				UNUSED(serverData);
				lwpp::SetSuperGlobal(global);

				LWMasterHandler *plugin = static_cast<LWMasterHandler *>(inst);

				InstanceAdaptor<T>::Activate(plugin->inst);
				if (noIO) // cause "warning C4127: conditional expression is constant" but is fine...
				{
					plugin->inst->load = 0;
					plugin->inst->save = 0;
				}
				ItemAdaptor<T>::Activate(plugin->item);        
				plugin->type = plugType;
				plugin->event = Event;
				plugin->flags = Flags;
				return AFUNC_OK;
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in MasterAdaptor::Activate():", e.what());
				return AFUNC_BADAPP;
			}
		}
	private:

		static double Event (LWInstance instance, const LWMasterAccess *ma)
		{
			try
			{
				T *plugin = (T *) instance;
				return plugin->Event(ma);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in MasterAdaptor::Event():", e.what());
				return 0.0;
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
				lwpp::LWMessage::Error("An exception occured in MasterAdaptor::Flags():", e.what());
				return 0;
			}
		}
};

	//! Class for MasterHandler with an XPanel
	//! @ingroup XPanelHandler
	IMPLEMENT_XPANELHANDLER(Master);

	//! @ingroup LWPanelHandler
	IMPLEMENT_LWPANELHANDLER(Master);

	IMPLEMENT_GIZMO_XPANELHANDLER(Master);
	IMPLEMENT_GIZMOHANDLER(Master);
	IMPLEMENT_GIZMO_LWPANELHANDLER(Master);

	//! @ingroup XPanelAdaptor
	template <class T, int Version, int plugType, bool noIO = false>
	class XPanelMasterAdaptor : public MasterAdaptor<T, Version, plugType, noIO>, public XPanelAdaptor<T>
	{
	public:
		XPanelMasterAdaptor(const char *name, ServerTagInfo *tags = 0)
			: XPanelAdaptor<T>(name, LWMASTER_ICLASS, tags),
				MasterAdaptor<T, Version, plugType, noIO>(name, tags)
		{
			;
		}
	};

	//! @ingroup LWPanelAdaptor
	template <class T, int Version, int plugType>
	class LWPanelMasterAdaptor : public MasterAdaptor<T, Version, plugType>, public LWPanelAdaptor<T>
	{
	public:
		LWPanelMasterAdaptor(const char *name, ServerTagInfo *tags = 0)
			: LWPanelAdaptor<T>(name, LWMASTER_ICLASS, tags),
				MasterAdaptor<T, Version, plugType>(name, tags)
		{
			;
		}
	};

	template <class T, int Version, int plugType>
	class GizmoXPanelMasterAdaptor : public MasterAdaptor<T, Version, plugType>, public GizmoAdaptor<T>, public XPanelAdaptor<T>
	{
	public:
		GizmoXPanelMasterAdaptor(const char *name, ServerTagInfo *tags = 0)
			: GizmoAdaptor<T>(name, LWMASTER_GCLASS, tags),
			XPanelAdaptor<T>(name, LWMASTER_ICLASS, tags),
			MasterAdaptor<T, Version, plugType>(name, tags)
		{
			;
		}
	};

	template <class T, int Version, int plugType, bool noIO = false>
	class GizmoMasterAdaptor : public MasterAdaptor<T, Version, plugType, noIO>, public GizmoAdaptor<T>
	{
	public:
		GizmoMasterAdaptor(const char* name, ServerTagInfo* tags = 0)
			: GizmoAdaptor<T>(name, LWMASTER_GCLASS, tags),
			MasterAdaptor<T, Version, plugType, noIO>(name, tags)
		{
			;
		}
	};

	//! @ingroup LWPanelAdaptor
	template <class T, int Version, int plugType>
	class GizmoLWPanelMasterAdaptor : public MasterAdaptor<T, Version, plugType>, public GizmoAdaptor<T>, public LWPanelAdaptor<T>
	{
	public:
		GizmoLWPanelMasterAdaptor(const char *name, ServerTagInfo *tags = 0)
			: GizmoAdaptor<T>(name, LWMASTER_GCLASS, tags),
			LWPanelAdaptor<T>(name, LWMASTER_ICLASS, tags),
			MasterAdaptor<T, Version, plugType>(name, tags)
		{
			;
		}
	};

} // namespace lwpp
#endif // LWPP_MASTER_HANDLER
