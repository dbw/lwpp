#ifndef COMRING_COMMUNICATOR_H
#define COMRING_COMMUNICATOR_H

#include <lwpp/global.h>
#include <lwcomring.h>
#include <string>

namespace lwpp
{
	//! Class to add support for the Communication Ring to plugins
	//! @ingroup Entities
	/*!
	 * You must inherit this class in EVERY plugin where you'd want to use a ComRing
	 */
	class comRingCommunicator : public GlobalBase<LWComRing> // needs to be declared as a global properly in global.cpp
	{
		std::string lastTopic;
	protected:
    //! Receive an event in the ComRing
		/*!
     * This needs to be implemented by the derived class if you want to receive events.
     */
		virtual void RingEvent(void *portData, int eventCode, void *eventData) 
		{
			return;
		}
		//! The static callback for LightWave
		static void RingEventCB(void *clientData, void *portData,int eventCode,void *eventData) 
		{
			// cast the instance back to the base class 
			comRingCommunicator *plugin = static_cast<comRingCommunicator *>(clientData); 
			// pass through the remaining arguments to the function
			plugin->RingEvent(portData, eventCode, eventData); 
		}
	public:
		comRingCommunicator() {;}
    /*!
     * @param[in] topic The name/topic of the ComRing to attach to
     */
		comRingCommunicator(const char *topic) 
		{
			if (available())
			{
				if (ringAttach (topic))
				{
					;
				}
			}
		}
		virtual ~comRingCommunicator()
		{
			if (!lastTopic.empty())	ringDetach (lastTopic.c_str());
		}
		//! Attach to a ComRing to receive messages
		bool ringAttach (const char *topic)
		{
			if (globPtr->ringAttach(const_cast<char *>(topic), this, RingEventCB))
			{
				lastTopic = topic;
				return true;
			}
			return false;
		}
		
		//! Detach from a comring to stop receiving messages
		void ringDetach (const char *topic)
		{
			globPtr->ringDetach(const_cast<char *>(topic), this);
		}
		
		//! Send a message to a ComRing
		void ringMessage (const char *topic,int eventCode,void *eventData = 0)
		{
			if (globPtr) globPtr->ringMessage(const_cast<char *>(topic), eventCode, eventData);
		}
	};

#ifndef LW_PLUGIN_LIMBO_STATES // defined starting with the 9.6 SDK, not documented though
#define LW_PLUGIN_LIMBO_STATES "LW Plugin Limbo States"

#define LW_LIMBO_START 0x00000001
#define LW_LIMBO_END   0x00000002
#endif

  //! Simple helper class for the limbo state
  /*!
   * Construct the class before requiring a limbo state, the destructor will automatically clear the state again.
   *
   * The limbo state mainly pauses VPR.
   */
  class LimboComRing : private comRingCommunicator
  {
  public:
    LimboComRing()
    {
      ringMessage(LW_PLUGIN_LIMBO_STATES, LW_LIMBO_START);
    }
    ~LimboComRing()
    {
      ringMessage(LW_PLUGIN_LIMBO_STATES, LW_LIMBO_END);
    }
  };

} // end namespace
#endif // COMRING_COMMUNICATOR_H