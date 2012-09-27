#include "lwpp/item.h"
#include <sstream>
#include <algorithm>
#include "lwpp/command.h"
#include "lwpp/Point3d.h"

namespace lwpp
{
  int LWItem::getExtension()
  {
    const std::string name = getName();

    if(name.empty())
      return 0;

    int len = static_cast<int>(name.length());
    int dEnd = -1, dStart = -1;

    for( int i = len-1; i > 0; --i)
    {
      if(isdigit(name[i]))
      {
        if(dEnd == -1)
        {
          dEnd = i;
        }
      }
      else
      {
        if(dEnd != -1 )
        {
          dStart = i;
          break;
        }
      }
    }

   return atoi( name.substr( dStart+1, dEnd-dStart ).c_str() );
  }
	Matrix4x4d LWItem::getObjectToWorld(LWTime time)
	{
		Point3d pos;
		Vector3d rt, up, fd;
		Param(LWIP_RIGHT, time, rt);
		Param(LWIP_UP, time, up);
		Param(LWIP_FORWARD, time, fd);
		Param(LWIP_POSITION, time, pos);
		return Matrix4x4d(rt, up, fd, pos);
		//return Matrix4x4d(rt, up, fd);
	}

	Matrix4x4d LWItem::getWorldTransform(LWTime time)
	{
		Point3d pos;
		Vector3d rt, up, fd;
		Param(LWIP_RIGHT, time, rt);
		Param(LWIP_UP, time, up);
		Param(LWIP_FORWARD, time, fd);
		Param(LWIP_W_POSITION, time, pos);
		return Matrix4x4d(rt, up, fd, pos);
	}

	void LWItem::transformToWorld(Point3d &pt, LWTime t)
	{
		double M[4][4];
		LWDVector pos, rt, up, fd;
		Param( LWIP_POSITION, t, pos );
		Param( LWIP_RIGHT, t, rt );
		Param( LWIP_UP, t, up );
		Param( LWIP_FORWARD, t, fd );

		for ( int i = 0; i < 3; i++ ) {
			M[ 0 ][ i ] = ( float ) rt[ i ];
			M[ 1 ][ i ] = ( float ) up[ i ];
			M[ 2 ][ i ] = ( float ) fd[ i ];
			M[ 3 ][ i ] = ( float ) pos[ i ];
			M[ i ][ 3 ] = 0.0;
		}
		M[ 3 ][ 3 ] = 1.0;

		Point3d a(pt);

			pt.x = a.x * M[ 0 ][ 0 ]	+ a.y * M[ 1 ][ 0 ]	+ a.z * M[ 2 ][ 0 ] + M[ 3 ][ 0 ];
			pt.y = a.x * M[ 0 ][ 1 ]	+ a.y * M[ 1 ][ 1 ]	+ a.z * M[ 2 ][ 1 ] + M[ 3 ][ 1 ];
			pt.z = a.x * M[ 0 ][ 2 ]	+ a.y * M[ 1 ][ 2 ]	+ a.z * M[ 2 ][ 2 ] + M[ 3 ][ 2 ];

		double w = a.x * M[ 0 ][ 3 ]	+ a.y * M[ 1 ][ 3 ]	+ a.z * M[ 2 ][ 3 ] + M[ 3 ][ 3 ];

		if ( w != 0.0f )
		{
			pt /= w;
		}
	}
	/*!
	 * Search all LW Items for an item with this name, and return a LWItem.
	 */
	LWItem *ItemInfo::FindItem(const std::string &itemName, LWItemType type)
	{
		return new LWItem(FindItemID(itemName, type));
	}
	/*!
	 * Search all LW Items for an item with this name, and return the Item ID.
	 */
	LWItemID ItemInfo::FindItemID(const std::string &itemName, LWItemType type)
	{
		// Take care of the easy case first.
		if (itemName.empty()) 	return LWITEM_NULL;

		// Loop through the entire list of items.
		for (LWItemID id = first(type, LWITEM_NULL); id != LWITEM_NULL; id = next(id))
		{
			// Get the current item's name, if it matches, return the Item ID.
			if (itemName == name(id)) return id;
		}
		// No match was found, return 0.
		return LWITEM_NULL;
	}
	/*!
	 * Search all LW Items for an item with this name, and return the Item ID.
	 */
	int ItemInfo::FindIndex(LWItemID lwi, LWItemType type)
	{
		int count = 0;
		// Take care of the easy case first.
		if (lwi == LWITEM_NULL)
			return -1;

		// Loop through the entire list of items.
		for (LWItemID id = first(type, LWITEM_NULL); id != LWITEM_NULL; id = next(id), count++)
		{
			if (id == lwi) return count;
		}
		// No match was found, return 0.
		return -1;
	}

	/*!
	 * Count all LW Items in a scene
	 */
	int ItemInfo::CountItems(LWItemType type)
	{
		int count = 0;
		// Loop through the entire list of items.
		for (LWItemID id = first(type, LWITEM_NULL); id != LWITEM_NULL; id = next(id))
			count ++;
		return count;
	}

	LWItem *ItemInfo::GetItemN(LWItemType type, int n)
	{
		int count = 0;
		// Loop through the entire list of items.
		for (LWItemID id = first(type, LWITEM_NULL); id != LWITEM_NULL ; id = next(id), count++)
			if (count == n) return new LWItem(id);
		// No match was found, return 0.
		return new LWItem();
	}

	int ItemInfo::findServer(const char *type, const char *name, LWItemID id)
	{
		int index = 1;
		while (const char *srv = server(id, type, index))
		{
			if (strcmp(srv, name) == 0) return index;
			index++;
		}
		return 0;
	}

	/** LWItemCallback **/

	// defines etc. for file i/o
	#define IO_ITEM LWID_('I','T','E','M')  // Name of the item
	#define IO_ITID LWID_('I','T','I','D')  // ID of the item
	#define IO_ITTY LWID_('I','T','T','Y')  // Type of the item
	//! For save and load operations
	static LWBlockIdent item_root[] =
	{
		IO_ITEM, "Item",
		IO_ITID, "ItemID",
		IO_ITTY, "ItemType",
		0
	};

	LWError LWItem::Save(const SaveState &ss )
	{
		LWError err = 0;
		unsigned int l;
		//! - Item name
		if (!lwpp::LightWave::isModeler())
		{
			if (getName()) m_itemName = getName();
		}

		if (!m_itemName.empty()) // otherwise LWSAVE_STR will crash.
		{
			LWSAVE_BEGIN(ss.getState(), &item_root[0], 1);
				LWSAVE_STR(ss.getState(), m_itemName.c_str()); // save the item name
			LWSAVE_END(ss.getState());
		}
		//! - ItemID
		LWSAVE_BEGIN(ss.getState(), &item_root[1], 1);
		{
			// required to get rid of compiler warning when compiling 64-bit code
			unsigned long jic = (unsigned long)mId;
		  l = (unsigned int) jic;
			LWSAVE_U4(ss.getState(), &l, 1);
		}
		LWSAVE_END(ss.getState());

		//! - Item type
		LWSAVE_BEGIN(ss.getState(), &item_root[2], 1);
		  l = (unsigned int) mType;
			LWSAVE_U4(ss.getState(), &l, 1);
		LWSAVE_END(ss.getState());

		return err;
	}

	/*!
	 * Loading an item is safe in Modeler as well.
	 * In Layout, lwpp will first try to find an item with a matching name. If that fails
	 * it will use the id that was stored to reference the item.
	 *
	 * In Modeler, lwpp will just store the item name and id and the use that for the next save.
	 */
	LWError LWItem::Load(const LoadState &ls )
	{
		LWError err = 0;
		unsigned int l;
		while(LWID id = ls.Find(item_root))
		{
			switch( id )
			{
				case IO_ITEM: //! - Item name
					m_itemName = ls.readString();
					break;
				case IO_ITID: //! - Item ID
						ls.read(l);
						mId = (LWItemID) l;
					break;
				case IO_ITTY: //! - Item type
						ls.read(l);
						mType = (LWItemType) l;
					break;

				default:
					break;
			}
			ls.End();
		}
		if (!lwpp::LightWave::isModeler())
		{
			LWItemID _id = mItemInfo.FindItemID(m_itemName, mType); // find matching item if possible
			if (_id != LWITEM_NULL) mId = _id; // only use the loaded id if the object has not been found
		}
		return err;
	}

	void StyledItemCallback::addChildren (LWItem &parent, int indent, LWItemType t)
	{
		if (parent.GetID())
		{
			if ((parent.Type() == t))
			{
				std::ostringstream fancyName;
				fancyName << " \03(g:" << indent << ")" ;
				static const char *icon[] = {"160", "159", "162", "161"};
				if (lwpp::LightWave::isLayout())
				{
					fancyName << "\03(i:" << icon[t] << ", " << iInfo.itemColor(parent.GetID()) << ")";
				}
				fancyName	<< parent.getName();
				itemList.push_back( itemEntry( parent.GetID(), fancyName.str() ));
			}
			lwpp::LWItem child(parent.getFirstChild());
			while (child.GetID())
			{
				addChildren(child, indent + 10, t);
				child.NextChild(parent);
			}
		}
	}

	void StyledItemCallback::buildItemList()
	{
		itemList.clear();
		LWItem root(LWI_OBJECT);
		while (root.GetID())
		{
			LWItem parent = root.Parent();
			if (!parent.GetID())
			{
				addChildren(root, 0, mType);
			}
			root.Next();
		}
		root = LWItem(LWI_LIGHT);
		while (root.GetID())
		{
			LWItem parent = root.Parent();
			if (!parent.GetID())
			{
				addChildren(root, 0, mType);
			}
			root.Next();
		}
		root = LWItem(LWI_CAMERA);
		while (root.GetID())
		{
			LWItem parent = root.Parent();
			if (!parent.GetID())
			{
				addChildren(root, 0, mType);
			}
			root.Next();
		}
	}

	/*
	 * LWItemTracker
	 */
	LWItemTracker::~LWItemTracker (void)
	{
		if (mTrackedIDs != 0) delete[] mTrackedIDs;
	}

	void LWItemTracker::refresh()
	{
		if (mTrackedIDs != 0) delete[] mTrackedIDs;
		mTrackedIDs = new LWItemID[mTrackedItems.size() + 1];
		for (unsigned int i = 0; i < mTrackedItems.size(); ++i)
		{
			mTrackedIDs[i] = mTrackedItems[i]->GetID();
		}
		mTrackedIDs[mTrackedItems.size()] = LWITEM_NULL; // last item must be null
	}

	/*!
	*  @note, the item must persist for the duration of this class, at least for as long as Refresh may be called
	*/
	void LWItemTracker::trackItem (LWItem *item)
	{
		if (item == 0) return;
		mTrackedItems.push_back(item);
	}
	void LWItemTracker::trackItem (LWItem &item)
	{
		if (&item == 0) return;
		mTrackedItems.push_back(&item);
	}

	void LWItemTracker::unTrackItem (LWItem &item)
	{
		if (&item == 0) return;
		std::vector<LWItem *>::iterator iter = find(mTrackedItems.begin(), mTrackedItems.end(), &item);
		if (iter != mTrackedItems.end())
		{
			mTrackedItems.erase(iter);
		}
	}

	void LWItemTracker::unTrackItem (LWItem *item)
	{
		if (item == 0) return;
		std::vector<LWItem *>::iterator iter = find(mTrackedItems.begin(), mTrackedItems.end(), item);
		if (iter != mTrackedItems.end())
		{
			mTrackedItems.erase(iter);
		}
	}

	const LWItemID *LWItemTracker::useItems()
	{
		refresh();
		return mTrackedIDs;
	}

	/*
	* Optionally LWITEM_NULL or LWITEM_ALL can be returned manuall instead of the return of this function
	* @note This relies on a properly Refresh'd list.
	*/
	void LWItemTracker::changeID(const LWItemID *idlist)
	{
		if (idlist == 0) return;
		for (unsigned int i = 0; i < mTrackedItems.size(); ++i)
		{
			mTrackedItems[i]->changeID(idlist);
		}
	}
} // end namespace lwpp