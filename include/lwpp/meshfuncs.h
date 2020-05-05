#ifndef LWPP_MESHFUNCS_H
#define LWPP_MESHFUNCS_H

#include <lwpp/debug.h>
#include <lwpp/global.h>
#include <lwmeshes.h>

namespace lwpp
{
	class MeshPointScan;
	class MeshPolygonScan;

	class PointVisitor
	{
	public:
		static size_t func(void* userdata, LWMeshID mesh, LWPntID pnt)
		{
			auto v = static_cast<PointVisitor*>(userdata);
			return v->process(mesh, pnt);
		}
		virtual size_t process(LWMeshID mesh, LWPntID pnt) = 0;
	};

	class PolygonPointVisitor
	{
	public:
		static size_t func(void* userdata, LWMeshID mesh, LWPolID pol, LWPntID pnt)
		{
			auto v = static_cast<PolygonPointVisitor*>(userdata);
			return v->process(mesh, pol, pnt);
		}
		virtual size_t process(LWMeshID mesh, LWPolID pol, LWPntID pnt) = 0;
	};

	template <typename T>
	size_t MeshPntPolFunc(void* userdata, LWMeshID mesh, LWPntID pnt, LWPolID pol)
	{
		if (userdata)
		{
			auto host = static_cast<T*>(userdata);
			return host->MeshPntPol(mesh, pnt, pol);
		}
	}


	//! Wrapper for a LWMeshInfo
	//! @ingroup Globals
	class MeshFuncs
	{
		GlobalBase<LWMeshFuncs> mFuncs;
	protected:
		LWMeshID mesh;
		MeshFuncs(const MeshFuncs &from) : mesh(from.mesh){;}
		MeshFuncs& operator=(const MeshFuncs &from)
		{
      if (this != &from)
      {
			  mesh = from.mesh;
      }
			return *this;
		}
	public:
		//!
		/*!
		 * @param _mesh A LWMeshInfoID as passed on by LightWave
		 * @param _destroy	Do a mesh destroy when the class gets deleted. Only recommended for the class that directly receives the LWMeshInfo
		 * from LightWave.
		 */
		MeshFuncs(const LWMeshID _mesh = 0) : mesh(_mesh) {;}
		void setMeshID(const LWMeshID _mesh)
		{
			mesh = _mesh;
		}
		~MeshFuncs(void)
		{
			;
		}

		bool hasVertices()
		{
			return (mFuncs->have_vertices(mesh) != 0);
		}

		bool getPosition(LWPntID pnt, LWFVector pos)
		{
			return(mFuncs->position(mesh, pnt, pos) != 0);
		}

		size_t foreach(PointVisitor &pv)
		{
			return mFuncs->foreach_v(mesh, PointVisitor::func, &pv);
		}

		size_t foreach(LWPolID pol, PolygonPointVisitor& pv)
		{
			return mFuncs->foreach_pv(mesh, pol, PolygonPointVisitor::func, &pv);
		}

		template <typename T>
		size_t foreach_vp(T* host, LWPntID pnt)
		{
			return mFuncs->foreach_vp(mesh, pnt, &MeshPntPolFunc<T>, host);
		}

		bool hasPolygons()
		{
			return (mFuncs->have_polygons(mesh) != 0);
		}

	};

}



#endif //LWPP_MESHFUNCS_H