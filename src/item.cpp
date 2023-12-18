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

		if (name.empty())
			return 0;

		int len = static_cast<int>(name.length());
		int dEnd = -1, dStart = -1;

		for (int i = len - 1; i > 0; --i)
		{
			if (isdigit(name[i]))
			{
				if (dEnd == -1)
				{
					dEnd = i;
				}
			}
			else
			{
				if (dEnd != -1)
				{
					dStart = i;
					break;
				}
			}
		}

		return atoi(name.substr(dStart + 1, dEnd - dStart).c_str());
	}

	Matrix4x4d LWItem::getWorld2Item(LWTime t) const
	{
		Point3d p;
		Vector3d r, u, f;
		//Param(LWIP_W_POSITION, t, p);
		Param(LWIP_W_RIGHT, t, r);
		Param(LWIP_W_UP, t, u);
		Param(LWIP_W_FORWARD, t, f);
		//return Matrix4x4d(r, u, f, p);
		return Matrix4x4d(r, u, f);
	}

	Matrix4x4d LWItem::getWorld2Object(LWTime t)
	{
		Point3d p;
		Vector3d r, u, f;
		Param(LWIP_W_POSITION, t, p);
		Param(LWIP_W_RIGHT, t, r);
		Param(LWIP_W_UP, t, u);
		Param(LWIP_W_FORWARD, t, f);
		return Matrix4x4d(r, u, f, p);
	}

	void LWItem::getItem2World(LWTime t, LWDVector toWorld[3])
	{
		Param(LWIP_RIGHT, t, toWorld[0]);
		Param(LWIP_UP, t, toWorld[1]);
		Param(LWIP_FORWARD, t, toWorld[2]);
	}

	Matrix4x4d LWItem::getObjectToWorld(LWTime time)
	{
		if (mId == LWITEM_NULL) return Matrix4x4d();
		Point3d pos;
		Vector3d rt, up, fd;
		Param(LWIP_RIGHT, time, rt);
		Param(LWIP_UP, time, up);
		Param(LWIP_FORWARD, time, fd);
		Param(LWIP_W_POSITION, time, pos);
		return Matrix4x4d(rt, up, fd, pos);
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

	void LWItem::transformToWorld(Point3d& pt, LWTime t)
	{
		double M[4][4];
		LWDVector pos, rt, up, fd;
		Param(LWIP_POSITION, t, pos);
		Param(LWIP_RIGHT, t, rt);
		Param(LWIP_UP, t, up);
		Param(LWIP_FORWARD, t, fd);

		for (int i = 0; i < 3; i++) {
			M[0][i] = (float)rt[i];
			M[1][i] = (float)up[i];
			M[2][i] = (float)fd[i];
			M[3][i] = (float)pos[i];
			M[i][3] = 0.0;
		}
		M[3][3] = 1.0;

		Point3d a(pt);

		pt.x = a.x * M[0][0] + a.y * M[1][0] + a.z * M[2][0] + M[3][0];
		pt.y = a.x * M[0][1] + a.y * M[1][1] + a.z * M[2][1] + M[3][1];
		pt.z = a.x * M[0][2] + a.y * M[1][2] + a.z * M[2][2] + M[3][2];

		double w = a.x * M[0][3] + a.y * M[1][3] + a.z * M[2][3] + M[3][3];

		if (w != 0.0f)
		{
			pt /= w;
		}
	}

	void LWItem::transformToWorld(Vector3d& pt, LWTime t)
	{
		double M[4][4];
		LWDVector pos, rt, up, fd;
		Param(LWIP_POSITION, t, pos);
		Param(LWIP_RIGHT, t, rt);
		Param(LWIP_UP, t, up);
		Param(LWIP_FORWARD, t, fd);

		for (int i = 0; i < 3; i++) {
			M[0][i] = (float)rt[i];
			M[1][i] = (float)up[i];
			M[2][i] = (float)fd[i];
			M[3][i] = (float)pos[i];
			M[i][3] = 0.0;
		}
		M[3][3] = 1.0;

		Point3d a(pt);

		pt.x = a.x * M[0][0] + a.y * M[1][0] + a.z * M[2][0] + M[3][0];
		pt.y = a.x * M[0][1] + a.y * M[1][1] + a.z * M[2][1] + M[3][1];
		pt.z = a.x * M[0][2] + a.y * M[1][2] + a.z * M[2][2] + M[3][2];

		double w = a.x * M[0][3] + a.y * M[1][3] + a.z * M[2][3] + M[3][3];

		if (w != 0.0f)
		{
			pt /= w;
		}
	}


	void LWItem::transformToLocal(Point3d& pt, LWTime t)
	{
		double M[4][4];
		LWDVector pos, rt, up, fd, piv;
		//Param(LWIP_W_POSITION, t, pos);
		Param(LWIP_W_RIGHT, t, rt);
		Param(LWIP_W_UP, t, up);
		Param(LWIP_W_FORWARD, t, fd);

		for (int i = 0; i < 3; i++) {
			M[0][i] = (float)rt[i];
			M[1][i] = (float)up[i];
			M[2][i] = (float)fd[i];
			M[3][i] = (float)pos[i];
			M[i][3] = 0.0;
		}
		M[3][3] = 1.0;

		Vector3d a(pt);

		pt.x = a.x * M[0][0] + a.y * M[1][0] + a.z * M[2][0] + M[3][0];
		pt.y = a.x * M[0][1] + a.y * M[1][1] + a.z * M[2][1] + M[3][1];
		pt.z = a.x * M[0][2] + a.y * M[1][2] + a.z * M[2][2] + M[3][2];
	}

	void LWItem::transformToLocal(Vector3d& pt, LWTime t)
	{
		double M[4][4];
		LWDVector pos, rt, up, fd;
		//Param(LWIP_W_POSITION, t, pos);
		Param(LWIP_W_RIGHT, t, rt);
		Param(LWIP_W_UP, t, up);
		Param(LWIP_W_FORWARD, t, fd);

		for (int i = 0; i < 3; i++) {
			M[0][i] = (float)rt[i];
			M[1][i] = (float)up[i];
			M[2][i] = (float)fd[i];
			M[3][i] = 0.0f;
		}
		Vector3d a(pt);
		pt.x = a.x * M[0][0] + a.y * M[1][0] + a.z * M[2][0] + M[3][0];
		pt.y = a.x * M[0][1] + a.y * M[1][1] + a.z * M[2][1] + M[3][1];
		pt.z = a.x * M[0][2] + a.y * M[1][2] + a.z * M[2][2] + M[3][2];
	}

	/*!
	 * Search all LW Items for an item with this name, and return a LWItem.
	 */
	LWItem* ItemInfo::FindItem(const std::string& itemName, LWItemType type)
	{
		return new LWItem(FindItemID(itemName, type));
	}
	/*!
	 * Search all LW Items for an item with this name, and return the Item ID.
	 */
	LWItemID ItemInfo::FindItemID(const std::string& itemName, LWItemType type)
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
			count++;
		return count;
	}

	LWItem* ItemInfo::GetItemN(LWItemType type, int n)
	{
		int count = 0;
		// Loop through the entire list of items.
		for (LWItemID id = first(type, LWITEM_NULL); id != LWITEM_NULL; id = next(id), count++)
			if (count == n) return new LWItem(id);
		// No match was found, return 0.
		return new LWItem();
	}

	int ItemInfo::countServer(const char* type, const char* name, LWItemID id)
	{
		int index = 1;
		int count = 0;
		while (const char* srv = server(id, type, index))
		{
			if (strcmp(srv, name) == 0) ++count;
			index++;
		}
		return count;
	}


	int ItemInfo::findServer(const char* type, const char* name, LWItemID id)
	{
		int index = 1;
		while (const char* srv = server(id, type, index))
		{
			if (strcmp(srv, name) == 0) return index;
			index++;
		}
		return 0;
	}

	void ItemInfo::editServer(const char* type, const char* name, LWItemID id)
	{
		int index = findServer(type, name, id);
		if (index == 0)
		{
			addUniqueServer(type, name, id);
			index = findServer(type, name, id);
		}
		std::ostringstream cmd;
		cmd << "EditServer " << type << " " << index;
		lwpp::SendCommand(cmd.str());
	}

	void ItemInfo::addUniqueServer(const char* type, const char* name, LWItemID id)
	{
		if (findServer(type, name, id) > 0) return;
		std::ostringstream cmd;
		cmd << "ApplyServer " << type << " " << name;
		lwpp::SendCommand(cmd.str());
	}

	void ItemInfo::removeServer(const char* type, const char* name, LWItemID id)
	{
		auto idx = findServer(type, name, id);
		if (idx == 0) return;
		std::ostringstream cmd;
		cmd << "RemoveServer " << type << " " << idx;
		lwpp::SendCommand(cmd.str());
	}

  void ItemInfo::enableServer(const char* type, const char* name, bool state, LWItemID id)
  {
    int index = findServer(type, name, id);
    if (index == 0)
      return;

    std::ostringstream cmd;
    if (id)
      cmd << "EnableServerByItemID " << std::hex << id << " " << type << " " << index << (state ? " 1" : " 0");
    else
      cmd << "EnableServer " << type << " " << index << (state ? " 1" : " 0");
    lwpp::SendCommand(cmd.str());
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

	LWError LWItem::Save(const SaveState& ss)
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
			l = (unsigned int)jic;
			LWSAVE_U4(ss.getState(), &l, 1);
		}
		LWSAVE_END(ss.getState());

		//! - Item type
		LWSAVE_BEGIN(ss.getState(), &item_root[2], 1);
		l = (unsigned int)mType;
		LWSAVE_U4(ss.getState(), &l, 1);
		LWSAVE_END(ss.getState());

		return err;
	}

	/*!
	 * Loading an item is safe in Modeler as well.
	 * In Layout, lwpp will first try to find an item with a matching name. If that fails
	 * it will use the mID that was stored to reference the item.
	 *
	 * In Modeler, lwpp will just store the item name and mID and the use that for the next save.
	 */
	LWError LWItem::Load(const LoadState& ls)
	{
		LWError err = 0;
		unsigned int l;
		while (LWID id = ls.Find(item_root))
		{
			switch (id)
			{
				case IO_ITEM: //! - Item name
					m_itemName = ls.readString();
					break;
				case IO_ITID: //! - Item ID
					ls.read(l);
					mId = INT2LWITEMID(l);
					break;
				case IO_ITTY: //! - Item type
					ls.read(l);
					mType = (LWItemType)l;
					break;

				default:
					break;
			}
			ls.End();
		}
		if (!lwpp::LightWave::isModeler())
		{
			LWItemID _id = mItemInfo.FindItemID(m_itemName, mType); // find matching item if possible
			if (_id != LWITEM_NULL) mId = _id; // only use the loaded mID if the object has not been found
		}
		return err;
	}

	/*
	* PrettyItem
	*/

	static const char* typeIcons[] = { "\03(i:160)", "\03(i:162)", "\03(i:163)", "\03(i:161)" };

	PrettyItem::PrettyItem() : cm(lwpp::lwcst_viewer) {}
	std::string PrettyItem::format(LWItemID id, int indent)
	{
		lwpp::LWItem item(id);
		return format(item, indent);
	}

	std::string PrettyItem::format(lwpp::LWItem& item, int indent)
	{
		float rgba[4];
		ii.getItemColour(item.GetID(), rgba);
		//bool visible = (ii.itemVisibility(item.GetID()) == LWIVIS_VISIBLE);
		cm.convertToColourSpace(rgba);
		name.str("");
		name.clear();
		auto icon = typeIcons[item.Type()];
		if (item.Type() == LWI_OBJECT)
		{
			oi.setObject(item.GetID());
			if (oi.numPoints() == 0) icon = "\03(i:190)";
		}
		name << "\03(g:" << indent * 12 << ")"
			<< "\03(c: " << rgba[0] * 256 << "," << rgba[1] * 256 << "," << rgba[2] * 256 << ")"
			<< icon
			//<< visible ? "\03(c:Nc_LABEL) " : "\03(c:Nc_HILIGHT) "
			<< "\03(c:Nc_LABEL) "
			<< item.getName();
		return name.str();
	}


	/*
	* StyledItemCallback
	*/

	bool filterAll(lwpp::LWItem& item) { return true; }

	void LWItemCallback::buildItemTree(lwpp::LWItem parent, int indent)
	{
		lwpp::LWItem item(parent.getFirstChild());
		while (item.exists())
		{
			if (filter && (*filter)(item))
			{
				if (mDisplayType == -1 || item.Type() == mDisplayType)
					itemList.push_back(itemEntry(item.GetID(), pretty.format(item, indent)));
#ifdef _DEBUG
					//for (int i = 0; i < indent; ++i) dout << "  ";
					//dout << item.getName() << "\n";
#endif
			}
			if (item.hasChildren())
				buildItemTree(item, indent + 1);
			item.NextChild(parent);
		}
	}

	void LWItemCallback::buildItemTree()
	{
		for (auto t = 0; t < 3; ++t)
		{
			lwpp::LWItem item(t);
			while (item.exists())
			{
				if (!item.Parent().exists())
				{
					if (filter && (*filter)(item))
					{
						if (mDisplayType == -1 || item.Type() == mDisplayType)
							itemList.push_back(itemEntry(item.GetID(), pretty.format(item)));
#ifdef _DEBUG
							//dout << item.getName() << "\n";
#endif
					}
					if (item.hasChildren())
						buildItemTree(item, 1);
				}
				item.Next();
			}
		}
	}

	size_t LWItemCallback::popCount()
	{
		itemList.clear();
		buildItemTree();
		return itemList.size() + (showNone ? 1 : 0); // add one for the (none) case
	}

	const char* LWItemCallback::popName(int idx)
	{
		if (lwpp::LightWave::isModeler()) return noneName; // No items in Modeler
		if (showNone)
		{
			if (idx == 0)	return noneName;
			--idx;
		}
		if (idx >= itemList.size()) return "";
		return itemList[idx].name.c_str();
	}

	/*
	 * LWItemTracker
	 */
	LWItemTracker::~LWItemTracker(void)
	{
		;
	}

	void LWItemTracker::refresh()
	{
		mUsedItems.clear();			
		for (auto item : mTrackedItems)
		{
			if (item->exists())	 
				mUsedItems.push_back(item->GetID());
		}
		mUsedItems.push_back(LWITEM_NULL);
		mUsedItems.shrink_to_fit();
	}

	/*!
	*  @note, the item must persist for the duration of this class, at least for as long as Refresh may be called
	*/
	void LWItemTracker::trackItem(LWItem* item)
	{
		if (item == 0) return;
		mTrackedItems.push_back(item);
	}
	void LWItemTracker::trackItem(LWItem& item)
	{
		//if (!item.exists()) return;
		mTrackedItems.push_back(&item);
	}

	void LWItemTracker::unTrackItem(LWItem& item)
	{
		//if (!item.exists()) return;
		std::vector<LWItem*>::iterator iter = find(mTrackedItems.begin(), mTrackedItems.end(), &item);
		if (iter != mTrackedItems.end())
		{
			mTrackedItems.erase(iter);
		}
	}

	void LWItemTracker::unTrackItem(LWItem* item)
	{
		if (item == 0) return;
		std::vector<LWItem*>::iterator iter = find(mTrackedItems.begin(), mTrackedItems.end(), item);
		if (iter != mTrackedItems.end())
		{
			mTrackedItems.erase(iter);
		}
	}

	const LWItemID* LWItemTracker::useItems()
	{
		refresh();
		return &mUsedItems[0];
	}

  void LWItemTracker::clear()
  {
    
    mTrackedItems.clear();
  }

	/*
* Optionally LWITEM_NULL or LWITEM_ALL can be returned manually instead of the return of this function
* @note This relies on a properly Refresh'd list.
*/
	void LWItemTracker::changeID(const LWItemID* idlist)
	{
		if (idlist == 0) return;
		for (unsigned int i = 0; i < mTrackedItems.size(); ++i)
		{
			mTrackedItems[i]->changeID(idlist);
		}
	}
} // end namespace lwpp
