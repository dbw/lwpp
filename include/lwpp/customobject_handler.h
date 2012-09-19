#ifndef CUSTOMOBJECT_HANDLER_H
#define CUSTOMOBJECT_HANDLER_H

#include <lwpp/plugin_handler.h>
#include <lwcustobj.h>

namespace lwpp
{

	class CustomObjAccess
	{
	protected:
		const LWCustomObjAccess *access;
	public:
		CustomObjAccess(const LWCustomObjAccess *acc) : access(acc) {;}
		int View() {return access->view;}
		int Flags() {return access->flags;}
		bool isSelected() {return (Flags() & LWCOFL_SELECTED) != 0;}
		bool isPicking() {return (Flags() & LWCOFL_PICKING) != 0;}
		void SetColor (float rgba[4]) {access->setColor(access->dispData, rgba);}
		void SetColor (float r, float g, float b, float a) 
		{
			float rgba[4] = {r, g, b, a};
			SetColor(rgba);
		}
		void SetPattern (int lpat) {access->setPattern(access->dispData, lpat);}
		void SetTexture (int size, unsigned char *image) {access->setTexture(access->dispData, size, image);}
		void SetUVs (double uv0[2], double uv1[2], double uv2[2], double uv3[2]) {access->setUVs(access->dispData, uv0, uv1, uv2, uv3);}
		void SetUVs (double uv[4][2]) {access->setUVs(access->dispData, uv[0], uv[1], uv[2], uv[3]);}
		void DrawPoint (double pos[3], int csys) {access->point(access->dispData, pos, csys);}
		void DrawLine (double from[3], double to[3], int csys) {access->line(access->dispData, from, to, csys);}
		void DrawTriangle (double v1[3], double v2[3], double v3[3], int csys) {access->triangle(access->dispData, v1, v2, v3, csys);}
		void DrawQuad (double v1[3], double v2[3], double v3[3], double v4[3], int csys) {access->quad(access->dispData, v1, v2, v3, v4, csys);}
		void DrawQuad (double v[4][3], int csys) {access->quad(access->dispData, v[0], v[1], v[2], v[3], csys);}
		void DrawCircle (double center[3], double radius, int csys) {access->circle(access->dispData, center, radius, csys);}
		void DrawText (double center[3], const char *text, int just, int csys) {access->text(access->dispData, center, text, just, csys);}

#if LW_SDK >= 93
		Point3d GetViewPos() {return Point3d(access->viewPos);}
		Vector3d GetViewDir() {return Vector3d(access->viewDir);}
		void	SetCoordinateSystem(LWItemID item) {access->setCSysItem(access->dispData, item);}
#endif
#if LW_SDK >= 95
		void DrawPolygon(unsigned int numv, double v[][3], int csys)
		{
			access->polygon(access->dispData, numv, v, csys);
		}
		void DrawPolygon(unsigned int numv, unsigned int verts[], double v[][3], int csys)
		{
			access->polyIndexed(access->dispData, numv, verts, v, csys);
		}
		void SetDrawMode(unsigned int mode) {access->setDrawMode(access->dispData, mode);}
		void DrawDisk(double p[3], double r, int csys)
		{
			access->disk(access->dispData, p, r, csys);
		}
#endif

#if LW_SDK >= 96
		// Todo, add part number
#endif

	};

	//! Base class for CustomObject plugins
	/*!
	* @note Work in progress
	* @ingroup Handler
	*/
	class CustomObjHandler : public InstanceHandler, public ItemHandler, public RenderHandler
	{
	protected:
		//! Stores the id of the object wich the plugin is applied to
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
	template <class T, int maxVersion, int minVersion>
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
			if ( version > maxVersion ) return AFUNC_BADVERSION;
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
	IMPLEMENT_XPANELADAPTOR(CustomObj, LWPP_CUSTOMOBJECT_VERSION, LWPP_CUSTOMOBJECT_VERSION );

	//! @ingroup LWPanelHandler
	IMPLEMENT_LWPANELHANDLER(CustomObj);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_LWPANELADAPTOR(CustomObj, LWPP_CUSTOMOBJECT_VERSION, LWPP_CUSTOMOBJECT_VERSION );

} // end namespace

#endif // CUSTOMOBJECT_HANDLER_H