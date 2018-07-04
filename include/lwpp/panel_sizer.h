#ifndef LWPP_PANEL_SIZER_H
#define LWPP_PANEL_SIZER_H

#include <lwpp/panel.h>
#include <vector>

/*
 This header implements a simple layout manager for LWPanels GUIs.
*/

namespace lwpp
{
	/* @ingroup PanelSizer
	*@{
	*/

	enum SizerFlags
	{
		STRETCH_HORIZONTAL = 1 << 0, //!< Allows the control to use all horizontal space and also allows the parent sizer to stretch
		STRETCH_VERTICAL   = 1 << 1,
		ALIGN_LEFT         = 1 << 2, //!< Align left within the assigned space
		ALIGN_RIGHT        = 1 << 3,
		ALIGN_CENTRE       = 1 << 4,
		ALIGN_LABEL        = 1 << 5,
		USE_HOT_W          = 1 << 6, //!< Use the "hot" size
		USE_HOT_H          = 1 << 7,
		FIT_WIDTH          = 1 << 8, //!< Allows the control to use all horizontal space but does not affect the parent sizer
		IS_COLLAPSED       = 1 << 9
	};

#define RIGHT_ALIGN (lwpp::STRETCH_HORIZONTAL | lwpp::ALIGN_RIGHT)
#define LEFT_ALIGN (lwpp::STRETCH_HORIZONTAL | lwpp::ALIGN_LEFT)
#define USE_HOT (lwpp::USE_HOT_W | lwpp::USE_HOT_H)
#define STRETCH_MASK (lwpp::STRETCH_HORIZONTAL | lwpp::STRETCH_VERTICAL)

  class Margin
  {
    int mLeft, mRight, mTop, mBottom;
  public:
    Margin(int val = 0)
      : mLeft(val), mRight(val), mTop(val), mBottom(val)
    {}
    Margin (int left, int right, int top, int bottom)
      : mLeft(left), mRight(right), mTop(top), mBottom(bottom)
    {}
    Margin &operator=(const Margin &from)
    {
      if ( &from != this )
      {
        mLeft = from.mLeft;
        mRight = from.mRight;
        mTop = from.mTop;
        mBottom = from.mBottom;
      }
      return *this;
    }
  };


	//! Virtual base class for all Sizers
	class Sizer
	{
	protected:
		int mFlags;
		int mWeight;
		int padWidth = 0, padHeight = 0;
		int top, left;
    Margin mMargin;
		public:
			Sizer(int flags, int weight = 0)
				: mFlags(flags), mWeight(weight)
			{;}
			virtual ~Sizer() { ; }
			
			//! Add children
			virtual Sizer *Add(Sizer *) = 0;
      //! Add a margin to the last child
      virtual Sizer *operator<<(Margin margin) { return nullptr; }
			//! Set the padding width and height
			void setPadding(int w, int h = 0)
			{
				padWidth = w;
				padHeight = h;
			}
      void setMargin(Margin &margin) { mMargin = margin; }
			//! Return the minimum size of the Sizer
			virtual void getMinSize(int &w, int &h) = 0;
			//! Return the maximum size of the Sizer
			virtual void getMaxSize(int &w, int &h) = 0;
			//! Return the current size of the Sizer
			virtual void getCurrentSize(int &w, int &h) = 0;
			//! Get the largest label width within the Sizer, used for alignment
			virtual int getLabelWidth() = 0;
			//! Layout the sizer to fit into the bounds
			virtual void Layout(int x, int y, int w, int h, int lw) = 0;
			//
			int getWeight() const { return (isCollapsed() ? 0 : mWeight); }
			void setWeight(int w) { mWeight = w; }

			virtual int getFlags() const { return mFlags; }
			virtual void setFlags(int flags) { mFlags = flags; }
			virtual void panelDraw(const lwpp::LWPanel&, DrMode) { ; }
			virtual void collapse()
			{
				mFlags |= IS_COLLAPSED;
			}
      virtual void expand()
      {
        mFlags &= ~IS_COLLAPSED;
      }
			virtual bool isCollapsed() const
			{
				return (mFlags & IS_COLLAPSED) !=0;
			}
	};

	class StaticSpace : public Sizer
	{
		int mWidth, mHeight;
	public:
		StaticSpace (int w = 20, int h = 20)
			: Sizer(0, 1), mWidth(w), mHeight(h)
		{}
		virtual Sizer *Add(Sizer *) { return nullptr; }
		void getMinSize(int &w, int &h)
		{
      if ( isCollapsed() )
      {
        w = h = 0;
      }
      else
      {
        w = mWidth;
        h = mHeight;
      }
		}
		//! Return the maximum size of the Sizer
		void getMaxSize(int &w, int &h) { getMinSize(w, h); }
		//! Return the current size of the Sizer
		void getCurrentSize(int &w, int &h) { getMinSize(w, h); }
		//! Get the biggest label width of the Sizer
		int getLabelWidth() { return 0; }
		void Layout(int x, int y, int w, int h, int lw) { ; }
	};

	//! a sizer containing a single PanelControl
	/* A ControlSizer contains a single LWPanel control
	*/
	class ControlSizer : public Sizer
	{
		PanelControl mControl;
		int minWidth, minHeight;
	public:
		ControlSizer(PanelControl control, int flags = 0, int weight = 0)
			: mControl(control), Sizer(flags, weight)
		{
			getCurrentSize(minWidth, minHeight);
		}
		virtual ~ControlSizer() { ; }
		virtual Sizer *Add(Sizer *sizer, SizerFlags) { return sizer; }
		virtual void getMinSize(int &w, int &h)
		{
      if ( isCollapsed() )
      {
        w = h = 0;
      }
      else
      {
			w = minWidth;
			h = minHeight;
      }
		}
		virtual void getMaxSize(int &w, int &h)
		{
      if ( isCollapsed() )
      {
        w = h = 0;
      }
      else
      {
        w = minWidth * 2;
        h = minHeight * 2;
      }
		}
		virtual void getCurrentSize(int &w, int &h)
		{
      if ( isCollapsed() )
      {
        w = h = 0;
      }
      else
      {
        if ( mControl.getID() )
        {
          int hw = mControl.HotW();
          int hh = mControl.HotH();
          int cw = mControl.W();
          int ch = mControl.H();
          w = mFlags & USE_HOT_W ? hw : cw;
          h = mFlags & USE_HOT_H ? hh : ch;
        }
        else h = w = 0;
        w += padWidth;
        h += padHeight;
      }
		}
		virtual int getLabelWidth()
		{
      return (isCollapsed() ? 0 : mControl.LW());
		}
		virtual void Layout(int x, int y, int w, int h, int lw)
		{
			//control.Move(x + lw - getLabelWidth() ,y);
			int cW = minWidth;
			int cH = minHeight;

			if((mFlags & STRETCH_HORIZONTAL) | (mFlags & FIT_WIDTH))
			{
				if(mFlags & ALIGN_RIGHT)
				{
					x += w - minWidth;
					cW = minWidth;
				}
				else if(mFlags & ALIGN_CENTRE)
				{
					x += (w - minWidth) / 2;
					cW = minWidth;
				}
				else if(mFlags & ALIGN_LEFT)
				{
					;
				}
				else // fill
				{
					cW = w;
				}
			}
			if(mFlags & STRETCH_VERTICAL) cH = h;
      mControl.Move(x, y);
			top = y; left = x;
      mControl.setSize(cW - padWidth, cH - padHeight);
		}
		virtual Sizer *Add(Sizer *s) { return s; }
	};

	class DynamicSizer : public Sizer
	{
	protected:
		PanelControl borderControl;
		std::vector<Sizer *> childList;
		typedef std::vector<Sizer *>::iterator childIter;
		int spacingX, spacingY;
		DynamicSizer(int flags) : Sizer(flags), spacingX(6), spacingY(6)
		{
			;
		}
		virtual ~DynamicSizer()
		{
			for (auto &i : childList)
			{
				//delete i;
			}
		}
	public:
		virtual Sizer *Add(Sizer *sizer)
		{
			if(sizer)
			{
				mFlags |= sizer->getFlags() & STRETCH_MASK;
			}
			childList.push_back(sizer);
			return sizer;
		}
    virtual Sizer *operator<<(Margin margin)
    {
      Sizer *sizer = childList.back();
      if ( sizer )
      {
        sizer->setMargin(margin);
      }
      return sizer;
    }
		virtual Sizer *Add(PanelControl &cnt, int flags)
		{
			Sizer *sizer = new ControlSizer(cnt, flags);
			return Add(sizer);
		}
		void setBorder(PanelControl &border)
		{
			borderControl = border;
		}
		virtual int getLabelWidth()
		{
			int w = 0;
			for(auto &i : childList)
			{
				w = lwpp::Max(w, i->getLabelWidth());
			}
			return w;
		}
		virtual void panelDraw(const lwpp::LWPanel& pan, DrMode mode)
		{
			for(auto &i : childList)
			{
				i->panelDraw(pan, mode);
			}
		}
    virtual void collapse()
    {
      Sizer::collapse();
      for ( auto &i : childList )
      {
        i->collapse();
      }
    }
    virtual void expand()
    {
      Sizer::expand();
      for ( auto &i : childList )
      {
        i->expand();
      }
    }
	};

	//! A sizer with components lined up horizontally
	class HorizontalSizer : public DynamicSizer
	{	
		public:
			HorizontalSizer(int flags = STRETCH_HORIZONTAL) : DynamicSizer(flags)
			{
				;
			}
			virtual ~HorizontalSizer() { ; }
			virtual void getMaxSize(int &w, int &h)
			{
        if ( isCollapsed() )
        {
          w = h = 0;
          return;
        }
				for(auto &i : childList)
				{
					int tw, th;
					i->getMaxSize(tw, th);
					w += tw;
					h = lwpp::Max(h, th);
				}
				w += spacingX * static_cast<int>(childList.size() - 1);
			}
			virtual void getMinSize(int &w, int &h)
			{
				w = h = 0;
        if ( isCollapsed() )
          return;
				for (auto &i : childList)
				{
					int tw, th;
					i->getMinSize(tw, th);
					w += tw;
					h = lwpp::Max(h, th);
				}
				w += spacingX * static_cast<int>(childList.size() - 1);
			}
			virtual void getCurrentSize(int &w, int &h)
			{
        if ( isCollapsed() )
        {
          w = h = 0;
          return;
        }
				for(auto &i : childList)
				{
					int tw, th;
					i->getCurrentSize(tw, th);
					w += tw;
					h = lwpp::Max(h, th);
				}
				w += spacingX * static_cast<int>(childList.size() - 1);
			}
			/*!
			Layout the child controls to fit into the specified box
			@param x left start
			@param y top start
			@param w width
			@param h height
			@param lw label width
			*/
			virtual void Layout(int x, int y, int w, int h, int lw)
			{
				int minWidth = 0;
				int minHeight = 0;
				getMinSize(minWidth, minHeight);
				int remWidth = w - minWidth;
				int remHeight = h - minHeight;

				int totalWeight = 0; // the number of childs that can be stretched
				for(auto &i : childList)
				{
					if(i->getFlags() & STRETCH_HORIZONTAL) totalWeight += (i->getWeight() + 1);
				}

				//remHeight
				int weightWidth = (totalWeight) ? remWidth / totalWeight : 0;
				remWidth -= weightWidth * totalWeight;
				/* Layout */
				top = y;
				left = x;
				for(auto &i : childList)
				{
					int cWidth, cHeight;
					i->getMinSize(cWidth, cHeight);
					if(i->getFlags() & STRETCH_HORIZONTAL)
					{
						cWidth += weightWidth * (i->getWeight() + 1);
						if(remWidth)
						{
							++cWidth;
							--remWidth;
						}
					}
					if(i->getFlags() & STRETCH_VERTICAL) cHeight = h;
					i->Layout(left, top, cWidth, cHeight, lw);
					left += cWidth + spacingX;
				}
				top = y;
				left = x;
				if(borderControl.getID())
				{
					borderControl.setPosition(x, y);
					borderControl.setSize(w, h);
				}
			}
	};

	//! A sizer with components lined up vertically
	class VerticalSizer : public DynamicSizer
	{
		public:
			VerticalSizer(int flags = STRETCH_VERTICAL) : DynamicSizer(flags)
			{
				;
			}
			virtual ~VerticalSizer() { ; }
			virtual void getMinSize(int &w, int &h)
			{
				w = h = 0;
        if ( isCollapsed() )
          return;
				for(auto &i : childList)
				{
					int tw, th;
					i->getMinSize(tw, th);
					h += th;
					w = lwpp::Max(w, tw);
				}
				h += spacingY * static_cast<int>(childList.size() - 1);
			}
			virtual void getMaxSize(int &w, int &h)
			{
				w = h = 0;
        if ( isCollapsed() )
          return;
				for(auto &i : childList)
				{
					int tw, th;
					i->getMaxSize(tw, th);
					h += th;
					w = lwpp::Max(w, tw);
				}
				h += spacingY * static_cast<int>(childList.size() - 1);
			}
			virtual void getCurrentSize(int &w, int &h)
			{
				w = h = 0;
        if ( isCollapsed() )
          return;
				for(auto &i : childList)
				{
					int tw, th;
					i->getCurrentSize(tw, th);
					h += th;
					w = lwpp::Max(w, tw);
				}
				h += spacingY * static_cast<int>(childList.size() - 1);
			}
			/*!
				Layout the child controls to fit into the specified box
				@param x left start
				@param y top start
				@param w width
				@param h height
				@param lw label width
			*/
			virtual void Layout(int x, int y, int w, int h, int lw)
			{
				int minWidth = 0;
				int minHeight = 0;
				getMinSize(minWidth, minHeight);
				int remWidth = w - minWidth;
				int remHeight = h - minHeight;
				
				int totalWeight = 0; // the number of childs that can be stretched
				for(auto &i : childList)
				{
					if(i->getFlags() & STRETCH_VERTICAL) totalWeight += (i->getWeight() + 1);
				}

				//remHeight
				int weightHeight = totalWeight ? remHeight / totalWeight : 0;
				remHeight -= weightHeight * totalWeight;
				/* Layout */
				top = y;
				left = x;
				for(auto &i : childList)
				{
					int cWidth, cHeight;
					i->getMinSize(cWidth, cHeight);
					if ( i->getFlags() & STRETCH_VERTICAL)
					{
						cHeight += weightHeight * (i->getWeight() + 1);
						if(remHeight)
						{
							++cHeight;
							--remHeight;
						}
					}
					i->Layout(left, top, w, cHeight, lw);
					top += cHeight + spacingY;
				}
				top = y;
				left = x;
				if(borderControl.getID())
				{
					borderControl.setPosition(x, y);
					borderControl.setSize(w, h);
				}
			}
	};

	//! a sizer containing a single PanelControl which measures the height based on the number of lines
	/* A LineControlSizer contains a single LWPanel control
	*/
	class LineControlSizer : public Sizer
	{
		PanelControl control;
		int minWidth, minHeight;
		int mLineHeight, mLineSpacing, mOverHead;
		int PixelsToLines(int pix)
		{
			pix = pix - mOverHead;
			pix /= (mLineHeight + mLineSpacing);
			pix = (pix < 0 ? 1 : pix);
			return pix;
		}
		int LinesToPixels(int lines)
		{
			lines *= (mLineHeight + mLineSpacing) + mOverHead;
			return lines;
		}
	public:
		LineControlSizer(PanelControl _control, int flags = 0, int weight = 0)
			: control(_control),
			Sizer(flags, weight),
			mLineHeight(8),
			mLineSpacing(2),
			mOverHead(10)
		{
			getCurrentSize(minWidth, minHeight);
		}
		void setParameters(int height, int spacing, int oh)
		{
			mLineHeight = height;
			mLineSpacing = spacing;
			mOverHead = oh;
		}
		virtual ~LineControlSizer() {;}
		virtual Sizer *Add(Sizer *sizer, SizerFlags) { return sizer; }
		virtual void getMinSize(int &w, int &h)
		{
			w = minWidth;
			h = minHeight;
#ifdef _DEBUG
			lwpp::dostream dout;
			dout << "        Line Control Min Size: (" << w << "," << h << ")\n";
#endif
		}
		virtual void getMaxSize(int &w, int &h)
		{
			w = minWidth * 2;
			h = minHeight * 2;
		}
		virtual void getCurrentSize(int &w, int &h)
		{
			w = control.W();
			h = control.H();
			h = LinesToPixels(h);
		}
		virtual int getLabelWidth()
		{
			return control.LW();
		}
		virtual void Layout(int x, int y, int w, int h, int lw)
		{
			//control.Move(x + lw - getLabelWidth() ,y);
			control.Move(x, y);

			if(!(mFlags & STRETCH_HORIZONTAL)) w = minWidth;
			if(!(mFlags & STRETCH_VERTICAL)) h = minHeight;

			h = PixelsToLines(h);

			control.setSize(w, h);
#ifdef _DEBUG
			lwpp::dostream dout;
			dout << "  Panel: " << control.getID() << " @ " << x << "," << y << " (" << w << "," << h << ")\n";
#endif
		}
		virtual Sizer *Add(Sizer *s) { return s; }
	};

	/* @ingroup PanelSizer */
	class ManagedPanel
	{
		Sizer *root; // root sizer for the panel
		int minWidth, minHeight;
		int labelWidth;
		void init()
		{
			if (root)
			{
				root->getMinSize(minWidth, minHeight);
				root->getLabelWidth();
			}
		}
	public:
		ManagedPanel() : root(0), minWidth(0), minHeight(0), labelWidth(0)
		{
			;
		}
		~ManagedPanel()
		{
			if (root) delete root;
		}
		void setRoot(Sizer *_root)
		{
			root = _root;
			init();
		}
		void Layout (LWPanel &panel)
		{
			int w = panel.GetWidth() + 8;
			int h = panel.GetHeight() + 8;
			int x = 4;
			int y = 4;
			if (root)
			{
				root->Layout(x, y, w, h, labelWidth);
			}
			panel.Draw(DR_REFRESH);
		}
	};
	/*
	@}
	*/
}
#endif // LWPP_PANEL_SIZER_H