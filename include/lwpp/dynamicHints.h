#pragma once
#ifndef LWPP_DYNAMIC_HINTS
#define LWPP_DYNAMIC_HINTS

#include <lwpp/xpanel.h>
#include <vector>

namespace lwpp
{
	class DynamicHints
	{
		std::vector<LWXPanelHint> mHints;
	public:
		~DynamicHints() { LWPP_DBG_ENTER_FUNC;}
		DynamicHints & operator,(LWXPanelHint hint)
		{
			mHints.push_back(hint);
			return *this;
		}
		DynamicHints & operator<<(LWXPanelHint hint)
		{
			mHints.push_back(hint);
			return *this;
		}
		DynamicHints & operator,(int hint)
		{
#pragma warning( suppress : 4312 ) 
			mHints.push_back(reinterpret_cast<LWXPanelHint>(hint));
			return *this;
		}
		DynamicHints & operator<<(int hint)
		{
#pragma warning( suppress : 4312 ) 
			mHints.push_back(reinterpret_cast<LWXPanelHint>(hint));
			return *this;
		}
		DynamicHints & operator+=(DynamicHints &hints)
		{			
			mHints.insert(mHints.end(), hints.mHints.begin(), hints.mHints.end());
			return *this;
		}
		LWXPanelHint *get()
		{
			if (mHints.empty()) return nullptr;
			return &mHints[0];
		}
		void clear()
		{
			mHints.clear();
		}
	};

	struct DynControl
	{
		const char *mLabel, *mCtrlclass, *mDatatype;
		unsigned int mCid;
		DynControl(unsigned int cid = 0, const char *label = nullptr, const char *ctrlclass = nullptr, const char *datatype = nullptr)
			: mCid(cid), mLabel(label), mCtrlclass(ctrlclass), mDatatype(datatype)
		{}
	};


	class DynamicControlData
	{
		typedef LWXPanelControl * pLWXPanelControl;
		typedef LWXPanelDataDesc * pLWXPanelDataDesc;

		std::vector<LWXPanelControl> mControls, mControlStore;
		std::vector<LWXPanelDataDesc> mDataDesc, mDataStore;
		bool dirty = true;

		void createStore()
		{
			if (!dirty) return;
			mControlStore = mControls;
			mDataStore = mDataDesc;
			LWXPanelControl ctrl = { 0, 0, 0 };
			mControlStore.push_back(ctrl);
			LWXPanelDataDesc desc = { 0, 0, 0 };
			mDataStore.push_back(desc);
			dirty = false;
		}
	public:
		~DynamicControlData()
		{
			LWPP_DBG_ENTER_FUNC;
		}
		DynamicControlData &Add(unsigned int cid, const char *label, const char *ctrlclass, const char *datatype = nullptr)
		{
			LWXPanelControl ctrl = { cid, label, ctrlclass };
			mControls.push_back(ctrl);
			LWXPanelDataDesc desc = { cid, label, datatype };
			mDataDesc.push_back(desc);
			dirty = true;
			return *this;
		}
		// add a datatype but no control
		DynamicControlData &Add(unsigned int cid, const char *datatype)
		{
			LWXPanelDataDesc desc = { cid, "", datatype };
			mDataDesc.push_back(desc);
			dirty = true;
			return *this;
		}

		void Add(LWXPanelControl ctrl[])
		{
			LWXPanelControl *c = ctrl;
			while (c && c->cid)
			{
				Add(c->cid, c->label, c->ctrlclass);
				c++;
			}
		}

		void Clear()
		{
			mControls.clear();
			mDataDesc.clear();
			dirty = true;
		}

		bool isEmpty() const
		{
			return (mControls.empty() && mDataDesc.empty());
		}

		LWXPanelControl *getControl()
		{
			createStore();
			return &mControlStore[0];
		}

		std::vector<LWXPanelControl> &getControlStore()
		{
			createStore();
			return mControlStore;
		}

		LWXPanelDataDesc *getDataDesc()
		{
			createStore();
			return &mDataStore[0];
		}
	};
}

#endif // LWPP_DYNAMIC_HINTS
