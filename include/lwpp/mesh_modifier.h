#pragma once

#ifndef LWPP_MESH_MODIFIER
#define  LWPP_MESH_MODIFIER

#include <lwpp/plugin_handler.h>
#include <lwpp/point3d.h>
#include <lwpp/vector3d.h>
#include <lwdisplce.h>
#include <lwmeshmodifier.h>
#include <lwmeshes.h>

namespace lwpp
{


	//! Base class for MeshDeformer plugins
	/*!
	 * @note Work in progress
	 * @ingroup Handler
	 */
	class MeshDeformerHandler : public InstanceHandler, public ItemHandler, public RenderHandler
	{
	protected:
		//! Stores the mID of the object wich the plugin is applied to
		LWItem Context;
	public:
		MeshDeformerHandler(void* g, void* context, LWError* err) : InstanceHandler(g, context, err, LWDISPLACEMENT_HCLASS)
		{
			//Context.SetID((LWItemID)context);
#if (LWSDKVER_MAJOR < 2020)
			auto id = static_cast<LWItemID*>(context);
			Context.SetID(static_cast<LWItemID>(*id));
#else
			auto ctx = static_cast<LWContext*>(context);
			Context.SetID((LWItemID)ctx->context.context);
#endif
		}

		virtual unsigned int    Flags    () 
		{
			return (0);
		}
		virtual unsigned int    Begin(LWMeshDeformerAccess*) { return 0; }
		virtual unsigned int    Evaluate(LWMeshDeformerAccess*) { return 0; }
		virtual void            End(LWMeshDeformerAccess*) { ; }
	};

	//! Wrapper for an DisplacementHandler
	/*!
	 * @ingroup Adaptor
	 */
	template <class T, int Version>
	class MeshDeformerAdaptor : public InstanceAdaptor <T>, public ItemAdaptor <T>, public RenderAdaptor <T>
	{
	public:
		MeshDeformerAdaptor(const char* name, ServerTagInfo* tags)
		{
			LWServer::AddPlugin(LWMESHDEFORMER_HCLASS, name, Activate, tags);
		}
		//! Set static callbacks for a LightWave ImageFilter
		static int Activate(int version, GlobalFunc* global, LWInstance inst, void* serverData)
		{
			if (version != Version) return AFUNC_BADVERSION;
			try
			{
				UNUSED(serverData);
				lwpp::SetSuperGlobal(global);

				auto plugin = static_cast<LWMeshDeformerHandler*>(inst);

				InstanceAdaptor<T>::Activate(plugin->inst);
				ItemAdaptor<T>::Activate(plugin->item);
				RenderAdaptor<T>::Activate(plugin->rend);
				plugin->evaluate = MeshDeformerAdaptor::Evaluate;
				plugin->flags = MeshDeformerAdaptor::Flags;
				plugin->begin = MeshDeformerAdaptor::Begin;
				plugin->end = MeshDeformerAdaptor::End;

				return AFUNC_OK;
			}
			catch (std::exception & e)
			{
                lwpp::LWMessage::Error(std::string("An exception occured in ") + __FUNCTION__, e.what());
				return AFUNC_BADAPP;
			}
		}
	private:

		static unsigned int Evaluate(LWInstance instance, LWMeshDeformerAccess* mda)
		{
			try
			{
				T* plugin = (T*)instance;
				return plugin->Evaluate(mda);
			}
			catch (std::exception & e)
			{
                lwpp::LWMessage::Error(std::string("An exception occured in ") + __FUNCTION__, e.what());
				return 0;
			}
		}

		static unsigned int Flags(LWInstance instance)
		{
			try
			{
				T* plugin = (T*)instance;
				return plugin->Flags();
			}
			catch (std::exception & e)
			{
				lwpp::LWMessage::Error(std::string("An exception occured in ") + __FUNCTION__, e.what());
				return 0;
			}
		}
		static unsigned int Begin(LWInstance instance, LWMeshDeformerAccess* mda)
		{
			try
			{
				T* plugin = (T*)instance;
				return plugin->Begin(mda);
			}
			catch (std::exception & e)
			{
				lwpp::LWMessage::Error(std::string("An exception occured in ") + __FUNCTION__, e.what());
				return 0;
			}
		}
		static void End(LWInstance instance, LWMeshDeformerAccess* mda)
		{
			try
			{
				T* plugin = (T*)instance;
				plugin->End(mda);
			}
			catch (std::exception & e)
			{
				lwpp::LWMessage::Error(std::string("An exception occured in ") + __FUNCTION__, e.what());
			}
		}
	};

	//! Class for Displacement Handler with an XPanel
	//! @ingroup XPanelHandler
	IMPLEMENT_XPANELHANDLER(MeshDeformer);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_XPANELADAPTOR(MeshDeformer, LWMESHDEFORMER_VERSION);

	//! @ingroup LWPanelHandler
	IMPLEMENT_LWPANELHANDLER(MeshDeformer);
	//! @ingroup LWPanelAdaptor
	IMPLEMENT_LWPANELADAPTOR(MeshDeformer, LWMESHDEFORMER_VERSION);

}
#endif
