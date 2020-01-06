#ifndef LWPP_GIZMO_HANDLER_H
#define LWPP_GIZMO_HANDLER_H

#if LW_SDK >= 93
#include <lwgizmo.h>
#include <lwpp/customobject_handler.h>

namespace lwpp
{
  //! Base class for Gizmos
  /*!
  * @note Work in progress
  * @ingroup Handler
  */
  class GizmoHandler
  {
  public:
    virtual void gzDone    () {;}
    virtual void gzDraw    (lwpp::CustomObjAccess &){;}
    virtual const char * gzHelp     ( LWToolEvent *){return "";}
    virtual bool gzDirty   (){return false;}
    virtual int  gzCount   (LWToolEvent *){return 0;}
    virtual int  gzHandle (LWToolEvent *, int i, LWDVector pos){return 0;}
    virtual int  gzStart  (LWToolEvent *){return 0;}
    virtual int  gzAdjust   ( LWToolEvent *, int i){return 0;}
    virtual bool gzDown     ( LWToolEvent *){return false;}
    virtual void gzMove     ( LWToolEvent *){;}
    virtual void gzUp       ( LWToolEvent *){;}
    virtual void gzEvent    ( int code){;}
    virtual LWXPanelID gzPanel (){return 0;}
    virtual int gzEnd(LWToolEvent*, int i) { ; }

    virtual const LWItemID* gzPickItems (const LWItemID* drawitems){return drawitems;}
    virtual const LWItemID* gzPickItems96 (const LWItemID* drawitems, const unsigned int* drawparts){return drawitems;}
  };

  //! Wrapper for Gizmos
  /*!
  * @ingroup Adaptor
  */
  template <class T, int maxVersion = 2, int minVersion = 2>
  class GizmoAdaptor
  {
  public:
    GizmoAdaptor(const char *className, const char *name, ServerTagInfo *tags)
    {
      LWServer::AddPlugin(className, name, Activate, tags);
    }
    //! Set static callbacks for a LightWave CustomObject
    static int Activate (int version, GlobalFunc *global, LWInstance inst, void *serverData)
    {
      if ( version > maxVersion ) return AFUNC_BADVERSION;
      if ( version < minVersion ) return AFUNC_BADVERSION;
      try
      {
        UNUSED(serverData);
        lwpp::SetSuperGlobal(global);

        LWGizmo *plugin = static_cast<LWGizmo *>(inst);
        //plugin->instance = inst;
        plugin->pickItems = GizmoAdaptor::PickItems;
        if (plugin->gizmo)
        {
          plugin->gizmo->done = GizmoAdaptor::Done;
          plugin->gizmo->draw = GizmoAdaptor::Draw;
          plugin->gizmo->help = GizmoAdaptor::Help;
          plugin->gizmo->dirty = GizmoAdaptor::Dirty;
          plugin->gizmo->count = GizmoAdaptor::Count;
          plugin->gizmo->handle = GizmoAdaptor::Handle;
          plugin->gizmo->start = GizmoAdaptor::Start;
          plugin->gizmo->adjust = GizmoAdaptor::Adjust;
          plugin->gizmo->down = GizmoAdaptor::Down;
          plugin->gizmo->move = GizmoAdaptor::Move;
          plugin->gizmo->up = GizmoAdaptor::Up;
          plugin->gizmo->event = GizmoAdaptor::Event;
          plugin->gizmo->panel = GizmoAdaptor::Panel;
          plugin->gizmo->end = GizmoAdaptor::End;
        }
        return AFUNC_OK;
      } 
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::Activate():", e.what());
        return AFUNC_BADAPP;
      }
    }
  private:

    static const LWItemID* PickItems (LWInstance instance, const LWItemID* drawitems)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->gzPickItems(drawitems);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::PickItems():", e.what());
        return 0;
      }
    }

    static void Done (LWInstance instance)
    {
      try
      {
        T *plugin = (T *) instance;
        plugin->gzDone();
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::Done():", e.what());
      }
    }

    static void Draw (LWInstance instance, LWCustomObjAccess *coa)
    {
      try
      {
        T *plugin = (T *) instance;
        plugin->gzDraw(lwpp::CustomObjAccess(coa));
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::Draw():", e.what());
      }
    }

    static const char * Help (LWInstance instance, LWToolEvent *lte)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->gzHelp(lte);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::Help():", e.what());
        return "";
      }
    }

    static int Dirty(LWInstance instance)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->gzDirty() ? 1 : 0;
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::Dirty():", e.what());
        return 0;
      }
    }

    static int Count (LWInstance instance, LWToolEvent *lte)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->gzCount(lte);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::Count():", e.what());
        return 0;
      }
    }

    static int Handle (LWInstance instance, LWToolEvent *lte, int i, LWDVector pos)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->gzHandle(lte, i, pos);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::Handle():", e.what());
        return 0;
      }
    }

    static int Start (LWInstance instance, LWToolEvent *lte)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->gzStart(lte);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::Start():", e.what());
        return 0;
      }
    }

    static int Adjust (LWInstance instance, LWToolEvent *lte, int i)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->gzAdjust(lte, i);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::Adjust():", e.what());
        return 0;
      }
    }

    static int Down (LWInstance instance, LWToolEvent *lte)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->gzDown(lte) ? 1 : 0;
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::Down():", e.what());
        return 0;
      }
    }

    static void Move (LWInstance instance, LWToolEvent *lte)
    {
      try
      {
        T *plugin = (T *) instance;
        plugin->gzMove(lte);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::Move():", e.what());
      }
    }

    static void Up (LWInstance instance, LWToolEvent *lte)
    {
      try
      {
        T *plugin = (T *) instance;
        plugin->gzUp(lte);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::Up():", e.what());
      }
    }

    static void Event (LWInstance instance, int code)
    {
      try
      {
        T *plugin = (T *) instance;
        plugin->gzEvent(code);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::Event():", e.what());
      }
    }

    static LWXPanelID Panel (LWInstance instance)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->gzPanel();
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::Panel():", e.what());
        return 0;
      }
    }

    static void End(LWInstance instance, LWToolEvent*evt, int i)
    {
      try
      {
        T* plugin = (T*)instance;
        return plugin->gzEnd(evt,i);
      }
      catch (std::exception & e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::End():", e.what());
      }
    }
  };

  //! Base class for Gizmos that only perform drawing
  /*!
  * @note Work in progress
  * @ingroup Handler
  */
  class GizmoDrawHandler
  {
  public:
    virtual void gzDraw    (lwpp::CustomObjAccess &){;}
    virtual const LWItemID* gzPickItems (const LWItemID* drawitems){return drawitems;}
    virtual const LWItemID* gzPickItems96 (const LWItemID* drawitems, const unsigned int* drawparts){return drawitems;}
  };

  //! Wrapper for Gizmos that only perform drawing
  /*!
  * @ingroup Adaptor
  */
  template <class T, int maxVersion = 2, int minVersion = 2>
  class GizmoDrawAdaptor
  {
  public:
    GizmoDrawAdaptor(const char *className, const char *name, ServerTagInfo *tags)
    {
      LWServer::AddPlugin(className, name, Activate, tags);
    }
    //! Set static callbacks for a LightWave CustomObject
    static int Activate (int version, GlobalFunc *global, LWInstance inst, void *serverData)
    {
      if ( version > maxVersion ) return AFUNC_BADVERSION;
      if ( version < minVersion ) return AFUNC_BADVERSION;
      try
      {
        UNUSED(serverData);
        lwpp::SetSuperGlobal(global);

        LWGizmo *plugin = static_cast<LWGizmo *>(inst);
        //plugin->instance = inst;
#if (LW_SDK < 96)
        {
            plugin->pickItems = GizmoDrawAdaptor::PickItems;
        }
#else
        {
          plugin->pickItems = GizmoDrawAdaptor::PickItems96;
        }
#endif
        if (plugin->gizmo)
        {
          plugin->gizmo->done = 0;
          plugin->gizmo->draw = GizmoDrawAdaptor::Draw;
          plugin->gizmo->help = 0;
          plugin->gizmo->dirty = 0;
          plugin->gizmo->count = 0;
          plugin->gizmo->handle = 0;
          plugin->gizmo->start = 0;
          plugin->gizmo->adjust = 0;
          plugin->gizmo->down = 0;
          plugin->gizmo->move = 0;
          plugin->gizmo->up = 0;
          plugin->gizmo->event = 0;
          plugin->gizmo->panel = 0;
          plugin->gizmo->end = 0;
        }
        return AFUNC_OK;
      } 
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::Activate():", e.what());
        return AFUNC_BADAPP;
      }
    }
  private:

    static const LWItemID* PickItems (LWInstance instance, const LWItemID* drawitems)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->gzPickItems(drawitems);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::PickItems():", e.what());
        return 0;
      }
    }

    static const LWItemID* PickItems96 (LWInstance instance, const LWItemID* drawitems, const unsigned int* drawparts)
    {
      try
      {
        T *plugin = (T *) instance;
        return plugin->gzPickItems96(drawitems, drawparts);
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::PickItems96():", e.what());
        return 0;
      }
    }

    static void Draw (LWInstance instance, LWCustomObjAccess *coa)
    {
      try
      {
        T *plugin = (T *) instance;
        plugin->gzDraw(lwpp::CustomObjAccess(coa));
      }
      catch (std::exception &e)
      {
        lwpp::LWMessage::Error("An exception occured in GizmoAdaptor::Draw():", e.what());
      }
    }
  };


  // #define ADD_GIZMO(pluginClass, type, min, max) \


} // end lwpp
#endif // version check

#endif // LWPP_GIZMO_HANDLER_H