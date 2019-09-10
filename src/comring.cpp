#include "lwpp/comring.h"

namespace lwpp
{

	void sendComringMessage(const char* topic, int eventCode, void* eventData)
	{
		GlobalBase<LWComRing> comRing;
		if (comRing.getGlobal()) comRing.getGlobal()->ringMessage(const_cast<char*>(topic), eventCode, eventData);
	}


  void MultiComRingCommunicator::ComRingAttach(const char *topic)
  {
    auto crTopic = std::make_shared<comRingTopic>(topic, this);
    topics.push_back(crTopic);
  }

	//! Send a message to a ComRing

	void MultiComRingCommunicator::ringMessage(const char * topic, int eventCode, void * eventData)
	{
		if (comRing.getGlobal()) comRing.getGlobal()->ringMessage(const_cast<char *>(topic), eventCode, eventData);
	}

}