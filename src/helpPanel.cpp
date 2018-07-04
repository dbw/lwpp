#include "lwpp/helpPanel.h"

namespace lwpp
{
  HelpPanel::HelpTab::HelpTab (LWPanel &_panel, const char *_title, const char* _leftCol[], const char* _rightCol[])
    : title(_title),
    leftCol(_leftCol),
    rightCol(_rightCol),
    panel(_panel)
  {
    ;
  }

  HelpPanel::HelpTab::HelpTab (const HelpPanel::HelpTab &from)
    : title(from.title),
    leftCol(from.leftCol),
    rightCol(from.rightCol),
    panel(from.panel),
    leftText(from.leftText),
    rightText(from.rightText)
  {
    ;
  }

  HelpPanel::HelpTab &HelpPanel::HelpTab::operator= (const HelpPanel::HelpTab &from)
  {
    if (&from != this)
    {
      title = from.title;
      leftCol = from.leftCol;
      rightCol = from.rightCol;
      panel = from.panel;
      leftText = from.leftText;
      rightText = from.rightText;
    }
    return *this;
  }

  int HelpPanel::HelpTab::Position(TabManager &manager, PanelControl &tab)
  {
    leftText = panel.AddText("", leftCol);    
    leftText.MoveBelowTab(tab);
    int bottom = leftText.H();
    manager << leftText;
    if (rightCol)
    {
      rightText = panel.AddText("", rightCol);
      rightText.MoveRightOf(leftText);
      manager << rightText;
      bottom = lwpp::Max(bottom, rightText.H());
    }
    return bottom + leftText.Y();
  }

  // About 
  void HelpPanel::draw(LWControlID, DrMode)
  {
    // get panel width
    int xs = canvas.HotX();
    int cw = canvas.HotW();
    int ch = canvas.HotH();
    int ys = canvas.HotY();
    LWDrawFuncs df = getDrawFuncs();
    df.drawBox(255,255,255, xs, ys, cw, ch); // clear background
    xs += (cw - width) / 2;
    ys += (ch - height) / 2;
    for(int y = 0; y < height; y++)
    {
      int index = (height - y - 1) * width * 3;
      for(int x = 0; x < width; x++)
      {
        df.drawPixel(image[index+2], image[index+1],	image[index],	xs + x, ys + y);
        index += 3;
      }
    }
  }

  void HelpPanel::UserDraw( LWControlID control, void *userdata, DrMode drawmode )
  {
    if (userdata)
    {
      HelpPanel *mypanel = static_cast<HelpPanel *>(userdata);
      mypanel->draw(control, drawmode);
    }
  }	

  /*!
   * @param[in] title Title of the panel
   * @param[in] message An optional message to display below the image
   * @param[in] _image an (optional) image to display above the tabs
   * @param[in] w,h width and height of the top image
   */
  HelpPanel::HelpPanel(const std::string title, const char**message, const unsigned char *_image, const int w, const int h)
    : LWPanel(title),
    image(_image),
    width(w),
    height(h),
    tabLabels(0),
    lastPos(4),
    finalised(false)
  {
    if (!LWPanel::available()) return;
    if (image)
    {
      canvas = AddCanvas("", width, height);
      lastPos = canvas.Y() + canvas.H();
      if (message)
      {
        text = AddText("", message);
        lastPos = text.Y() + text.H();
      }
      int cw = GetWidth();
      canvas.setWidth(cw);
      canvas.MoveX(((GetWidth() - cw) / 2) + 6);
      canvas.SetUserData(this);
      canvas.SetUserDraw(&HelpPanel::UserDraw);
      SetUserData(this);
    }
    else
    {
      if (message)
      {
        text = AddText("", message);
        lastPos = text.Y() + text.H();
      }
    }
  }

  HelpPanel::~HelpPanel()
  {
    /*
    if (tabLabels)
    {
      for (int i = 0; i < tabList.size(); ++i)
      {
        delete[] tabLabels[i];
      }
    }
    */
    delete[] tabLabels;    
  }

  /*!
   * @param[in] title Label of the tab
   * @param[in] _leftCol Left column of text. This will be a single column if the right column is not specified
   * @param[in] _rightCol Optional right column of text to display
   */
  void HelpPanel::AddTab(const char *title, const char* _leftCol[], const char* _rightCol[])
  {
    if (finalised) return;
    if (!LWPanel::available()) return;
    HelpTab hTab(*this, title, _leftCol, _rightCol);
    tabList.push_back(hTab);
  }

  void  HelpPanel::controlEvent(lwpp::PanelControl control)
  {
    if (control == tabs)
    {
      mainTab.draw();
    }
  }

  void HelpPanel::panelDraw(const LWPanel&, DrMode mode)
  {
    LWDrawFuncs df = getDrawFuncs();
    int y = tabs.Y() + tabs.HotH() + 1;
    int w = LWPanel::GetWidth() - 8;
    int h = LWPanel::GetHeight() - y - 6;
    DrawBorder(0, 4, y, w, h);
  }

  void HelpPanel::Finalize()
  {
    if (finalised) return;
    if (!LWPanel::available()) 
    {
      finalised = true;
      return;
    }
    tabLabels = new char *[tabList.size()+1];
    for (int i = 0; i < tabList.size(); ++i)
    {
      tabLabels[i] = tabList[i].GetTitle();
    }
    tabLabels[tabList.size()] = 0;
    tabs = AddTabs("",const_cast<const char **>(tabLabels));
    mainTab.init(tabs);
    tabs.MoveX(8);
    tabs.MoveY(lastPos);
    tabs.SetUserData(this);
    int bottom = 0;
    for (int i = 0; i < tabList.size(); ++i)
    {
      mainTab << i;
      bottom = lwpp::Max(tabList[i].Position(mainTab, tabs), bottom);
    }
    LWPanel::SetHeight(bottom + 4);
    mainTab.draw();
    SetCallbacks(this);
    tabs.SetEventCallback<HelpPanel, GetPanelEventHandler>();
    finalised = true;
  }

  void HelpPanel::Open()
  {
    if (!finalised) return;
    LWPanel::Open(PANF_NOBUTT);
  }
}
