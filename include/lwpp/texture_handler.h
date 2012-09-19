/*!
 * @file
 * @brief
 * @version $Id: texture_handler.h 72 2009-03-17 11:10:32Z mwolf $
 */

#ifndef LWPP_PROCEDURAL_TEXTURE_HANDLER
#define LWPP_PROCEDURAL_TEXTURE_HANDLER
#include "lwpp/plugin_handler.h"
#include <lwtexture.h>

namespace lwpp
{
//! Class for Procedural Textures
/*!
 * @ingroup Handler
 * @note LWTextureAccess  is still the original struct and not replaced by the wrapper with a class
 */
class TextureHandler : public InstanceHandler, public ItemHandler, public RenderHandler
{
	protected:
		LWTLayerID Context;
	public:
	  TextureHandler(void *g, void *context, LWError *err) : InstanceHandler(g, context, err, LWTEXTURE_HCLASS)
		{
			Context = static_cast<LWTLayerID>(context) ;
		}
		virtual ~TextureHandler() {;}
		virtual double Evaluate(LWTextureAccess *ta) {UNUSED(ta); return 0;}
	  virtual unsigned int Flags() {return 0;}
};

//! Wrapper for an ImageFilterHandler
/*!
 * @ingroup Adaptor
 */
	template <class T, int maxVersion, int minVersion>
	class TextureAdaptor : public InstanceAdaptor <T>, public ItemAdaptor <T>, public RenderAdaptor <T>
	{
		public:
		TextureAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWTEXTURE_HCLASS, name, Activate, tags);
		}
		//! Set static callbacks for a LightWave ImageFilter
		static int Activate (int version, GlobalFunc *global, LWInstance inst, void *serverData)
		{
			UNUSED(serverData);
			if ( version > maxVersion ) return AFUNC_BADVERSION;
			if ( version < minVersion ) return AFUNC_BADVERSION;
		  lwpp::SetSuperGlobal(global);
			LWTextureHandler *plugin = static_cast<LWTextureHandler *>(inst);

			InstanceAdaptor<T>::Activate(plugin->inst);
			ItemAdaptor<T>::Activate(plugin->item);
			RenderAdaptor<T>::Activate(plugin->rend);
			plugin->evaluate = TextureAdaptor::Evaluate;
			plugin->flags   = TextureAdaptor::Flags;
			return AFUNC_OK;
		}
		private:
		static double Evaluate (LWInstance instance, LWTextureAccess *ta)
		{
			try
			{
				T *plugin = (T *) instance;
				return plugin->Evaluate(ta);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in TextureHandler::Evaluate():", e.what());
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
				lwpp::LWMessage::Error("An exception occured in TextureHandler::Flags():", e.what());
				return 0;
			}
		}
	};

	//! Class for Procedural Textures with an XPanel
	//! @ingroup XPanelHandler
	IMPLEMENT_XPANELHANDLER(Texture);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_XPANELADAPTOR(Texture, LWPP_TEXTURE_VERSION, LWPP_TEXTURE_VERSION);
}

#endif //  LWPP_PROCEDURAL_TEXTURE_HANDLER