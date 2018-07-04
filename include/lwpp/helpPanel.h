#ifndef LWPP_HELP_PANEL_H
#define LWPP_HELP_PANEL_H

#include <lwpp/panel.h>
#include <lwpp/tab_manager.h>
#include <map>
namespace lwpp
{
  /*
  * Different Pre-fab panels
  */

  //! A Help panel that displays tabs of single- or dual-column text as well as an optional image
  /*!
   * @ingroup LWPanels
   */
  class HelpPanel : private LWPanel, public LWPanelCallbacks
  {
  private:  
    //! Helper class for the help panel
    class HelpTab
    {
      const char *title;
      const char** leftCol;
      const char** rightCol; //right column is only valid for DUAL
      PanelControl leftText, rightText;
      LWPanel &panel;
    public:
      HelpTab (LWPanel &_panel, const char *_title, const char* _leftCol[], const char* _rightCol[] = 0);
      HelpTab (const HelpTab &from);
      HelpTab &operator= (const HelpTab &from);
      int Position(TabManager &manager, PanelControl &tab);
      char *GetTitle() const {return const_cast<char*>(title);}
    };
    std::vector<HelpTab> tabList;
    const unsigned char *image;
    int width;
    int height;
    int lastPos;
    char** tabLabels;
    PanelControl canvas, tabs, text;
    TabManager mainTab;
    bool finalised;
    void draw(LWControlID control, DrMode drawmode);
  public:
    //! Constructor for a help panel
    HelpPanel(const std::string title, const char**message = 0, const unsigned char *_image = 0, const int w = 0, const int h = 0);
    ~HelpPanel();
    //! Add a new tab with a title and one or two columns of text
    void AddTab(const char *title, const char* _leftCol[], const char* _rightCol[] = 0);
    //! Finalises the adding of tabs and computes the layout. Subsequent calls to AddTab() or Finalize() will be ignored.
    void Finalize();
    //! Open or Reopen the panel
    void Open();
    static void UserDraw( LWControlID control, void *userdata, DrMode drawmode );
    void controlEvent(PanelControl control);
    void panelDraw(const LWPanel&, DrMode );
  };
}

#endif // LWPP_HELP_PANEL_H