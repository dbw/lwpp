#include "lwpp/comring.h"

namespace lwpp
{

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