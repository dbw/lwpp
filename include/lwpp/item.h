#ifndef LWPP_ITEM_H
#define LWPP_ITEM_H
#include <lwpp/storeable.h>
#include <lwpp/wrapper.h>
#include <lwpp/matrix4x4.h>
#include <lwpp/vparm.h>
#include <lwpp/interface.h>
#include <lwpp/command.h>
#include "lwpp/objectinfo.h"
#include "lwpp/colour_management.h"
#include <vector>
#include "utility.h"
#ifdef _DEBUG
#include "lwpp/debug.h"
#endif

namespace lwpp
{
	class LWItem; // forward declaration

	//! Straight LWItemInfo function wrapper
	//! @ingroup Globals
	class ItemInfo : protected GlobalBase<LWItemInfo>
	{
	public:
		ItemInfo()
		{
			;
		}
		bool isValid() const { return available(); }
		LWItemID	first(LWItemType t, LWItemID i)
		{
			return globPtr->first(t, i);
		}
		LWItemID	next(LWItemID i)
		{
			return globPtr->next(i);
		}
		LWItemID	firstChild(LWItemID parent)
		{
			return globPtr->firstChild(parent);
		}
		LWItemID	nextChild(LWItemID parent, LWItemID prevChild)
		{
			return globPtr->nextChild(parent, prevChild);
		}
		LWItemID	parent(LWItemID i)
		{
			return globPtr->parent(i);
		}
		LWItemID	target(LWItemID i)
		{
			return globPtr->target(i);
		}
		LWItemID	goal(LWItemID i)
		{
			return globPtr->goal(i);
		}
		LWItemType	type(LWItemID i)
		{
			return globPtr->type(i);
		}
		const char* name(LWItemID i)
		{
			return globPtr->name(i);
		}
		bool selected(LWItemID i)
		{
			return (globPtr->selected(i) != 0);
		}
		void param(LWItemID i, LWItemParam p, LWTime t, LWDVector v)
		{
			if (i != LWITEM_NULL) globPtr->param(i, p, t, v);
		}
		void param(LWItemID i, LWItemParam p, LWTime t, double v[4][4])
		{
			if (i != LWITEM_NULL) globPtr->param(i, p, t, &v[0][0]);
		}
		unsigned int limits(LWItemID i, LWItemParam p, LWDVector min, LWDVector max)
		{
			return globPtr->limits(i, p, min, max);
		}
		const char* getTag(LWItemID i, int t)
		{
			return globPtr->getTag(i, t);
		}
		void			setTag(LWItemID i, int t, const char* tag)
		{
			globPtr->setTag(i, t, tag);
		}
		LWChanGroupID	chanGroup(LWItemID i)
		{
			return globPtr->chanGroup(i);
		}
		const char* server(LWItemID i, const char* s, int d)
		{
			return globPtr->server(i, s, d);
		}
		unsigned int serverFlags(const char* s, int d, LWItemID i = 0)
		{
			return globPtr->serverFlags(i, s, d);
		}
		void controller(LWItemID i, LWItemParam p, int type[3])
		{
			globPtr->controller(i, p, type);
		}
		unsigned int	flags(LWItemID i)
		{
			return globPtr->flags(i);
		}
		LWTime			lookAhead(LWItemID i)
		{
			return globPtr->lookAhead(i);
		}
		double	goalStrength(LWItemID i, LWTime t)
		{
			return globPtr->goalStrength(i, t);
		}
		void stiffness(LWItemID i, LWItemParam p, LWDVector v)
		{
			globPtr->stiffness(i, p, v);
		}
		unsigned int	axisLocks(LWItemID i, LWItemParam p)
		{
			return globPtr->axisLocks(i, p);
		}
		// additional functionality
		//! Find a LWItem that matches a name and type
		LWItem* FindItem(const std::string& itemName, LWItemType type);
		//! Find a LWItemID that matches a name and type
		LWItemID	FindItemID(const std::string& itemName, LWItemType type);
		//! Find a LWItem that matches an ID and type
		int			FindIndex(LWItemID, LWItemType type);
		//! Count the number of LWItems of a type
		int	CountItems(LWItemType type);
		//! Get the n'th LWitem of a type
		LWItem* GetItemN(LWItemType type, int n);
		int countServer(const char* type, const char* name, LWItemID id = 0);
		//! Find the index of a certain handler
		int findServer(const char* type, const char* name, LWItemID id = 0);

		void editServer(const char* type, const char* name, LWItemID id = 0);

		void addUniqueServer(const char* type, const char* name, LWItemID id = 0);

		void removeServer(const char* type, const char* name, LWItemID id = 0);

    void enableServer(const char* type, const char* name, bool state = true, LWItemID id = 0);

		int getServerFlags(const char* name, int index, LWItemID id = 0)
		{
			return globPtr->serverFlags(id, name, index);
		}

	};

	//! @ingroup Entities
	class LWItem : public Storeable
	{
	protected:
		ItemInfo	mItemInfo;
		LWItemID	mId; //!< ID of the stored object reference
		LWItemType mType;
		std::string m_itemName; //! Used only for storing the name when loading and saving in Modeler
	public:
		explicit LWItem(LWItemID id = LWITEM_NULL, LWItemType t = LWI_OBJECT) : mId(id), mType(t)
		{
			;
		}

		LWItem(const LWItem& item)
		{
			mId = item.mId;
			mType = item.mType;
		}

		LWItem(const LWItemType t)
			: mId(LWITEM_NULL), mType(t)
		{
			First(t);
		}

		//! Checks if the item exists
		bool exists() const { return mId != LWITEM_NULL; }

		//! Checks if the LWItemInfo global is available or not
		bool isValid() const { return mItemInfo.isValid(); }

		bool isSelected() { return mItemInfo.selected(mId); }

		void SetID(LWItemID _id)
		{	
			mId = _id;
			mType = Type();
		}
		LWItemID GetID() const { return mId; }

    unsigned int GetNumID() const
    {
      return (LWITEMID2UINT(mId) & 0xfffffff);
    }
		//  Standard wraps
		LWItem& First(LWItemType t)																																	  
		{
			if (mItemInfo.isValid())
			{
				mId = mItemInfo.first(t, mId);
			}
			return *this;
		}
		LWItemID GetFirstBone()
		{
			if (mItemInfo.isValid())
			{
				return mItemInfo.first(LWI_BONE, mId);
			}
			return 0;
		}
		LWItem& Next(void)
		{
			mId = mItemInfo.next(mId);
			return *this;
		}

		bool hasChildren(void)
		{
			return (mItemInfo.firstChild(mId) != LWITEM_NULL);
		}

		LWItemID getFirstChild(void)
		{
			return mItemInfo.firstChild(mId);
		}

		LWItem& FirstChild(void)
		{
			mId = getFirstChild();
			return *this;
		}

		LWItem& NextChild(LWItem& parent)
		{
			mId = mItemInfo.nextChild(parent.mId, mId);
			return *this;
		}

		LWItem Parent(void)
		{
			return LWItem(mItemInfo.parent(mId));
		}
		LWItem& ToParent()
		{
			mId = mItemInfo.parent(mId);
			return *this;
		}

		LWItem& Target(void)
		{
			mId = mItemInfo.target(mId);
			return *this;
		}

		LWItem& Goal(void)
		{
			mId = mItemInfo.goal(mId);
			return *this;
		}

		LWItemType Type(void)
		{
			return mItemInfo.type(mId);
		}

		const char* getName(void)
		{
			if (mId != LWITEM_NULL) return mItemInfo.name(mId);
			return "(none)";
		}

		void Param(LWItemParam p, LWTime t, LWDVector v)
		{
			mItemInfo.param(mId, p, t, v);
		}

		void Param(LWItemParam p, LWTime t, Vector3d& v)
		{
			mItemInfo.param(mId, p, t, v.asLWVector());
		}

		//! @name Param related helper functions
		///@{
		Point3d getPosition(LWTime t)
		{
			Point3d pos;
			Param(LWIP_POSITION, t, pos);
			return pos;
		}
		Point3d getWorldPosition(LWTime t)
		{
			Point3d pos;
			Param(LWIP_W_POSITION, t, pos);
			return pos;
		}
		Vector3d getPivot(LWTime t)
		{
			Vector3d pos;
			Param(LWIP_PIVOT, t, pos);
			return pos;
		}
		Vector3d getScaling(LWTime t)
		{
			Vector3d v;
			Param(LWIP_SCALING, t, v);
			return v;
		}
		Vector3d getRotation(LWTime t)
		{
			Vector3d v;
			Param(LWIP_ROTATION, t, v);
			return v;
		}
		Vector3d getPivotRotation(LWTime t)
		{
			Vector3d v;
			Param(LWIP_PIVOT_ROT, t, v);
			return v;
		}

		void getItem2World(LWTime t, LWDVector toWorld[3]);

		Matrix4x4d getWorld2Item(LWTime t);
		Matrix4x4d getObjectToWorld(LWTime time);
		Matrix4x4d getWorldTransform(LWTime time);
		void transformToWorld(Point3d& pt, LWTime time);
		void transformToLocal(Point3d& pt, LWTime time);
		void transformToLocal(Vector3d& pt, LWTime time);
		///@} 

		void Param(LWItemParam p, LWTime t, Point3d& v)
		{
			mItemInfo.param(mId, p, t, v.asLWVector());
		}

		void Param(LWItemParam p, LWTime t, double v[4][4])
		{
			mItemInfo.param(mId, p, t, v);
		}

		unsigned int Limits(LWItemParam p, LWDVector min, LWDVector max)
		{
			return mItemInfo.limits(mId, p, min, max);
		}

		const char* GetTag(int t)
		{
			return mItemInfo.getTag(mId, t);
		}

		void SetTag(int t, const char* tag)
		{
			mItemInfo.setTag(mId, t, tag);
		}

		LWChanGroupID ChanGroup()
		{
			return mItemInfo.chanGroup(mId);
		}

		ChannelGroup getChannelGroup(void)
		{
			return ChannelGroup(ChanGroup());
		}

		const char* Server(const char* s, int d)
		{
			return mItemInfo.server(mId, s, d);
		}

		unsigned int ServerFlags(const char* s, int d)
		{
			return mItemInfo.serverFlags(s, d, mId);
		}

		void Controller(LWItemParam p, int type[3])
		{
			mItemInfo.controller(mId, p, type);
		}

		unsigned int Flags(void)
		{
			return mItemInfo.flags(mId);
		}

		LWTime LookAhead(void)
		{
			return mItemInfo.lookAhead(mId);
		}
		double GoalStrength(LWTime t)
		{
			return mItemInfo.goalStrength(mId, t);
		}
		void Stiffness(LWItemParam p, LWDVector v)
		{
			mItemInfo.stiffness(mId, p, v);
		}

		unsigned int AxisLocks(LWItemParam p)
		{
			return mItemInfo.axisLocks(mId, p);
		}
		//! Check if the current item != LWITEM_NULL
		bool operator!()
		{
			return mId == LWITEM_NULL;
		}

		//! Helper function
		void changeID(const LWItemID* id_array)
		{
			int i = 0;
			while (id_array[i] != LWITEM_NULL)
			{
				if (id_array[i] == mId) mId = id_array[i + 1];
				i += 2;
			}
		}
		int getExtension();
		//! Load on object, this will store the item ID in the end
		virtual LWError Load(const LoadState& ls);
		//! Save and object, using the name
		virtual LWError Save(const SaveState& ls);
		bool operator==(const LWItem& rhs) const
		{
			return mId == rhs.mId;
		}
	};

	class PrettyItem
	{
		lwpp::ObjectInfo oi;
		lwpp::InterfaceInfo ii;
		lwpp::ColourManager cm;
		std::ostringstream name;
	public:
		PrettyItem();
		std::string format(LWItemID id, int indent = 0);
		std::string format(lwpp::LWItem& item, int indent = 0);
	};

	//! Provide callback functions for LW Items in XPanels...
	/*!
	 * @ingroup Entities
	 * This class takes care of an item, including loading and saving item references
	 * @todo turn this into a more universal wrapper, a managed variable/item or something like it...
	 */

	typedef bool (filterItemCB)(lwpp::LWItem& item);

	bool filterAll(lwpp::LWItem& item);

	class LWItemCallback : public PopUpCallback, public LWItem
	{
		filterItemCB *filter = nullptr;
	protected:
		PrettyItem pretty;
		class itemEntry
		{
		public:
			LWItemID id;
			std::string name;
			itemEntry(LWItemID _id, std::string _name)
				: id(_id), name(_name)
			{
				;
			}
		};
		std::vector<itemEntry> itemList;
		const char* noneName;
		LWItemType mDisplayType;
		bool showNone = false;
		void buildItemTree(lwpp::LWItem parent, int indent);
		void buildItemTree();

	protected:
		//! Return number of items
		virtual size_t popCount() override;
		//! return name of item number idx
		virtual const char* popName(int idx) override;

		int findID(LWItemID id)
		{
			if (id == LWITEM_NULL) return 0;
			int idx = 0;
			for (auto& i : itemList)
			{
				if (i.id == id)
					return idx + (showNone ? 1 : 0);
				++idx;
			}
			return 0;
		}
	public:
		//! Constructor, default to an object and no object selected
		LWItemCallback(LWItemType t = LWI_OBJECT,
			LWItemID id = LWITEM_NULL,
			bool _showNone = true,
			const char* _noneName = "(none)",
			filterItemCB *filterCB=filterAll)

			: LWItem(id, t),
			mDisplayType(t),
			showNone(_showNone),
			noneName(_noneName),
			filter(filterCB)
		{
			buildItemTree();
			index = findID(mId);
		}
		LWItemCallback& operator = (const lwpp::LWItemCallback& from)
		{
			if (this != &from)
			{
				showNone = from.showNone;
				noneName = from.noneName;
				mId = from.mId;
				mType = from.mType;
				m_itemName = from.m_itemName;
				mDisplayType = from.mDisplayType;
			}
			return *this;
		}

		//! return mID of item number idx
		LWItemID GetIndexID(int idx)
		{
			if (showNone)
			{
				if (idx == 0)
				{
					mId = LWITEM_NULL;
					return mId;
				}
				--idx;
			}
			buildItemTree();
			if (idx < itemList.size())
				mId = itemList[idx].id;
			else
				mId = LWITEM_NULL;

			return mId;
		}

		size_t getNumItems() { return popCount(); }
		const char* getNumName(int idx) { return popName(idx); }

		//! return the index of object ID
		int GetIndex()
		{
			buildItemTree();
			return findID(mId);
		}
		//! XPanels compatible DataGet
		int* DataGet()
		{
			static int idx;
			if (lwpp::LightWave::isModeler()) return 0;
			idx = GetIndex();
			return &idx;
		}
		//! XPanels compatible DataSet
		void DataSet(void* value)
		{
			index = *((int*)value);
			GetIndexID(index);
		}
	};


	//! This class tracks the IDs of LW items, especially related to the LWItemFuncs callbacks
	/*!
	 * @ingroup Entities
	 * LW Callbacks served:
	 *
	 * - idlist = useItems( instance )
	 *
	 * Returns an array of items this instance depends on. If your plug-in's behavior is based on the parameters of other items (such as the positions of objects), you'll want to be re-evaluated after those parameters change, and you use this function to inform Layout of that. The array is terminated by LWITEM_NULL. The function can return NULL if the instance doesn't use any items. It can also return LWITEM_ALL to indicate that it wants to be evaluated after any change occurs.
	 *
	 * - changeID( instance, idlist )
	 *
	 * Notification about a change in item IDs. This function is called if the IDs of items are going to change for any reason. The null-terminated item array passed to this function is of the form "old-1, new-1, old-2, new-2, ..." where the old ID is the value that is changing and the new ID is its new value. Clients should be careful to renumber each item only once.
	 * The changeID callback may also be called when an item's data, such as the geometry of an object, has changed, and when called for this reason, the old and new item IDs will be the same.
	 *
	 * @todo Actually implement this class
	 * This is how it is supposed to work:
	 * - Items to be tracked are added to the class using TrackItem()...
	 * - This allows the class to dynamically create a use items list
	 * - changeIDs are passed on to the tracked objects if needed
	 */
	class LWItemTracker
	{
	private:
		std::vector<LWItem*> mTrackedItems;
		std::vector<LWItemID> mUsedItems;
	public:
		LWItemTracker() { ; }

		~LWItemTracker(void);
		//! Refresh the list of IDs created from the tracked Items
		void refresh();
		//! Add an item to track.
		void trackItem(LWItem* item);

		void trackItem(LWItem& item);

		void unTrackItem(LWItem& item);
		void unTrackItem(LWItem* item);

		//! Should be used for LWs useItems callback.

		const LWItemID* useItems();

		//! should be used for LWs changeID callback.
		void changeID(const LWItemID* idlist);
    void clear();
	};
} // end of namespace lwpp

#endif // LWPP_ITEM_H
