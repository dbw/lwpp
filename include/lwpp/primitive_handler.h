#ifndef PRIMITIVE_HANDLER_H
#define PRIMITIVE_HANDLER_H

#include <lwpp/plugin_handler.h>
#include <lwpp/customobject_access.h>
#include <lwprimitive.h>

namespace lwpp
{	
	//! Base class for Primitive plugins
	/*!
	* @ingroup Handler
	*/
	class PrimitiveHandler : public InstanceHandler, public ItemHandler, public RenderHandler
	{
	protected:
		//! Stores the mID of the object wich the plugin is applied to
		LWItem Context;
	public:
		PrimitiveHandler(void *g, void *context, LWError *err) : InstanceHandler(g, context, err, LWCUSTOMOBJ_HCLASS)
		{
#if (LWSDKVER_MAJOR < 2020)
			auto id = static_cast<LWItemID *>(context);
			Context.SetID(static_cast<LWItemID>(*id));
#else
			auto ctx = static_cast<LWContext*>(context);
			Context.SetID((LWItemID)ctx->context.context);
#endif
		}
		virtual ~PrimitiveHandler() {;}
		virtual unsigned int Flags() { return 0; }
		virtual LWError NewFrame(const LWFrameInfo* frameinfo) { return nullptr; }
		virtual int Bounds(LWPrimitiveType, LWDVector min, LWDVector max, LWPrimitiveCoordinateSys* coordSys) { return 0; }

		// Solids
		virtual size_t MemorySize() { return 0; }
		virtual int NearestPoint( const LWPrimitiveInstance* pinst, const LWRay* ray, LWShadingGeometry* is) { return 0; }
		virtual int Intersect( const LWPrimitiveInstance* pinst, const LWRay* ray, LWShadingGeometry* is) { return 0; }
		virtual double EdgePixelRadius() { return 0.0; }
		virtual double EdgeWorldRadius() { return 0.0; }
		virtual LWSurfaceID Surface( const LWShadingGeometry* is) { return nullptr; }
		virtual double Area( const LWPrimitiveInstance* pinst, LWTime fracTime) { return 0.0; }
		virtual int Sample( const LWPrimitiveInstance* pinst, LWTime fracTime, const LWDVector randomSample, LWDVector p, LWDVector n) { return 0; }
		virtual double Pdf( const LWPrimitiveInstance* pinst, const LWRay* ray, const LWShadingGeometry* is) { return 0.0; }
		virtual void Opacity(const LWPrimitiveInstance* pinst, const LWRay* ray, const LWShadingGeometry* is, LWDVector opa) { ; }

		// Volumes
		virtual int VolumeSegment( const LWPrimitiveInstance* pinst, const LWVolumeSpot*, double tMin, double tMax, LWVolumeSegmentFunc* segfunc) { return 0; }
		virtual int VolumeExtinction( const LWPrimitiveInstance* pinst, const LWVolumeSpot*, double length, double u, LWDVector extinction) { return 0; }
		virtual int VolumeSample( const LWPrimitiveInstance* pinst, const LWVolumeSpot*, LWDVector emission, LWDVector scattering, LWDVector absorption) { return 0; }
		virtual double VolumePhase( const LWPrimitiveInstance* pinst, const LWVolumeSpot*, const LWDVector wi, const LWDVector wo) { return 0.0; }
		virtual double VolumeSamplePhase(const LWPrimitiveInstance* pinst, const LWVolumeSpot*, const LWDVector sample, LWDVector wo) { return 0.0; }
	};

	//! Wrapper for a Primitive
	/*!
	* @ingroup Adaptor
	*/
	template <class T, int minVersion>
	class PrimitiveAdaptor : public InstanceAdaptor <T>, public ItemAdaptor <T>, public RenderAdaptor <T>
	{
	public:
		PrimitiveAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWPRIMITIVE_HCLASS, name, Activate, tags);
		}
		//! Set static callbacks for a LightWave CustomObject
		static int Activate (int version, GlobalFunc *global, LWInstance inst, void *serverData)
		{
			if ( version < minVersion ) return AFUNC_BADVERSION;
			try
			{
				UNUSED(serverData);
				lwpp::SetSuperGlobal(global);

				LWPrimitiveHandler *plugin = static_cast<LWPrimitiveHandler *>(inst);

				InstanceAdaptor<T>::Activate(plugin->inst);
				ItemAdaptor<T>::Activate(plugin->item);
				RenderAdaptor<T>::Activate(plugin->rend);
				plugin->flags   = PrimitiveAdaptor::Flags;
				plugin->newFrame = PrimitiveAdaptor::NewFrame;
				plugin->bounds = PrimitiveAdaptor::Bounds;

				return AFUNC_OK;
			} 
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in CustomObject::Activate():", e.what());
				return AFUNC_BADAPP;
			}
		}
	private:
		static unsigned int Flags(LWInstance inst)
		{
			try
			{
				T *plugin = (T *)inst;
				return plugin->Flags();
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::Flags():", e.what());
			}
			return 0;
		}
		static LWError NewFrame(LWInstance inst, const LWFrameInfo* frameinfo)
		{			
			try
			{
				T *plugin = (T *)inst;
				return plugin->NewFrame(frameinfo);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::NewFrame():", e.what());
			}
			return nullptr;
		}
		static int Bounds(LWInstance inst, LWPrimitiveType type, LWDVector min, LWDVector max, LWPrimitiveCoordinateSys* coordSys)
		{
			try
			{
				T *plugin = (T *)inst;
				return plugin->Bounds(type, min, max, coordSys);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::Bounds():", e.what());
			}
			return 0;
		}

		// Solids
		static size_t MemorySize(LWInstance inst)
		{
			try
			{
				T *plugin = (T *)inst;
				return plugin->MemorySize();
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::MemorySize():", e.what());
			}
			return 0;
		}
		static int NearestPoint(LWInstance inst, const LWPrimitiveInstance* pinst, const LWRay* ray, LWShadingGeometry* is)
		{
			try
			{
				T *plugin = (T *)inst;
				return plugin->NearestPoint(pinst, ray, is);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::NearestPoint():", e.what());
			}
			return 0;
		}
		static int Intersect(LWInstance inst, const LWPrimitiveInstance* pinst, const LWRay* ray, LWShadingGeometry* is)
		{
			try
			{
				T *plugin = (T *)inst;
				return plugin->Intersect(pinst, ray, is);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::Intersect():", e.what());
			}
			return 0;
		}
		static double EdgePixelRadius(LWInstance inst)
		{
			try
			{
				T *plugin = (T *)inst;
				return plugin->EdgePixelRadius();
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::EdgePixelRadius():", e.what());
			}
			return 0.0;
		}
		static double EdgeWorldRadius(LWInstance inst)
		{
			try
			{
				T *plugin = (T *)inst;
				return plugin->EdgeWorldRadius();
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::EdgeWorldRadius():", e.what());
			}
			return 0.0;
		}
		static LWSurfaceID Surface(LWInstance inst, const LWShadingGeometry* is)
		{
			try
			{
				T *plugin = (T *)inst;
				return plugin->Surface(is);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::Surface():", e.what());
			}
			return nullptr;
		}
		static double Area(LWInstance inst, const LWPrimitiveInstance* pinst, LWTime fracTime)
		{
			try
			{
				T *plugin = (T *)inst;
				return plugin->Area(pinst, fracTime);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::Area():", e.what());
			}
			return 0.0;
		}
		static int Sample(LWInstance inst, const LWPrimitiveInstance* pinst, LWTime fracTime, const LWDVector randomSample, LWDVector p, LWDVector n)
		{
			try
			{
				T *plugin = (T *)inst;
				return plugin->Sample(pinst, fracTime, randomSample, p, n);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::Sample():", e.what());
			}
			return 0;
		}
		static double Pdf(LWInstance inst, const LWPrimitiveInstance* pinst, const LWRay* ray, const LWShadingGeometry* is)
		{
			try
			{
				T *plugin = (T *)inst;
				return plugin->Pdf(pinst, ray, is);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::Pdf():", e.what());
			}
			return 0.0;
		}
		static void Opacity(LWInstance inst, const LWPrimitiveInstance* pinst, const LWRay* ray, const LWShadingGeometry* is, LWDVector opa)
		{
			try
			{
				T *plugin = (T *)inst;
				plugin->Opacity(pinst, ray, is, opa);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::Opacity():", e.what());
			}
		}

		// Volumes
		static int VolumeSegment(LWInstance inst, const LWPrimitiveInstance* pinst, const LWVolumeSpot* vSpot, double tMin, double tMax, LWVolumeSegmentFunc* segfunc)
		{
			try
			{
				T *plugin = (T *)inst;
				return plugin->VolumeSegment(pinst, vSpot, tMin, tMax, segfunc);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::VolumeSegment():", e.what());
			}
			return 0;
		}
		static int VolumeExtinction(LWInstance inst, const LWPrimitiveInstance* pinst, const LWVolumeSpot* vSpot, double length, double u, LWDVector extinction)
		{
			try
			{
				T *plugin = (T *)inst;
				return plugin->VolumeExtinction(pinst, vSpot, length, u, extinction);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::VolumeExtinction():", e.what());
			}
			return 0;
		}
		static int VolumeSample(LWInstance inst, const LWPrimitiveInstance* pinst, const LWVolumeSpot* vSpot, LWDVector emission, LWDVector scattering, LWDVector absorption)
		{
			try
			{
				T *plugin = (T *)inst;
				return plugin->VolumeSample(pinst, vSpot, emission, scattering, absorption);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::VolumeSample():", e.what());
			}
			return 0;
		}
		static double VolumePhase(LWInstance inst, const LWPrimitiveInstance* pinst, const LWVolumeSpot* vSpot, const LWDVector wi, const LWDVector wo)
		{
			try
			{
				T *plugin = (T *)inst;
				return plugin->VolumePhase(pinst, vSpot, wi, wo);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::VolumePhase():", e.what());
			}
			return 0.0;
		}
		static double VolumeSamplePhase(LWInstance inst, const LWPrimitiveInstance* pinst, const LWVolumeSpot* vSpot, const LWDVector sample, LWDVector wo)
		{
			try
			{
				T *plugin = (T *)inst;
				return plugin->VolumeSamplePhase(pinst, vSpot, sample, wo);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PrimitiveHandler::VolumeSamplePhase():", e.what());
			}
			return 0.0;
		}
	};

	//! Class for Primitive Handler with an XPanel
	//! @ingroup XPanelHandler
	IMPLEMENT_XPANELHANDLER(Primitive);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_XPANELADAPTOR(Primitive, LWPRIMITIVE_VERSION);

	//! @ingroup LWPanelHandler
	IMPLEMENT_LWPANELHANDLER(Primitive);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_LWPANELADAPTOR(Primitive, LWPRIMITIVE_VERSION);

	IMPLEMENT_GIZMO_XPANELHANDLER(Primitive);
	IMPLEMENT_GIZMO_XPANELADAPTOR(Primitive, LWPRIMITIVE_VERSION);
	IMPLEMENT_GIZMO_LWPANELHANDLER(Primitive);
	IMPLEMENT_GIZMO_LWPANELADAPTOR(Primitive, LWPRIMITIVE_VERSION);

} // end namespace

#endif // PRIMITIVE_HANDLER_H
