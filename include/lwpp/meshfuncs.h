#ifndef LWPP_MESHFUNCS_H
#define LWPP_MESHFUNCS_H

#include <lwpp/debug.h>
#include <lwpp/global.h>
#include <lwmeshes.h>
#include <lwpp/objectinfo.h>

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

	template <typename T>
	size_t DeformableMeshPntFunc(void *userdata, LWDeformableMeshID mesh, LWPntID pnt)
	{
		T plugin = static_cast<T>(userdata);
		return plugin->DeformableMeshPnt(mesh, pnt);
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

		int getCapability(unsigned int capability, void *val = nullptr)
		{
			return mFuncs->capability(mesh, capability, val);
		}

		bool hasVertices()
		{
			return (mFuncs->have_vertices(mesh) != 0);
		}

		unsigned int getVertexCount()
		{
			return mFuncs->num_vertices(mesh);
		}

		bool getPosition(LWPntID pnt, LWFVector pos) const
		{
			return(mFuncs->position(mesh, pnt, pos) != 0);
		}

		lwpp::Vector3f getPosition(LWPntID pnt) const
		{
			LWFVector pos;
			if (getPosition(pnt, pos)) return lwpp::Vector3f(pos);
			return lwpp::Vector3f();
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

		LWVMapID vMapLookup(LWID vmap_type, const char * vmap_name)
		{
			return mFuncs->vmap_lookup(mesh, vmap_type, vmap_name);
		}
		LWVMapID vMapLookup(const VMapCallbacks &vmp)
		{
			return mFuncs->vmap_lookup(mesh, vmp.getType(), vmp.getName());
		}
		int vMapDimention(LWVMapID vmap)
		{
			return mFuncs->vmap_dimension(mesh, vmap);
		}
		int vMapGet(LWVMapID vmap, LWPntID pnt, LWPolID pol, float *value) const
		{
			return mFuncs->vmap_get(mesh, vmap, pnt, pol, value);			
		}

		int vMapGet(LWVMapID vmap, LWPntID pnt, float *value) const
		{
			return mFuncs->vmap_get(mesh, vmap, pnt, nullptr, value);
		}

		int vMapGetPosition(LWVMapID vmap, LWPntID pnt, lwpp::Vector3f &pos) const
		{
			if (vmap)
				return vMapGet(vmap, pnt, pos.asLWVector());
			else
				return getPosition(pnt, pos.asLWVector());
		}
		int vMapGetRelPosition(LWVMapID vmap, LWPntID pnt, lwpp::Vector3f &pos) const
		{
			if (vmap)
				return vMapGet(vmap, pnt, pos.asLWVector());
			else
				return 1;
		}

		int setPosition(LWMeshDeformID dmesh, LWPntID pnt, const LWFVector pos)
		{
			return mFuncs->set_position(dmesh, pnt, pos);
		}

		template<typename T>
		size_t foreach_v_deform(LWMeshDeformID mesh, void *host)
		{
			auto f = &lwpp::DeformableMeshPntFunc <T *>;
			return mFuncs->foreach_v_deform(mesh, f, host);
		}

	};
}

#endif //LWPP_MESHFUNCS_H
