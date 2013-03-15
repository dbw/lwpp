#ifndef DISPLACEMENT_HANDLER_H
#define DISPLACEMENT_HANDLER_H

#include <lwpp/plugin_handler.h>
#include <lwpp/point3d.h>
#include <lwpp/vector3d.h>
#include <lwdisplce.h>
#include <lwmeshes.h>

namespace lwpp
{
	/*
	 *  The DisplacementAccess passed to the evaluation function contains two point positions, 
	 *	the point ID, and a mesh info for the object the point belongs to.
	*/
	class DisplacementAccess
	{
		protected:
			LWDisplacementAccess *access;
		public:
			DisplacementAccess(LWDisplacementAccess *acc) : access(acc) { ;}
			/*!
			*	    Returns the original point location in object coordinates. This is read-only.
			*/
			const Point3d getPointPosition( void ) const { return Point3d(access->oPos); }
			/*!
			*	Returns the location to be transformed in place by the displacement. 
			*	If the flags function returned the LWDMF_WORLD bit, the source is in world coordinates 
			*	and has already been modified by morphing, bones and object motion. 
			*	Otherwise the source is in object coordinates (after morphing, before item motion, 
			*	and before or after bone effects, depending on whether the flags function returned LWDMF_BEFOREBONES).
			*/
			const Vector3d getSource( void ) const { return Vector3d(access->source); }
			/*!
			*	Returns the point ID. This can be used to retrieve other information about the point from the mesh info structure.
			*/
			const LWPntID getPointID( void ) const {return access->point;}
			/*!
			*	Returns a mesh info structure for the object.
			*/
			const LWMeshInfo *getMeshInfo( void ) const { return access->info; }
			/*!
			*	The average normal of the polygon normals of the polygons surrounding the currently evaluated vertex.
			*	This member will only be filled with valid data when the LWDMF_NEED_NORMALS flag has been specified.
			*/
			const Vector3d getAvgNormal( void ) const {return Vector3d(access->wNorm);}

			/*!
			*	The average normal of the polygon normals of the polygons surrounding the currently evaluated vertex.
			*/
			void setAvgNormal(Vector3d &normal)
			{
				access->wNorm[0] = normal.x;
				access->wNorm[1] = normal.y;
				access->wNorm[2] = normal.z;
			
			}
			/*!
			*	Set the source with a Point3d to apply the displacement
			*/
			void setSource(Point3d &sspt)
			{
				access->source[0] = sspt.x;
				access->source[1] = sspt.y;
				access->source[2] = sspt.z;		
			}
			/*!
			*	Set the source with a Point3f to apply the displacement
			*/
			void setSource(Point3f &sspt)
			{
				access->source[0] = (double) sspt.x;
				access->source[1] = (double) sspt.y;
				access->source[2] = (double) sspt.z;		
			}

	
	};

//! Base class for Displacement plugins
/*!
 * @note Work in progress
 * @ingroup Handler
 */
class DisplacementHandler : public InstanceHandler, public ItemHandler, public RenderHandler
{
	protected:
		//! Stores the id of the object wich the plugin is applied to
		LWItem Context;
	public:
	  DisplacementHandler(void *g, void *context, LWError *err) : InstanceHandler(g, context, err, LWDISPLACEMENT_HCLASS )
		{
			Context.SetID( (LWItemID) context);
		}
		virtual ~DisplacementHandler() {;}
		virtual void Evaluate(DisplacementAccess &da)
		{
			;
		}
		virtual unsigned int Flags() {return 0;}
};

//! Wrapper for an DisplacementHandler
/*!
 * @ingroup Adaptor
 */
template <class T, int maxVersion, int minVersion>
class DisplacementAdaptor : public InstanceAdaptor <T>, public ItemAdaptor <T>, public RenderAdaptor <T>
{
	public:
		DisplacementAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWDISPLACEMENT_HCLASS, name, Activate, tags);
		}
		//! Set static callbacks for a LightWave ImageFilter
		static int Activate (int version, GlobalFunc *global, LWInstance inst, void *serverData)
		{
			if ( version > maxVersion ) return AFUNC_BADVERSION;
			if ( version < minVersion ) return AFUNC_BADVERSION;
			try
			{
				UNUSED(serverData);
				lwpp::SetSuperGlobal(global);

				LWDisplacementHandler *plugin = static_cast<LWDisplacementHandler *>(inst);

				InstanceAdaptor<T>::Activate(plugin->inst);
				ItemAdaptor<T>::Activate(plugin->item);
				RenderAdaptor<T>::Activate(plugin->rend);
				plugin->evaluate = DisplacementAdaptor::Evaluate;
				plugin->flags   = DisplacementAdaptor::Flags;
				
				return AFUNC_OK;
			} 
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in Displacement::Activate():", e.what());
				return AFUNC_BADAPP;
			}
		}
	private:

		static void Evaluate (LWInstance instance, LWDisplacementAccess *da)
		{
			try
			{
				T *plugin = (T *) instance;
        DisplacementAccess dAcc(da); // required due to C++ standard
				plugin->Evaluate(dAcc);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in Displacement::Evaluate():", e.what());
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
				lwpp::LWMessage::Error("An exception occured in Displacement::Flags():", e.what());
				return 0;
			}
		}
};

//! Class for Displacement Handler with an XPanel
//! @ingroup XPanelHandler
IMPLEMENT_XPANELHANDLER(Displacement);
//! @ingroup XPanelAdaptor
IMPLEMENT_XPANELADAPTOR(Displacement, LWPP_DISPLACEMENT_VERSION, LWPP_DISPLACEMENT_VERSION );

//! @ingroup LWPanelHandler
IMPLEMENT_LWPANELHANDLER(Displacement);
//! @ingroup LWPanelAdaptor
IMPLEMENT_LWPANELADAPTOR(Displacement, LWPP_DISPLACEMENT_VERSION, LWPP_DISPLACEMENT_VERSION );

} // end namespace

#endif // DISPLACEMENT_HANDLER_H