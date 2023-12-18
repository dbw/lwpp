/*!
 * @file
 * @brief C++ Wrapper implementation for LightWave VParms
 */
#include <lwpp/vparm.h>

namespace lwpp 
{

	static int VParmEventFuncCB(LWVParmID vp,
								void *userData,
								en_lwvpec eventCode,
								void *eventData)
	{
		if (userData)
    {
			auto *sink = static_cast<VParmEventSink *>(userData);
			return sink->VParmEvent(vp, eventCode, eventData);
		}
		return 0;
	}
	/*!
	 * @param type Type of VParm 
	 * @param *name Name of the VParm
	 * @param *plug_name Name of the Plugin this VParm belongs to
	 * @param group Group to create this VParm in, may be 0
	 * @return true if the creation succeeded.
	 */
	bool VParm::create(int type, const std::string name, const std::string plug_name, LWChanGroupID group, VParmEventSink *sink)
	{
		if ((globPtr) && (( vparmID = globPtr->create( type, LWVPDT_NOTXTR )) != 0)) 
		{
			globPtr->setup( vparmID, name.c_str(), group, 0, (sink ? VParmEventFuncCB : nullptr), plug_name.c_str(), sink );
			globPtr->setVal( vparmID, asLWVector() );
			mGroup = group;
			return true;
		}
		return false;
	}

	/*!
	 * @param type Type of VParm 
	 * @param *name Name of the VParm
	 * @param *plug_name Name of the Plugin this VParm belongs to
	 * @param group Group to create this VParm in, may be 0
	 * @param v Initial value
	 */
	VParm::VParm(int type, const std::string name, const std::string plug_name, LWChanGroupID group, double v, VParmEventSink *sink)		
	{
		Set(v);
		create(type, name, plug_name, group, sink);
	}

	/*!
	 * @param type Type of VParm 
	 * @param *name Name of the VParm
	 * @param *plug_name Name of the Plugin this VParm belongs to
	 * @param group Group to create this VParm in, may be 0
	 * @param &v Initial value
	 */
	 VParm::VParm(int type, const std::string name, const std::string plug_name, LWChanGroupID group, const Vector3d &v, VParmEventSink *sink)		
	{
		Set(v);
		create(type, name, plug_name, group, sink);
	}

	/*!
	 * @param type Type of VParm 
	 * @param *name Name of the VParm
	 * @param *plug_name Name of the Plugin this VParm belongs to
	 * @param group Group to create this VParm in, may be 0
	 * @param v Initial value
	 */
	VParm::VParm(int type, const std::string name, const std::string plug_name, ChannelGroup &group, double v, VParmEventSink *sink)
	{
		Set(v);
		create(type, name, plug_name, group.getID(), sink);
	}

	/*!
	 * @param type Type of VParm 
	 * @param *name Name of the VParm
	 * @param *plug_name Name of the Plugin this VParm belongs to
	 * @param group Group to create this VParm in, may be 0
	 * @param &v Initial value
	 */
	 VParm::VParm(int type, const std::string name, const std::string plug_name, ChannelGroup &group, const Vector3d &v, VParmEventSink *sink)
	{
		Set(v);
		create(type, name, plug_name, group.getID(), sink);
	}

	VParm::~VParm(void)
	{
		if(vparmID) (*globPtr->destroy)(vparmID);
	}

	LWError	VParm::Load(const lwpp::LoadState &ls)
	{
		return (*globPtr->load)( vparmID, ls.getState() );
	}

	LWError	VParm::Save(const lwpp::SaveState &ss)
	{
		return (*globPtr->save)( vparmID, ss.getState());
	}

	void VParm::enableEnvelopes(bool enable)
	{
		auto state = (getState() & ~LWVPSF_ENV);
		setState(state | (enable ? LWVPSF_ENV : 0));
	}

	std::vector<Envelope> VParm::getEnv()
	{
		LWEnvelopeID envlist[3] = { nullptr, nullptr,nullptr };
		globPtr->getEnv(vparmID, envlist);
		std::vector<Envelope> ret;
		ret.reserve(3);
		for (int i = 0; i < 3; ++i)
			ret.push_back(envlist[i]);
		return ret;
	}

	//! Hides all related envelopes in the graph editor

	void VParm::hide(bool doHide)
	{
		int visible = (doHide ? 0 : 1);
		auto envs = getEnv();
		for (auto& e : envs)
			e.EgSet(mGroup, LWENVTAG_VISIBLE, &visible);
	}

	void VParm::editEnv()
	{
		globPtr->editEnv(vparmID);
	}

	VParm &VParm::operator=(const VParm &from)
	{
		if (this != &from)
		{
			if ((vparmID != 0) && (from.vparmID != 0))	(*globPtr->copy) (vparmID, from.vparmID);      
			x = from.x;
			y = from.y;
			z = from.z;
		}
		return *this;
	}
} // end namespace lwpp