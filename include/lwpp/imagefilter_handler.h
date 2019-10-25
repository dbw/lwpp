#ifndef LWPP_IMAGEFILTER_HANDLER_H
#define LWPP_IMAGEFILTER_HANDLER_H

#include "lwpp/plugin_handler.h"
#include <set>

#include <lwfilter.h>
#include <lwaovs.h>

namespace lwpp
{
	//typedef std::set<int> BufferSet;
	typedef std::set<std::string> BufferNameSet;

	class AOVFuncs : protected GlobalBase<LWAOVFuncs>
	{
	public:
		unsigned int numAOVs()
		{
			return globPtr->numAOVs();
		}
		const char *name(unsigned int i)
		{
			return globPtr->name(i);
		}
		const LWAOVDefinition* definition(unsigned int i)
		{
			return globPtr->definition(i);
		}
		int setChangeEvent(LWAOVEventFunc evntFun, void* userData)
		{
			return globPtr->setChangeEvent(evntFun, userData);
		}
		LWAOVID registerAOV (const char *name, const LWAOVDefinition &def)
		{
			return globPtr->registerAOV(name, &def);
		}
		void unregisterAOV (LWAOVID id)
		{
			return globPtr->unregisterAOV(id);
		}
		void addCustomAOV(const char*name)
		{
			globPtr->addCustomAOV(name);
		}
		void removeCustomAOV(const char*name)
		{
			globPtr->removeCustomAOV(name);
		}
	};

	template <class T>
	int AOVEventFunc (void* userData, void* priv, int event, LWAOVEventData* aovEventData)
	{
		if ( userData == nullptr ) return 0;
		T* host = static_cast<T *>(userData);
		return host->AOVEvent(event, aovEventData);
	}
 
	class AOVClient
	{
	public:
		AOVFuncs aovFuncs;
		virtual int AOVEvent(int event, LWAOVEventData* aovEventData) = 0;

		AOVClient()
		{
			//register the callback
			aovFuncs.setChangeEvent(AOVEventFunc<AOVClient>, this);
		}
		virtual ~AOVClient()
		{
			aovFuncs.setChangeEvent(nullptr, this); // remove the callback
		}
	};

	class FilterInfo : protected GlobalBase<LWFilterGlobal>
	{
		BufferNameSet m_inUse, m_Available;

		void copyArrayToSet(const char **buffers, BufferNameSet &set)
		{
			set.clear();
			if ( buffers )
			{
				int i = 0;
				while ( buffers[i] )
				{
					set.insert(buffers[i]);
					++i;
				}
			}
		}

	public:
		BufferNameSet &UpdateAvailable()
		{
			const char **buffers = globPtr->buffersAvailable();
			copyArrayToSet(buffers, m_Available);
			return m_Available;
		}
		BufferNameSet &UpdateInUse()
		{
			const char **buffers = globPtr->buffersAvailable();
			copyArrayToSet(buffers, m_inUse);
			return m_inUse;
		}
		BufferNameSet &GetAvailable()
		{
			if ( m_Available.empty() ) return UpdateAvailable();
			return m_Available;
		}
		BufferNameSet &GetUsed()
		{
			if ( m_inUse.empty() ) return UpdateInUse();
			return m_inUse;
		}
		bool isBufferInUse(const char *name)
		{
			return (globPtr->isBufferInUse(name) != 0);
		}
		bool isBufferInUse(const std::string &name)
		{
			return (isBufferInUse(name.c_str()));
		}
		bool isBufferAvailable(const char *name)
		{
			for ( auto i : m_inUse )
			{
				if (i == name) return true;
			}
			return false;
		}
		bool isBufferAvailable(const std::string &name)
		{
			return (isBufferAvailable(name.c_str()));
		}
	};

//! Class for Imagefilters
/*!
 * @ingroup Handler
 * @note LWFilterAccess is still the original struct and not replaced by the wrapper with a class
 */
class ImageFilterHandler : public InstanceHandler, public ItemHandler
{
protected:
	BufferNameSet m_bufferNameSet;
	const char **m_bufferNameReturn;
	public:
		ImageFilterHandler(void *g, void *context, LWError *err)
			: InstanceHandler(g, context, err, LWIMAGEFILTER_HCLASS),
			m_bufferNameReturn(0)
		{
			;
		}
		virtual ~ImageFilterHandler() 
		{
			if (m_bufferNameReturn) delete[] m_bufferNameReturn;
		}
		virtual LWError Process(const LWFilterAccess *fa)
		{
			// default image copy
			float *rgba[4];
			for ( int y = 0; y < fa->height; y++ )
			{
				/* get each scanline */
				for (int c = 0; c < 4; c++ )
				{
					rgba[c] = fa->getLine(LWBUFFER_FINAL_RENDER_RGBA, c , y );
				}
				for ( int x = 0; x < fa->width; x++ )
				{
					float out[] = {rgba[0][x], rgba[1][x], rgba[2][x]};
					fa->setRGB( x, y, out );
					if ( rgba[3] != nullptr ) fa->setAlpha( x, y, rgba[3][ x ] );
				}
			}
			return 0;
		}

		virtual void UpdateFlags() {;} // Used by LW 10 and higher, update m_bufferSet
		// return a null terminated array of buffer names used by this plugin
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
};

//! Wrapper for an ImageFilterHandler
/*!
 * @ingroup Adaptor
 */
	template <class T, int Version = LWIMAGEFILTER_VERSION>
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
			if ( version != Version ) return AFUNC_BADVERSION;
			try
			{
				lwpp::SetSuperGlobal(global);
				LWImageFilterHandler *plugin = static_cast<LWImageFilterHandler *>(inst);
				InstanceAdaptor<T>::Activate(plugin->inst);
				ItemAdaptor<T>::Activate(plugin->item);
				plugin->process = ImageFilterAdaptor::Process;
				plugin->flags = ImageFilterAdaptor::Flags;
				return AFUNC_OK;				
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in ImageFilter::Activate():", e.what());
				return AFUNC_BADAPP;
			}
		}
		private:
		static LWError Process (LWInstance instance, const LWFilterAccess *fa)
		{
			try
			{
				T *plugin = (T *) instance;
				return plugin->Process(fa);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in ImageFilter::Process():", e.what());
				return "An exception occured in ImageFilter::Process()";
			}
			return 0;
		}

		static const char ** Flags (LWInstance instance)
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
	};

	//! Class for Imagefilters with an XPanel
	//! @ingroup XPanelHandler
	IMPLEMENT_XPANELHANDLER(ImageFilter);
	//! @ingroup XPanelAdaptor
	IMPLEMENT_XPANELADAPTOR(ImageFilter, LWIMAGEFILTER_VERSION);
	//! @ingroup LWPanelHandler
	IMPLEMENT_LWPANELHANDLER(ImageFilter);
	//! @ingroup LWPanelAdaptor
	IMPLEMENT_LWPANELADAPTOR(ImageFilter, LWIMAGEFILTER_VERSION);
}

#endif // LWPP_IMAGEFILTER_HANDLER_H