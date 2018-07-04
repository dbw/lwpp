#ifndef LWPP_HANDLER_H
#define LWPP_HANDLER_H

#include <lwfilter.h>
#include "lwpp/message.h"
#include "lwpp/lwpanel_handler.h"
#include "lwpp/item.h"

#pragma warning( push )
#pragma warning( disable : 4100 )

namespace lwpp
{
	class InstanceUpdate : public GlobalBase<LWInstUpdate> 
	{		
	public:
		virtual ~InstanceUpdate() {}
		static void Update (const char *className, LWInstance inst)
		{
			if ((globPtr) && (className))	globPtr(className, inst);
		}
	};

	//! Base class for (almost) any LW plugin
	class InstanceHandler : public virtual Storeable
	{
	protected:
		std::string m_description;
		const char *m_pluginName;
		const char *m_className;
		InstanceUpdate m_update;
	public:
		InstanceHandler(void *priv, void *context, LWError *err, const char *_className) : m_pluginName(0), m_className(_className)
		{
			UNUSED(priv);
			UNUSED(context);
			UNUSED(err);
			return;
		}
		virtual ~InstanceHandler() {;}
		virtual const char *DescLn() {return m_description.c_str();}
		void SetName(const char *n) {m_pluginName = n;}
		void SetClassName(const char *n) {m_className = n;}
		void SetDescription(const char *n) {m_description = n;}
		void SetDescription(const std::string& s) {m_description = s;}
		void Update () {m_update.Update(m_className, this);}
		void Update (const char *cName) {m_update.Update(cName, this);}
	};

	//! Wrapper for InstanceHandler
	/*!
	* @ingroup Adaptor
	*/
	template <class T>
	class InstanceAdaptor
	{
	public:
		//! Set static callbacks for the LightWave instance handler
		static void Activate(LWInstanceFuncs *inst)
		{
			if (inst)
			{
				inst->create	= InstanceAdaptor::Create;
				inst->destroy	= InstanceAdaptor::Destroy;
				inst->copy		= InstanceAdaptor::Copy;
				inst->load		= InstanceAdaptor::Load;
				inst->save		= InstanceAdaptor::Save;
				inst->descln	= InstanceAdaptor::DescLn;
				inst->priv		= 0; // pass on nothing
			}
		}
	private:
		static LWInstance Create (void *priv, void *context, LWError *err)
		{
			try
			{
				LWError localErr = "\0";
				T *plugin = new T(priv, context, &localErr);
				if (*localErr)
				{
					delete plugin;
					plugin = 0;
					*err = localErr;
				}
				return static_cast<LWInstance> (plugin);
			}
			catch (std::exception &e)
			{
				static LWError error = e.what();
				err = &error;
				return 0;
			}
		}

		static void Destroy (LWInstance instance)
		{
			try
			{
				T *plugin = static_cast<T *>(instance);
				delete plugin;
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in Instance::Destroy():", e.what());
			}
		}

		static LWError Copy (LWInstance _to, LWInstance _from)
		{
			try
			{
				T *to = (T *) _to;
				T *from = (T *) _from;
				*to = *from;
				return NULL;
			}
			catch (std::exception &e)
			{
				return e.what();
			}
		}

		static LWError Load (LWInstance instance, const LWLoadState *ls)
		{
			try
			{
				T *plugin = static_cast<T *>(instance);
				return plugin->Load(lwpp::LoadState(ls));
			}
			catch (std::exception &e)
			{
				return e.what();
			}
		}

		static LWError Save (LWInstance instance, const LWSaveState *ss)
		{
			try
			{
				T *plugin = static_cast<T *>(instance);
				return plugin->Save(lwpp::SaveState(ss));
			}
			catch (std::exception &e)
			{

				return e.what();
			}
		}

		static const char *DescLn (LWInstance instance)
		{
			try
			{
				T *plugin = static_cast<T *>(instance);
				return plugin->DescLn();
			}
			catch (std::exception &e)
			{
				return e.what();
			}
		}
	};

	//! LW ItemFuncs class extension
	class ItemHandler
	{
	protected:
		lwpp::LWItemTracker ItemTracker;
	public:	
		virtual const LWItemID *UseItems()
		{
			return ItemTracker.useItems();
		}
		virtual void ChangeID(const LWItemID *id_list) 
		{
			ItemTracker.changeID(id_list);
		}
	};

	//! Wrapper for ItemHandler
	/*!
	* @ingroup Adaptor
	*/
	template <class T>
	class ItemAdaptor
	{
	public:
		//! Set static callbacks for the LightWave item handler
		static void Activate(LWItemFuncs *inst)
		{
			if ((inst) && (!lwpp::LightWave::isModeler()))
			{
				inst->useItems = ItemAdaptor::UseItems;
				inst->changeID = ItemAdaptor::ChangeID;
			}
		}
	private:
		static const LWItemID *UseItems (LWInstance instance)
		{
			try
			{
				T *plugin = static_cast<T *>(instance);
				return plugin->UseItems();
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in Item::UseItems():", e.what());
				return 0;
			}
		}

		static void ChangeID (LWInstance instance, const LWItemID *ids)
		{
			try
			{
				T *plugin = static_cast<T *>(instance);
				if (plugin) 
				{
					plugin->ChangeID(ids);
				}
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in Item::ChangeID():", e.what());
				return;
			}
		}
	};

	//! LW RenderFuncs class extension

#define LWINIT_NONE -1

	class RenderHandler
	{
	protected:
		LWFrame currentFrame;
		LWTime currentTime;
		int renderMode;
	public:
		RenderHandler(void) : currentFrame(0), currentTime(0), renderMode(LWINIT_NONE) {;}

		virtual LWError Init(int mode)
		{
			renderMode = mode;
			return 0;
		}
		virtual void Cleanup()
		{
			renderMode = LWINIT_NONE;
		}

		virtual LWError NewTime(LWFrame frame, LWTime time)
		{
			currentFrame = frame;
			currentTime = time;
			return 0;
		}
	};

	//! Wrapper for RenderHandler
	/*!
	* @ingroup Adaptor
	*/
	template <class T>
	class RenderAdaptor
	{
	public:
		//! Set static callbacks for the LightWave item handler
		static void Activate(LWRenderFuncs *inst) 
		{
			if (inst)
			{
				inst->init = RenderAdaptor::Init;
				inst->cleanup = RenderAdaptor::Cleanup;
				inst->newTime = RenderAdaptor::NewTime;
			}
		}
	private:
		static LWError Init (LWInstance instance, int mode)
		{
			try
			{
				T *plugin = static_cast<T *>(instance);
				return plugin->Init(mode);
			}
			catch (std::exception &e)
			{
				return e.what();
			}
		}
		static void Cleanup (LWInstance instance)
		{
			try
			{
				T *plugin = static_cast<T *>(instance);
				plugin->Cleanup();
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in Render::Cleanup():", e.what());
				return;
			}
		}

		static LWError NewTime (LWInstance instance, LWFrame frame, LWTime time)
		{
			try
			{
				T *plugin = static_cast<T *>(instance);
				return plugin->NewTime(frame, time);
			}
			catch (std::exception &e)
			{
				return e.what();
			}
		}
	};

	//! Base class for a Lightwave UI
	class InterfaceHandler
	{
	public:
		virtual LWError Command(const char *command) {UNUSED(command);return NULL;}
	};

	//! Wrapper for InterfaceHandler
	/*!
	* @ingroup Adaptor
	*/
	template <class T>
	class InterfaceAdaptor
	{
	protected:
		//! Set static callbacks for the LightWave Interface handler
		static int ActivateUI(LWInterface *ui)
		{
			ui->command = InterfaceAdaptor::Command;
			return AFUNC_OK;
		}
	private:
		static LWError Command (LWInstance instance, const char* command)
		{
			T *plugin = (T *) instance;
			return plugin->Command(command);
		}
	};

	//! Base class for all plugins that use an XPanels user interface
	class XPanelInterface : public XPanelView
	{
	public:
		XPanel LW_XPanel;

		//! Put data into a XPanel View
		virtual void *DataGet(unsigned int ) 
		{
			return 0;
		}
		//! Retrieve data fom a XPanel View
		virtual LWXPRefreshCode DataSet(unsigned int , void *)
		{
			return LWXPRC_NONE;
		}

		virtual void ButtonClick(unsigned int)
		{
			;
		}

		virtual void ChangeNotify (LWXPanelID , unsigned int , unsigned int , int)
		{
			;
		}

		virtual void PanelDestroyNotify(void)
		{
			LW_XPanel.setID(0); // just set the ID to 0 and don't actually close the XPanel
			LW_XPanel.DestroyOnExit(false); // let LW handle this now
		}

		virtual void ControlDraw ( unsigned int , XPDrawArea &)
		{
			;
		}
		virtual void ControlZoom ( unsigned int , int , int , int *, int )
		{
			;
		}

		void PopCommand (int, int)
		{
			;
		}
		// protected:
		// Callbacks
		static void LWXPanelPopCmdFunc (LWXPanelID panel, int cid, int cmd);

		static void LWXPanelControlDrawFunc (LWXPanelID panel, unsigned int cid, LWXPDrAreaID reg, int w, int h );

		static void LWXPanelControlZoomFunc (LWXPanelID panel, unsigned int cid, int x, int y, int *region, int clickcount );

		//! Put data into a XPanel View
		static void *CB_DataGet(LWInstance inst, unsigned int vid );

		//! Retrieve data fom a XPanel View
		static LWXPRefreshCode CB_DataSet(LWInstance inst,unsigned int vid, void *value);

		//! Callback for XPanel button Clicks
		static void LWXPanelBtnClickFunc (LWXPanelID panel, int cid);

		static void LWXPanelChangeNotifyFunc (LWXPanelID panel, unsigned int cid, unsigned int vid, int event_type);

		static void LWXPanelDestroyNotifyFunc(void *data);

		virtual void CreateViewXPanel(LWXPanelControl *controls, LWXPanelDataDesc *desc, LWXPanelHint *hints = 0, bool do_destroy = false);

		virtual void CreateViewXPanel(void *host, LWXPanelControl *controls, LWXPanelDataDesc *desc, LWXPanelHint *hints = 0, bool do_destroy = false);

		XPanelInterface(){;}

		virtual ~XPanelInterface(){;}

		virtual int Interface (int version, LWInterface *local, void *serverdata)
		{
			UNUSED(version);
			UNUSED(local);
			UNUSED(serverdata);
			return AFUNC_OK;
		}
	};

	template <class T>
	class XPanelAdaptor
	{
	public:
		XPanelAdaptor(const char *name, const char *className, ServerTagInfo tags[])	
		{
			LWServer::AddPlugin(className, name, Interface, tags);
		}

		static int Interface (int version, GlobalFunc *global, void *loc, void *serverdata)
		{
			if ( version < LWINTERFACE_VERSION ) return AFUNC_BADVERSION;
			try
			{
				if (loc)
				{
					lwpp::SetSuperGlobal(global);
					LWInterface *local = static_cast<LWInterface *>(loc);
					T *plugin = static_cast<T *>(local->inst);
					return plugin->Interface(version, local, serverdata);
				}
				return AFUNC_OK;
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in XPanelAdaptor::Interface():", e.what());
				return AFUNC_BADAPP;
			}
		}
	};

	// Macros

#define IMPLEMENT_XPANELHANDLER(pluginClass) \
	class XPanel##pluginClass##Handler : public pluginClass##Handler,	public XPanelInterface \
	{ \
	public: \
	XPanel##pluginClass##Handler(void *g, void *context, LWError *err) \
	: pluginClass##Handler(g, context, err) \
	{	;	} \
	}

#define IMPLEMENT_XPANELADAPTOR(pluginClass, vers) \
	template <class T, int version = vers> class XPanel##pluginClass##Adaptor : public pluginClass##Adaptor<T, vers>, public XPanelAdaptor<T> \
	{ \
	public: \
	XPanel##pluginClass##Adaptor(const char *name, ServerTagInfo tags[] = 0) \
	: XPanelAdaptor<T>(name, #pluginClass "Interface", tags), \
	pluginClass##Adaptor<T, vers>(name, tags) \
	{ ; } \
	}

#define IMPLEMENT_LWPANELHANDLER(pluginClass) \
	class LWPanel##pluginClass##Handler : public pluginClass##Handler,	public LWPanelInterface \
	{ \
	public: \
	LWPanel##pluginClass##Handler(void *g, void *context, LWError *err) \
	: pluginClass##Handler(g, context, err) \
	{	;	} \
	}

#define IMPLEMENT_LWPANELADAPTOR(pluginClass, vers) \
	template <class T, int version = vers> class LWPanel##pluginClass##Adaptor : public pluginClass##Adaptor<T, vers>, public LWPanelAdaptor<T> \
	{ \
	public: \
	LWPanel##pluginClass##Adaptor(const char *name, ServerTagInfo tags[] = 0) \
	: LWPanelAdaptor<T>(name, #pluginClass "Interface", tags), \
	pluginClass##Adaptor<T, vers>(name, tags) \
	{ ; } \
	}

} // end of namespace lwpp

#pragma warning( pop ) 
#endif // LWPP_HANDLER_H