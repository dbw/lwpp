/*!
 * @file
 * @brief C++ Wrapper implementation for LightWave VParms
 * @version $Id: LW_texture.cpp 83 2006-04-19 23:43:07Z mwolf $
 */

#include <lwvparm.h>
#include "lwpp/storeable.h"
#include "lwpp/envelope.h"
#include <memory>
#include <vector>

#ifndef LW_VPARM_H
#define LW_VPARM_H

#include <lwpp/Vector3d.h>

namespace lwpp
{
	class ChannelGroup;

	class VParmEventSink
	{
	public:
		virtual int VParmEvent(LWVParmID vp, en_lwvpec eventCode, void *eventData) = 0;
	}; 

	//! The Variant Parameter Class Wrapper.
	/*!
	 * The actual value is stored in a Vector3d, and you can treat the VParm as such.
	 * @note Since the values are cached in the Vector3d, make sure to VParm::NewTime
	 */
	class VParm : protected GlobalBase<LWVParmFuncs>, public Vector3d, public Storeable
	{
		LWVParmID vparmID;
		LWChanGroupID mGroup = 0;
		bool	create(int type, const std::string name, const std::string plug_name, LWChanGroupID group, VParmEventSink *sink = nullptr);
		VParm(const VParm &from); //! no copy constructor, since it could lead to problems
		//! Update VParm with the currently cached values
		void set()
		{
			(*globPtr->setVal)(vparmID, asLWVector());
		}
	public:
		VParm(int type, const std::string name, std::string plug_name, LWChanGroupID group = 0, double v = 0.0, VParmEventSink *sink = nullptr);
		VParm(int type, const std::string name, std::string plug_name, LWChanGroupID group, const Vector3d &v, VParmEventSink *sink = nullptr);
		VParm(int type, const std::string name, std::string plug_name, ChannelGroup& group, double v = 0.0, VParmEventSink *sink = nullptr);    
		VParm(int type, const std::string name, std::string plug_name, ChannelGroup& group, const Vector3d &v, VParmEventSink *sink = nullptr);
		virtual ~VParm();
		//! Copy values
		VParm &operator=(const VParm &from);

		//! Load a VParm from a LW scene/object
		virtual LWError Load(const lwpp::LoadState &ls );
		//! Save a VParm to a LW scene/object
		virtual LWError Save(const lwpp::SaveState &ss );

		//! Return the LWVParmID for the current object
		LWVParmID	ID() {return vparmID;}
		//! Update the current time value (which is cached for faster access)
		void NewTime(LWTime t)
		{
			(*globPtr->getVal)(vparmID, t, 0, asLWVector() );
		}

		void Evaluate(const LWTime t, Vector3d &val)
		{
			(*globPtr->getVal)(vparmID, t, 0, val.asLWVector());
		}

		void Evaluate(const LWTime t, Point3d& val)
		{
			(*globPtr->getVal)(vparmID, t, 0, val.asLWVector());
		}

		void Evaluate(const LWTime t, double *val)
		{
			(*globPtr->getVal)(vparmID, t, 0, val);
		}

		void Clear()
		{
			(*globPtr->setState)(vparmID, 0);
		}

		void Reset(const double d)
		{
			Clear();
			x = y = z = d;
			set();
		}
		void Reset(const Vector3d &v)
		{
			Clear();
			x = v.x; y = v.y;	z = v.z;
			set();
		}
		//! Set the current time Value to a new Value
		void Set(const double d)
		{
			x = y = z = d;
			set();
		}
		void Set(const Vector3d& v)
		{
			x = v.x; y = v.y;	z = v.z;
			set();
		}
		void Set(const LWDVector v)
		{
			x = v[0];	y = v[1];	z = v[2];
			set();
		}
		void Set(const LWFVector v)
		{
			x = v[0];	y = v[1];	z = v[2];
			set();
		}

		double GetValue() const
		{
			return x;
		}
		float GetValuef() const
		{
			return static_cast<float>(x);
		}
		bool isEnveloped()
		{
			return (globPtr->getState(vparmID) & LWVPSF_ENV) == LWVPSF_ENV;
		}
		void getEnv(LWEnvelopeID envlist[3])
		{
			globPtr->getEnv(vparmID, envlist);
		}

		int getState() { return globPtr->getState(vparmID); };
		void setState(int state) { globPtr->setState(vparmID, state); };

		void enableEnvelopes(bool enable = true)
		{
			auto state = (getState() & ~LWVPSF_ENV);
			setState(state | (enable ? LWVPSF_ENV : 0));
		}

		std::vector<Envelope> getEnv()
		{
			LWEnvelopeID envlist[3];
			globPtr->getEnv(vparmID, envlist);
			std::vector<Envelope> ret;
			ret.reserve(3);
			for (int i = 0; i < 3; ++i)
				ret.push_back(envlist[i]);
			return ret;
		}

		//! Hides all related envelopes in the graph editor
		void hide(bool doHide = true)
		{
			int visible = (doHide ? 0 : 1);
			auto envs = getEnv();
			for (auto& e : envs)
				e.EgSet(mGroup, LWENVTAG_VISIBLE, &visible);
		}

		void editEnv()
		{
			globPtr->editEnv(vparmID);
		}
	};
  
  typedef std::unique_ptr<VParm> auto_VParm;
	typedef std::shared_ptr<VParm> shared_VParm;   
  typedef std::unique_ptr<VParm> unique_VParm;

/*
  const char           *(*channelName)    ( LWChannelID chan);
  LWChanGroupID        (*channelParent)   ( LWChannelID chan);
  int                  (*channelType)     ( LWChannelID chan);
  double               (*channelEvaluate) ( LWChannelID chan, LWTime time);
  const LWEnvelopeID   (*channelEnvelope) ( LWChannelID chan);
  int                  (*setChannelEvent) ( LWChannelID chan, LWChanEventFunc ev, void *data );
  const char           *(*server)         ( LWChannelID chan, const char *cls, int idx );
  unsigned int         (*serverFlags)     ( LWChannelID chan, const char *cls, int idx );
  LWInstance           (*serverInstance)  ( LWChannelID chan, const char *cls, int idx );
  int                  (*serverApply)     ( LWChannelID chan, const char *cls, const char *name, int flags );
  void                 (*serverRemove)    ( LWChannelID chan, const char *cls, const char *name, LWInstance inst );
*/

	class ChannelGroup : protected GlobalBase<LWChannelInfo>
	{
			LWChanGroupID groupID;
			GlobalBase<LWEnvelopeFuncs> envf;
			bool destroy_on_exit;
		public:
			ChannelGroup()
				: groupID(0),
          destroy_on_exit(false)
			{
				;
			}
			ChannelGroup(const char *name, LWChanGroupID parent = 0, bool destroy = true)
				:	groupID(0), destroy_on_exit(destroy)
			{
				groupID = createUniqueGroup(name, parent);
			}

			ChannelGroup(const char *name, ChannelGroup& parent)
				: groupID(0), destroy_on_exit(true)
			{
				groupID = createUniqueGroup(name, parent.getID());
			}

			ChannelGroup(LWChanGroupID id)
				: groupID(id), destroy_on_exit(false)
			{
				;
			}

			virtual ~ChannelGroup()
			{
				Reset();
			}

			void Delete()
			{
				if (groupID) envf->destroyGroup(groupID);
				groupID = 0;
			}

			void Create(const char *name, LWChanGroupID parent, bool destroy = true)
			{
				Reset();
				groupID = createUniqueGroup(name, parent);
				destroy_on_exit = destroy;
			}

			void Reset()
			{
				if (groupID && destroy_on_exit)
				{
					envf->destroyGroup(groupID);
					groupID = 0;
				}
			}

			LWChanGroupID getID() const {return groupID;}
			LWChanGroupID nextGroup( LWChanGroupID parent, LWChanGroupID group) {return globPtr->nextGroup(parent, group);}
      LWChanGroupID nextGroup(LWChanGroupID group) { return globPtr->nextGroup(groupID, group); }
			LWChannelID  nextChannel( LWChanGroupID parent, LWChannelID chan) {return globPtr->nextChannel(parent, chan);}
			const char  *groupName ( LWChanGroupID group) {return globPtr->groupName(group);}
			const char  *groupName ( void ) {return globPtr->groupName(groupID);}
			LWChanGroupID groupParent ( LWChanGroupID group) {return globPtr->groupParent(group);}
			LWChanGroupID groupParent () const {return globPtr->groupParent(groupID);}

			LWChanGroupID createUniqueGroup(const char *name, LWChanGroupID parent)
			{
				int i = 0;

				LWChanGroupID return_group;
				char *channel_name = new char[strlen(name) + 10];
				//sprintf(channel_name, name); // create new name
				strcpy(channel_name, name);
				for ( ; ; )
				{
					bool chan_found = false;

					LWChanGroupID grp = nextGroup(parent, 0); // get first channel
					// iterate through all channels
					while (!chan_found && (grp))
					{
						if (strcmp(channel_name, groupName(grp)) == 0)
						{
							chan_found = true;
							continue;
						}

						grp = nextGroup(parent, grp);
					}
					// Current channel name does not exist!
					if (chan_found)
					{
						sprintf(channel_name, "%s(%i)", name, i++); // create new name
					}
					else
					{
						return_group = envf->createGroup(parent, channel_name);
						delete[] channel_name;
						return return_group; // exit
					}
				}
			}

	 LWEnvelopeID findEnvelope(std::string name )
   {
      LWChannelID chan = nextChannel( groupID, NULL );
      while ( chan )
			{
        if ( name == globPtr->channelName( chan ))
				{
            return globPtr->channelEnvelope( chan );
				}
         chan = nextChannel( groupID, chan );
      }
      return 0;
   }

	};
}
#endif // LW_VPARM_H
