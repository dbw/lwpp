#ifndef LWPP_SURFACE_H
#define LWPP_SURFACE_H

#include <lwpp/global.h>
#include <lwsurf.h>
#include <set>
#include <lwpp/wrapper.h>

namespace lwpp
{
	class LWSurface : public GlobalBase<LWSurfaceFuncs>
	{
	protected:
		LWSurfaceID id;
	public:
		//! Default contructor, set this surface to the first surface
		LWSurface() :  id(0)
		{
			first();
		}
		//! Constructor from an existing surface ID
		LWSurface(LWSurfaceID _id) : id(_id){;}
		//! Constructor creating a new surface
		LWSurface(const char *objName, const char *surfName) : id(0)
		{
			id = globPtr->create(objName, surfName);
		}
		const char *name() const {return globPtr->name(id);}
    void first ()
		{
			id = globPtr->first();
		}
    bool next()
		{
			id = globPtr->next(id);
			return (id != 0);
		}
		bool exists()
		{
			return (id != 0);
		}
		LWSurfaceID getID() const
		{
			return id;
		}
		bool operator<(const LWSurface &s) const {return id < s.id;}
		const char *sceneObject() const {return globPtr->sceneObject(id);}

		LWTextureID getTexture(const char *channel) const { return globPtr->getTex(id, channel);}
		int getInteger(const char *channel) const { return globPtr->getInt(id, channel);}
		double *getDouble(const char *channel) const { return globPtr->getFlt(id, channel);}
		LWEnvelopeID getEnvelope(const char *channel) const { return globPtr->getEnv(id, channel);}
		LWImageID getImage(const char *channel) const { return globPtr->getImg(id, channel);}
		LWChanGroupID getChannelGroup() const { return globPtr->chanGrp(id);}
		const char *getColorVMap() const {return globPtr->getColorVMap(id);}
		void setColorVMap(const char *vmapName, int type) {globPtr->setColorVMap(id, vmapName, type);}
		NodeEditorID getNodeEditor( ) const
		{
			return globPtr->getNodeEditor(id);
		}
		typedef std::set<LWSurface> SurfaceSet;
		typedef SurfaceSet::iterator iterSurfaceSet;
		SurfaceSet byName(std::string name, std::string obj) const;
		SurfaceSet byName(std::string name) const;
		SurfaceSet byObject(std::string name) const;
	};

		//! Provide callback functions for LW Items in XPanels...
	/*!
	 * @ingroup Entities
	 * This class takes care of an item, including loading and saving item references
	 * @todo turn this into a more universal wrapper, a managed variable/item or something like it...
	 */
	template <bool showNone = true>
	class SurfaceReference : public PopUpCallback, public LWSurface
	{
	protected:
		//! Return number of items
		virtual size_t popCount (void)
		{
			LWSurface surf;
			surf.first();
			int count = 0;
			while (surf.exists())
			{
				++count;
				surf.next();
			}
			return count + (showNone ? 1 : 0);
		}
		//! return name of item number idx
		virtual const char *popName (int idx)
		{
			if (showNone)
			{
				if (idx == 0)	return "(none)";
				--idx;
			}
			LWSurface surf(GetIndexID (idx));
			if (surf.exists())
			{
				return surf.name();
			}
			return "";
		}
	public:
		//! Constructor, default to an object and no object selected
		SurfaceReference(LWSurfaceID _id = 0)
		: LWSurface(_id)
		{
			if (id != 0)
			{
				index = GetIndex();
			}
		}
		//! return id of item number idx
		LWSurfaceID GetIndexID (int idx)
		{
			if (showNone)
			{
				if (idx == 0)
				{
					id = 0;
					return 0;
				}
				--idx;
			}
			LWSurface surf;
			surf.first();
			for (int i = 1; i <= idx; ++i)
			{
				surf.next();
			}
			id = surf.getID();

			return id;
		}
		int GetIndex()
		{
			if (id == 0) return 0;
			int idx = (showNone ? 1 : 0);
			LWSurface surf;
			surf.first();
			while (surf.exists())
			{
				if (id == surf.getID()) return idx;
				++idx;
				surf.next();
			}
			return 0;
		}
		//! XPanels compatible DataGet
		int *DataGet ()
		{
			index = GetIndex();
			return &index;
		}
		//! XPanels compatible DataSet
		void DataSet(void *value)
		{
			index = *((int *)value);
			GetIndexID(index);
		}
	};
}

/*
   typedef struct st_LWSurfaceFuncs {
      LWSurfaceID   (*create)      (const char *objName,
                                      const char *surfName);
      LWSurfaceID * (*byName)      (const char *name, const char *obj);
      LWSurfaceID * (*byObject)    (const char *name);

			int           (*getInt)(LWSurfaceID surf,const char *channel);
			double       *(*getFlt)(LWSurfaceID surf,const char *channel);
			LWEnvelopeID  (*getEnv)(LWSurfaceID surf,const char *channel);
			LWTextureID   (*getTex)(LWSurfaceID surf,const char *channel);
			LWImageID     (*getImg)(LWSurfaceID surf,const char *channel);

			LWChanGroupID (*chanGrp)(LWSurfaceID surf);
			const char   *(*getColorVMap)(LWSurfaceID surf);
			void          (*setColorVMap)(LWSurfaceID surf,const char *vmapName,int type);

			LWSurfLibID   (*createLib)(void);
			void          (*destroyLib)(LWSurfLibID lib);
			void          (*copyLib)(LWSurfLibID to, LWSurfLibID from);
			LWSurfLibID   (*objectLib)(const char *objname);
			LWSurfLibID   (*loadLib)(const char *name);
			LWError       (*saveLib)(LWSurfLibID lib, const char *name);
			int           (*slibCount)(LWSurfLibID lib);
			LWSurfaceID   (*slibByIndex)(LWSurfLibID lib,int idx);
			NodeEditorID  (*getNodeEditor)( LWSurfaceID surf);
   } LWSurfaceFuncs;
*/

#endif // LWPP_SURFACE_H