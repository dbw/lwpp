#ifndef LWPP_FRAMEBUFFER_HANDLER_H
#define LWPP_FRAMEBUFFER_HANDLER_H

#include "lwpp/plugin_handler.h"
#include <lwframbuf.h>

namespace lwpp
{
	//! @ingroup Handler
	class FrameBufferHandler : public InstanceHandler, public ItemHandler
	{
	public:
		FrameBufferHandler(void* g, void* context, LWError* err)
			: InstanceHandler(g,context,err, LWFRAMEBUFFER_HCLASS)
		{
			;
		}
		virtual ~FrameBufferHandler() {;}

		virtual LWError Open (int w, int h)
		{
			return 0;
		}

		virtual void Close ()
		{
			;
		}

		virtual LWError Begin ()
		{
			return 0;
		}

		virtual LWError Write (const void *R, const void *G, const void *B, const void *alpha)
		{
			return 0;
		}

		virtual void Pause ()	
		{
			;
		}
	};

	//! @ingroup Adaptor
	template <class T, int FBType=LWFBT_FLOAT>
	class FrameBufferAdaptor : public InstanceAdaptor <T>, public ItemAdaptor <T>
	{
	public:
		FrameBufferAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWFRAMEBUFFER_HCLASS, name, Activate, tags);
		}
		static int Activate (int version , GlobalFunc* global, LWInstance inst, void* serverData)
		{
			if(version!=LWFRAMEBUFFER_VERSION) return AFUNC_BADVERSION;
			lwpp::SetSuperGlobal(global);

			LWFrameBufferHandler* plugin = static_cast<LWFrameBufferHandler*>(inst);
			InstanceAdaptor<T>::Activate(plugin->inst);
			ItemAdaptor<T>::Activate(plugin->item);

			plugin->type = FBType;
			plugin->open = FrameBufferAdaptor::Open;
			plugin->close = FrameBufferAdaptor::Close;
			plugin->begin = FrameBufferAdaptor::Begin;
			plugin->write = FrameBufferAdaptor::Write;
			plugin->pause = FrameBufferAdaptor::Pause;


			UNUSED(serverData);
			return AFUNC_OK;
		}

	private:
		static LWError Open (LWInstance inst, int w, int h)
		{
			T* plugin = static_cast<T*>(inst);
			return plugin->Open(w,h);
		}

		static void Close (LWInstance inst)
		{
			T* plugin = static_cast<T*>(inst);
			plugin->Close();
		}

		static LWError Begin (LWInstance inst)
		{
			T* plugin = static_cast<T*>(inst);
			return plugin->Begin();
		}
		static LWError Write (LWInstance inst, const void *R, const void *G, const void *B, const void *alpha)
		{
			T* plugin = static_cast<T*>(inst);
			return plugin->Write(R,G,B,alpha);
		}
		static void Pause (LWInstance inst)	
		{
			T* plugin = static_cast<T*>(inst);
			plugin->Pause();
		}
	};

}
#endif //LWPP_FRAMEBUFFER_HANDLER_H