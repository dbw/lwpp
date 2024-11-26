#ifndef COMRING_COMMUNICATOR_H
#define COMRING_COMMUNICATOR_H

#include <lwpp/global.h>
#include <lwpp/utility.h>
#include <lwcomring.h>
#include <string>
#include <vector>

namespace lwpp
{
  //! Class to add support for the Communication Ring to plugins
  //! @ingroup Entities
  /*!
   * You must inherit this class in EVERY plugin where you'd want to use a ComRing
   */
  class comRingCommunicator  // needs to be declared as a global properly in global.cpp
  {
  protected:
    TransientGlobal<LWComRing> comRing;
    std::string lastTopic;
    //! Receive an event in the ComRing
    /*!
     * This needs to be implemented by the derived class if you want to receive events.
     * @param void *portData
     * @param int eventCode
     * @param void *eventData
     */
    virtual void RingEvent(void* portData, int eventCode, void* eventData)
    {
      return;
    }
    //! The static callback for LightWave
    static void RingEventCB(void* clientData, void* portData, int eventCode, void* eventData)
    {
      // cast the instance back to the base class 
      comRingCommunicator* plugin = static_cast<comRingCommunicator*>(clientData);
      // pass through the remaining arguments to the function
      plugin->RingEvent(portData, eventCode, eventData);
    }
  public:
    comRingCommunicator() { ; }
    /*!
     * @param[in] topic The name/topic of the ComRing to attach to
     */
    comRingCommunicator(const char* topic)
    {
      if (comRing.available())
      {
        if (ringAttach(topic))
        {
          ;
        }
      }
    }
    virtual ~comRingCommunicator()
    {
      if (!lastTopic.empty()) ringDetach(lastTopic.c_str());
    }
    //! Attach to a ComRing to receive messages
    bool ringAttach(const char* topic)
    {
      if (comRing.getGlobal()->ringAttach(const_cast<char*>(topic), this, RingEventCB))
      {
        lastTopic = topic;
        return true;
      }
      return false;
    }

    //! Detach from a comring to stop receiving messages
    void ringDetach(const char* topic)
    {
      comRing.getGlobal()->ringDetach(const_cast<char*>(topic), this);
    }

    //! Send a message to a ComRing
    void ringMessage(const char* topic, int eventCode, void* eventData = 0)
    {
      if (comRing.getGlobal()) comRing.getGlobal()->ringMessage(const_cast<char*>(topic), eventCode, eventData);
    }
    void ringMessage(int eventCode, void* eventData = 0)
    {
      ringMessage(lastTopic.c_str(), eventCode, eventData);
    }
  };

  void sendComringMessage(const char* topic, int eventCode, void* eventData = 0);

  //! Class to add support for the Communication Ring to plugins. This version allows for multiple ComRings
  //! @ingroup Entities
  /*!
  * You must inherit this class in EVERY plugin where you'd want to use a ComRing
  */
  class MultiComRingCommunicator
  {
    class comRingTopic : public comRingCommunicator
    {
      MultiComRingCommunicator* communicator;
    protected:
      virtual void RingEvent(void* portData, int eventCode, void* eventData)
      {
        communicator->MultiRingEvent(lastTopic, portData, eventCode, eventData);
      }
    public:
      comRingTopic(const char* topic, MultiComRingCommunicator* multiCom)
        : comRingCommunicator(topic), communicator(multiCom)
      {
      }
      virtual ~comRingTopic()
      {
        ;
      }
    };
    std::vector <std::shared_ptr<comRingTopic> > topics;
    TransientGlobal<LWComRing> comRing;

  protected:
  public:
    MultiComRingCommunicator() { ; }
    /*!
    * @param[in] topic The name/topic of the ComRing to attach to
    */
    virtual ~MultiComRingCommunicator()
    {
      ;
    }
    void ComRingAttach(const char* topic);

    virtual void MultiRingEvent(const std::string& topic, void* portData, int eventCode, void* eventData)
    {
      return;
    }

    //! Send a message to a ComRing
    void ringMessage(const char* topic, int eventCode, void* eventData = 0);
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
      : comRingCommunicator(LW_PLUGIN_LIMBO_STATES)
    {
      ringMessage(LW_LIMBO_START);
    }
    ~LimboComRing()
    {
      ringMessage(LW_LIMBO_END);
    }
  };

#ifndef LW_PORT_MASTER_EVENT 
#define LW_PORT_MASTER_EVENT "master-event" // available as of 11.0 - not documented
#endif


} // end namespace
#endif // COMRING_COMMUNICATOR_H