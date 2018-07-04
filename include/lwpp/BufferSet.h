#ifndef LWPP_BUFFERSET_H

#define LWPP_BUFFERSET_H

#include <lwbufferset.h>
#include <lwpp/global.h>

namespace lwpp 
{

	//typedef int LWBufferEventFunc(void *userData, void *bufferSet, unsigned int event, void *eventData);

	//! Information about a single buffer
	class BufferRecord : private GlobalBase<LWBufferSetGlobal>
	{
	private:
		LWBufferListID mID = 0;
	public:
		BufferRecord(LWBufferListID id = 0) :
			mID(id)
		{	}
		BufferRecord(const BufferRecord &from)
		{
			mID = from.mID;
		}
		~BufferRecord() {}
		BufferRecord &operator= (const BufferRecord &from)
		{
			if (this != &from)
			{
				mID = from.mID;
			}
			return *this;
		}
		bool available() const
		{
			return (mID != 0);
		}

		LWBufferListID getID() const
		{
			return mID;
		}

		const char *getName()
		{
			return globPtr->getBufferName(mID);
		}
		int getFlags()
		{
			return globPtr->getBufferFlags(mID);
		}
		bool getFlag(int flag)
		{
			return ((getFlags() & flag) == flag);
		}
		void setFlags(int flags)
		{
			globPtr->setBufferFlags(mID, flags);
		}
		void setFlag(int flag, bool set)
		{
			int flags = (getFlags() & ~flag);
			flags |= (set ? flag : 0);
			setFlags(flags);
		}
		const char *getCustomName()
		{
			return globPtr->getCustomName(mID);
		}
		void setCustomName(const char *name)
		{
			globPtr->setCustomName(mID, name);
		}
		const char *getSaveFormat()
		{
			return globPtr->getSaveFormat(mID);
		}
		void setSaveFormat(const char *name)
		{
			globPtr->setSaveFormat(mID, name);
		}
		const char *getColourSpace()
		{
			return globPtr->getColorSpace(mID);
		}
		void setColourSpace(const char *name)
		{
			globPtr->setColorSpace(mID, name);
		}
		const char *getOutputPath()
		{
			return globPtr->getOutputPath(mID);
		}
		void setOutputPath(const char *name)
		{
			globPtr->setOutputPath(mID, name);
		}
		const char *getFilename()
		{
			return globPtr->getFilename(mID);
		}
		void setFilename(const char *name)
		{
			globPtr->setFilename(mID, name);
		}
		int getReconstructionFilter()
		{
			return globPtr->getReconstructionFilter(mID);
		}
		void setReconstructionFilter(int filter)
		{
			globPtr->setReconstructionFilter(mID, filter);
		}
		LWBufferSetID getLightBufferSet()
		{
			return globPtr->lightBufferSet(mID);
		}
		bool isLightBuffer()
		{
			return (getLightBufferSet() != 0);
		}
	};

	class BufferSet : private GlobalBase<LWBufferSetGlobal>
	{
	protected:
		LWBufferSetID mID = 0;
		bool mDestroy = false;
		void *mUserData = 0;
		LWBufferSetID create(int set_type)
		{
			return globPtr->create(set_type);
			mDestroy = (set_type != 1);
		}
		void destroy(LWBufferSetID id)
		{
			if (mDestroy) globPtr->destroy(id);
		}
	public:
		BufferSet(int set_type = -1)
		{
			mID = create(set_type);
		}
		BufferSet(LWBufferSetID id)
			: mID(id)
		{
			mDestroy = false;
		}
		~BufferSet()
		{
			clearBufferEvent();
			if (mID != 0)
			{
				destroy(mID);
				mID = 0;
			}
		}

		bool available() const
		{
			return (mID != 0);
		}

		void rebase()
		{
			globPtr->rebase(mID);
		}

		int numberOfGroups()
		{
			return globPtr->numberOfGroups(mID);
		}
		int numberOfItems(int group)
		{
			return globPtr->numberOfItems(mID, group);
		}

		BufferRecord getBufferRecord(int group, int item)
		{
			return BufferRecord(globPtr->bufferRecord(mID, group, item));
		}

		BufferRecord findBufferRecord(const char *name)
		{
            return findBufferRecord(std::string(name));
		}

		BufferRecord findBufferRecord(const std::string &name)
		{
			int groups = numberOfGroups();
			for (int group = 0; group < groups; group++) 
			{
				int items = numberOfItems(group);
				for (int item = 0; item < items; item++)
				{
					auto buffer = globPtr->bufferRecord(mID, group, item);
					if (buffer)
					{
						if (name == globPtr->getBufferName(buffer))
						{
							return BufferRecord(buffer);
						}
					}
				}
			}
			return BufferRecord();
		}

		const char *groupName(LWBufferSetID id, int group)
		{
			return globPtr->groupName(id, group);
		}

		const char *groupName(int group)
		{
			return globPtr->groupName(mID, group);
		}


		int clearBufferEvent()
		{
			if (mUserData)
			{ 
				return globPtr->setBufferEvent(mID, 0, mUserData);
			}
			return 0;
		}

		int setBufferEvent(LWBufferEventFunc *userFunc, void *userData)
		{
			clearBufferEvent();
			mUserData = userData;
			return globPtr->setBufferEvent(mID, userFunc, userData);
		}

		int sendBufferEvent(int eventCode)
		{
			return globPtr->sendBufferEvent(mID, eventCode);
		}

	};

	template <class T>
	int LWBufferEventFuncCB(void *userData, void *bufferSet, unsigned int event, void *eventData)
	{
		if (userData == nullptr) return 0;
		T* host = static_cast<T *>(userData);
		return host->LWBufferEvent(bufferSet, event, eventData);
	}

	class LWBufferClient
	{
	public:
		BufferSet bufferFuncs;
		virtual int LWBufferEvent(void *bufferSet, unsigned int event, void *eventData) = 0;

		LWBufferClient()
		{
			//register the callback
			bufferFuncs.setBufferEvent(LWBufferEventFuncCB<LWBufferClient>, this);
		}
		virtual ~LWBufferClient()
		{
			bufferFuncs.setBufferEvent(nullptr, this); // remove the callback
		}
	};
}

#endif // !LWPP_BUFFERSET_H
