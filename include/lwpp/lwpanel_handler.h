#ifndef LWPP_LWPANEL_HANDLER_H
#define LWPP_LWPANEL_HANDLER_H

#include "lwpp/panel.h"

#pragma warning( push )
#pragma warning( disable : 4100 )

namespace lwpp
{
/*
 * LWPanel related classes
 */
		class LWPanelInterface : protected LWPanelCallbacks
	{
		friend class LWPanel;
		protected:
			LWPanel Panel;	
		public:
			LWPanelInterface()
			{;}
			virtual ~LWPanelInterface()
			{
				;
			}
			//! Open LWPanel
			virtual LWError Options (void)
			{
				return 0;
			}
	};


	template <class T>
	class LWPanelAdaptor
	{
		public:
			LWPanelAdaptor(const char *name, const char *className, ServerTagInfo tags[])	
			{
				LWServer::AddPlugin(className, name, Interface, tags);
			}

			static LWError Options (LWInstance inst)
			{
				T *plugin = static_cast<T *>(inst);
				return plugin->Options();
			}

			static int Interface (int version, GlobalFunc *global, void *loc, void *serverdata)
			{
				if ( version < LWINTERFACE_VERSION ) return AFUNC_BADVERSION;
				try
				{
					if (loc)
					{
						lwpp::SetSuperGlobal(global);
						LWInterface *local = static_cast<LWInterface *>(loc);
						local->panel = 0;
						local->options = Options;
						local->command = 0;
					}
					return AFUNC_OK;
				}
				catch (std::exception &e)
				{
					lwpp::LWMessage::Error("An exception occured in LWPanel::Activate():", e.what());
					return AFUNC_BADAPP;
				}
			}
	};

} // end of namespace lwpp

#pragma warning( pop ) 
#endif // LWPP_LWPANEL_HANDLER_H