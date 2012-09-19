/*!
 * @file
 * @brief
 * @version $Id: volumetric_handler.h 89 2010-04-20 11:44:57Z mwolf $
 */

#ifndef VOLUMETRIC_HANDLER_H
#define VOLUMETRIC_HANDLER_H

#include <lwpp/plugin_handler.h>
#include <lwpp/Point3d.h>
#include <lwpp/Vector3d.h>
#include <lwvolume.h>
#include <lwmath.h>
#include <lwshader.h>

namespace lwpp
{

	class VolumetricAccess
	{
		protected:
			const LWVolumeAccess *access;
		public:
			VolumetricAccess(const LWVolumeAccess *acc) : access(acc) { ;}
			
			int getFlags() {return access->flags;}
			const LWItemID getSource() const {return access->source;}
			
			/*! Returns the origin of the ray, it is the position of the source item or of a spot
			* on the surface of the source object.
			* Due that it returns a Point3d you can get access to the individual axis like:
			*	VolumetricAccess va
			*	const double origin[3] = {va.getOrigin().x,va.getOrigin().y, va.getOrigin().z };
			*/
			const Point3d getOrigin( void ) const {return Point3d(access->o);}

			/*! Returns the direction of the ray.
			* Due that it returns a Point3d you can get access to the individual axis like:
			*	VolumetricAccess va
			*	const double rayDir[3] = {va.getDirection().x,va.getDirection().y, va.getDirection().z };
			*/
			const Vector3d getDirection() const {return Vector3d(access->dir);}


			const Vector3d getRayColor( void ) const {return Vector3d(access->rayColor);}
			const double getRayColorR( void ) const {return access->rayColor[0];}
			const double getRayColorG( void ) const {return access->rayColor[1];}
			const double getRayColorB( void ) const {return access->rayColor[2];}

			const double getFarClip() const {return access->farClip;}
			const double getNearClip() const {return access->nearClip;}
			const double getOriginDistance() const {return access->oDist;}
			const double getFrustrum() const {return access->frustum;}
			
			void addSample (LWVolumeSample *smp) {access->addSample(access->ray, smp);}
			const double getOpacity (double dist, double opa[3]) const {return access->getOpacity(access->ray, dist, opa);}
		
			const double getSubPixelX ( void ) const { return access->sx; }
			const double getSubPixelY ( void ) const { return access->sy; }
			const int getBounces (void) const { return access->bounces; }
			

			// CALLBACKS
			const int illuminate(LWItemID light, const LWDVector position, LWDVector direction, LWDVector color) const
			{ 
				return access->illuminate(light, position, direction, color);
			}

			const double rayTrace(const LWDVector position, const LWDVector direction, LWDVector color) const
			{
				return access->rayTrace(position, direction, color);
			}

			const double rayCast(const LWDVector position, const LWDVector direction) const
			{
				return access->rayCast(position, direction);
			}
			
			const double rayShade(const LWDVector position, const LWDVector direction, LWShaderAccess shaderaccess) const
			{
				return access->rayShade(position, direction, &shaderaccess);
			}

			const double rayTraceMode(const LWDVector position, const LWDVector direction, LWDVector color, const double eta, const int rtmode ) const
			{
				return access->rayTraceMode(position, direction, color, eta, rtmode);	
			}
#if (LW_SDK != 75)
			const double rayTraceReflection(const LWDVector position, const LWDVector direction, LWDVector color, const double eta ) const
			{
				return access->rayTraceMode(position, direction, color, eta, RTMODE_REFLECTION);	
			}

			const double rayTraceRefraction(const LWDVector position, const LWDVector direction, LWDVector color, const double eta ) const
			{
				return access->rayTraceMode(position, direction, color, eta, RTMODE_REFRACTION);	
			}

			const double rayTraceDistance(const LWDVector position, const LWDVector direction, LWDVector color, const double eta ) const
			{
				return access->rayTraceMode(position, direction, color, eta, RTMODE_DISTANCE);	
			}

			const double rayTraceDissolve(const LWDVector position, const LWDVector direction, LWDVector color, const double eta ) const
			{
				return access->rayTraceMode(position, direction, color, eta, RTMODE_DISSOLVE);	
			}

			const double rayTraceShadow(const LWDVector position, const LWDVector direction, LWDVector color, const double eta ) const
			{
				return access->rayTraceMode(position, direction, color, eta, RTMODE_SHADOW);	
			}

			const double rayTraceOcclusion(const LWDVector position, const LWDVector direction, LWDVector color, const double eta ) const
			{
				return access->rayTraceMode(position, direction, color, eta, RTMODE_OCCLUSION);	
			}

			const double rayTraceBackdrop(const LWDVector position, const LWDVector direction, LWDVector color, const double eta ) const
			{
				return access->rayTraceMode(position, direction, color, eta, RTMODE_BACKDROP);
			}
#endif // LW92
			const double illuminateSample( LWItemID light, const LWDVector position, LWDVector direction, LightSampleFunc sampler, void *data ) const 
			{
				return access->illuminateSample(light, position, direction, &sampler, &data);
			}
	};

	//! Class for Volumetric plugins
	/*!
	 * @ingroup Handler
	 * @note Work in progress
	 * note LWVolumeAccess has been replaced by the wrapper with the class VolumetricAccess
	 */
	class VolumetricHandler : public InstanceHandler, public ItemHandler, public RenderHandler
	{
		protected:
			LWItemID Context;
		public:
			VolumetricHandler(void *g, void *context, LWError *err) : InstanceHandler(g, context, err, LWVOLUMETRIC_HCLASS)
			{
				Context = LWItemID(context);
			}
			virtual ~VolumetricHandler() {;}
			virtual double Evaluate(VolumetricAccess &voa)
			{
				UNUSED(voa);
				return 1.0;
			}
			virtual unsigned int Flags() {return 0;}
	};

//! Wrapper for an VolumetricHandler
/*!
 * @ingroup Adaptor
 */
	template <class T, int min, int max> 
	class VolumetricAdaptor : public InstanceAdaptor <T>, public ItemAdaptor <T>, public RenderAdaptor <T>
	{
		public:
		VolumetricAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWVOLUMETRIC_HCLASS, name, Activate, tags);
		}
		//! Set static callbacks for a LightWave Volumetric
		static int Activate (int version, GlobalFunc *global, LWInstance inst, void *serverData)
		{
			UNUSED(serverData);
			
			if ( version != LWVOLUMETRIC_VERSION ) return AFUNC_BADVERSION;
			try
			{
				lwpp::SetSuperGlobal(global);
				LWVolumetricHandler *plugin = static_cast<LWVolumetricHandler *>(inst);

				InstanceAdaptor<T>::Activate(plugin->inst);
				ItemAdaptor<T>::Activate(plugin->item);
				RenderAdaptor<T>::Activate(plugin->rend);
				plugin->evaluate = VolumetricAdaptor::Evaluate;
				plugin->flags   = VolumetricAdaptor::Flags;

				return AFUNC_OK;
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in Volumetric::Activate():", e.what());
				return AFUNC_BADAPP;
			}
		}
		private:

		static double Evaluate (LWInstance instance, LWVolumeAccess *lw_voa)
		{
			try
			{
				T *plugin = (T *) instance;
				return plugin->Evaluate(VolumetricAccess(lw_voa));
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in Volumetric::Evaluate():", e.what());
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
				lwpp::LWMessage::Error("An exception occured in Volumetric::Flags():", e.what());
				return AFUNC_BADAPP;
			}
		}
	};

	//! Class for Volumetric Handler with an XPanel
	//! @ingroup XPanelHandler
	IMPLEMENT_XPANELHANDLER(Volumetric);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_XPANELADAPTOR(Volumetric, min, max);
	//! Class for Volumetric Handler with an LWPanels
	/*!
	 * @ingroup LWPanelHandler
	 */
	IMPLEMENT_LWPANELHANDLER(Volumetric);
	//! Adaptor class for LWPanelVolumetricHandler
	/*!
	 * @ingroup Adaptor
	 */
	IMPLEMENT_LWPANELADAPTOR(Volumetric, min, max);

} // end namespace

#endif // VOLUMETRIC_HANDLER_H