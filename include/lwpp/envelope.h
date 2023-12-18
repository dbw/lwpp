#ifndef LWPP_ENVELOPE_H
#define LWPP_ENVELOPE_H

#include <lwpp/global.h>
#include <lwenvel.h>
#include <map>

namespace lwpp
{
  //! @ingroup Globals
  class Envelopes : public TransientGlobal<LWEnvelopeFuncs>
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
  class Envelope : public TransientGlobal<LWEnvelopeFuncs>
  {
    LWEnvelopeID env;
    bool close_on_destroy;

    struct EnvKey
    {
      double value;
      int shape;
      double t, c, b;
      double p0, p1, p2, p3;
    };

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

	LWEnvelopeID getID() const { return env; }

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

    LWEnvKeyframeID  CreateKey ( LWTime keytime, double value)
    {
      return globPtr->createKey(env, keytime, value);
    }

    void DestroyKey ( LWEnvKeyframeID key)
    {
      globPtr->destroyKey(env, key);
    }

    LWEnvKeyframeID FindKey ( const LWTime keytime)
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
      return (globPtr->keySet(env, key, tag, value) != 0);
    }

    bool KeySet(LWEnvKeyframeID key, LWKeyTag tag, int value)
    {
      return KeySet(key, tag, &value);
    }

    bool KeySet(LWEnvKeyframeID key, LWKeyTag tag, double value)
    {
      return KeySet(key, tag, &value);
    }  

    bool KeyGet ( LWEnvKeyframeID key, LWKeyTag tag, void *value)
    {
      return (globPtr->keyGet(env, key, tag, value) != 0);
    }

    void Get(LWEnvKeyframeID key, EnvKey& envkey)
    {
      KeyGet(key, LWKEY_VALUE, &envkey.value);
      KeyGet(key, LWKEY_SHAPE, &envkey.shape);
      KeyGet(key, LWKEY_TENSION, &envkey.t);
      KeyGet(key, LWKEY_CONTINUITY, &envkey.c);
      KeyGet(key, LWKEY_BIAS, &envkey.b);

      KeyGet(key, LWKEY_PARAM_0, &envkey.p0);
      KeyGet(key, LWKEY_PARAM_1, &envkey.p1);
      KeyGet(key, LWKEY_PARAM_2, &envkey.p2);
      KeyGet(key, LWKEY_PARAM_3, &envkey.p3);
    }

    void Set(LWEnvKeyframeID key, EnvKey& envkey)
    {
      KeySet(key, LWKEY_VALUE, &envkey.value);
      KeySet(key, LWKEY_SHAPE, &envkey.shape);
      KeySet(key, LWKEY_TENSION, &envkey.t);
      KeySet(key, LWKEY_CONTINUITY, &envkey.c);
      KeySet(key, LWKEY_BIAS, &envkey.b);

      KeySet(key, LWKEY_PARAM_0, &envkey.p0);
      KeySet(key, LWKEY_PARAM_1, &envkey.p1);
      KeySet(key, LWKEY_PARAM_2, &envkey.p2);
      KeySet(key, LWKEY_PARAM_3, &envkey.p3);
    }

    void Set(LWEnvKeyframeID key, double val, LWKeyTag tag = LWKEY_VALUE)
    {
        KeySet(key, tag, val);
    }

    int SetEnvEvent ( LWEnvEventFunc  ev, void *data)
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
    void shiftValues(const LWTime start, const LWTime end, const double shift)
    {
      LWEnvKeyframeID key = NextKey(0);
      while (key)
      {
        double keyTime = 0.0;
        KeyGet(key, LWKEY_TIME, &keyTime);
        if (keyTime >= start && keyTime <= end)
        {
          double value;
          KeyGet(key, LWKEY_VALUE, &value);
          value += shift;
          KeySet(key, LWKEY_VALUE, &value);
        }
        key = NextKey(key);
      }
    } 

    void multiplyValues(const LWTime start, const LWTime end, const double mult)
    {
      LWEnvKeyframeID key = NextKey(0);
      while (key)
      {
        double keyTime = 0.0;
        KeyGet(key, LWKEY_TIME, &keyTime);
        if (keyTime >= start && keyTime <= end)
        {
          double value;
          KeyGet(key, LWKEY_VALUE, &value);
          value *= mult;
          KeySet(key, LWKEY_VALUE, value);
        }
        key = NextKey(key);
      }
    }
    void shiftKeys(const LWTime start, const LWTime end, const LWTime shift, bool doMove = true)
    {
      std::map<LWTime, EnvKey> keyMap;
      // copy keys
      LWEnvKeyframeID key = NextKey(0);
      while (key)
      {
        double keyTime = 0.0;
        KeyGet(key, LWKEY_TIME, &keyTime);
        if (keyTime >= start && keyTime <= end)
        {
          EnvKey keyData;
          Get(key, keyData);
          keyMap.insert(std::make_pair(keyTime, keyData));
          auto delKey = key;
          key = NextKey(key);
          if (doMove)
            DestroyKey(delKey);
        }
        else
          key = NextKey(key);
      }

      for (auto& k : keyMap)
      {
        key = CreateKey(k.first + shift, k.second.value);
        Set(key, k.second);
      }                                                       

    }
    void copyKeys(const LWTime start, const LWTime end, const LWTime shift)
    {
      shiftKeys(start, end, shift, false);
    }

    void shiftKey(const LWTime time, const LWTime shift, const bool copy = false)
    {
      auto key = FindKey(time);
      if (key)
      {
        double value;
        EnvKey envKey;
        KeyGet(key, LWKEY_VALUE, &value);
        Get(key, envKey);
        if (!copy) 
          DestroyKey(key);
        key = CreateKey(time + shift, value);
        Set(key, envKey);
      }
    }
  };

}

#endif // LWPP_ENVELOPE_H