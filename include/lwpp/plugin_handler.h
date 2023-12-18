#ifndef LWPP_HANDLER_H
#define LWPP_HANDLER_H

#include <lwfilter.h>
#include <lwgizmo.h>
#include "lwpp/message.h"
#include "lwpp/lwpanel_handler.h"
#include <lwpp/dynamicHints.h>
#include "lwpp/item.h"
#include "lwpp/customobject_access.h"

#pragma warning( push )
#pragma warning( disable : 4100 )

namespace lwpp
{
	class InstanceUpdate 
	{		
		static TransientGlobal<LWInstUpdate> globPtr;
	public:
		virtual ~InstanceUpdate() {
			globPtr.init();
		}
		static void Update (const char *className, LWInstance inst)
		{
			if ((globPtr.available()) && (className))	globPtr.getGlobal()(className, inst);
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
		virtual ~InstanceHandler() = default;
		virtual const char *DescLn() {return m_description.c_str();}
		void SetName(const char *n) {m_pluginName = n;}
		void SetClassName(const char *n) {m_className = n;}
		void SetDescription(const char *n) {m_description = n;}
		void SetDescription(const std::string& s) {m_description = s;}
		void Update () {
#ifdef _DEBUG
			dout << "Update: " << this << " (" << m_className <<  ")\n";
#endif	
			m_update.Update(m_className, this);
		}
		void Update (const char *cName) {m_update.Update(cName, this);}
		void Update(void *inst) { m_update.Update(m_className, inst); }
		void Update(const char* cName, void* inst) { m_update.Update(cName, inst); }
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
			LWPP_DBG_ENTER_FUNC;
			try
			{
				LWError localErr = "\0";
				T *plugin = new T(priv, context, &localErr);
#ifdef _DEBUG
				dout << "Created plugin instance: " << plugin << "\n";
#endif	
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
			LWPP_DBG_ENTER_FUNC;
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
			LWPP_DBG_ENTER_FUNC;
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
			LWPP_DBG_ENTER_FUNC;
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
			LWPP_DBG_ENTER_FUNC;
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
			LWPP_DBG_ENTER_FUNC;
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
		virtual ~ItemHandler() = default;
		virtual const LWItemID *UseItems()
		{
			return ItemTracker.useItems();
		}
		virtual void ChangeID(const LWItemID *id_list) 
		{
			ItemTracker.changeID(id_list);
		}
		void trackItem(LWItem &item)
		{
			ItemTracker.trackItem(item);
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
			LWPP_DBG_ENTER_FUNC;
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
			LWPP_DBG_ENTER_FUNC;
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
		RenderHandler() : currentFrame(0), currentTime(0), renderMode(LWINIT_NONE) {;}
		virtual ~RenderHandler() = default;

		virtual LWError Init(int mode)
		{
			renderMode = mode;
			return nullptr;
		}
		virtual void Cleanup()
		{
			renderMode = LWINIT_NONE;
		}

		virtual LWError NewTime(LWFrame frame, LWTime time)
		{
			currentFrame = frame;
			currentTime = time;
			return nullptr;
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
			LWPP_DBG_ENTER_FUNC;
			T *plugin = (T *) instance;
			return plugin->Command(command);
		}
	};

//#define XPAN_STAT virtual lwpp::XPanel& GetXPanel() override {static lwpp::XPanel pan; return pan;}
#define XPAN_STAT 

	//! Base class for all plugins that use an XPanels user interface
	class XPanelInterface : public XPanelView
	{
	public:
		XPanel LW_XPanel;
		//virtual XPanel &GetXPanel() = 0;
		XPanel& GetXPanel() { return LW_XPanel; }
	protected:
		DynamicHints mDynaHints;
		DynamicControlData mDynaControl;
	public:
		XPanelInterface() = default;
		virtual ~XPanelInterface()
		{				
			if (!mDynaControl.isEmpty())
			{
				// make sure a panel is destroyed if there's dynamic control data
				GetXPanel().setAutoDestroy(true);
			}			
			GetXPanel().Destroy();
		}

		//! Put data into a XPanel View
		virtual void *DataGet(unsigned int ) override
		{
			return nullptr;
		}
		//! Retrieve data fom a XPanel View
		virtual LWXPRefreshCode DataSet(unsigned int , void *) override 
		{
			return LWXPRC_DFLT;
		}

		virtual void ButtonClick(unsigned int) override
		{
			;
		}

		virtual LWXPRefreshCode ChangeNotify (LWXPanelID , unsigned int , unsigned int , int) override
		{
			return LWXPRC_NONE;
		}

		virtual void PanelDestroyNotify(void)
		{
			GetXPanel().setID(0); // just set the ID to 0 and don't actually close the XPanel
			GetXPanel().DestroyOnExit(false); // let LW handle this now
		}

		virtual void ControlDraw ( unsigned int , XPDrawArea &)
		{
			;
		}
		virtual void ControlZoom ( unsigned int cid, int x, int y, int *rect, int clickcount )
		{
			;
		}

		virtual void PopCommand (int cid, int i_cmd) override
		{
			;
		}
		// protected:
		// Callbacks
		static void LWXPanelPopCmdFunc (LWXPanelID panel, int cid, int cmd);

		static void LWXPanelControlDrawFunc (LWXPanelID panel, unsigned int cid, LWXPDrAreaID reg, int w, int h );

		static void LWXPanelControlZoomFunc (LWXPanelID panel, unsigned int cid, int x, int y, int *region, int clickcount );
		static void LWXPanelZoomFunc(LWXPanelID panel, unsigned int cid, int x, int y, int* region, int clickcount);

		//! Put data into a XPanel View
		static void *CB_DataGet(LWInstance inst, unsigned int vid );

		//! Retrieve data fom a XPanel View
		static LWXPRefreshCode CB_DataSet(LWInstance inst,unsigned int vid, void *value);

		//! Callback for XPanel button Clicks
		static void LWXPanelBtnClickFunc (LWXPanelID panel, int cid);

		static LWXPRefreshCode LWXPanelChangeNotifyFunc (LWXPanelID panel, unsigned int cid, unsigned int vid, int event_type);

		static void LWXPanelDestroyNotifyFunc(void *data);

		virtual void CreateViewXPanel(lwpp::DynamicControlData &controls, lwpp::DynamicHints &hints, bool do_destroy = false);

		virtual void CreateViewXPanel(void *host, lwpp::DynamicControlData &controls, lwpp::DynamicHints &hints, bool do_destroy = false);

		virtual void CreateViewXPanel(LWXPanelControl *controls, LWXPanelDataDesc *desc, LWXPanelHint *hints = 0, bool do_destroy = false);

		virtual void CreateViewXPanel(void *host, LWXPanelControl *controls, LWXPanelDataDesc *desc, LWXPanelHint *hints = 0, bool do_destroy = false);

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
   
	class GizmoHandler
	{
		LWGizmo *gizmo = nullptr;
	public:
		int gzInterface(int version, LWGizmo *global)
		{
			gizmo = global;
			return AFUNC_OK;
		}

		virtual ~GizmoHandler() = default;

		virtual void gzDone() { ; }
		virtual void gzDraw(lwpp::CustomObjAccess &access) { ; }
		virtual const char * /* language encoded */ gzHelp(LWToolEvent *) { return ""; }
		virtual int gzDirty() { return 0; }
		virtual int gzCount(LWToolEvent *) { return 1; }
		virtual int gzHandle(LWToolEvent *, int i, LWDVector pos) { return 0; }
		virtual int gzStart(LWToolEvent *) { return 0; }
		virtual int gzAdjust(LWToolEvent *, int i) { return 0; }
		virtual int  gzDown(LWToolEvent *) { return false; }
		virtual void gzMove(LWToolEvent *) { ; }
		virtual void gzUp(LWToolEvent *) { ; }
		virtual void gzEvent(int code) { ; }
		virtual LWXPanelID gzPanel() { return nullptr; }
		virtual int gzEnd(LWToolEvent *, int i) {  return 0; }
	};

	template <class T>
	class GizmoAdaptor
	{
	public:
		GizmoAdaptor(const char *name, const char *className, ServerTagInfo tags[])
		{
			LWServer::AddPlugin(className, name, Gizmo, tags);
		}
		static int Gizmo(int version, GlobalFunc *global, void *loc, void *serverdata)
		{
			if (version < LWGIZMO_VERSION) return AFUNC_BADVERSION;
			try
			{
				if (loc)
				{
					lwpp::SetSuperGlobal(global);
					LWGizmo *local = static_cast<LWGizmo *>(loc);
					T *plugin = static_cast<T *>(local->instance);
					local->gizmo->done = GizmoAdaptor::cbDone;
					local->gizmo->draw = GizmoAdaptor::cbDraw;
					local->gizmo->help = GizmoAdaptor::cbHelp;
					local->gizmo->dirty = GizmoAdaptor::cbDirty;
					local->gizmo->count = GizmoAdaptor::cbCount;
					local->gizmo->handle = GizmoAdaptor::cbHandle;
					local->gizmo->start = GizmoAdaptor::cbStart;
					local->gizmo->adjust = GizmoAdaptor::cbAdjust;
					local->gizmo->down = GizmoAdaptor::cbDown;
					local->gizmo->move = GizmoAdaptor::cbMove;
					local->gizmo->up = GizmoAdaptor::cbUp;
					local->gizmo->event = GizmoAdaptor::cbEvent;
					local->gizmo->panel = GizmoAdaptor::cbPanel;
					local->gizmo->end = GizmoAdaptor::cbEnd;
					return plugin->gzInterface(version, local);
				}
				return AFUNC_OK;
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::Interface():", e.what());
				return AFUNC_BADAPP;
			}
		}

		static void cbDone(LWInstance inst)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) plugin->gzDone();
		}
		static void cbDraw(LWInstance inst, LWCustomObjAccess *cobj)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) 
			{
				lwpp::CustomObjAccess custom(cobj);
				plugin->gzDraw(custom);
			}
		}
		static const char *cbHelp(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->gzHelp(event);
			return "";
		}
		static int cbDirty(LWInstance inst)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->gzDirty();
			return 0;
		}
		static int cbCount(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->gzCount(event);
			return 0;
		}
		static int cbHandle(LWInstance inst, LWToolEvent *event, int i, LWDVector pos)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->gzHandle(event, i, pos);
			return 0;
		}
		static int cbStart(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->gzStart(event);
			return 0;
		}
		static int cbAdjust(LWInstance inst, LWToolEvent *event, int i)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->gzAdjust(event, i);
			return 0;
		}
		static int cbDown(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->gzDown(event);
			return 0;
		}
		static void cbMove(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) plugin->gzMove(event);
		}
		static void cbUp(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) plugin->gzUp(event);
		}
		static void cbEvent(LWInstance inst, int code)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) plugin->gzEvent(code);
		}
		static LWXPanelID cbPanel(LWInstance inst)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) plugin->gzPanel();
			return nullptr;
		}
		static int cbEnd(LWInstance inst, LWToolEvent *event, int i)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->gzEnd(event, i);
			return 0;
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

#define IMPLEMENT_GIZMOHANDLER(pluginClass) \
	class Gizmo##pluginClass##Handler : public pluginClass##Handler,	public GizmoHandler \
	{ \
	public: \
	Gizmo##pluginClass##Handler(void *g, void *context, LWError *err) \
	: pluginClass##Handler(g, context, err) \
	{	;	} \
	}

#define IMPLEMENT_GIZMOADAPTOR(pluginClass, vers) \
	template <class T, int version = vers> class Gizmo##pluginClass##Adaptor : public pluginClass##Adaptor<T, vers>, public GizmoAdaptor<T> \
	{ \
	public: \
	Gizmo##pluginClass##Adaptor(const char *name, ServerTagInfo tags[] = 0) \
	: GizmoAdaptor<T>(name, #pluginClass "Gizmo", tags), \
	pluginClass##Adaptor<T, vers>(name, tags) \
	{ ; } \
	}

#define IMPLEMENT_GIZMO_XPANELHANDLER(pluginClass) \
	class GizmoXPanel##pluginClass##Handler : public pluginClass##Handler, public GizmoHandler,	public XPanelInterface\
	{ \
	public: \
	GizmoXPanel##pluginClass##Handler(void *g, void *context, LWError *err) \
	: pluginClass##Handler(g, context, err) \
	{	;	} \
	}

#define IMPLEMENT_GIZMO_XPANELADAPTOR(pluginClass, vers) \
	template <class T, int version = vers> class GizmoXPanel##pluginClass##Adaptor : public pluginClass##Adaptor<T, vers>, public GizmoAdaptor<T>,  public XPanelAdaptor<T>\
	{ \
	public: \
	GizmoXPanel##pluginClass##Adaptor(const char *name, ServerTagInfo tags[] = 0) \
	: GizmoAdaptor<T>(name, #pluginClass "Gizmo", tags), \
  XPanelAdaptor<T>(name, #pluginClass "Interface", tags), \
	pluginClass##Adaptor<T, vers>(name, tags) \
	{ ; } \
	}

#define IMPLEMENT_GIZMO_LWPANELHANDLER(pluginClass) \
	class GizmoLWPanel##pluginClass##Handler : public pluginClass##Handler,	public GizmoHandler, public LWPanelInterface \
	{ \
	public: \
	GizmoLWPanel##pluginClass##Handler(void *g, void *context, LWError *err) \
	: pluginClass##Handler(g, context, err) \
	{	;	} \
	}

#define IMPLEMENT_GIZMO_LWPANELADAPTOR(pluginClass, vers) \
	template <class T, int version = vers> class GizmoLWPanel##pluginClass##Adaptor : public pluginClass##Adaptor<T, vers>, public GizmoAdaptor<T>,  public LWPanelAdaptor<T> \
	{ \
	public: \
	GizmoLWPanel##pluginClass##Adaptor(const char *name, ServerTagInfo tags[] = 0) \
	: GizmoAdaptor<T>(name, #pluginClass "Gizmo", tags), \
  LWPanelAdaptor<T>(name, #pluginClass "Interface", tags), \
	pluginClass##Adaptor<T, vers>(name, tags) \
	{ ; } \
	}

} // end of namespace lwpp

#pragma warning( pop ) 
#endif // LWPP_HANDLER_H
