#ifndef LWPP_PIXELFILTER_HANDLER_H
#define LWPP_PIXELFILTER_HANDLER_H

#include "lwpp/imagefilter_handler.h"

namespace lwpp
{

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
    BufferNameSet m_bufferNameSet;
    const char **m_bufferNameReturn;
  public:
    PixelFilterHandler(void* g, void* context, LWError* err)
      : InstanceHandler(g, context, err, LWPIXELFILTER_HCLASS),
      m_bufferNameReturn(0)
    {
      ;
    }
    virtual ~PixelFilterHandler()
    {
			if (m_bufferNameReturn) delete[] m_bufferNameReturn;
    }

    virtual void Evaluate(const LWPixelAccess *pfa)
    {
      float rgba[4];
      pfa->getVal(LWBUFFER_FINAL_RENDER_RGBA, 4, rgba);
      pfa->setRGBA(rgba);
    }
    virtual const char **Flags()
    {
      m_bufferNameSet.clear(); // clear the set of used buffers
      UpdateFlags(); // update the set of used buffers
      if ( m_bufferNameReturn ) delete[] m_bufferNameReturn;
      m_bufferNameReturn = new const char *[m_bufferNameSet.size() + 1];
      auto idx = m_bufferNameReturn;
      for ( auto iter = m_bufferNameSet.begin(); iter != m_bufferNameSet.end(); ++iter )
      {
        *idx++ = iter->c_str();
      }
      *idx = 0;
      return m_bufferNameReturn;
    }
    virtual void UpdateFlags() {;} // update m_bufferNameSet
    virtual unsigned int renderFlags()
    {
      return LWPFF_MULTITHREADED;
    }
	};

	//! @ingroup Adaptor
	template <class T, int Version>
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
			if ( version != Version ) return AFUNC_BADVERSION;
			try
			{
				lwpp::SetSuperGlobal(global);
				LWPixelFilterHandler* plugin = static_cast<LWPixelFilterHandler*>(inst);
				InstanceAdaptor<T>::Activate(plugin->inst);
				ItemAdaptor<T>::Activate(plugin->item);
				RenderAdaptor<T>::Activate(plugin->rend);

				plugin->evaluate = PixelFilterAdaptor::Evaluate;
        plugin->renderFlags = PixelFilterAdaptor::renderFlags;
				plugin->flags    = PixelFilterAdaptor::Flags;
        
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
			return plugin->renderFlags();
		}
		static const char **Flags (LWInstance inst)
		{
			T* plugin = static_cast<T*>(inst);
			return plugin->Flags();
		}
	};

	/* GUI Classes */
	//! @ingroup XPanelHandler
	IMPLEMENT_XPANELHANDLER(PixelFilter);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_XPANELADAPTOR(PixelFilter, LWPIXELFILTER_VERSION);

	//! @ingroup LWPanelHandler
	IMPLEMENT_LWPANELHANDLER(PixelFilter);
	//! @ingroup LWPanelAdaptor
	IMPLEMENT_LWPANELADAPTOR(PixelFilter, LWPIXELFILTER_VERSION);
}
#endif // LWPP_PIXELFILTER_HANDLER_H