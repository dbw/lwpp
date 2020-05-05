#ifndef LWPP_CHANNEL_HANDLER_H
#define LWPP_CHANNEL_HANDLER_H
 
#include "lwpp/plugin_handler.h"
#include <lwchannel.h>

namespace lwpp
{

	class ChannelAccess
	{	
		const LWChannelAccess* lwca;
	public:						 		
		ChannelAccess(const LWChannelAccess *ca)
			: lwca(ca)
		{	
		}
#if (LWSDKVER_MAJOR < 2020)
		LWChannelID getChannel() const { return lwca->chan; }
		double get(LWChannelID chan, LWTime t)
		{
			double value;
			lwca->getChannel(chan, t, &value);
			return value;
		}
		void set(LWChannelID chan, const double value)
		{
			return lwca->setChannel(getChannel(), value);
		}
#else
		LWChannelID getChannel() const { return lwca->channel; }
		double get(LWChannelID chan, LWTime t)
		{
			double value;
			lwca->getChannel(lwca->priv, chan, t, &value);
			return value;
		}
		void set(LWChannelID chan, const double value)
		{
			return lwca->setChannel(lwca->priv, getChannel(), value);
		}
		LWCStringUTF8 name()
		{
			return lwca->channelName(lwca->priv, getChannel());
		}
#endif
		LWFrame getFrame() const { return lwca->frame; }
		LWTime getTime() const { return lwca->time; }
		double getValue() const { return lwca->value; }

		double get(LWTime t)
		{
			return get(getChannel(), t);
		}
		double get()
		{	
			return get(getTime());
		}

		void set(const double value)
		{
			return set(getChannel(), value);
		}
	};

	class ChannelHandler : public InstanceHandler, public ItemHandler
	{
	protected:
		lwpp::lwChannel Context;
	public:
		ChannelHandler(void* g, void* context, LWError* err)
			: InstanceHandler(g,context,err, LWCHANNEL_HCLASS),
			Context(static_cast<LWChannelID>(context))
		{				
		}
		virtual ~ChannelHandler()
		{
			;
		}
		
		virtual void Evaluate(ChannelAccess &ca)
		{
			;
		}
		virtual unsigned int Flags()
		{
			return 0; //says This Plugin doesn't get applied after IK by default.
		}
	};

	//! @ingroup Adaptor
	template <class T, int Version>
	class ChannelAdaptor : public InstanceAdaptor <T>
	{
	public:
		ChannelAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWCHANNEL_HCLASS, name, Activate, tags);
		}
		static int Activate (int version , GlobalFunc* global, LWInstance inst, void* serverData)
		{
			if(version!=Version) return AFUNC_BADVERSION;
			lwpp::SetSuperGlobal(global);
			
			LWChannelHandler* plugin = static_cast<LWChannelHandler*>(inst);
			InstanceAdaptor<T>::Activate(plugin->inst);
			ItemAdaptor<T>::Activate(plugin->item);
 
			plugin->evaluate = ChannelAdaptor::Evaluate;
			plugin->flags =    ChannelAdaptor::Flags;
	
			UNUSED(serverData);
			return AFUNC_OK;
		}
	private:
		static void Evaluate (LWInstance inst, const LWChannelAccess* ca)
		{
			T* plugin = static_cast<T*>(inst);
			ChannelAccess lwca(ca);
			plugin->Evaluate(lwca);
		}
		static unsigned int Flags (LWInstance inst)
		{
			T* plugin = static_cast<T*>(inst);
			return plugin->Flags();
		}
	};
		 
	/* GUI Classes */
	//! @ingroup XPanelHandler
	IMPLEMENT_XPANELHANDLER(Channel);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_XPANELADAPTOR(Channel, LWCHANNEL_VERSION);
	//! @ingroup LWPanelHandler
	IMPLEMENT_LWPANELHANDLER(Channel);
	//! @ingroup LWPanelAdaptor
	IMPLEMENT_LWPANELADAPTOR(Channel, LWCHANNEL_VERSION);
}

#endif //LWPP_CHANNEL_HANDLER_H