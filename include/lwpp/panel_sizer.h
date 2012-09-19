#ifndef LWPP_PANEL_SIZER_H
#define LWPP_PANEL_SIZER_H

#include <lwpp/panel.h>
#include <vector>

namespace lwpp
{

	enum SizerFlags
	{
		STRETCH_HORIZONTAL = 1 << 0,
		STRETCH_VERTICAL = 1 << 1,
	};

	//! Virtual base class for all Sizers
	class Sizer
	{
		public:
			Sizer() {;}
			
			//! Add a sizer
			virtual Sizer *Add(Sizer *) = 0;
			//! Return the minimum size of the Sizer
			virtual void getMinSize(int &w, int &h) = 0;
			//! Return the maximum size of the Sizer
			virtual void getMaxSize(int &w, int &h) = 0;
			//! Return the current size of the Sizer
			virtual void getCurrentSize(int &w, int &h) = 0;
			//! Get the biggest label width of the Size
			virtual int getLabelWidth() = 0;
			//! Layout the sizer to fit into the bounds
			virtual void Layout(int x, int y, int w, int h, int lw) = 0;

			virtual int getFlags() = 0;
	};

	class DynamicSizer : public Sizer
	{
	protected:
		std::vector<Sizer *> childList;
		typedef std::vector<Sizer *>::iterator childIter;
		int Flags;
		DynamicSizer(int flags) : Flags(flags)
		{
			;
		}
		~DynamicSizer()
		{
			for (childIter i = childList.begin(); i != childList.end(); ++i)
			{
				delete *i;
			}
		}
	public:
		virtual Sizer *Add(Sizer *sizer)
		{
			childList.push_back(sizer);
			return sizer;
		}
		virtual int getLabelWidth()
		{
			int w = 0;
			for (childIter i = childList.begin(); i != childList.end(); ++i)
			{
				w = lwpp::Max(w, (*i)->getLabelWidth());
			}
			return w;
		}
		virtual int getFlags() {return Flags;}
	};

	//! A sizer with components lined up horizontally
	class HorizontalSizer : public DynamicSizer
	{	
		public:
			HorizontalSizer(int flags) : DynamicSizer(flags)
			{
				;
			}
			virtual void getMaxSize(int &w, int &h)
			{
				for (childIter i = childList.begin(); i != childList.end(); ++i)
				{
					int tw, th;
					(*i)->getMaxSize(tw, th);
					w += tw;
					h = lwpp::Max(h, th);
				}
			}
			virtual void getMinSize(int &w, int &h)
			{
				for (childIter i = childList.begin(); i != childList.end(); ++i)
				{
					int tw, th;
					(*i)->getMinSize(tw, th);
					w += tw;
					h = lwpp::Max(h, th);
				}
			}
			virtual void getCurrentSize(int &w, int &h)
			{
				for (childIter i = childList.begin(); i != childList.end(); ++i)
				{
					int tw, th;
					(*i)->getCurrentSize(tw, th);
					w += tw;
					h = lwpp::Max(h, th);
				}
			}
			virtual void Layout(int x, int y, int w, int h, int lw)
			{
				int currentW = 0;
				int currentH = 0;
				getCurrentSize(currentW, currentH);
				int remW = w - currentW;
				int remH = h - currentH;
				int numChilds = 0; // the number of childs that can be stretched
				for (childIter i = childList.begin(); i != childList.end(); ++i)
				{
					if ( (*i)->getFlags() & STRETCH_HORIZONTAL ) numChilds++;
				}
				remW /= numChilds;
				int top = x;
				int left = y;
				for (childIter i = childList.begin(); i != childList.end(); ++i)
				{
					int tw, th;
					(*i)->getCurrentSize(tw, th);
					if ( (*i)->getFlags() & STRETCH_HORIZONTAL)
					{
						tw += remW;
					}
					(*i)->Layout(left, top, tw, th, lw);
					top += th;
					left += tw;
				}
			}
	};

	//! A sizer with components lined up vertically
	class VerticalSizer : public DynamicSizer
	{
		public:
			VerticalSizer(int flags) : DynamicSizer(flags)
			{
				;
			}
			virtual void getMinSize(int &w, int &h)
			{
				for (childIter i = childList.begin(); i != childList.end(); ++i)
				{
					int tw, th;
					(*i)->getMinSize(tw, th);
					h += th;
					w = lwpp::Max(w, tw);
				}
			}
			virtual void getMaxSize(int &w, int &h)
			{
				for (childIter i = childList.begin(); i != childList.end(); ++i)
				{
					int tw, th;
					(*i)->getMaxSize(tw, th);
					h += th;
					w = lwpp::Max(w, tw);
				}
			}
			virtual void getCurrentSize(int &w, int &h)
			{
				for (childIter i = childList.begin(); i != childList.end(); ++i)
				{
					int tw, th;
					(*i)->getCurrentSize(tw, th);
					h += th;
					w = lwpp::Max(w, tw);
				}
			}
			virtual void Layout(int x, int y, int w, int h, int lw)
			{
				int currentW = 0;
				int currentH = 0;
				getCurrentSize(currentW, currentH);
				int remW = w - currentW;
				int remH = h - currentH;
				int numChilds = 0; // the number of childs that can be stretched
				for (childIter i = childList.begin(); i != childList.end(); ++i)
				{
					if ( (*i)->getFlags() & STRETCH_VERTICAL) numChilds++;
				}
				remH /= numChilds;
				int top = x;
				int left = y;
				for (childIter i = childList.begin(); i != childList.end(); ++i)
				{
					int tw, th;
					(*i)->getCurrentSize(tw, th);
					if ( (*i)->getFlags() & STRETCH_VERTICAL)
					{
						th += remH;
					}
					(*i)->Layout(left, top, tw, th, lw);
					top += th;
					left += tw;
				}
			}
	};

	//! a sizer containing a single PanelControl
	/* A ControlSizer contains a single LWPanel control
	 *
	 */
	class ControlSizer : public Sizer
	{
		PanelControl control;
		int minWidth, minHeight;
		int Flags;
	public:
		ControlSizer(PanelControl _control, int flags = 0) : control(_control), Flags(flags)
		{
			getCurrentSize(minWidth, minHeight);
		}
		virtual Sizer *Add(Sizer *sizer, SizerFlags) {return sizer;}
		virtual void getMinSize(int &w, int &h)
		{
			w = minWidth;
			h = minHeight;
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
		}
		virtual int getLabelWidth()
		{
			return control.LW();
		}
		virtual void Layout(int x, int y, int w, int h, int lw)
		{
			control.Move(x + lw - getLabelWidth() ,y);

			if (!(Flags & STRETCH_HORIZONTAL)) w = minWidth;
			if (!(Flags & STRETCH_VERTICAL)) h = minHeight;
			control.setSize(w,h);
		}
		virtual int getFlags() {return Flags;}
		virtual Sizer *Add(Sizer *s) {return s;}
	};

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

}

#endif // LWPP_PANEL_SIZER_H