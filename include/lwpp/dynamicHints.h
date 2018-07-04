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
		}
		DynamicControlData &Add(unsigned int cid = 0, const char *label = nullptr, const char *ctrlclass = nullptr, const char *datatype = nullptr)
		{
			LWXPanelControl ctrl = { cid, label, ctrlclass };
			mControls.push_back(ctrl);
			LWXPanelDataDesc desc = { cid, label, datatype };
			mDataDesc.push_back(desc);
			dirty = true;
			return *this;
		}

		void Clear()
		{
			mControls.clear();
			mDataDesc.clear();
			dirty = true;
		}

		LWXPanelControl *getControl()
		{
			createStore();
			return &mControlStore[0];
		}

		LWXPanelDataDesc *getDataDesc()
		{
			createStore();
			return &mDataStore[0];
		}
	};
}

#endif // LWPP_DYNAMIC_HINTS
