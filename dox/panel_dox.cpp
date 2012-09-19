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





