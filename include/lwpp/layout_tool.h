#pragma once
#ifndef LWPP_LAYOUT_TOOL_H
#define LWPP_LAYOUT_TOOL_H

#include <lwpp/plugin_handler.h>
#include <lwpp/customobject_access.h>
#include <lwlaytool.h>

namespace lwpp
{
	//! Base class for Layout Tool plugins
	/*!
	* @note Work in progress
	* @ingroup Handler
	* 
	* 
	* Event Processing:
	* 
 	*   Down() == true // manual mouse evaluation
	*     move()
	*     up()
    *   else
	*		Count() == 0	   
	*   		Start() // manually find the intersected handle
	*		else
	*			Handle() // provide handles
	*		Adjust()
	*		End()     
	* 
	*/
	class LayoutTool
	{
	public:
		LayoutTool()
		{

		}
		virtual ~LayoutTool() { ; }
		virtual void Done() { ; }
		virtual void Draw(CustomObjAccess &access) { ; }
		virtual const char *  Help(LWToolEvent *) { return nullptr; }
		virtual int Dirty() { return 0; }
		virtual int Count(LWToolEvent *) { return 0; }
		virtual int Handle(LWToolEvent *, int i, LWDVector pos) { return 0; }
		virtual int Start(LWToolEvent *) { return 0; }
		virtual int Adjust(LWToolEvent *, int i) { return 0; }
		virtual int Down(LWToolEvent *) { return 0; }
		virtual void Move(LWToolEvent *) { ; }
		virtual void Up(LWToolEvent *) { ; }
		virtual void Event(int code) { ; }
		virtual LWXPanelID Panel() { return nullptr; }
		virtual int End(LWToolEvent *, int i) { return 0; }
	};

#define TOOL_CB(t) local->tool->t = cb_ ## t
	//! Wrapper for an LayoutTool
	/*!
	* @ingroup Adaptor
	*/
	template <class T, int minVersion>
	class LayoutToolAdaptor
	{
	public:
		LayoutToolAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWLAYOUTTOOL_CLASS, name, Activate, tags);
		}
		//! Set static callbacks for a LightWave LayoutToolAdaptor
		static int Activate(int version, GlobalFunc *global, LWInstance inst, void *serverData)
		{
			if (version < minVersion) return AFUNC_BADVERSION;
			try
			{
				UNUSED(serverData);
				lwpp::SetSuperGlobal(global);
				auto local = static_cast<LWLayoutTool *>(inst);

				auto plugin = new T();
				local->instance = plugin;
				if (local->tool)
				{
					TOOL_CB(done);
					TOOL_CB(draw);
					TOOL_CB(help);
					TOOL_CB(dirty);
					TOOL_CB(count);
					TOOL_CB(handle);
					TOOL_CB(start);
					TOOL_CB(adjust);
					TOOL_CB(down);
					TOOL_CB(move);
					TOOL_CB(up);
					TOOL_CB(event);
					TOOL_CB(panel);
					TOOL_CB(end);
				}
				return AFUNC_OK;
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in LayoutToolAdaptor::Activate():", e.what());
				return AFUNC_BADAPP;
			}
		}
	private:
		static void cb_done(LWInstance inst)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) plugin->Done();
			delete plugin;
		}
		static void cb_draw(LWInstance inst, LWCustomObjAccess *cobj)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin)
			{
				CustomObjAccess custom(cobj);
				plugin->Draw(custom);
			}
		}
		static const char *cb_help(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->Help(event);
			return "";
		}
		static int cb_dirty(LWInstance inst)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->Dirty();
			return 0;
		}
		static int cb_count(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->Count(event);
			return 0;
		}
		static int cb_handle(LWInstance inst, LWToolEvent *event, int i, LWDVector pos)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->Handle(event, i, pos);
			return 0;
		}
		static int cb_start(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->Start(event);
			return 0;
		}
		static int cb_adjust(LWInstance inst, LWToolEvent *event, int i)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->Adjust(event, i);
			return 0;
		}
		static int cb_down(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->Down(event);
			return 0;
		}
		static void cb_move(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) plugin->Move(event);
		}
		static void cb_up(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) plugin->Up(event);
		}
		static void cb_event(LWInstance inst, int code)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) plugin->Event(code);
		}
		static LWXPanelID cb_panel(LWInstance inst)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->Panel();
			return nullptr;
		}
		static int cb_end(LWInstance inst, LWToolEvent *e, int i)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->End(e, i);
			return 0;
		}
	};

} // lwpp

#endif // LWPP_LAYOUT_TOOL_H
