/*!
 * @file
 * @brief Threading tools for LightWrap++
 */
#ifndef LWPP_THREADS_H
#define LWPP_THREADS_H

#include "lwmtutil.h"
#include "lwpp/global.h"

namespace lwpp 
{

	//! Mutex ids. Use these since LW only provides us with 10 mutexes
	enum MutexID {MUTEX0 = 0, MUTEX1, MUTEX2, MUTEX3, MUTEX4, MUTEX5, MUTEX6, MUTEX7, MUTEX8, MUTEX9};

	//! @ingroup Globals
	class MTUtilBase : public GlobalBase<LWMTUtilFuncs>
	{
	public:
		MTUtilBase(void) {	;	}
		virtual ~MTUtilBase() {;}
		virtual int Lock (const MutexID mutex) const = 0;
		virtual int UnLock (const MutexID mutex) const = 0;
	};

	class LWMTUtil_Pre9 : public MTUtilBase
	{
	private:
		LWMTUtilID	id;
	public:
		LWMTUtil_Pre9(void) : id(0)
		{
			id = globPtr->create();
		}
		virtual ~LWMTUtil_Pre9()
		{
			if (id)	globPtr->destroy(id);
		}
		virtual int Lock (const MutexID mutex) const
		{
			return globPtr->lock(id, mutex);
		}
		virtual int UnLock (const MutexID mutex) const
		{
			return globPtr->unlock(id, mutex);
		}
	};

	//! @ingroup Globals
	class LWMTUtil9 : public MTUtilBase
	{
	private:
		LWMTGroupID mtgroup;
	public:
		LWMTUtil9(void) : mtgroup(0)
		{
			mtgroup = globPtr->groupCreate(1);
		}
		virtual ~LWMTUtil9()
		{
			if (mtgroup) globPtr->groupDestroy(mtgroup);
		}
		virtual int Lock (const MutexID mutex) const
		{
			return globPtr->groupLockMutex(mtgroup, mutex);
		}
		virtual int UnLock (const MutexID mutex) const
		{
			return globPtr->groupUnlockMutex(mtgroup, mutex);
		}
	};

	//! Wrap of LWMTUtilFuncs - management of multi-thread mutexes (mutices?)
	//! @ingroup Globals
	class LWMTUtil
	{
	private:
		MTUtilBase	*MTUtil;
	public:
		LWMTUtil() : MTUtil(0)
		{
			if (lwpp::LightWave::Major() > 8)
				MTUtil = new LWMTUtil9;
			else
				MTUtil = new LWMTUtil_Pre9;
		}
		virtual ~LWMTUtil()
		{
			if (MTUtil)	delete MTUtil;
		}
		virtual int Lock (const MutexID mutex) const
		{
			return MTUtil->Lock(mutex);
		}
		virtual int UnLock (const MutexID mutex) const
		{
			return MTUtil->UnLock(mutex);
		}
	};

	//! Automatic mutex to protect a complete function. Should get created and destroyed on the heap
	//! @ingroup Globals
	class AutoMutex {
			MutexID id;
			LWMTUtil *mt_util;
		public:
			//! Construct the object and lock a mutex
			/*!
			 * @param *u The LWMTUtil to use
			 * @param m The MutexID to lock
			 */
			AutoMutex(LWMTUtil *u, const MutexID m) :
				mt_util(u),
				id(m)
			{
				mt_util->Lock(id);
			}
			AutoMutex(LWMTUtil *u, int m) :
				mt_util(u),
				id((MutexID)m)
			{
				mt_util->Lock(id);
			}
				//! Destroy the object and release the mutex
			virtual ~AutoMutex()
			{
				mt_util->UnLock(id);
			}
	};

	//typedef int (*LWMTThreadFunc)(void *arg);
	//! @ingroup Globals
	class ThreadUtils : public GlobalBase<LWMTUtilFuncs>
	{
	public:
		LWMTThreadID getID() 
		{
			return globPtr->threadGetID();
		}

		void setData(void* ptr) 
		{
			globPtr->threadSetData(ptr);
		}

		void* getData() 
		{
			return globPtr->threadGetData();
		}

		void* getArg() 
		{
			return globPtr->threadGetArg();
		}

    void* getArgByID(LWMTThreadID thrdid) 
		{
			return globPtr->threadGetArgByID(thrdid);
		}

    int getIndex()
		{
			return globPtr->threadGetIndex();
		}

    int getIndexByID(LWMTThreadID thrdid)
		{
			return globPtr->threadGetIndexByID(thrdid);
		}

    int getThreadCount(LWMTThreadID thrdid)
		{
			return globPtr->threadGetThreadCount(thrdid);
		}

    LWMTGroupID getGroupID(LWMTThreadID thrdid)
		{
			return globPtr->threadGetGroupID(thrdid);
		}

    int checkAbort(void)
		{
			return globPtr->threadCheckAbort();
		}

    int checkAbortByID(LWMTThreadID thrdid)
		{
			return globPtr->threadCheckAbortByID(thrdid);
		}

    void sleep(int delay)
		{
			globPtr->threadSleep(delay);
		}
    void setArg(void* ptr)
		{
			globPtr->threadSetArg(ptr);
		}
    void setIndex(int in_index)
		{
			globPtr->threadSetIndex(in_index);
		}
	};

	//! @ingroup Globals
	/*!
		Base class for threads running within LightWave 3D

		To use derive your class from it and implement run().
		Add the Thread using ThreadGroup.

		@sa ThreadGroup
	 */
	class Thread : public GlobalBase<LWMTUtilFuncs>
	{
	protected:
		//! Returns the identifier of the currently running thread.
		LWMTThreadID getID() 
		{
			return globPtr->threadGetID();
		}
		//! Sets the private data storage for the currently running thread. 
		void setData(void* ptr) 
		{
			globPtr->threadSetData(ptr);
		}
		//! Gets the private data storage for the currently running thread. 
		void* getData() 
		{
			return globPtr->threadGetData();
		}
		//! Returns the group index value of the current thread. 
    int getIndex()
		{
			return globPtr->threadGetIndex();
		}
		//! Allows the thread to retrieve the identifier for the group to which it belongs. 
    LWMTGroupID getGroupID()
		{
			return globPtr->threadGetGroupID(getID());
		}

		//! This function allows the current or identified thread to check for an abort signal from the main group.
		/*!
		If this function returns a true value (!0), the thread should finish up (release resources, close files, etc.), 
		and gracefully terminate. 
		*/
    bool checkAbort(void)
		{
			return (globPtr->threadCheckAbort() != 0);
		}
		//! Causes the thread to "sleep" for a specified number of milliseconds.
		/*!
		In most cases, this causes the thread to release its timeslice, avoiding potential hammering of the CPU(s). 
		*/
    void sleep(int delay)
		{
			globPtr->threadSleep(delay);
		}
		//! The actual function to run, to be overriden by the derived class
		virtual int Run() = 0;
	public:
		static int LWMTThreadFunc(void *arg)
		{
			Thread *func = static_cast<Thread *>(arg);
			return func->Run();
		}
	};

	/*!
	@ingroup Globals
	Cross-platform threading support is provided by the functions in the following sections.
	Threads are powerful tools for the developer that allow simultaneous paths of execution to exist within the same application.
	On single-core, single-processor systems, threads typically execute synchronously.
	However, on multi-processor (or multi-core) systems, threads will often execute in true parallel,
	allowing the application to be more responsive than would be allowed by a single-path execution model.

	@note These functions only provide the mechanism for multi-threading.
	What you do within the threads you create is entirely your concern.
	The responsibility lies with you, the developer, to ensure that the functions you call
	from within your thread that lead to paths outside your thread are safe to be called in that fashion (i.e., "thread-safe").
	The practice of calling LightWave® Plug-In API functions from within a thread is not officially supported.
	Please be aware that such a practice is undertaken by you at your own risk.
	*/
	class ThreadGroup : public GlobalBase<LWMTUtilFuncs>
	{
		LWMTGroupID mtgrpid;
	public:
		/*!
		Threads in LightWave® are contained in, and managed by, "thread groups".
		Each thread group can contain any number of threads, and must be created with the number of threads to be managed provided in count. 
		*/
		ThreadGroup(int _count)
			: mtgrpid(0)
		{
			mtgrpid = globPtr->groupCreate(_count);
		}
		
		//! Destroys the thread group created by createGroup().
		/*!
		If active threads exist in the group when it is destroyed, they will be forcibly stopped.
		*/
		~ThreadGroup()
		{
			if (mtgrpid) 
				globPtr->groupDestroy(mtgrpid);
			mtgrpid = 0;
		}
		//! Attempts to gain a lock on the group's indicated mutex.
		/*!
		Groups contain ten mutexes, numbered from 0 to 9.
		The call will block until the mutex is locked. 
		*/
    int lockMutex(int mutexID)
		{
			if (mtgrpid) 
				return globPtr->groupLockMutex(mtgrpid, mutexID);
			return 0;
		}
		//! Releases a lock on a group's mutex.
    int unlockMutex(int mutexID)
		{
			if (mtgrpid) 
				return globPtr->groupUnlockMutex(mtgrpid, mutexID);
			return 0;
		}
		//! Adds thread functions to the thread group.
		/*!
		The function to be added must match the LWMTThreadFunc signature.
		Each thread must accept a single void * argument, which is a pointer to the thread argument data argvalue.

		This thread argument can be specified by the argvalue parameter.
		If argsize is greater than zero (i.e., it specifies the size of the data pointed to by argvalue),
		then the value pointed to by argvalue will be passed to the thread fuction by value (a copy is made).
		On the other hand, if argsize is zero, then argvalue will be passed to the thread function by reference
		(i.e., the actual pointer will be provided).

		As threads are added to the group, they are maintained in a suspended state.
		Threads do not start until the group launches them with one of the execution functions (see below). 
		*/
    LWMTThreadID addThread(LWMTThreadFunc func, int size, void *arg)
		{
			if (mtgrpid) 
			{
				return globPtr->groupAddThread(mtgrpid, func, size, arg);
			}
			return 0;
		}
    LWMTThreadID addThread(Thread *func)
		{
			return addThread(Thread::LWMTThreadFunc, 0, func);
		}
		//! Given a thread index (thrdndx), this function will return that thread's identifier. 
    LWMTThreadID getThreadID(int in_index)
		{
			if (mtgrpid) 
			{
				return globPtr->groupGetThreadID(mtgrpid,in_index);
			}
			return 0;
		}
		//! Returns the number of threads the specified group is managing.
    int getThreadCount()
		{
			if (mtgrpid) 
			{
				return globPtr->groupGetThreadCount(mtgrpid);
			}
			return 0;
		}
		//! Causes the threads managed by the group to leave their suspended state, and begin execution (simultaneously, blocking).
		/*!
		This function blocks until all threads complete their processing (i.e., terminate).
		@return true upon success and false if there was an error.
		*/
    bool run()
		{
			if (mtgrpid) 
			{
				return (globPtr->groupRun(mtgrpid) != 0);
			}
			return false;
		}
		//! Causes the threads managed by the group to leave their suspended state, and begin execution (simultaneously, non-blocking).
		/*!
		This function does not block, returning after all threads are started (or an error occurs).
		@return true upon success and false if there was an error. 
		*/
    bool begin()
		{
			if (mtgrpid) 
			{
				return (globPtr->groupBegin(mtgrpid) != 0);
			}
			return false;
		}
		//! Once started by groupBegin, this function will block the calling process until all active threads in the group complete.
		/*!
		Calling groupBegin followed immediately by groupSync is therefore functionally equivalent to a single call to groupRun. 
		*/
    void sync()
		{
			if (mtgrpid) 
			{
				globPtr->groupSync(mtgrpid);
			}
		}
		//! Signals to all active threads in the group that it is time to stop.
		/*!
		This is a cooperative function -- the threads must periodically check for the abort condition, and then terminate. 
		*/
    void abort()
		{
			if (mtgrpid) 
			{
				globPtr->groupAbort(mtgrpid);
			}
		}
		//! This function summarily terminates all active threads in the group.
		/*!
		Threads are not afforded the opportunity for graceful termination. 
		*/
    void kill()
		{
			if (mtgrpid) 
			{
				globPtr->groupKill(mtgrpid);
			}
		}

		//! This query function tests to see if all the threads in the group have terminated. 
    bool isDone()
		{
			if (mtgrpid) 
			{
				return (globPtr->groupIsDone(mtgrpid) != 0);
			}
			return true;
		}

		//! This query function tests to see if the threads in the group were aborted for some reason.
    bool isAborted()
		{
			if (mtgrpid) 
			{
				return (globPtr->groupIsAborted(mtgrpid) != 0);
			}
			return true;
		}

		//! The result (i.e, the return code) of an individual thread can be queried using this function. 
    int threadResult(int in_index)
		{
			if (mtgrpid) 
			{
				return globPtr->groupThreadResult(mtgrpid, in_index);
			}
			return 0;
		}
	};
}

#endif // LWPP_THREADS_H