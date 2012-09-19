#ifndef LWPP_MONITOR_H
#define LWPP_MONITOR_H

#include <lwpp/global.h>
#include <lwmonitor.h>

namespace lwpp
{

	class BaseMonitor 
	{
	public:
		virtual ~BaseMonitor(void)
		{
			;
		}		

		virtual void Setup(const std::string title, unsigned int flags = 0, const char *histfile = 0) = 0;

		virtual void init(unsigned int total, std::string message = std::string("Processing...")) = 0;

		virtual bool step(unsigned int incr = 1, std::string message = std::string("Processing...")) = 0;

		virtual bool step(std::string message)
		{
			return step(0, message);
		}

		virtual void done(void) = 0;

		virtual void close(void) = 0;
	};

	//! Wrapped LWLMonFuncs
	class Monitor : public BaseMonitor, protected GlobalBase<LWLMonFuncs>
	{
		LWLMonID id;
	public:
		Monitor(const std::string title, unsigned int flags = LMO_IMMUPD, const char *histfile = 0)
			:	id(0)
		{
			id = globPtr->create();
			if (id)	globPtr->setup(id, const_cast<char *>(title.c_str()), flags, histfile);
		}

		Monitor()
			:	id(0)
		{
			;
		}

		void Setup(const std::string title, unsigned int flags = LMO_IMMUPD, const char *histfile = 0)
		{
		  close();
      if (globPtr)
      {
			  id = globPtr->create();
			  if (id)	globPtr->setup(id, const_cast<char *>(title.c_str()), flags, histfile);
      }
     }

		void setwinpos(int x, int y, int w, int h)
		{
      if (globPtr) globPtr->setwinpos(id, x, y, w, h);
		}

		void init(unsigned int total, std::string message)
		{
			if (globPtr) globPtr->init(id, total, message.c_str());
		}

		void init(unsigned int total = 1)
		{
			if (globPtr) globPtr->init(id, total, 0);
		}

		bool step(unsigned int incr, std::string message)
		{
      if (globPtr) return (globPtr->step(id, incr, message.c_str()) == 1);
      return false;
		}
		bool step(unsigned int incr = 1)
		{
			if (globPtr) return (globPtr->step(id, incr, 0) == 1);
      return false;
		}
		bool step(std::string message)
		{
			if (globPtr) return (globPtr->step(id, 0, message.c_str()) == 1);
      return false;
		}

		void done(void)
		{
			if (id)
      {
        if (globPtr) globPtr->done(id);
      }
		}
		void close(void)
		{
			if (id) 
			{
				if (globPtr) globPtr->destroy(id);
				id = 0;
			}
		}
		~Monitor(void)
		{
			close();
		}
	};

	class SimpleMonitor 
	{
		private:
			LWMonitor *monitor;
		public:
			SimpleMonitor(LWMonitor *mon) : monitor(mon) {}
      SimpleMonitor(const SimpleMonitor &mon) : monitor(mon.monitor) {}
			virtual ~SimpleMonitor() 
			{
				Done();
			}
			void setMonitor (LWMonitor *mon)
			{
				monitor = mon;
			}
			void Init(int total)
			{
				if (monitor) monitor->init(monitor->data, total);
			}
			bool Step(int step = 1)
			{
				if (monitor) return (monitor->step(monitor->data, step) != 0);
        return true;
			}
			void Done()
			{
				if (monitor) monitor->done(monitor->data);
				monitor = 0;
			}
	};
}
#endif // LWPP_MONITOR_H