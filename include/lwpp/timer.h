#ifndef LWPP_TIMER_H
#define LWPP_TIMER_H

#include <lwpp/global.h>
#include <lwtimer.h>

namespace lwpp
{
	//!	Adds timer based events to LightWave.
	/*!
	 * The functions expect the adress of a class that implements a member function with the signature "bool TimerEvent()".
	 * This functions returns true if it should be removed, false if it shall be called again after the next interval.
	 */
	class Timer : public GlobalBase<LWTimer>
	{
		template <typename T>
		static int CB_TimerProc(void *data)
		{
			if (data)
			{
				T *func = static_cast<T *>(data);
				return (func->TimerEvent() ? 1 : 0);
			}
			return 1; // remove event
		}
	public:
    /*!
     * @param *host callback class for timer expiration. This must have a bool TimerEvent() function implemented
     * @param interval timeout value expressed in milliseconds
     */
		template <typename T>
		void addTimer(T *host, unsigned long interval)
		{
			globPtr->timerAdd(CB_TimerProc<T>, host, interval);
		}
    /*!
     * Timers can be explicitly removed by calling the following function and
     * specifying the 'host' as well as an optional interval.  If interval
     * is 0, then the first instance of a timer with 'proc' as its callback will be
     * removed. If interval is >0, then only the 'proc' instance with that interval
     * will be removed.
     *
     * Unless the timer instance implicitly removes itself by returning 1 from its
     * callback function, each call to timerAdd() MUST HAVE a corresponding call to
     * timerRemove(),  Otherwise you run the risk of leaving a dangling function
     * pointer within Layout, which can (and probably will) eventually lead to
     * a crash of the application.
     *
     * @param *host callback class for timer expiration. This must have a int TimerEvent() function implemented
     * @param interval timeout value expressed in milliseconds (same used for timerAdd())
     */
		template <typename T>
		void removeTimer(T *host, unsigned long interval = 0)
		{
			globPtr->timerRemove(CB_TimerProc<T>, interval);
		}
	};
} // namespace lwpp

#endif // LWPP_TIMER_H