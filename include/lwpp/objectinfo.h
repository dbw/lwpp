#ifndef LWPP_OBJECTINFO_H
#define LWPP_OBJECTINFO_H

#include <lwpp/wrapper.h>
#include <lwsurf.h>
#include <set>

namespace lwpp
{

	//! Wrapper for LWObjectInfo
	//! @ingroup Globals
	class ObjectInfo  : protected GlobalBase<LWObjectInfo>
	{
		LWItemID item;
	public:
		//! Constructor
		/*!
		 * @param _item the LWItemID of the item to retrieve information from.
		 */
		ObjectInfo(LWItemID _item = 0)
			: item (_item)
		{
			;
		}
		void setObject(LWItemID _item)
		{
			item = _item;
		}
		const char *filename() const {return globPtr->filename(item);}
		int        numPoints() const {return globPtr->numPoints(item);}
		int        numPolygons() const {return globPtr->numPolygons(item);}
		unsigned int shadowOpts() const {return globPtr->shadowOpts(item);}
		double	dissolve(LWTime time) const {return globPtr->dissolve(item, time);}
		LWMeshInfoID meshInfo(bool frozen = false) const   {return globPtr->meshInfo(item, frozen);}
		unsigned int flags() const {return globPtr->flags(item);}
		double fog(LWTime time) const {return globPtr->fog(item, time);}
		LWTextureID dispMap() const {return globPtr->dispMap(item);}
		LWTextureID clipMap() const {return globPtr->clipMap(item);}
		void patchLevel(int *display, int *render) const {globPtr->patchLevel(item, display, render);}
		void metaballRes(double *display, double *render) const {globPtr->metaballRes(item, display, render);}
		LWItemID boneSource() const {return globPtr->boneSource(item);}
		LWItemID morphTarget() const {return globPtr->morphTarget(item);}
		double morphAmount(LWTime time)	const {return globPtr->morphAmount(item, time);}
		unsigned int edgeOpts()	const {return globPtr->edgeOpts(item);}
		void edgeColor(LWTime time, LWDVector color)	const {globPtr->edgeColor(item, time, color);}
		int  subdivOrder() const {return globPtr->subdivOrder(item);}
		double polygonSize(LWTime time) const {return globPtr->polygonSize(item, time);}
		bool excluded (LWItemID light) const 
    {
      if (!available()) return false; // safguard to evaluation in Modeler
      return globPtr->excluded(item, light) != 0;
    }
		void matteColor(LWTime time, LWDVector color)	const {globPtr->matteColor(item, time, color);}
		double thickness(LWTime time, int type) const {return globPtr->thickness(item, time, type);}
		double edgeZScale(LWTime time) const {return globPtr->edgeZScale(item, time);}
	};

	class SceneObjects : protected GlobalBase<LWObjectFuncs>
	{
	public:
		SceneObjects(void) {;}

		int numObjects(void) const {return globPtr->numObjects();}
		const char *filename(int obj) const {return globPtr->filename(obj);}
		int maxLayers(int obj) const {return globPtr->maxLayers(obj);}
		int layerExists(int obj, int layer) const {return globPtr->layerExists(obj, layer);}
		LWMeshInfoID layerMesh(int obj, int layer) const {return globPtr->layerMesh(obj, layer);}
		const char *vmapName(LWID type, int index) const {return globPtr->vmapName(type, index);}
		int numVMaps (LWID type) const {return globPtr->numVMaps(type);}
		int vmapDim  (LWID type, int index) const {return globPtr->vmapDim(type, index);}
    const char *userName(int obj) const {return globPtr->userName(obj);}
    const char *refName(int obj) const {return globPtr->refName(obj);}
		// Helper functions
		int findVmapIndex(const std::string &name, LWID id)
		{
			if (!name.empty())
			{
				int max = numVMaps(id);
				for (int i = 0; i < max; ++i)
				{
					if (name == vmapName(id, i)) return i;
				}
			}
			return -1;
		}
		int findObject (const std::string &objectName);
		std::set <LWSurfaceID> getSurfaces(int idx);
/*
   typedef struct st_LWObjectFuncs {
      void         (*pivotPoint)  (int obj, int lnum, LWFVector pos);
      LWID         (*vmapType)    (int index);
      const char * (*layerName)   (int obj, int lnum);
      int          (*layerVis)    (int obj, int lnum);
   } LWObjectFuncs;
*/

	};

		//! Provide callback functions for LW Items in XPanels...
	/*!
	 * @ingroup Entities
	 * This class takes care of an item, including loading and saving item references
	 * @todo turn this into a more universal wrapper, a managed variable/item or something like it...
	 */
	template <bool showNone = true>
	class SceneObjectReference : public PopUpCallback, public SceneObjects
	{
	protected:
		//! Return number of items
		virtual size_t popCount (void) 
		{
			return numObjects() + (showNone ? 1 : 0);
		}
		//! return name of item number idx
		virtual const char *popName (int idx)
		{
			if (showNone)
			{
				if (idx == 0)	return "(none)";
				--idx;
			}
			return userName(idx);
		}
	public:
		//! Constructor
		SceneObjectReference()	{;}
		//! XPanels compatible DataGet
		int *DataGet ()
		{
			return &index;
		}
		//! XPanels compatible DataSet
		void DataSet(void *value)
		{
			index = *((int *)value);
		}
	};

	class VMapCallbacks : public PopUpCallback
	{
	private:
		LWID type; // VMap ID
		SceneObjects so;
		bool showNone;
	protected:
		virtual size_t popCount(void)
		{
			return so.numVMaps(type) + (showNone ? 1 : 0);
		}
		virtual const char *popName(int n)
		{
			if (showNone)
			{
				if (n == 0) return "(none)";
				--n;
			}
			return so.vmapName(type, n );
		}
	public:
		VMapCallbacks (LWID _type, bool _showNone = true) : type(_type), showNone(_showNone) {;}
		virtual ~VMapCallbacks() {;}
	};
}
#endif //LWPP_OBJECTINFO_H