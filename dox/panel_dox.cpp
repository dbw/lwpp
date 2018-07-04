/*! @brief */

/*! \fn LWControl *lwpp::LWPanel::AddItem (const char *title, LWItemType type, int width = 0)
 	@code
    /* Define a lwpp::PanelControl member on your class */
    lwpp::PanelControl control;

    /* On the Options() of your class */
    LWError pluginclass::Options ()
    {
      /*
       Creates the panel with title
       Panel is a member of LWPanelInterface
       used by the IMPLEMENT_LWPANELHANDLER(pluginClass) macro
       */
      Panel.Create("Plugin title");

      /* Assign the type of control */
      /* How to get the Objects list on the scene */
      control = Panel.AddItem("Objects", LWI_OBJECT, 80);

      /* How to get the Objects list on the scene */
      control = Panel.AddItem("Objects", LWI_OBJECT, 80);

      /* How to get the Lights list on the scene */
      control = Panel.AddItem("Lights", LWI_LIGHT, 80);

      /* How to get the Cameras list on the scene */
      control = Panel.AddItem("Cameras", LWI_CAMERA, 80);

      /* How to get the Bones list on the scene */
      control = Panel.AddItem("Bones", LWI_BONE, 80);

      /* How to get the Images list on the scene */
      control = Panel.AddItem("Images", LWI_IMAGE, 80);

      /* How to get the All the items list on the scene */
      control = Panel.AddItem("Items", LWI_ANY, 80);
      ...
    }
    @endcode
 */
    
/*! \fn LWControl *lwpp::LWPanel::AddListbox (const std::string &title, int width, int (*count)(void *), char *(*name)(void *, int), int items, int topVisible = 0)
 *  @image html panellistbox.jpg "A List Box for 5 items. 150 pixels width."
 *  How to add a AddListbox
 *	@code
	static char *choices[] = { "One", "Two", "Three", NULL };

	static int lboxCount(void *);
	static char * lboxName(void *data, int index);
	
	int yourclass::lboxCount(void *){	return 3; }
	
	char * yourclass::lboxName(void *data, int index)
	{
		if ( index >= 0 && index < 3 )
			return choices[ index ];
		else
			return NULL;
	}
	
	// On yourclass::Options()
	control = Panel.AddListbox(std::string("List Box"), 150, &yourclass::lboxCount, &yourclass::lboxName, 5, 0);
 
	@endcode
 */
    
/*! \fn template<typename T> LWControl *lwpp::LWPanel::AddListbox (const std::string &title, int width, int items, int topVisible = 0)
	 *  @image html panellistboxt.jpg "A List Box for 5 items. 150 pixels width."
	 *  How to add a AddListbox
		@code
		// On your header(yourclass.h) file:

		class yourclass: public lwpp::LWPanelMasterHandler,
						 public lwpp::LWPanelControlCallBacks,
		{
		public:
			// CALLBACKS
			//! listBox count 
			int controlCount(lwpp::PanelControl lwcontrol);

			//! control events 
			virtual void controlEvent(lwpp::PanelControl& lwcontrol);

			//! listBox name 
			const char * controlName(lwpp::PanelControl lwcontrol, int index);
			

			// Constructor & Destructor
			yourclass (void *priv, void *context, LWError *err);
			~yourclass ();
		protected:
			// The member for the List Box control
			lwpp::PanelControl control;
		};

		// On your source (yourclass.cpp) file:
		static char *choices[] = { "One", "Two", "Three", NULL };

		//Constructor
		yourclass::yourclass( void *priv, void *context, LWError *err) :
			lwpp::LWPanelMasterHandler(priv, context, err),
		{
		}
		// Destructor
		yourclass::~yourclass(){ ; }

		// CONTROL COUNT CALLBACK
		int yourclass::controlCount(lwpp::PanelControl lwcontrol)
		{ 
			if (lwcontrol == control)
			{
				return 3;
			}
			return 0;
		}
		
		// CONTROL NAME CALLBACK
		const char * yourclass::controlName(lwpp::PanelControl lwcontrol, int index)
		{
			if (lwcontrol == control)
			{
				if ( index >= 0 && index < 3 )
					return choices[ index ];
				else
					return NULL;
			}
			return "";
		}
		
		// CONTROL EVENT CALLBACK
		void yourclass::controlEvent(lwpp::PanelControl& lwcontrol)
		{
			// If it's the list box
			if (lwcontrol == control)
			{
				// Do here what you want when the user clicks the control
				....
			}
		}

		// LWPANEL CREATION
		LWError yourclass::Options ()
		{
			// Creates the panel with title
			Panel.Create("Your Panel Title Here");

			// ADD THE CONTROLS
			control = Panel.AddListbox<yourclass>(std::string("List Box Templated"), 150, 5, 0);
			
			// SET THE CALLBACKS
			control.SetEventCallback<yourclass>();
			
			// SET THE PANEL USER DATA
			Panel.SetUserData(this);

			// Opens a NON Modal Panel
			Panel.Open(PANF_FRAME | PANF_NOBUTT | PANF_RESIZE);

			// Opens a Modal Panel
			// Panel.Open(PANF_FRAME | PANF_BLOCKING);

			return AFUNC_OK;

		}

		ServerTagInfo yourclass_tags[] =
		{
			{ "Your Plugin Name", SRVTAG_USERNAME },
			{ "Your Plugin Description", SRVTAG_DESCRIPTION },
			{ NULL }
		};

		lwpp::LWPanelMasterAdaptor<yourclass, 4, 4, LWMAST_SCENE> Yourclass_Instance("Your Master Plugin Name", yourclass_tags);
		
	 *	@endcode
	 */
/*! \fn LWControl *lwpp::LWPanel::AddMultiListbox (const char *title, int width, int items, int topVisible, int (*count)(void *), char *(*name)(void *, int, int),int (*colwidth)(void *, int i))
	 *  @image html panelmultilistbox.jpg "A Multi List Box, 2 columns, for 5 items. 200 pixels width."
	 *	@code
		// On your header(yourclass.h) file:

		class yourclass: public lwpp::LWPanelMasterHandler,
						 public lwpp::LWPanelControlCallBacks,
						 public lwpp::LWPanelMultiListControlCallBacks,
		{
		public:
			// CALLBACKS
			//! MultilistBox count 
			static int mlboxCount(void *);

			//! MultilistBox name 
			static char * mlboxName(void *userdata, int index, int column);
			
			//! MultilistBox Column widths
			static int mlboxColW(void *userdata, int colindex);

			//! Control Count 
			int yourclass::controlCount(lwpp::PanelControl lwcontrol);

			//! Control Events 
			virtual void controlEvent(lwpp::PanelControl& lwcontrol);

			// Constructor & Destructor
			yourclass (void *priv, void *context, LWError *err);
			~yourclass ();
		protected:
			// The member for the Multi List Box control
			lwpp::PanelControl control;
		};

		// On your source (yourclass.cpp) file:
		// List of items for the control
		static char *choices[] = { "One", "Two", "Three", NULL };
		
		// The checks of each item
		static bool choicesState[] = { true, false, true, NULL };

		//Constructor
		yourclass::yourclass( void *priv, void *context, LWError *err) :
			lwpp::LWPanelMasterHandler(priv, context, err),
		{ ; }
		// Destructor
		yourclass::~yourclass(){ ; }

		// CONTROL COUNT CALLBACK
		int yourclass::controlCount(lwpp::PanelControl lwcontrol)
		{ 
			if (lwcontrol == control)
			{
			  return 3;
			}
			return 0;
		}


		// MULTILIST BOX COUNT CALLBACK
		int yourclass::mlboxCount(void *)
		{	
			return 3; // The number of items
		}
					
		// MULTILIST BOX NAME CALLBACK
		char * yourclass::mlboxName(void *userdata, int index, int column)
		{
			// LW is asking for the column names
			if (index == -1) 
			{
				switch (column)
				{
					case 0:	return "State"; break; // Title of the 1st column
					case 1: return "Choices"; break; // Title of the 2nd column
					default: return NULL; break;
				}
			}

			// String used to return the check mark
			static std::string ret;
			ret.clear();

			// LW is asking for the values
			if ( index >= 0 && index < 3 )
			{
				// Check the column
				switch (column)
				{
					case 0:	
					{
						if(choicesState[index])		
						{	ret = "\03(i:MIMG_CHECKMARK)"; // The check symbol
						} else
						{
							ret = "";
						}
						return (char *)ret.c_str(); 
						break;
						
					}

					case 1: return choices[ index ];break;

					default:
						return NULL;
						break;
				}
			}
			return (char *)ret.c_str();
		}

		// MULTILIST BOX COLUMN WIDTH CALLBACK
		int yourclass::mlboxColW(void *userdata, int colindex)
		{
			// Return different widths depending of the colum index
			switch (colindex)
			{
				case 0: return 35; // 15 pixels for the 1st column
				case 1: return 165; // 185 pixels for the 2nd column
				case 2: return 0; // Returning 0 for a column with indicates is the last one
				default: return 0;
			}
		}

		// CONTROL EVENT CALLBACK
		void yourclass::controlEvent(lwpp::PanelControl& lwcontrol)
		{
			// If it's the multilist box
			if (lwcontrol == control)
			{
				// Do here what you want when the user clicks the control
				....
			}
		}

		// LWPANEL CREATION
		LWError yourclass::Options ()
		{
			// Creates the panel with title
			Panel.Create("Your Panel Title Here");

			// ADD THE CONTROLS
			control = Panel.AddMultiListbox("Multi List Box", 200, 5, 0, &yourclass::mlboxCount, &yourclass::mlboxName,  &yourclass::mlboxColW);
			
			// SET THE CALLBACKS
			control.SetEventCallback<yourclass>();
			
			// SET THE CONTROL USER DATA
			control.SetUserData(this);
			
			// SET THE PANEL USER DATA
			Panel.SetUserData(this);

			// Opens a NON Modal Panel
			Panel.Open(PANF_FRAME | PANF_NOBUTT | PANF_RESIZE);

			// Opens a Modal Panel
			// Panel.Open(PANF_FRAME | PANF_BLOCKING);

			return AFUNC_OK;

		}

		ServerTagInfo yourclass_tags[] =
		{
			{ "Your Plugin Name", SRVTAG_USERNAME },
			{ "Your Plugin Description", SRVTAG_DESCRIPTION },
			{ NULL }
		};

		lwpp::LWPanelMasterAdaptor<yourclass, 4, 4, LWMAST_SCENE> Yourclass_Instance("Your Master Plugin Name", yourclass_tags);
		
	 *	@endcode
	 *  How to add a AddMultiListbox
	 *	control = Panel.AddMultiListbox("Multi List Box", 200, 5, 0, &yourclass::mlboxCount, &yourclass::mlboxName,  &yourclass::mlboxColW);
	*/





