#ifndef LWPP_MESHINFO_H
#define LWPP_MESHINFO_H

#include <lwpp/debug.h>
#include <lwmeshes.h>

namespace lwpp
{
	class MeshPointScan;
	class MeshPolygonScan;
	//! Wrapper for a LWMeshInfo
	//! @ingroup Globals
	class MeshInfo
	{
	protected:
		LWMeshInfoID mesh;
		const bool destroy;
		MeshInfo(const MeshInfo &from) : mesh(from.mesh), destroy(false) {;}
		MeshInfo& operator=(const MeshInfo &from)
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
		MeshInfo(const LWMeshInfoID _mesh = 0, bool _destroy = false) : mesh(_mesh), destroy(_destroy) {;}
		void setMeshInfo(const LWMeshInfoID _mesh)
		{
			mesh = _mesh;
		}
		~MeshInfo(void)
		{
			if ((mesh) && (destroy) && (mesh->destroy != 0)) mesh->destroy(mesh);
		}
		int numPoints(void) const {return mesh->numPoints(mesh);}
		int numPolygons(void) const {return mesh->numPolygons(mesh);}
		size_t scanPoints(LWPntScanFunc *func, void *userData) const {return mesh->scanPoints(mesh, func, userData);}
		size_t scanPolys(LWPolScanFunc *func, void *userData) const {return mesh->scanPolys(mesh, func, userData);}
		void pntBasePos(LWPntID pnt, LWFVector pos) const {mesh->pntBasePos(mesh, pnt, pos);}
		void pntBasePos(LWPntID pnt, LWDVector pos) const
		{
			LWFVector tPos;
			mesh->pntBasePos(mesh, pnt, tPos);
			pos[0] = (double)tPos[0];
			pos[1] = (double)tPos[1];
			pos[2] = (double)tPos[2];
		}
		void pntOtherPos(LWPntID pnt, LWFVector pos) const {mesh->pntOtherPos(mesh, pnt, pos);}
		void pntOtherPos(LWPntID pnt, LWDVector pos) const {
			LWFVector tPos;
			mesh->pntOtherPos(mesh, pnt, tPos);
			pos[0] = (double)tPos[0];
			pos[1] = (double)tPos[1];
			pos[2] = (double)tPos[2];
		}
		void *pntVLookup(LWID id, const char*name) const {return mesh->pntVLookup(mesh, id, name);}
		void *pntVLookup(LWID id, const std::string &name) const {return pntVLookup(id, name.c_str());}
		int pntVSelect(void *vmap) const {return mesh->pntVSelect(mesh, vmap);}
		bool pntVGet(LWPntID pnt, float *vector) const {return (mesh->pntVGet(mesh, pnt, vector) != 0);}
		LWID polType(LWPolID id) const {return mesh->polType(mesh, id);}
		int polSize(LWPolID id) const {return mesh->polSize(mesh, id);}
		LWPntID polVertex(LWPolID id, int n) const {return mesh->polVertex(mesh, id, n);}
		inline const char *polTag(LWPolID id, LWID tag) const {return mesh->polTag(mesh, id, tag);}
		bool pntVPGet(LWPntID pnt, LWPolID pol, float *vector) const {return (mesh->pntVPGet(mesh, pnt, pol, vector) != 0);}
		unsigned int polFlags(LWPolID pol) const {return mesh->polFlags(mesh, pol);}
		int pntVIDGet(LWPntID pnt, float *vector, void *vmap) const {return mesh->pntVIDGet(mesh, pnt, vector, vmap);}
		int pntVPIDGet(LWPntID pnt, LWPolID pol, float *vector, void *vmap) const {return mesh->pntVPIDGet(mesh, pnt, pol, vector, vmap);}
		bool polBaseNormal(LWPolID polID, LWFVector out){ return (mesh->polBaseNormal(mesh, polID, out) != 0);}
		bool polOtherNormal(LWPolID polID, LWFVector out){ return (mesh->polOtherNormal(mesh, polID, out) != 0);}
		LWMeshIteratorID createMeshIterator(int type){ return (mesh->createMeshIterator(mesh, type)); }
		void destroyMeshIterator(LWMeshIteratorID mid){ mesh->destroyMeshIterator(mesh, mid); }
		LWPolID iterateMesh(LWMeshIteratorID mid){ return (LWPolID)(mesh->iterateMesh(mesh, mid)); }
		LWPntID iterateMeshPnt(LWMeshIteratorID mid){ return (LWPntID)(mesh->iterateMesh(mesh, mid)); }
		void resetMeshIterator(LWMeshIteratorID mid){ mesh->resetMeshIterator(mesh, mid); }

		size_t scanPolys(MeshPolygonScan *scanner);
		bool isValid(void) const {return (mesh != 0);}
	};

	//! Base Class for Point Scanners
	/*!
	 * This can only be used as a base class for custom point scanners
	 * @note This might not work at all and might need ot be replaced by a template
	 * @sa MeshInfo
	 */
	//! @ingroup Entities
	class MeshPointScan
	{
	protected:
		MeshInfo *mi;
		/*!
		 * return 0 to stop the scan, anything else to continue
		 */
		virtual int scanFunc(LWPntID id) = 0;
	public:
		MeshPointScan (MeshInfo *_mi) : mi(_mi) {;}
		static int LWPntScanFunc (void *userData, LWPntID id)
		{
			MeshPointScan *mps = static_cast<MeshPointScan *>(userData);
			return mps->scanFunc(id);
		}
	};

	//! Base Class for Polygon Scanners
	/*!
	 * This can only be used as a base class for custom polygon scanners
	 * @note This might not work at all and might need ot be replaced by a template
	 * @sa MeshInfo
	 */
	//! @ingroup Entities
	class MeshPolygonScan
	{
	protected:
		MeshInfo *mi;
		LWPolID polygon;
		//! Evaluation function, to be overriden by the implementing class
		/*
		 * @return true to abort the scan
		 */
		virtual bool evaluate() = 0;
		//! Wrapper to prepare the virtual evaluate member function
		/*!
		 * Stores the polygon ID in the designated member variable and calls evaluate.
		 * @param mID LWPolID of the currenty polygon, stored in the polygon member variable
		 * @return true to abort the scan
		 */
		bool scanFunc(LWPolID id) {polygon = id; return evaluate();}
		//! Helper function, retrive the type of polygon
		LWID polType(void) const {return mi->polType(polygon);}
		int polSize(void) const {return mi->polSize(polygon);}
		LWPntID polVertex(int n) const {return mi->polVertex(polygon,n);}
		inline const char*polTag(LWID tag) const {return mi->polTag(polygon, tag);}
		int pntVPGet(LWPntID pnt, float *vector) const {return mi->pntVPGet(pnt, polygon, vector);}
		unsigned int polFlags(void) const {return mi->polFlags(polygon);}
		int pntVPIDGet(LWPntID pnt, float *vector, void *vmap) const {return mi->pntVPIDGet(pnt, polygon, vector, vmap);}

	public:
		virtual ~MeshPolygonScan() {;}
		void setMeshInfo(MeshInfo *_mi) {mi = _mi;}
		//! Callback function for LightWave
		static size_t LWPolyScanFunc (void *userData, LWPolID id)
		{
			MeshPolygonScan *mps = static_cast<MeshPolygonScan *>(userData);
			return mps->scanFunc(id);
		}
	};
}

#endif //LWPP_MESHINFO_H