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
		: vparmID(0)
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
		 : vparmID(0)
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
		:	vparmID(0)
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
		 : vparmID(0)		 
	{
		Set(v);
		create(type, name, plug_name, group.getID(), sink);
	}

	VParm::~VParm(void)
	{
		(*globPtr->destroy)(vparmID);
	}

	LWError	VParm::Load(const lwpp::LoadState &ls)
	{
		return (*globPtr->load)( vparmID, ls.getState() );
	}

	LWError	VParm::Save(const lwpp::SaveState &ss)
	{
		return (*globPtr->save)( vparmID, ss.getState());
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