#ifndef LWPP_ENVELOPE_H
#define LWPP_ENVELOPE_H
#include <lwpp/global.h>
#include <lwenvel.h>

namespace lwpp
{
  //! @ingroup Globals
  class Envelopes : public GlobalBase<LWEnvelopeFuncs>
  {
  public:
    Envelopes( )  {;}

    LWEnvelopeID Create ( LWChanGroupID group, const char *name, int type )
    {
      return globPtr->create(group, name, type);
    }

    void Destroy ( LWEnvelopeID env )
    {
      globPtr->destroy (env);
    }

    LWChanGroupID createGroup ( LWChanGroupID parent, const char *name )
    {
      return globPtr->createGroup(parent, name);
    }

    void destroyGroup ( LWChanGroupID group)
    {
      globPtr->destroyGroup(group);
    }

    LWError Copy ( LWEnvelopeID to, LWEnvelopeID from)
    {
      return globPtr->copy(to, from);
    }

    LWError Load ( LWEnvelopeID env, const LWLoadState *load)
    {
      return globPtr->load(env, load);
    }

    LWError Save ( LWEnvelopeID env, const LWSaveState *save)
    {
      return globPtr->save(env, save);
    }

    double Evaluate ( LWEnvelopeID env, LWTime attime)
    {
      return globPtr->evaluate(env, attime);
    }

    int Edit ( LWChanGroupID group, LWEnvelopeID env, int flags, void *data)
    {
      return globPtr->edit(group, env, flags, data);
    }

    int envAge ( LWEnvelopeID env )
    {
      return globPtr->envAge(env);
    }

    LWEnvKeyframeID CreateKey ( LWEnvelopeID env, LWTime keytime, double value)
    {
      return globPtr->createKey(env, keytime, value);
    }

    void DestroyKey ( LWEnvelopeID env, LWEnvKeyframeID key)
    {
      globPtr->destroyKey(env, key);
    }

    LWEnvKeyframeID FindKey ( LWEnvelopeID env, LWTime keytime)
    {
      return globPtr->findKey(env, keytime);
    }

    LWEnvKeyframeID  NextKey ( LWEnvelopeID env, LWEnvKeyframeID key)
    {
      return globPtr->nextKey(env, key);
    }

    LWEnvKeyframeID  PrevKey ( LWEnvelopeID env, LWEnvKeyframeID key)
    {
      return globPtr->prevKey(env, key);
    }

    int KeySet ( LWEnvelopeID env, LWEnvKeyframeID key, LWKeyTag tag, void *value)
    {
      return globPtr->keySet(env, key, tag, value);
    }

    int KeyGet ( LWEnvelopeID env, LWEnvKeyframeID key, LWKeyTag tag, void *value)
    {
      return globPtr->keyGet(env, key, tag, value);
    }

    int  SetEnvEvent ( LWEnvelopeID env, LWEnvEventFunc  ev, void *data)
    {
      return globPtr->setEnvEvent(env, ev, data);
    }

    int EgSet ( LWEnvelopeID env, LWChanGroupID group, int tag, void *value)
    {
      return globPtr->egSet(env, group, tag, value);
    }
    int EgGet ( LWEnvelopeID env, LWChanGroupID group, int tag, void *value)
    {
      return globPtr->egGet(env, group, tag, value);
    }
  };

  //! @ingroup Entities
  class Envelope : public GlobalBase<LWEnvelopeFuncs>
  {
    LWEnvelopeID env;
    bool close_on_destroy;
  public:
    Envelope(LWEnvelopeID _env = 0 )
      : env(_env),
        close_on_destroy(false)
    {;}

    ~Envelope()
    {
      if (close_on_destroy) Destroy();
    }

    Envelope &operator=(LWEnvelopeID from)
    {
      close_on_destroy = false;
      env = from;
      return *this;
    }

    bool isValid() {return env != 0;}

    LWEnvelopeID Create ( LWChanGroupID group, const char *name, int type )
    {
      env = globPtr->create(group, name, type);
      return env;
    }

    void Destroy ()
    {
      if (env) globPtr->destroy (env);
    }

    LWError Copy ( LWEnvelopeID to, LWEnvelopeID from)
    {
      return globPtr->copy(to, from);
    }

    LWError Load ( const LWLoadState *load)
    {
      return globPtr->load(env, load);
    }

    LWError Save ( const LWSaveState *save)
    {
      return globPtr->save(env, save);
    }

    double Evaluate ( LWTime attime)
    {
      return globPtr->evaluate(env, attime);
    }

    int Edit ( LWChanGroupID group, int flags, void *data)
    {
      return globPtr->edit(group, env, flags, data);
    }

    int envAge ( )
    {
      return globPtr->envAge(env);
    }

    LWEnvKeyframeID  CreateKey    ( LWTime keytime, double value)
    {
      return globPtr->createKey(env, keytime, value);
    }

    void DestroyKey ( LWEnvKeyframeID key)
    {
      globPtr->destroyKey(env, key);
    }

    LWEnvKeyframeID FindKey ( LWTime keytime)
    {
      return globPtr->findKey(env, keytime);
    }

    LWEnvKeyframeID  NextKey ( LWEnvKeyframeID key)
    {
      return globPtr->nextKey(env, key);
    }

    LWEnvKeyframeID  PrevKey ( LWEnvKeyframeID key)
    {
      return globPtr->prevKey(env, key);
    }

    bool KeySet ( LWEnvKeyframeID key, LWKeyTag tag, void *value)
    {
      return (globPtr->keySet(env, key, tag, value) == 0);
    }

    bool KeyGet ( LWEnvKeyframeID key, LWKeyTag tag, void *value)
    {
      return (globPtr->keyGet(env, key, tag, value) == 0);
    }

    int  SetEnvEvent ( LWEnvEventFunc  ev, void *data)
    {
      return globPtr->setEnvEvent(env, ev, data);
    }

    int EgSet ( LWChanGroupID group, int tag, void *value)
    {
      return globPtr->egSet(env, group, tag, value);
    }
    int EgGet ( LWChanGroupID group, int tag, void *value)
    {
      return globPtr->egGet(env, group, tag, value);
    }
  };

  class Channel : public GlobalBase<LWChannelInfo>
  {
    LWChannelID chanID;
  public:
    Channel (LWChannelID chan = 0)
      : chanID(chan)
    {
      ;
    }
    bool next(LWChanGroupID cgroup)
    {
      chanID = globPtr->nextChannel(cgroup, chanID);
      return (chanID != 0);
    }
    Envelope getEnvelope()
    {
      return Envelope(globPtr->channelEnvelope(chanID));
    }
    const char *getName()
    {
      return globPtr->channelName(chanID);
    }
  };
  /*
typedef struct st_LWChannelInfo {
  // next group, first on NULL in parent group or root on NULL
  LWChanGroupID        (*nextGroup)       ( LWChanGroupID parent,
                                            LWChanGroupID group);
  // next channel, first on NULL in parent group or root on NULL
  LWChannelID          (*nextChannel)     ( LWChanGroupID parent,
                                            LWChannelID chan);
  const char           *(*groupName)      ( LWChanGroupID group);
  const char           *(*channelName)    ( LWChannelID chan);
  LWChanGroupID        (*groupParent)     ( LWChanGroupID group);
  LWChanGroupID        (*channelParent)   ( LWChannelID chan);
  int                  (*channelType)     ( LWChannelID chan);
  double               (*channelEvaluate) ( LWChannelID chan, LWTime chantime);
  LWEnvelopeID         (*channelEnvelope) ( LWChannelID chan);
  int                  (*setChannelEvent) ( LWChannelID chan,
                                            LWChanEventFunc ev, void *data );
  const char           *(*server)         ( LWChannelID chan,
                                            const char *cls, int idx );

  // Version 2 additions, all index arg.s are 1-based, matching layout
  unsigned int         (*serverFlags)     ( LWChannelID chan,
                                          const char *cls, int idx );
  LWInstance           (*serverInstance)  ( LWChannelID chan,
                                          const char *cls, int idx );
  //  Return 1-based index, or 0 on failure
  int                  (*serverApply)     ( LWChannelID chan, const char *cls,
                                          const char *name, int flags );
  void                 (*serverRemove)    ( LWChannelID chan, const char *cls,
                                          const char *name, LWInstance inst );
} LWChannelInfo;

  */
}

#endif // LWPP_ENVELOPE_H