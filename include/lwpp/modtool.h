#pragma once

#ifndef LWPP_MODTOOL_H
#define LWPP_MODTOOL_H

#include <lwmodtool.h>
#include <lwpp/mod_command.h>
#include <lwpp/message.h>
#include <string>

namespace lwpp
{
	class WireDrawAccess
	{
		LWWireDrawAccess *access = nullptr;
	public:
		WireDrawAccess(LWWireDrawAccess *acc)
			: access (acc)
		{}
		void moveTo(LWFVector pos, int line_mode)
		{
			access->moveTo(access->data, pos, line_mode);
		}

		void lineTo(LWFVector pos, int coord_system)
		{
			access->lineTo(access->data, pos, coord_system);
		}
		/// draw a spline from the current cursor position.
		void spline(LWFVector cp1, LWFVector cp2, LWFVector cp3, int coord_system)
		{
			access->spline(access->data,cp1, cp2, cp3, coord_system);
		}
		/// draw a circle with the given radius around the current point.
		void circle(void *data, double radius, int coord_system)
		{
			access->circle(access->data, radius, coord_system);
		}
		/// 0, 1, or 2 for orthogonal views, -1 for a perspective view.
		int getAxis() const { return access->axis; }
		/// draw the specified text at the current position
		void text(const char * /* language encoded */txt, int coord_system)
		{
			access->text(access->data, txt, coord_system);
		}
		void text(const std::string txt, int coord_system)
		{
			text(txt.c_str(), coord_system);
		}

		/// specifies the approximate size of a pixel in the world
		double getpxScale() const { return access->pxScale; }
		/// the index of the viewport being drawn to
		int getVPNumber() const { return access->viewportnumber; }

	};

	class Tool
	{
	public:
		virtual ~Tool() { ; }
		virtual void tlDone() { ; }
		virtual void tlDraw(WireDrawAccess &access) { ; }
		virtual const char * /* language encoded */ tlHelp(LWToolEvent *) { return ""; }
		virtual int tlDirty() { return 0; }
		virtual int tlCount(LWToolEvent *) { return 1; }
		virtual int tlHandle(LWToolEvent *, int i, LWDVector pos) { return 0; }
		virtual int tlStart(LWToolEvent *) { return 0; }
		virtual int tlAdjust(LWToolEvent *, int i) { return 0; }
		virtual int  tlDown(LWToolEvent *) { return false; }
		virtual void tlMove(LWToolEvent *) { ; }
		virtual void tlUp(LWToolEvent *) { ; }
		virtual void tlEvent(int code) { ; }
		virtual LWXPanelID tlPanel() { return nullptr; }
		virtual int tlUndo() { return 1; }
		virtual int tlRedo() { return 1; }
		virtual void tlTrack(LWToolEvent *) { ; }
		virtual int tlDrawScene(int) { return 0; }
		virtual int tlKey(LWDualKey) { return 0; }
		virtual void tlDrawOverlay(WireDrawAccess &, int vp, int pass) { ; }
	};

	class ModelerTool : public Tool
	{
		public:
			ModelerTool(int version, LWMeshEditTool* local){}
			~ModelerTool() {}
			virtual int Activate(void *serverdata) { return AFUNC_OK; }
			virtual int Test() { return 0; }
			virtual LWError Build(MeshEditOperator &mop) { return 0; }
			virtual void End(int keep) { ; }
			virtual LWError Select(MeshEditOperator &mop) { return 0; }
	};


#define TOOL_CB(t) local->tool->t = cb_ ## t

	//! @ingroup Adaptor
	template <typename T>
	class ModelerToolAdaptor
	{
	public:
		ModelerToolAdaptor(const char *name, ServerTagInfo *tags = 0)
		{
			LWServer::AddPlugin(LWMESHEDITTOOL_CLASS, name, Activate, tags);
		}
		//! Set static callbacks for the LightWave instance handler
		static int Activate(int version, GlobalFunc* global, void* loc, void* serverData)
		{
			if (version < LWMESHEDITTOOL_VERSION) return AFUNC_BADVERSION;
			try
			{
				if (loc)
				{
#ifdef _DEBUG
					DBG_ENTER_FUNC;
#endif		
					auto local = static_cast<LWMeshEditTool *>(loc);
					lwpp::SetSuperGlobal(global);
					//T plugin(version, local);
					//static T plugin(version, local);
					auto plugin = new T(version, local);
					local->instance = plugin;
					local->test = cbTest;
					local->build = cbBuild;
					local->end = cbEnd;
					local->select = cbSelect;
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
					TOOL_CB(undo);
					TOOL_CB(redo);
					TOOL_CB(track);
					TOOL_CB(drawScene);
					TOOL_CB(key);
					TOOL_CB(drawOverlay);
					return plugin->Activate(serverData);
				}
				return AFUNC_OK;
			}
			catch (std::exception &e)
			{
				LWMessage::Error("An exception occured in ModelerToolAdaptor::Activate():", e.what());
				return AFUNC_BADAPP;
			}
		}

		static int cbTest(LWInstance inst)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->Test();
			return LWT_TEST_NOTHING;
		}

		static LWError cbBuild(LWInstance inst, MeshEditOp* op)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) 
			{
				MeshEditOperator mop(op);
				return plugin->Build(mop);
			}
			return nullptr;
		}

		static void cbEnd(LWInstance inst, int keep)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) plugin->End(keep);
		}

		static LWError cbSelect(LWInstance inst, MeshEditOp* op)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin)
			{
				MeshEditOperator mop(op);
				return plugin->Select(mop);
			}
			return nullptr;
		}

		static void cb_done(LWInstance inst)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) plugin->tlDone();
			delete plugin;
		}
		static void cb_draw(LWInstance inst, LWWireDrawAccess *cobj)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin)
			{
				WireDrawAccess custom(cobj);
				plugin->tlDraw(custom);
			}
		}
		static const char *cb_help(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->tlHelp(event);
			return "";
		}
		static int cb_dirty(LWInstance inst)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->tlDirty();
			return 0;
		}
		static int cb_count(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->tlCount(event);
			return 0;
		}
		static int cb_handle(LWInstance inst, LWToolEvent *event, int i, LWDVector pos)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->tlHandle(event, i, pos);
			return 0;
		}
		static int cb_start(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->tlStart(event);
			return 0;
		}
		static int cb_adjust(LWInstance inst, LWToolEvent *event, int i)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->tlAdjust(event, i);
			return 0;
		}
		static int cb_down(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->tlDown(event);
			return 0;
		}
		static void cb_move(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) plugin->tlMove(event);
		}
		static void cb_up(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) plugin->tlUp(event);
		}
		static void cb_event(LWInstance inst, int code)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) plugin->tlEvent(code);
		}
		static LWXPanelID cb_panel(LWInstance inst)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->tlPanel();
			return nullptr;
		}
		static int cb_undo(LWInstance inst)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->tlUndo();
			return 0;
		}
		static int cb_redo(LWInstance inst)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->tlRedo();
			return 0;
		}
		static void cb_track(LWInstance inst, LWToolEvent *event)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) plugin->tlTrack(event);
		}
		static int cb_drawScene(LWInstance inst, int i)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->tlDrawScene(i);
			return 0;
		}
		static int cb_key(LWInstance inst, LWDualKey key)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin) return plugin->tlKey(key);
			return 0;
		}
		static void cb_drawOverlay(LWInstance inst, LWWireDrawAccess *cobj, int vp, int pass)
		{
			T *plugin = static_cast<T *>(inst);
			if (plugin)
			{
				WireDrawAccess custom(cobj);
				plugin->tlDrawOverlay(custom, vp, pass);
			}
		}
	};

} // end namespace lwpp

#endif // LWPP_MODTOOL_H
