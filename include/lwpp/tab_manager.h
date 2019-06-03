#ifndef LWPP_TAB_MANAGER_H
#define LWPP_TAB_MANAGER_H

#include <lwpp/panel.h>
#include <stdarg.h>
#include <vector>
#include <set>

namespace lwpp
{
  //! Manages the drawing of a single tab control and all children in a LWPanel
  /*!
   * Manages the visibility of multiple controls depending on which tab has been selected by the user.
   * 
   * The overloaded operator<< allows for adding tabs like this:
   *
   *     mainTab << 3 << settings.previewMode << settings.syncTL << settings.solidBG;
   *
   * Controls are not owned by the TabManager
   */
  class TabManager
  {
    typedef std::set<PanelControl *> TabSet;
    typedef TabSet::iterator SetIter;
    typedef std::vector<TabSet> TabsCollection;

    PanelControl mTab;
    TabsCollection mControls;
    int mCurrentTab;
   public:
    TabManager(){}
    // Initialize the TabManager
    /*!
     * @param[in] tab lwpp::PanelControl that represents the panel
     *
     */
    void init(PanelControl &tab)
    {
      mCurrentTab = 0;
      mTab = tab;
      mControls.clear();
    }
    //! Adds a control to a specific tab
    void add(const int tab, PanelControl &control)
    {
      //if (tab > (mControls.size()-1)) mControls.resize(tab+1);
      mControls.resize(tab+1);
      mControls.at(tab).insert(&control);
    }
    //! Convenience function to change the tab that new controls are added to
    /*!
     * @param[in] tab the tab to add subsequent controls to
     * @return a reference to the tab manager to allow for daisy chaining
     */
    TabManager &operator<<(const int tab)
    {
      mCurrentTab = tab;
      return *this;
    }
    //! Adds a control to the current tab
    /*!
     * @param[in] control The control to add to the current tab. It is owned by the caller, only a pointer is stored
     * @return a reference to the tab manager to allow for daisy chaining
     */
    TabManager &operator<<(PanelControl &control)
    {
      add (mCurrentTab, control);
      return *this;
    }
    //! Ghosts a control managed by the TabManager
    /*!
     * @param[in] control The control to ghost
     * @param[in] doGhost true if the control is to be ghosted, false if not
     */
    void Ghost(PanelControl &control, const bool doGhost = true)
    {
      if (!control.isValid()) return;  

      //int curr = mTab.GetInt();
      for (int i = 0; i < mControls.size(); ++i)
      {
        for (SetIter iter = mControls.at(i).begin(); iter != mControls.at(i).end(); ++iter)
        {
          PanelControl *ctr = *iter;
          if (ctr->getID() == control.getID())
          {
            if (doGhost)
            {
              ctr->Ghost();
 #ifdef _DEBUG
              dout << "Ghost " << ctr->getID() << "\n";
#endif
            }
            else
            {
              ctr->UnGhost();
 #ifdef _DEBUG
              dout << "Unghost " << ctr->getID() << "\n";
#endif
            }

            return;
          }
        }
      }
    };

    //! Convenience function to unghost a specific control
    /*!
     * @param[in] control The control to unghost
     */
    void UnGhost(PanelControl &control)
    {      
      Ghost(control, false);
    }

    //! Draw the controls depending on the status of the tabs
    void draw()
    {
      int curr = mTab.GetInt();
#ifdef _DEBUG
      dout << "Visible tab " << curr << "\n";
#endif      
      for (int i = 0; i < mControls.size(); ++i)
      {
#ifdef _DEBUG
        dout << "Processing tab " << i << "\n";
#endif  
        if (i != curr)
        {
          for (SetIter iter = mControls.at(i).begin(); iter != mControls.at(i).end(); ++iter)
          {
            PanelControl *ctr = *iter;
            //(i == curr) ? ctr.Render() : ctr.Erase();
            ctr->Erase();
#ifdef _DEBUG
            dout << "Erase control " << ctr->getID() << "\n";
#endif 
          }
        }
      }
      for (SetIter iter = mControls.at(curr).begin(); iter != mControls.at(curr).end(); ++iter)
      {
        PanelControl *ctr = *iter;
        ctr->Display();
#ifdef _DEBUG
        dout << "Rendering control " << ctr->getID() << (ctr->isGhosted() ? " (ghost)\n" : "\n") ;
#endif 
      }
    }

  };

  //! position controls in a grid
  class FixedGridLayout
  {
    int mTotalWidth;
    int mBorderWidth;
    int mInterSpacing;
    int mNumCols;
    int mTop, mLeft; 
    int mControlWidth;
    int mCurrentCol, mCurrentRow;
    bool mAdjustWidth, mAlignRight;
    void ComputeControlWidth()
    {
      mControlWidth = (mTotalWidth - 2 * mBorderWidth);
      if (mNumCols > 1)
      {
       mControlWidth -= (mNumCols - 1) * mInterSpacing;
       mControlWidth /= mNumCols;       
      }
    }
    void incRowCol()
    {      
      ++mCurrentCol;
      if (mCurrentCol >= mNumCols)
      {
        mCurrentCol = 0;
        ++mCurrentRow;
      }
    }
    void positionRow(PanelControl &control, const int row) const
    {
      int y = mTop + control.H() * row;
      control.MoveY(y);
    }
  public:
    //! Initialise the Layout engine
    /*!
     * @param[in] x Left edge of the grid. The border size will be added to it
     * @param[in] y Top edge of the grid. This is the top of the first control
     * @param[in] w Width of the grid, including borders
     * @param[in] numCols The number of coluns in the grid
     * @param[in] border The border size in pixels to the left and the right of the side of the grid
     * @param[in] spacing Horizontal spacing between the controls in pixels
     */
    FixedGridLayout (const int x, const int y, const int w, const int numCols, const int border = 6, const int spacing = 4)
      : mTop(y), mLeft(x), mNumCols(numCols), mInterSpacing(spacing), mBorderWidth(border), mTotalWidth(w),
      mCurrentRow(0), mCurrentCol(0), mAdjustWidth(true), mAlignRight(true)
    {
      ComputeControlWidth();
    }
    //! Get the computed maximum width for a control
    int GetControlWidth() const {return mControlWidth;}
    //! Set a control to the computed maximum width for a control
    void SetControlWidth(PanelControl &control) const
    {
      control.setWidth(mControlWidth);
    }
    //! Manually position a control on the grid
    void Position(PanelControl &control, const int col, const bool doWidth = true) const
    {
      if (doWidth) SetControlWidth(control);
      int x = mLeft + mBorderWidth + (mInterSpacing + mControlWidth) * col;
      if (mAlignRight) x += mControlWidth - control.W();

      control.MoveX(x);
    }
    //! Manually position a control on the grid
    /*!
     * @param[in] control The control to position
     * @param[in] col The column to position the control into
     * @param[in] row The row to position the control into
     * @param[in] doWidth Adjust the width of the control to the computed width or leave as is
     */
    void Position(PanelControl &control, const int col, const int row, const bool doWidth = true ) const
    {      
      Position(control, col, doWidth);
      positionRow(control, row);
    }
    //! Directly set the row and column for the next control that is added
    void SetRowCol(const int r = 0, const int c = 0)
    {
      mCurrentRow = r;
      mCurrentCol = c;
    }
    //! Add and position a control on the grid
    /*!
     * The column and/or row will be incremented after the control is added.
     *
     * Formatting depends on the Formatter enums that can be sent to operator<<(const Formatter sw)
     * @param[in] control The control to position
     */
    FixedGridLayout &operator<<(PanelControl &control)
    {
      Position(control, mCurrentCol, mCurrentRow, mAdjustWidth);
      incRowCol();
      return *this;
    }
    //! Control the layout and formatting of controls using operator<<(const Formatter sw)
    enum Formatter {
      Blank, //!< inserts a "blank" control, incrementing the column and row if required
      NewRow, //!< Switch to colum 0 on the next row, essentially a "carriage return"
      AdjustWidth, //!< Adjust the width of al following controls to fit (default)
      KeepWidth, //!< Keep the width of all following controls
      AlignLeft, //!< Align the following controls on their left edge
      AlignRight //!< Align the following controls on their right edge (default)
    };

    //! Change the formatting of the controls
    /*
     * @param[in] sw On of the Formatter enums to control the positioning within the grid
     */
    FixedGridLayout &operator<<(const Formatter sw)
    {
      switch (sw)
      {
      case Blank:
        incRowCol();
        break;

      case NewRow:
        if (mCurrentCol)
        {
          mCurrentCol = 0;
          ++mCurrentRow;
        }
        break;

      case AdjustWidth:
        mAdjustWidth = true;
        break;

      case KeepWidth:
        mAdjustWidth = false;
        break;

      case AlignLeft:
        mAlignRight = false;
        break;

      case AlignRight:
        mAlignRight = true;
        break;

      default:
        break;
      }
      return *this;
    }
  };
}

#endif // LWPP_TAB_MANAGER_H