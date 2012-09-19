#ifndef LAYOUTTOOL_H
#define LAYOUTTOOL_H

#include <lwpp/plugin_handler.h>
#include <lwpp/customobject_handler.h>

#include <lwlaytool.h>

namespace lwpp
{
	//! Base class for Layout Tool plugins
	/*!
		* @note Work in progress
		* @ingroup Handler
	*/
	class LayoutToolHandler : public InstanceHandler
	{		
		public:			
			//! Constructor
			LayoutToolHandler(void *g, void *context, LWError *err) : InstanceHandler(g, context, err, LWLAYOUTTOOL_CLASS)
			{
				;
			}
			/// Destructor
			virtual ~LayoutToolHandler() {;}

			//! Destroy the instance. Called when the user discards the tool.
			virtual void done ()
			{
				;
			}

			//! Display a wireframe representation of the tool in a 3D viewport. Typically this draws the handles.
      /*!
			*   @param co_access A custom object structure access
			*/
			virtual void draw (LWCustomObjAccess *co_access)
			{
				;
			}
			//! Returns a text string to be displayed as a help tip for this tool.
      /*!
			*   @param lwtool_event The event to show the help string for.
			*/
			virtual const char *help (LWToolEvent *lwtool_event)
			{
				return "";
			}
			//! Returns flag bit if either the wireframe or help string need to be refreshed.
			virtual int dirty ()
			{
				return 0;
			}
			//! Returns the number of handles. If zero, then start is used to set the initial handle point.
      /*!
			*   @param lwtool_event The event to the handle.
			*/
			virtual int count (LWToolEvent *lwtool_event)
			{
				return 1;
			}
			//! Returns the 3D location and priority of the handle, or zero if the handle is currently invalid.
      /*!
			* @param lwtool_event The event to the handle.
			*	@param i The number of the handle
			*	@param handle_pos The 3D location of the handle.
			*/
			int handle (LWToolEvent *lwtool_event, int i, LWDVector handle_pos)
			{
				return 0;
			}
			//! Take an initial mouse-down position and return the index of the handle that should be dragged.
      /*!
			* @param lwtool_event The event to the handle.
			*/
			int start (LWToolEvent *lwtool_event)
			{
				return 0;
			}
			//! Drag the given handle to a new location.
      /*!
			*	Drag the given handle to a new location and return the index of the handle that should continue being dragged (often the same as the input).
			* @param lwtool_event The event to the handle.
			*	@param i The number of the handle
      * @return the index of the handle that should continue being dragged (often the same as the input)
			*/
			virtual int adjust (LWToolEvent *lwtool_event, int i)
			{
				return 0;
			}
			//! Process a mouse-down event. If this function returns false, handle processing will be done
      /*!
			*	If this function returns false, handle processing will be done instead of raw mouse event processing.
			* @param lwtool_event The event to the handle.
      * @return false if handle processing will be done instead of raw mouse event processing
			*/
			int down (LWToolEvent *lwtool_event)
			{
				return 0;
			}
			//! Process a mouse-move event. This is only called if the down function returned true.
      /*
			*   @param lwtool_event The event to the handle.
			*/
			virtual void move (LWToolEvent *lwtool_event)
			{
				;
			}
			//! Process a mouse-up event. This is only called if the down function returned true.
      /*
			*	@param inst A pointer to your user data. This will be passed to each of the tool callbacks.
			*   @param lwtool_event The event to the handle.
			*/
			void up (LWToolEvent *lwtool_event)
			{
				;
			}
			//! Process a general event: DROP, RESET or ACTIVATE
      /*
			*	@param inst A pointer to your user data. This will be passed to each of the tool callbacks.
			*   @param code The event to the handle.
			*/
			virtual void event (int code)
			{
				;
			}
			//! Create and return a view-type xPanel for the tool instance.
      /*
			*	@param inst A pointer to your user data. This will be passed to each of the tool callbacks.
			*/
			virtual LWXPanelID panel ()
			{
				return 0;
			}

	};


	//! Wrapper for a Layout Tool
	/*!
	 * @ingroup Adaptor
	 */
	template <class T>
	class LayoutToolAdaptor : public InstanceAdaptor <T>
	{
		public:

			LayoutToolAdaptor(const char *name, ServerTagInfo *tags)
			{
				LWServer::AddPlugin(LWLAYOUTTOOL_CLASS, name, Activate, tags);
			}
			//! Set static callbacks for a LightWave Layout Tool
			static int Activate (int version, GlobalFunc *global, LWInstance inst, void *serverData)
			{
				UNUSED(serverData);
				try
				{
					if ( version != LWLAYOUTTOOL_VERSION ) return AFUNC_BADVERSION;
					lwpp::SetSuperGlobal(global);
					LWLayoutTool *plugin = static_cast<LWLayoutTool *>(inst);

					InstanceAdaptor<T>::Activate(static_cast<LWInstanceFuncs *>(plugin->instance));
					if (plugin->tool)
					{
						// Set the callback function for done
						plugin->tool->done = Done;
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
		// Every callback needs to be wrapped like this
		// Make sure the function signature matches the one expected _exactly_ except for adding "static"
		static void Done (LWInstance instance)
		{
			try
			{
				T *plugin = (T *) instance;
				plugin->done();
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in LayoutToolAdaptor::Done():", e.what());
			}
		}
		
	}; // end LayoutToolAdaptor


} // end namespace


#endif // LAYTOOL_H