#ifndef LWPP_PANEL_TOOLS_H
#define LWPP_PANEL_TOOLS_H

#include <lwpp/panel.h>

namespace lwpp
{
  //! Used to extract information from an input qualifier
  class Qualifier
  {
    int mQualifier;
  public:
    Qualifier(const int qual = 0)
      : mQualifier(qual)
    {}
    void set(const int qual) { mQualifier = qual; }
    bool isRightClick() const { return RCLICK(mQualifier); }
    bool isLeftClick() const { return LCLICK(mQualifier); }
    bool isMiddleClick() const { return MCLICK(mQualifier); }
    bool isMouseDown() const { return (mQualifier & MOUSE_DOWN) == MOUSE_DOWN; }
    bool isCtrl() const { return (mQualifier & IQ_CTRL) == IQ_CTRL; }
    bool isShift() const { return (mQualifier & IQ_SHIFT) == IQ_SHIFT; }
    bool isAlt() const { return (mQualifier & IQ_ALT) == IQ_ALT; }
    bool isConstrain() const { return (mQualifier & IQ_CONSTRAIN) == IQ_CONSTRAIN; }
    bool isAdjust() const { return (mQualifier & IQ_ADJUST) == IQ_ADJUST; }
    int getQualifier() const { return mQualifier; }
  };

  //! Handle various states of mouse interaction
  class MouseState
  {
  private:
    int mCurrentX = 0, mCurrentY = 0; // current mouse position
    int mClickedX = 0, mClickedY = 0; // position of the last click
    lwpp::Qualifier mCurrentQualifier, mClickedQualifier;
    bool mDragging = false; // we're currently dragging     
  public:
    MouseState() { ; }

    void getDragDelta(int *x, int *y) const
    {
      if ( mDragging )
      {
        *x = mClickedX - mCurrentX;
        *y = mClickedY - mCurrentY;
      }
    }
    void addEvent(const int code, const int x, const int y)
    {
      mCurrentX = x;
      mCurrentY = y;
      mCurrentQualifier.set(code);
      if ( current().isMouseDown() != clicked().isMouseDown() )
      {
        // mouse buttons have been pressed or released
        restartDrag(code);
      }
    }
    //! restart a drag operation - this happens if the mouse button comibnation has changed, but there are still buttons pressed
    void restartDrag(const int code)
    {
      mClickedY = mCurrentY;
      mClickedX = mCurrentX;
      mClickedQualifier.set(code);
      mDragging = current().isMouseDown();
    }
    const lwpp::Qualifier &current() { return mCurrentQualifier; }
    const lwpp::Qualifier &clicked() { return mClickedQualifier; }
    bool isDragging() const { return mDragging; }
    bool clickedInside(const int x0, const int y0, const int x1, const int y1) const
    {
      return (lwpp::inRange(mClickedX, x0, x1) && lwpp::inRange(mClickedY, y0, y1));
    }
    bool clickedInside(PanelControl &cont) const
    {
      int cx = cont.HotX();
      int cy = cont.HotY();
      return (clickedInside(cx, cy, cx + cont.HotW(), cy + cont.HotH()));
    }
    bool isInside(const int x0, const int y0, const int x1, const int y1) const
    {
      return ( lwpp::inRange(mCurrentX, x0, x1) && lwpp::inRange(mCurrentY, y0, y1)) ;
    }
    bool isInside(PanelControl &cont) const
    {
      int cx = cont.HotX();
      int cy = cont.HotY();
      return (isInside(cx, cy, cx + cont.HotW(), cy + cont.HotH()));
    }
    void getPosition(int *x, int *y) const
    {
      *x = mCurrentX;
      *y = mCurrentY;
    }
    int getX() const { return mCurrentX; }
    int getY() const { return mCurrentY; }
    void dpiScale(const float scl)
    {
      mCurrentX = static_cast<int>(static_cast<float>(mCurrentX) * scl);
      mCurrentY = static_cast<int>(static_cast<float>(mCurrentY) * scl);
      mClickedX = static_cast<int>(static_cast<float>(mClickedX) * scl);
      mClickedY = static_cast<int>(static_cast<float>(mClickedY) * scl);
    }
  };  

  /*! Wraps a control and a string stream to display status messaged */
  class StatusControl 
  {
    PanelControl mControl;    
    std::stringstream mStream;
  public:
    struct _EOL {};
    static _EOL eol;
    StatusControl() {}
    virtual ~StatusControl() {}

    void SetControl(lwpp::PanelControl control)
    {
      mControl = control;
    }
    StatusControl& operator<< (_EOL const _eol)
    {
      if ( mControl.getID() )
      {
        mControl.Set(mStream.str());
        mControl.Draw();
      }
      mStream.str("");    // Clear the string buffer
      return *this;
    }
    template <typename T>
    StatusControl& operator<< (T data)
    {
      mStream << data;
      return *this;
    }

  };
#define EOL StatusControl::eol;
}

#endif // LWPP_PANEL_TOOLS_H
