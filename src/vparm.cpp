/*!
 * @file
 * @brief C++ Wrapper implementation for LightWave VParms
 */
#include <lwpp/vparm.h>

namespace lwpp 
{
	/*!
	 * @param type Type of VParm 
	 * @param *name Name of the VParm
	 * @param *plug_name Name of the Plugin this VParm belongs to
	 * @param group Group to create this VParm in, may be 0
	 * @return true if the creation succeeded.
	 */
	bool VParm::create(int type, const std::string name, const std::string plug_name, LWChanGroupID group)
	{
		if ((globPtr) && (( vparmID = globPtr->create( type, LWVPDT_NOTXTR )) != 0)) 
		{
			globPtr->setup( vparmID, name.c_str(), group, 0, 0, plug_name.c_str(), 0 );
			globPtr->setVal( vparmID, asLWVector() );
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
	VParm::VParm(int type, const std::string name, const std::string plug_name, LWChanGroupID group, double v)
		: vparmID(0)
	{
		Set(v);
		create(type, name, plug_name, group);
	}

	/*!
	 * @param type Type of VParm 
	 * @param *name Name of the VParm
	 * @param *plug_name Name of the Plugin this VParm belongs to
	 * @param group Group to create this VParm in, may be 0
	 * @param &v Initial value
	 */
	 VParm::VParm(int type, const std::string name, const std::string plug_name, LWChanGroupID group, Vector3d &v)
		 : vparmID(0)
	{
		Set(v);
		create(type, name, plug_name, group);
	}

	/*!
	 * @param type Type of VParm 
	 * @param *name Name of the VParm
	 * @param *plug_name Name of the Plugin this VParm belongs to
	 * @param group Group to create this VParm in, may be 0
	 * @param v Initial value
	 */
	VParm::VParm(int type, const std::string name, const std::string plug_name, ChannelGroup &group, double v)
		:	vparmID(0)
	{
		Set(v);
		create(type, name, plug_name, group.getID());
	}

	/*!
	 * @param type Type of VParm 
	 * @param *name Name of the VParm
	 * @param *plug_name Name of the Plugin this VParm belongs to
	 * @param group Group to create this VParm in, may be 0
	 * @param &v Initial value
	 */
	 VParm::VParm(int type, const std::string name, const std::string plug_name, ChannelGroup &group, Vector3d &v)
		 : vparmID(0)		 
	{
		Set(v);
		create(type, name, plug_name, group.getID());
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