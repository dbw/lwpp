#ifndef LWPP_SHADER_HANDLER
#define LWPP_SHADER_HANDLER
#include "lwpp/plugin_handler.h"
#include <lwshader.h>
#include <lwsurf.h>

namespace lwpp
{
//! Class for Shaders
/*!
 * @ingroup Handler
 * @note LWShaderAccess  is still the original struct and not replaced by the wrapper with a class
 */
class ShaderHandler : public InstanceHandler, public ItemHandler, public RenderHandler
{
	protected:
	  LWSurfaceID Context;
	public:
	  ShaderHandler(void *g, void *context, LWError *err) : InstanceHandler(g, context, err, LWSHADER_HCLASS)
		{
			Context = static_cast<LWSurfaceID>(context);
		}
		virtual ~ShaderHandler() {;}
		virtual void Evaluate(LWShaderAccess *sa) = 0;
	  virtual unsigned int Flags() {return 0;}
};

//! Wrapper for an ImageFilterHandler
/*!
 * @ingroup Adaptor
 */
	template <class T, int maxVersion, int minVersion> 
	class ShaderAdaptor : public InstanceAdaptor <T>, public ItemAdaptor <T>, public RenderAdaptor <T>
	{
		public:
		ShaderAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWSHADER_HCLASS, name, Activate, tags);
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
				LWShaderHandler *plugin = static_cast<LWShaderHandler *>(inst);

				InstanceAdaptor<T>::Activate(plugin->inst);
				ItemAdaptor<T>::Activate(plugin->item);
				RenderAdaptor<T>::Activate(plugin->rend);
				plugin->evaluate = ShaderAdaptor::Evaluate;
				plugin->flags   = ShaderAdaptor::Flags;
				return AFUNC_OK;
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in ShaderAdaptor::Activate():", e.what());
				return AFUNC_BADAPP;
			}
		}
		private:
		static void Evaluate (LWInstance instance, LWShaderAccess *sa)
		{
			try
			{
				T *plugin = (T *) instance;
				plugin->Evaluate(sa);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in ShaderHandler::Evaluate():", e.what());
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
				lwpp::LWMessage::Error("An exception occured in ShaderHandler::Evaluate():", e.what());
				return 0;
			}
		}
	};

	//! Class for Procedural Textures with an XPanel
	//! @ingroup XPanelHandler
	IMPLEMENT_XPANELHANDLER(Shader);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_XPANELADAPTOR(Shader, LWPP_SHADER_VERSION, LWPP_SHADER_VERSION);	
}

#endif //  LWPP_SHADER_HANDLER