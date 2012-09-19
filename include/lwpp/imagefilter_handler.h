#ifndef LWPP_IMAGEFILTER_HANDLER_H
#define LWPP_IMAGEFILTER_HANDLER_H

#include "lwpp/plugin_handler.h"
#include <set>

#include <lwfilter.h>

namespace LWBUF
{
  enum
  {
      SPECIAL,
      LUMINOUS,
      DIFFUSE,
      SPECULAR,
      MIRROR,
      TRANS,
      RAW_RED,
      RAW_GREEN,
      RAW_BLUE,
      SHADING,
      SHADOW,
      GEOMETRY,
      DEPTH,
      DIFFSHADE,
      SPECSHADE,
      MOTION_X,
      MOTION_Y,
      REFL_RED,
      REFL_GREEN,
      REFL_BLUE,
      DIFF_RED,
      DIFF_GREEN,
      DIFF_BLUE,
      SPEC_RED,
      SPEC_GREEN,
      SPEC_BLUE,
      BACKDROP_RED,
      BACKDROP_GREEN,
      BACKDROP_BLUE,
      PREEFFECT_RED,
      PREEFFECT_GREEN,
      PREEFFECT_BLUE,
      RED,
      GREEN,
      BLUE,
      ALPHA,
      REFR_RED,
      REFR_GREEN,
      REFR_BLUE,
      REFR_ALPHA,
      NORMAL_X,
      NORMAL_Y,
      NORMAL_Z,
      SURFACEID,
      OBJECTID,
      RADIOSITY_RED,
      RADIOSITY_GREEN,
      RADIOSITY_BLUE,
      AMBIENTOCCLUSION_RED,
      AMBIENTOCCLUSION_GREEN,
      AMBIENTOCCLUSION_BLUE,
      UV_TANGENTSPACE_T_X,
      UV_TANGENTSPACE_T_Y,
      UV_TANGENTSPACE_T_Z,
      UV_TANGENTSPACE_B_X,
      UV_TANGENTSPACE_B_Y,
      UV_TANGENTSPACE_B_Z,
      UV_TANGENTSPACE_N_X,
      UV_TANGENTSPACE_N_Y,
      UV_TANGENTSPACE_N_Z,
      CAMERA_TANGENTSPACE_X,
      CAMERA_TANGENTSPACE_Y,
      CAMERA_TANGENTSPACE_Z,
      MAX_CHANNELS
  };
}

namespace lwpp
{
  namespace lw10
  {
	  struct LWImageFilterHandler
	  {
		  LWInstanceFuncs  *inst;
		  LWItemFuncs      *item;
		  void            (*process) (LWInstance, const LWFilterAccess *);
		  int *           (*flags) (LWInstance);
	  };
  }

  typedef std::set<int> BufferSet;

//! Class for Imagefilters
/*!
 * @ingroup Handler
 * @note LWFilterAccess is still the original struct and not replaced by the wrapper with a class
 */
class ImageFilterHandler : public InstanceHandler, public ItemHandler
{
protected:
  BufferSet m_bufferSet;
  int *m_bufferReturn;
	public:
	  ImageFilterHandler(void *g, void *context, LWError *err)
      : InstanceHandler(g, context, err, LWIMAGEFILTER_HCLASS),
        m_bufferReturn(0)
		{
      ;
    }
		virtual ~ImageFilterHandler() {;}
		virtual void Process(const LWFilterAccess *fa)
		{
			// default image copy
			for ( int y = 0; y < fa->height; y++ )
			{
				/* get each scanline */
				float *r = fa->getLine( LWBUF::RED, y );
				float *g = fa->getLine( LWBUF::GREEN, y );
				float *b = fa->getLine( LWBUF::BLUE, y );
				float *a = fa->getLine( LWBUF::ALPHA, y );

				for ( int x = 0; x < fa->width; x++ )
				{
					float out[] = {r[x], g[x], b[x]};
					fa->setRGB( x, y, out );
					fa->setAlpha( x, y, a[ x ] );
				}
			}
		}
	  virtual unsigned int Flags() {return 0;}
    virtual void UpdateFlags() {;} // Used by LW 10 and higher, update m_bufferSet
		int *Flags10()
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

//! Wrapper for an ImageFilterHandler
/*!
 * @ingroup Adaptor
 */
	template <class T, int maxVersion, int minVersion>
	class ImageFilterAdaptor : public InstanceAdaptor <T>, public ItemAdaptor <T>
	{
		public:
		ImageFilterAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWIMAGEFILTER_HCLASS, name, Activate, tags);
		}
		//! Set static callbacks for a LightWave ImageFilter
		static int Activate (int version, GlobalFunc *global, LWInstance inst, void *serverData)
		{
			if ( version > maxVersion ) return AFUNC_BADVERSION;
			if ( version < minVersion ) return AFUNC_BADVERSION;
			try
			{
				lwpp::SetSuperGlobal(global);

				if (version < 5)
				{
					LWImageFilterHandler *plugin = static_cast<LWImageFilterHandler *>(inst);

					InstanceAdaptor<T>::Activate(plugin->inst);
					ItemAdaptor<T>::Activate(plugin->item);
					plugin->process = ImageFilterAdaptor::Process;
					plugin->flags   = ImageFilterAdaptor::Flags;
					return AFUNC_OK;
				}
				else
				{
					lw10::LWImageFilterHandler *plugin = static_cast<lw10::LWImageFilterHandler *>(inst);

					InstanceAdaptor<T>::Activate(plugin->inst);
					ItemAdaptor<T>::Activate(plugin->item);
					plugin->process = ImageFilterAdaptor::Process;
					plugin->flags   = ImageFilterAdaptor::Flags10;
					return AFUNC_OK;
				}
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in ImageFilter::Activate():", e.what());
				return AFUNC_BADAPP;
			}
		}
		private:
		static void Process (LWInstance instance, const LWFilterAccess *fa)
		{
			try
			{
				T *plugin = (T *) instance;
				plugin->Process(fa);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in ImageFilter::Process():", e.what());
			}
		}

		static unsigned int Flags (LWInstance instance)
		{
			try
			{
				T *plugin = (T *) instance;
				return plugin->Flags();
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in ImageFilter::Flags():", e.what());
				return 0;
			}
		}
		static int *Flags10 (LWInstance instance)
		{
			try
			{
				T *plugin = (T *) instance;
				return plugin->Flags10();
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in ImageFilter::Flags10():", e.what());
				return 0;
			}
		}

	};

	//! Class for Imagefilters with an XPanel
	//! @ingroup XPanelHandler
	IMPLEMENT_XPANELHANDLER(ImageFilter);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_XPANELADAPTOR(ImageFilter, LWPP_IMAGEFILTER_VERSION, LWPP_IMAGEFILTER_VERSION);
	//! @ingroup LWPanelHandler
	IMPLEMENT_LWPANELHANDLER(ImageFilter);
	//! @ingroup LWPanelAdaptor
	IMPLEMENT_LWPANELADAPTOR(ImageFilter, LWPP_IMAGEFILTER_VERSION, LWPP_IMAGEFILTER_VERSION);
}

#endif // LWPP_IMAGEFILTER_HANDLER_H