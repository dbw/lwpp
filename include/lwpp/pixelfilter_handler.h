#ifndef LWPP_PIXELFILTER_HANDLER_H
#define LWPP_PIXELFILTER_HANDLER_H

#include "lwpp/imagefilter_handler.h"

namespace lwpp
{
  namespace lw10
  {
    typedef struct st_LWPixelFilterHandler {
      LWInstanceFuncs  *inst;
      LWItemFuncs      *item;
      LWRenderFuncs    *rend;
      void            (*evaluate)    (LWInstance, const LWPixelAccess *);
      int *           (*flags)       (LWInstance);
      unsigned int    (*renderFlags) (LWInstance);
    } LWPixelFilterHandler;
  }

  enum {
    LW10PFF_MULTITHREADED = 1<<0,
    LW10PFF_EVERYPIXEL    = 1<<1,
    LW10PFF_BEFOREVOLUME  = 1<<2,
    LW10PFF_RAYTRACE      = 1<<3
  };
	//! @ingroup Handler
	class PixelFilterHandler : public InstanceHandler, public ItemHandler, public RenderHandler
	{
  protected:
    BufferSet m_bufferSet;
    int *m_bufferReturn;
	public:
		PixelFilterHandler(void* g, void* context, LWError* err)
			: InstanceHandler(g,context,err, LWPIXELFILTER_HCLASS),
        m_bufferReturn(0)
		{;}
		virtual ~PixelFilterHandler()
		{
			;
		}

		virtual void Evaluate(const LWPixelAccess *pfa)
		{
			float rgba[4];
			pfa->getVal(LWBUF::RED,4,rgba);
			pfa->setRGBA(rgba);
		}
		virtual unsigned int Flags9()
		{
      m_bufferSet.clear(); // clear the set of used buffers
      UpdateFlags(); // update the set of used buffers
      unsigned int flag = 0;
			for (BufferSet::iterator iter = m_bufferSet.begin(); iter != m_bufferSet.end(); ++iter)
      {
        if (*iter < LWBUF_BACKDROP_GREEN) flag |= 1 << (*iter);
      }
      return flag | Flags();
		}
		virtual unsigned int Flags()
		{
      return 0;
		}

    virtual void UpdateFlags() {;} // Used by LW 10 and higher, update m_bufferSet
		virtual int *Flags10()
		{
      m_bufferSet.clear(); // clear the set of used buffers
      UpdateFlags(); // update the set of used buffers
      if (m_bufferReturn) delete[] m_bufferReturn;
      m_bufferReturn = new int[m_bufferSet.size()+1];
      int *idx = m_bufferReturn;
      *idx++ = static_cast<int>(m_bufferSet.size()); // store the number of buffers
      for (BufferSet::iterator iter = m_bufferSet.begin(); iter != m_bufferSet.end(); ++iter)
      {
        *idx++ = *iter;
      }
			return m_bufferReturn;
		}
	};

	//! @ingroup Adaptor
	template <class T, int minVersion, int maxVersion>
	class PixelFilterAdaptor : public InstanceAdaptor <T>, public ItemAdaptor <T> , public RenderAdaptor<T>
	{
	public:
		public:
		PixelFilterAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWPIXELFILTER_HCLASS, name, Activate, tags);
		}
		static int Activate (int version , GlobalFunc* global, LWInstance inst, void* serverData)
		{
			if ( version > maxVersion ) return AFUNC_BADVERSION;
			if ( version < minVersion ) return AFUNC_BADVERSION;
			try
			{
				lwpp::SetSuperGlobal(global);
        if (version < 7)
        {
				  LWPixelFilterHandler* plugin = static_cast<LWPixelFilterHandler*>(inst);
				  InstanceAdaptor<T>::Activate(plugin->inst);
				  ItemAdaptor<T>::Activate(plugin->item);
				  RenderAdaptor<T>::Activate(plugin->rend);

				  plugin->evaluate = PixelFilterAdaptor::Evaluate;
				  plugin->flags    = PixelFilterAdaptor::Flags9;
        }
        else
        {
				  lw10::LWPixelFilterHandler* plugin = static_cast<lw10::LWPixelFilterHandler*>(inst);
				  InstanceAdaptor<T>::Activate(plugin->inst);
				  ItemAdaptor<T>::Activate(plugin->item);
				  RenderAdaptor<T>::Activate(plugin->rend);

				  plugin->evaluate = PixelFilterAdaptor::Evaluate;
          plugin->renderFlags = PixelFilterAdaptor::renderFlags;
				  plugin->flags    = PixelFilterAdaptor::Flags10;
        }
				UNUSED(serverData);
				return AFUNC_OK;
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in PixelFilter::Activate():", e.what());
				return AFUNC_BADAPP;
			}
		}
	private:
		static void Evaluate (LWInstance inst, const LWPixelAccess* ma)
		{
			T* plugin = static_cast<T*>(inst);
			plugin->Evaluate(ma);
		}
		static unsigned int renderFlags (LWInstance inst)
		{
			T* plugin = static_cast<T*>(inst);
			return plugin->Flags();
		}
		static unsigned int Flags9 (LWInstance inst)
		{
			T* plugin = static_cast<T*>(inst);
			return plugin->Flags9();
		}
		static int *Flags10 (LWInstance inst)
		{
#ifdef _DEBUG
  lwpp::dostream dout;
  dout << "PixelFilterAdaptor::Flags10()\n";
#endif
			T* plugin = static_cast<T*>(inst);
			return plugin->Flags10();
		}
	};

	/* GUI Classes */
	//! @ingroup XPanelHandler
	IMPLEMENT_XPANELHANDLER(PixelFilter);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_XPANELADAPTOR(PixelFilter, LWPP_PIXELFILTER_VERSION, LWPP_PIXELFILTER_VERSION);

	//! @ingroup LWPanelHandler
	IMPLEMENT_LWPANELHANDLER(PixelFilter);
	//! @ingroup LWPanelAdaptor
	IMPLEMENT_LWPANELADAPTOR(PixelFilter, LWPP_PIXELFILTER_VERSION, LWPP_PIXELFILTER_VERSION);
}
#endif // LWPP_PIXELFILTER_HANDLER_H