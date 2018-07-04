/*!
 * @brief C++ Wrapper implementation for LightWave Panels
 */
#ifndef LWPP_PANEL_H
#define LWPP_PANEL_H

#include <lwpp/global.h>
#include <lwpanel.h>
#include <lwpp/math.h>

//! sussed out by Greg Malick
#define LWRGB(red, green, blue) (0x01000000 | (red<<16) | (blue<<8) | green)

namespace lwpp
{
		//! Dynamic Values
		/*! DynaValues are used as function arguments, with commands  and requesters, 
		* for example, when the function must accept values of multiple types. 
		* LWValues are a variation on DynaValues used by Panels and defined in lwpanel.h. 
		* @ingroup LWPanels
		*/
	class LWDynaValue
	{
		LWValue val; //!The value stored.
		//! Resets the dynamic value.
		void _reset()
		{
			if (val.type == LWT_STRING)
			{
				delete [] val.str.buf;
			}
		}
	public:
		//! Destructor
		~LWDynaValue()
		{
			_reset();
		}
		LWDynaValue()
		{
		}
		/*! @brief Constructor for a LWValString
		 * @param s The string value.
		 * @param len The lenght of the string. */
		LWDynaValue(std::string s, int len = 0)
		{
			val.type = LWT_STRING;
			len = Max<int>(len, (int)s.length() + 1);
			if (len < 2) len = 2048; // use a large buffer just in case
			val.str.buf = new char[len];
			strcpy(val.str.buf, s.c_str());
			val.str.bufLen = len;
		}
		/*! @brief Constructor for a LWValInt
		 *  @param i The integer value. */
		LWDynaValue(int i)
		{
			val.type = LWT_INTEGER;
			val.intv.value = i;
		}
		/*! @brief Constructor for a LWValInt
		 *  @param i The unsigned integer value. */
		LWDynaValue(unsigned int i)
		{
			val.type = LWT_INTEGER;
			val.intv.value = i;
		}
		/*! @brief Constructor for a LWValFloat
		 *  @param d The double value. */
		LWDynaValue(double d)
		{
			val.type = LWT_FLOAT;
			val.flt.value = d;
		}
		/*! @brief Constructor for a LWValIVector
		 *  @param i The int i component.
		 *  @param j The int j component. 
		 *  @param k The int k component. */
		LWDynaValue(int i, int j, int k)
		{
			val.type = LWT_VINT;
			val.ivec.val[0] = i;
			val.ivec.val[1] = j;
			val.ivec.val[2] = k;
		}
		/*! @brief Constructor for a LWValIVector
		 *  @param v A vector of three integers
		 */
		LWDynaValue(int v[3])
		{
			val.type = LWT_VINT;
			val.ivec.val[0] = v[0];
			val.ivec.val[1] = v[1];
			val.ivec.val[2] = v[2];
		}
		/*! @brief Constructor for a LWValFVector
		 *  @param i The double i component.
		 *  @param j The double j component. 
		 *  @param k The double k component. */
		LWDynaValue(double i, double j, double k)
		{
			val.type = LWT_VFLOAT;
			val.fvec.val[0] = i;
			val.fvec.val[1] = j;
			val.fvec.val[2] = k;
		}
		/*! @brief Constructor for a LWValFVector
		 *  @param v vector with three components
		 */
		LWDynaValue(double v[3])
		{
			val.type = LWT_VFLOAT;
			val.fvec.val[0] = v[0];
			val.fvec.val[1] = v[1];
			val.fvec.val[2] = v[2];
		}
		/*! @brief Constructor for a LWValFVector
		 *  @param v vector with three components
		 */
		LWDynaValue(float v[3])
		{
			val.type = LWT_VFLOAT;
			val.fvec.val[0] = v[0];
			val.fvec.val[1] = v[1];
			val.fvec.val[2] = v[2];
		}
		/*! @brief Constructor for a LWValPointer
		 *  @param p A pointer. */
		LWDynaValue(void *p)
		{
			val.type = LWT_POINTER;
			val.ptr.ptr = p;
		}
		void setType (int type)
		{
			val.type = type;
		}
		/*! @brief Get the LWValInt
		 *  @param i reference of an int variable. */
		void getValue(int &i) const
		{
			i = val.intv.value;
		}
		/*! @brief Get the LWValFloat
		 *  @param d reference of a double variable. */
		void getValue(double &d) const
		{
			d = val.flt.value;		}

		/*! @brief Get the LWValIVector
		 *  @param i reference of a int variable.
		 *  @param j reference of a int variable.
		 *  @param k reference of a int variable. */
		void getValue(int &i, int &j, int &k) const
		{
			i = val.ivec.val[0];
			j = val.ivec.val[1];
			k = val.ivec.val[2];
		}
		void getValue(int v[]) const
		{
			v[0] = val.ivec.val[0];
			v[1] = val.ivec.val[1];
			v[2] = val.ivec.val[2];
		}
		/*! @brief Get the LWValFVector
		 *  @param i reference of a double variable.
		 *  @param j reference of a double variable.
		 *  @param k reference of a double variable. */
		void getValue(double &i, double &j, double &k) const
		{
			i = val.fvec.val[0];
			j = val.fvec.val[1];
			k = val.fvec.val[2];
		}
		/*! @brief Get the LWValPointer
		 *  @param *&p reference of a pointer variable. */
		void getValue(void *&p) const
		{
			p = val.ptr.ptr;
		}
		/*! @brief Get the std::string value
		 *  @param &s reference of a std::string variable. */
		void getValue(std::string &s) const
		{
			s = val.str.buf;
		}
		/*! @brief Get's the value adress
		 *  @return &val The address of the value. */
		LWValue *get() {return &val;}
	};
    
    /*! @class PanelTreeNode panel.h <lwpp/panel.h>
     *  @ingroup LWPanels
     * Encapsulates the information needed by the treeview to display items
     * This class is the base class for any item displayed in a tree control
     * All items need to implement getName()
     * Items with children need to implement getChildCount() and getChild() as well.
     */
    class PanelTreeNode
    {
        int m_flags;
    public:
        PanelTreeNode() : m_flags(NODE_FLAG_EXPND) { ; }
        virtual ~PanelTreeNode() {;}
        //! Returns the i'th child of the current item or 0 if there isn't one.
        virtual PanelTreeNode *getChild(int ) {return 0;}
        //! Returns the number of children of the current item
        virtual int getChildCount() {return 0;}
        //! Returns the name of the current item
        virtual const char *getName() = 0;
        //! Handles the flags that LWPanels uses to track the open/close state of a branch in the tree
        virtual const char *getInfo(int *flags)
        {
            if (*flags)
            {
                m_flags = *flags;
            }
            else
            {
                *flags = m_flags;
            }
            return getName();
        }
    };
    
    //! @class LWDrawFuncs panel.h <lwpp/panel.h>
	//! @ingroup LWPanels
	class LWDrawFuncs
	{
		LWPanelID pan; //! ID of the panel
		DrawFuncs *df; //! Pointer to the Draw Functions
		public:
			//! Constructor
			LWDrawFuncs(DrawFuncs *_df = 0, LWPanelID _pan = 0) : df(_df), pan(_pan) {;}
			//! Draw a single pixel using one of the colours defined in lwpanel.h
			void drawPixel(int c, int x,int y) const
			{
				df->drawPixel(pan, c, x, y);
			}
			//! Draw a single pixel using RGB colours
			void drawPixel(int r,int g,int b,int x,int y) const
			{
				df->drawRGBPixel(pan, r, g, b, x, y);
			}
			//! Draw a line using one of the colours defined in lwpanel.h
			void drawLine(int c, int x1, int y1, int x2, int y2) const
			{
				df->drawLine(pan, c, x1, y1, x2, y2);
			}
			void drawLine(int r, int g, int b, int x1, int y1, int x2, int y2) const
			{
				drawBox(r, g, b, x1, y1, x2 - x1, y2 - y1);
			}
			//! Draw a box using one of the colours defined in lwpanel.h
			void drawBox(int c, int x, int y, int w, int h) const
			{
				df->drawBox(pan, c, x, y, w, h);
			}
			void drawWireBox(int c, int x, int y, int w, int h) const
			{
				h -= 1; w -= 1;
				drawLine(c, x  , y  , x+w, y);
				drawLine(c, x+w, y  , x+w, y+h);
				drawLine(c, x+w, y+h, x  , y+h);
				drawLine(c, x  , y+h, x  , y);
			}
			//! Draw a box using RGB colours
			void drawBox(int r, int g, int b, int x, int y, int w, int h) const
			{
				df->drawRGBBox(pan, r, g, b, x, y, w, h);
			}

			//! Returns the width os a string in pixels, as if it was drawn on a panel
			int textWidth(const char *s) const
			{
				return df->textWidth(pan, s);
			}
			//! Returns the width os a string in pixels, as if it was drawn on a panel
			int textWidth(const std::string s) const
			{
				return textWidth(const_cast<char *>(s.c_str()));
			}
			//! Returns the width os a string in pixels, as if it was drawn on a panel
			int textWidth(const std::ostringstream &s) const
			{
				return textWidth(s.str());
			}

			//! Draw text in a panel
			void drawText(const std::string s, int c, int x, int y) const
			{
				df->drawText(pan, const_cast<char *>(s.c_str()), c, x, y);
			}
			//! Draw text in a panel
			void drawText(const std::ostringstream &s, int c, int x, int y) const
			{
				df->drawText(pan, const_cast<char *>(s.str().c_str()), c, x, y);
			}
			void drawBorder(int indent, int x,int y,int w,int h = 0) const
			{
				df->drawBorder(pan, indent, x, y, w, h);
			}
			//! Returns the display metrics of the current display
			/*
				int              width, height;
				int              pixX, pixY;
				int              maxColors, depth;
				int              textHeight;
				int              textAscent;
			 */
			const LWDisplayMetrics *displayMetrics() const
			{
				return df->dispMetrics();
			}
	};

	//! Helper function for callbacks

	class PanelControl;
    class LWPanel;

	void *getPanelHandler(LWControlID control);
	void *getControlHandler(LWControlID control);

	template <typename T>
	struct GetPanelEventHandler
	{		
		static T *getHandler(LWControlID control)
		{
			return static_cast<T*>(getPanelHandler(control));
		}		
	};

	template <typename T>
	struct GetControlEventHandler
	{
		static T *getHandler(LWControlID control)
		{
			return static_cast<T*>(getControlHandler(control));
		}
	};

		//! Class to adapt callbacks for Panel controls on a LW Panel.
		/*! @ingroup LWPanelAdaptor */
	template <typename T, template <typename U> class F = GetPanelEventHandler>
	class PanelControlAdaptor
	{
	public:
		//! Control event
		static void cb_Event(LWControlID id, void *userdata)
		{
			if ( T *eventHandler = F<T>::getHandler(id) )
			{
				eventHandler->controlEvent(id);
			}
		}
		//! Draw Control
		static void cb_Draw(LWControlID id, void *userdata, DrMode mode)
		{
			if ( T *eventHandler = F<T>::getHandler(id) )
			{
				eventHandler->controlDraw(id, mode);
			}
		}
		//! Popup Count
		static int cb_Count(void *userdata)
		{
      LWControlID control = static_cast<LWControlID>(userdata);
			if ( T *eventHandler = F<T>::getHandler(control) )
			{
				return static_cast<int>(eventHandler->controlCount(control));
			}
			return 0;
		}
		//! Popup Name
		static const char *cb_Name(void *userdata, int index)
		{
			LWControlID control = static_cast<LWControlID>(userdata);
			if ( T *eventHandler = F<T>::getHandler(control) )
			{
				return eventHandler->controlName(control, index);
			}
			return "";
		}
		//! MultiList Name 
		static const char *cb_MultiListName(void *userdata, int index, int column)
		{
			auto control = static_cast<LWControlID>(userdata);
			if ( T *eventHandler = F<T>::getHandler(control) )
			{
				return eventHandler->controlMultiListName(control, index, column);
			}
			return "";
		}
		//! MultiList Column width
		static int cb_MultiListColumnWidth(void *userdata, int index)
		{
			auto control = static_cast<LWControlID>(userdata);
			if ( T *eventHandler = F<T>::getHandler(control) )
			{
				return eventHandler->controlMultiListColumnWidth(control, index);
			}
			return 0;
		}
		//! Tree Control Child
		static void *cb_TreeChild(void *userdata, void *node, int i)
		{
			auto control = static_cast<LWControlID>(userdata);
			if ( T *eventHandler = F<T>::getHandler(control) )
			{
				return eventHandler->controlTreeChild(control, (PanelTreeNode *)node, i);
			}
			return 0;
		}
		//! Tree Count Child
		static int cb_TreeCount(void *userdata, void *node)
		{
			auto control = static_cast<LWControlID>(userdata);
			if ( T *eventHandler = F<T>::getHandler(control) )
			{
				return eventHandler->controlTreeCount(control, (PanelTreeNode *)node);
			}
			return 0;
		}
		//! Tree Info
		static char *cb_TreeInfo(void *userdata, void *node, int *flags)
		{
			auto control = static_cast<LWControlID>(userdata);
			if ( T *eventHandler = F<T>::getHandler(control) )
			{
				return const_cast<char *>(eventHandler->controlTreeInfo(control, (PanelTreeNode *)node, flags));
			}
			return const_cast<char *>("");
		}
		//! Tree Child Move
		static void cb_TreeMove(void *userdata, void *node, void *parent, int i)
		{
			auto control = static_cast<LWControlID>(userdata);
			if ( T *eventHandler = F<T>::getHandler(control) )
			{
				eventHandler->controlTreeMove(control, (PanelTreeNode *)node, parent, i);
			}
		}
	};	

	/*! @class PanelControl panel.h <lwpp/panel.h>
		* @ingroup LWPanels
		* Class to adapt callbacks for Panel controls on a LW Panel.
	*/
	class PanelControl
	{
	private:
		LWControl *control; //! Pointer to the control
		bool ghosted;
	protected:
		/*! Get the control attributes, including it's value.
		 * @param tag One of the cTag defined in lwpanel.h (line 186).
		 * @param *val Pointer to an LWValue. */
		void get (cTag tag, LWValue *val)	{	control->get(control, tag, val); }
		/*! Set the control attributes, including it's value.
		 * @param tag One of the TAGS defined in lwpanel.h (line 186).
		 * @param *val Pointer to an LWValue. */
		void set (cTag tag, LWValue *val)	{	control->set(control, tag, val); }
	public:
		/*! Constructor
		 *  @param ctl The LWControlID of the control */
		PanelControl (LWControlID ctl = 0)
			: ghosted(false)
		{
			SetControl (ctl);
		}
        PanelControl (const PanelControl &ctl)
			: ghosted(ctl.ghosted)
		{
			SetControl (ctl.control);
		}
		PanelControl &operator=(const PanelControl &from)
		{
			if (this != &from)
			{
				control = from.control;
				ghosted = from.ghosted;
			}
			return *this;
		}
		/*! Set the mID of LWControl managed by this PanelControl
		 *  @param ctl The LWControlID of the control */
		void SetControl(LWControlID ctl)
		{
			control = static_cast<LWControl *>(ctl);
		}
		//! Returns the ID of the LWControl managed by this PanelControl
		LWControlID getID() const
		{
			return (void *) control;
		}
		//! Returns LWControl managed by this PanelControl
		LWControl *getControl()
		{
			return control;
		}
		//! Asignment operator
		void operator=(LWControlID ctl)
		{
			SetControl(ctl);
		}
		//! Asignment operator
		void operator=(LWControl *ctl)
		{
			control = ctl;
		}
		//! Equal operator
		bool operator==(const LWControlID ctl)
		{
			return (control == static_cast<LWControl *>(ctl));
		}
		//! Equal operator
		bool operator==(const PanelControl ctl)
		{
			return (getID() == ctl.getID());
		}
		bool operator<(const PanelControl ctl) const
		{
			return (getID() < ctl.getID());
		}
		//! Equal operator
		bool operator==(const LWControl *ctl)
		{
			return (control == ctl);
		}
		//! Returns true if is a valid control
		bool isValid() const
		{
			return (control != 0);
		}

		//! Changes the draw mode of the control
		//! @param mode The draw mode for the control. Render by default.
		void Draw(DrMode mode = DR_RENDER)
		{
			if (control) control->draw(control, mode);
		}
		//! Erases the control from the panel.
		void Erase()
		{
			Draw(DR_ERASE);
		}
		//! Redraws the control in the panel.
		void Redraw() 
		{
			Draw (DR_REFRESH);
		}
		void Refresh() 
		{
			Draw (DR_REFRESH);
		}
		//! Ghost the control in the panel.
		//! How to show to the user a 
		//! control is deactivated.
		void Ghost(bool render = true)
		{
			ghosted = true;
			if (render) Render();
		}
		void UnGhost(bool render = true)
		{
			ghosted = false;
			if (render) Render();
		}
		//! Renders the control in the panel.
		void Render()
		{      
			Draw();
			if (ghosted)
			{
				Draw(DR_GHOST);
			}
		}
		bool isGhosted() const
		{
			return ghosted;
		}
		
		/*! Template version. Sets the control attributes, including it's value.
		 * @param val The value for the tag.
		 * @param tag One of the TAGS defined in lwpanel.h (line 186). */
		template<typename T>
		void Set (T val, cTag tag = CTL_VALUE)
		{
			LWDynaValue dval(val);
			set(tag, dval.get());
		}
		/*! Template version. Gets the control attributes, including it's value.
		 * @param &val reference to the value for the tag.
		 * @param tag One of the TAGS defined in lwpanel.h (line 186). */
		template<typename T>
		void Get (T &val, cTag tag = CTL_VALUE)
		{
			LWDynaValue dval(val);
			get(tag, dval.get());
			dval.getValue(val);
		}
		/*! Gets an int value of a control.
		 * @param tag One of the TAGS defined in lwpanel.h (line 186). */
		int GetInt(cTag tag = CTL_VALUE)
		{
			int v;
			Get (v, tag);
			return v;
		}
		void GetVec(int &a, int &b, int &c)
		{
			LWDynaValue dval;
			dval.setType(LWT_VINT);
			get(CTL_VALUE, dval.get());
			dval.getValue(a,b,c);
		}
		void GetVec(int v[])
		{
			LWDynaValue dval;
			dval.setType(LWT_VINT);
			get(CTL_VALUE, dval.get());
			dval.getValue(v);
		}
		/*! Gets an boolean value of a control.
		 * @param tag One of the TAGS defined in lwpanel.h (line 186). */
		bool GetBool(cTag tag = CTL_VALUE)
		{
			return (GetInt(tag) != 0);
		}
		//! Gets an pointer value of a control.
		/*! @param tag One of the TAGS defined in lwpanel.h (line 186). 
		 * @return A pointer to the value 
		 */
		void *GetPointer(cTag tag = CTL_USERDATA)
		{
			LWValue ival = {LWT_POINTER};
			get(tag, &ival);
			return ival.ptr.ptr;
		}
		//! Gets a double value of a control.
		double GetFloat()
		{
			LWValue fval = {LWT_FLOAT};
			get(CTL_VALUE, &fval);
			return fval.flt.value;
		}

		//! Set the minimum range
		int RangeMin() {return GetInt(CTL_RANGEMIN);}
		int RangeMax() {return GetInt(CTL_RANGEMAX);}

		void RangeMin(int m) {return Set(m, CTL_RANGEMIN);}
		void RangeMax(int m) {return Set(m, CTL_RANGEMAX);}


		/*! @name Control Layout
		 *  Functions to place any layout controls
		 */
		//@{

		// control positioning
		//! Return the X position of the control
		int X() {return GetInt(CTL_X);}
		//! Return the Y position of the control
		int Y() {return GetInt(CTL_Y);}
		//! Return the width of the control
		int W() {return GetInt(CTL_W);}
		//! Return the height of the Controls
		int H() {return GetInt(CTL_H);}
		//! Return the label width of the control
		int LW() {return GetInt(CTL_LABELWIDTH);}
		//! Return the left position of the clickable area of the control
		int HotX() {return GetInt(CTL_HOTX);}
		//! Return the top position of the clickable area of the control
		int HotY() {return GetInt(CTL_HOTY);}
		//! Return the width of the clickable area of the control
		int HotW() {return GetInt(CTL_HOTW);}
		//! Return the left height of the clickable area of the control
		int HotH() {return GetInt(CTL_HOTH);}
		//! Return the mouse X position on the control for the most recent event
		int MouseX() {return GetInt(CTL_MOUSEX);}
		//! Return the mouse Y position on the control for the most recent event
		int MouseY() {return GetInt(CTL_MOUSEY);}
		// @}
		/*! @name Interactions
		*  Functions for interaction with the user
		*/
		//@{
		//! Return the mouse button that was pressed
		mBut getMouseButton()
		{
			return static_cast<mBut>(GetInt(CTL_MOUSEBUTTON));
		}

		int MouseClickCount() { return GetInt(CTL_MOUSECLICKCNT); }

		//! Check if a coordinate is within the hot area of the control
		bool isInsideHot(int x, int y)
		{
			int cx = HotX();
			int cy = HotY();
			if ( lwpp::inRange(x, cx, cx + HotW()) || !lwpp::inRange(y, cy, cy + HotH()) ) return true;
			return false;
		}
		//! Check if a coordinate is within the area of the control
		bool isInside(int x, int y)
		{
			int cx = X();
			int cy = Y();
			if ( lwpp::inRange(x, cx, cx + W()) || !lwpp::inRange(y, cy, cy + H()) ) return true;
			return false;
		}

		// @}
		//! Returns the LWPanelID of the panel
		LWPanelID PanelID()
		{
			return static_cast<LWPanelID>(GetPointer(CTL_PANEL));
		}
		//! Returns a pointer to the panel functions
		LWPanelFuncs *PanelFuncs()
		{
			return static_cast<LWPanelFuncs *>(GetPointer(CTL_PANFUN));
		}
		/*! Moves a control by the number of pixels indicated on x, and y.
		 *  @param x Number of pixels to move the control horizontally
		 *  @param y Number of pixels to move the control vertically */
		void Move(int x, int y)	{	Set(x, CTL_X); Set(y, CTL_Y);	}
		/*! Moves a control by the number of pixels indicated on x, and y.
		*  @param x Number of pixels to move the control horizontally
		*  @param y Number of pixels to move the control vertically */
		void setPosition(int x, int y) { Set(x, CTL_X); Set(y, CTL_Y); }
		/*! Moves a control to the right side of another control
		 *  @param &ref reference to the original control used to position this control at is right side
		 *  @param space Number of pixels to separate the control horizontally */
		void MoveRightOf(PanelControl &ref, int space = 0)	{
			MoveX(ref.X() + ref.W() + space);
			MoveY(ref.Y());
		}
		/*! Moves a control to the left side of another control
		 *  @param &ref reference to the original control used to position this control at is left side
		 *  @param space Number of pixels to separate the control horizontally */
		void MoveLeftOf(PanelControl &ref, int space = 0)	{
			MoveX(ref.X() - W() - space);
			MoveY(ref.Y());
		}
		/*! Moves a control under another control
		 *  @param &ref reference to the original control used to position this control belows it
		 *  @param space Number of pixels to separate the control vertically */
		void MoveBelow(PanelControl &ref, int space = 0)	{
			MoveX(ref.X());
			MoveY(ref.Y() + ref.H() + space);
		}
		void MoveBelowTab(PanelControl &ref)	{
			MoveX(ref.X() + 6);
			MoveY(ref.Y() + ref.H());
		}
		/*! Aligns a control to the left of another control
		 *  @param &ref reference to the original control used to left align this control */
		void LeftAlign(PanelControl &ref, int nudge = 0)	{
			MoveX(ref.X() + nudge);
		}
		/*! Aligns a control to the right of another control
		 *  @param &ref reference to the original control used to right align this control */
		void RightAlign(PanelControl &ref, int nudge = 0)	{
			MoveX(ref.X() + ref.W() - W() + nudge);
		}
		/*! Moves horizontally a control by the number of pixels indicated on x
		 *  @param x Number of pixels to move the control horizontally */
		void MoveX(int x)	{	Set(x, CTL_X);}
		/*! Moves vertically a control by the number of pixels indicated on y
		 *  @param y Number of pixels to move the control vertically */
		void MoveY(int y)	{	Set(y, CTL_Y);}
		/*! Sets the width and height of a control by the number of pixels indicated on w, and h.
		 *  @param w Number of pixels to indicate control's width
		 *  @param h Number of pixels to indicate control's height */
		void setSize(int w, int h)	{setWidth(w); setHeight(h);}
		/*! Sets the width of a control by the number of pixels indicated on w.
		 *  @param w Number of pixels to indicate control's width */
		void setWidth(int w)	{	Set(w, CTL_W);}
		/*! Sets the height of a control by the number of pixels indicated on h.
		 *  @param h Number of pixels to indicate control's height */
		void setHeight(int h)	{	Set(h, CTL_H);}

		/*! Sets a control draw callback
		 *  You can draw anywhere on the panel from within this callback.
		 * @param d A pointer to a function: typedef void (*LWCtlDrawHook)(LWControlID, void *,DrMode); */
		void SetUserDraw(LWCtlDrawHook d)
		{
			Set(reinterpret_cast<void *>(d), CTL_USERDRAW);
		}
		/*! Sets a pointer to your data for this control.
		 *  This data pointer is passed as the second argument to the control callbacks.
		 *  Note: For some control types, calling set with this tag has important side effects.
		 *  Even if your userdata is NULL for those controls, you'll want to explicitly set it before opening the panel. 
		 *  @param[in] *d Pointer to your data
		 */
		void SetUserData(void *d)
		{
			Set(d, CTL_USERDATA);
		}
		//! Gets the pointer to your data for this control.
		void *GetUserData()
		{
			return GetPointer(CTL_USERDATA);
		}
		/*! Template version of the CON_SETEVENT macro.
		 * Sets the PanelControlAdaptor<T>::cb_Event callback for the control events */
		template <typename T, template <typename U> class F = GetPanelEventHandler> void SetEventCallback()
		{
			Set<void *>((void *)PanelControlAdaptor<T, F>::cb_Event, CTL_USEREVENT);
		}
		/*! Template version of the CON_SETUSERDRAW macro.
		 * Sets the PanelControlAdaptor<T>::cb_Draw callback for the control draw */
		template <typename T, template <typename U> class F = GetPanelEventHandler> void SetDrawCallback()
		{
			Set<void *>((void *)PanelControlAdaptor<T, F>::cb_Draw, CTL_USERDRAW);
		}
		void initMultiListBox()
		{
			initListBox();
		}
		void initListBox()
		{
			SetUserData(control);
		}
	};

	/*! @class LWPanelControlCallBacks panel.h <lwpp/panel.h>
	*  @ingroup LWPanels
	*  Inherit from this and override the virtual functions to handle PopUp control events */
	class LWPanelControlCallBacks
	{
	public:
		/*! Control Event Callback
		 *  
		 *  @param control reference to the control */
		virtual void controlEvent(PanelControl control) {;}
		/*! Control Draw Callback
		 *  @param control reference to the control
		 *  @param mode The draw mode of the control */
		virtual void controlDraw(PanelControl control, DrMode ) {;}
	};

	/*! @class LWPanelListCallBacks panel.h <lwpp/panel.h>
	 * @ingroup LWPanels 
	 * Inherit from this and override the virtual functions to handle List Box control events */
	class LWPanelListCallBacks
	{
	public:
		//! Return the number of items on the ListBox
		/*!
		 *  @param control reference to the control 
		 */
		virtual size_t controlCount(PanelControl control) = 0;
		//! Return the name of the item showed on the ListBox
		/*
		 *  @param control reference to the control 
		 *  @param index A 0 based integer used to find the name of the item 
		 */
		virtual const char* controlName(PanelControl control, int index) = 0;
	};

	 //! Callbacks for multi-line list boxes
	 /*!  Inherit from this and override the virtual functions to handle MultiList Box control events 
		*  A MultiList Box is like a List Box, but divided by multiple colums.
		*   @ingroup LWPanels
		*/
	class LWPanelMultiListControlCallBacks
	{
	public:
		//! Return a string for a position and column on MultiList control
		/*!
		 * @param control reference to the control 
		 * @param index A 0 based integer used to find the name of the item 
		 * @param column the column for the string to be returned
		 * @return the label for the indexed item
		 */
		virtual const char* controlMultiListName(PanelControl control, int index, int column) = 0;
		//! Return the width of each column in pixels, it can have up to 10 columns.
		/*!
		 * @param control reference to the control 
		 * @param index A 0 based integer indicating the column number 
		 * @return the width in pixels of the respective column
		 */
		virtual int controlMultiListColumnWidth(PanelControl control, int index) = 0;
	};

	/*! @class LWPanelTreeControlCallBacks panel.h <lwpp/panel.h>
	 *  @ingroup LWPanels
	 * Inherit from this and override the virtual functions to handle Tree control events
	 *
	 * A tree control is like a listbox, but with the menu items organized hierarchically.
	 * Child nodes of the tree can be revealed, hidden and sometimes moved by the user.
	 * The value of a tree control is a pointer to a node in your tree data, which can be obtained and set through GET_ADDR() and SET_ADDR().
	 * Trees don't prescribe the internal form of your tree data, but you have to be able to answer the questions about that data asked by the callbacks, which look like this.
	 */
	class LWPanelTreeControlCallBacks
	{
	public:
		/*!
		 * Returns a pointer to the root of the node
		 *  @param control reference to the tree control 
		 */
		virtual PanelTreeNode* controlTreeGetRoot(PanelControl control) = 0;
		/*!
		 * Returns a pointer to the i-th child of the node
		 *  @param control reference to the control 
		 *  @param node is a pointer returned by a previous call to this callback, or NULL for the root
		 *  @param i An integer indicating the i-th child */
		virtual PanelTreeNode* controlTreeChild(PanelControl control, PanelTreeNode *node, int i)
		{
			if (node) return node->getChild(i);
			return controlTreeGetRoot(control)->getChild(i);
		}
		/*!
		 * Returns the number of child nodes for a given node.
		 *  @param control reference to the control 
		 *  @param node is a pointer returned by a previous call to this callback, or NULL for the root */
		virtual int controlTreeCount(PanelControl control, PanelTreeNode *node)
		{
			if (node) return node->getChildCount();
			return controlTreeGetRoot(control)->getChildCount();
		}
		/*!
		 * Returns the name of the node.
		 * This is what is displayed in the tree control.
		 * If flags is non-zero, store the flags value in your node data, and if it's 0, retrieve it from your data and put it into flags.
		 *  @param control reference to the control 
		 *  @param node is a pointer returned by a previous call to this callback, or NULL for the root
		 *	@param flags The flags of the node, indicate if it's collapsed or not */
		virtual const char* controlTreeInfo(PanelControl control, PanelTreeNode *node, int *flags)
		{
			if (node) return node->getInfo(flags);
			return controlTreeGetRoot(control)->getInfo(flags);
		}
		/*!
		 * Called when the user moves a node.
		 * The node becomes the i-th child of the parent node.
		 * This isn't in the TREE_CTL macro's argument list
		 * TREE_CTL sets this to NULL, which prevents the user from moving your nodes.
		 * If you want to allow the user to move your nodes, use the code in the body of the TREE_CTL macro to create the control, putting your move callback in desc.tree.moveFn.
		 *  @param control reference to the control 
		 *  @param node is a pointer to the moved node
		 *  @param parent is a pointer to the parent node
		 *  @param i An integer indicating the i-th child */
		virtual void controlTreeMove(PanelControl , PanelTreeNode *, PanelTreeNode *, int ) {;}
	};

	/*! @class LWPanel panel.h <lwpp/panel.h>
	 *  @ingroup Globals
	 *  @ingroup LWPanels
	 *  Wrapper for LWPanelFuncs
	 *	The functions associated with the panel.
	 */
	class LWPanel : public GlobalBase<LWPanelFuncs>
	{
	private:
		LWPanelID id; //!< ID of the panel
		bool close_on_destroy; //!< bool value to indicate how to deal with the panel when it's destroyed
		std::string mTitle; //!< Title of the panel
	protected:
		//! Get a panel attribute
		/*
		 * @param tag One of the pTag defined in lwpanel.h (line 175).
		 * @param *data is the panel attribute cast as a void *
		 */
		void get (pTag tag, void *data) const	{	globPtr->get(id, tag, data);}

		//! Set a panel attributes
		/*!
		 * @param tag One of the pTag defined in lwpanel.h (line 175).
		 * @param *data is the panel attribute cast as a void *
		 */
		void set (pTag tag, void *data)	{	globPtr->set(id, tag, data);}

		//! Add a control to the panel
		/*!
		 * It must be called after create the panel
		 * but before opening it. The control is positioned below the previous created
		 * one. The order of creation of the controls affect it's drawing order.
		 * The control can be moved after create it. 
		 * The panel will change it's size automatically to fit all the controls.
		 *
		 * @param *type pointer to a string indicating the type of control.
		 * @param *desc a pointer to a LWPanControlDesc structure
		 * @param *label a pointer to a string with the label for the control
		 * @return A pointer to a LWControl structure
		 */
		LWControl *addControl (const char *type, LWPanControlDesc *desc, const char *label)
		{
			return globPtr->addControl(id, const_cast<char *>(type), desc, const_cast<char *>(label));
		}
	public:
		//! Default Constructor
		LWPanel ()
			: id(0),
				close_on_destroy(false)
		{
			;
		}
		//! Constructor defining a panel title as well
		/*!
		 * @param _title a std:string with the title for the panel
		 */
		LWPanel (const std::string _title)
			: close_on_destroy(true),
				mTitle(_title)
		{
			if (!available()) return; // in the case LWPanels aren't available - i.e. LWSN
			id = globPtr->create(const_cast<char *>(mTitle.c_str()), globPtr);
		}
		//! Constructor from an existing LWPanel
		/*!
		 * @param pan_id The LWPanelID of an existing panel
		 */
		LWPanel (LWPanelID pan_id)
			: id(pan_id),
				close_on_destroy(false)
		{
			;
		}
		//! Destructor
		~LWPanel ()
		{
			if (!available()) return; // in the case LWPanels aren't available - i.e. LWSN
			if (close_on_destroy)
			{
				globPtr->destroy(id);
				id = 0;
			}
		}
		//! Copy constructor
		LWPanel &operator=(const LWPanel &from)
		{
			if (&from != this)
			{
				id = from.id;
				close_on_destroy = false;
				mTitle = from.mTitle;
			}
			return *this;
		}
		//! Creates a panel
		/*
		 * @param _title a std:string with the title for the panel
		 * @param _close_on_destroy a bool value indicating if the panel is closed when the plugin is destroyed???
		 */
		void Create (const std::string _title, bool _close_on_destroy = true)
		{
			mTitle = _title;
			close_on_destroy = _close_on_destroy;
			id = globPtr->create(const_cast<char *>(mTitle.c_str()), globPtr);
		}

		//! Returns the ID of the panel as a LWPanelID
		LWPanelID GetID() const	{	return id; }

		//! return if we have a valid panel
        bool isValid() const { return id != nullptr; }

		//! Opens and displays the panel
		/*!
		 * The panel must be created first, as it's controls created, sized, positioned and initialized as well.
		 * @param flags an | combination of the PAN_FLAGS defined in lwpanel.h (line 300).
		 */
		int Open (int flags)
		{
			return globPtr->open(id, flags);
		}
		/*! Opens and displays the panel
		 * The panel must be created first, as it's controls created, sized, positioned and initialized as well.
		 * @param flags an | combination of PANF_BLOCKING | PANF_CANCEL | PANF_FRAME  and the flags passed, defined in lwpanel.h (line 300).
		*/
		int Post (int flags = 0)
		{
			return globPtr->open(id, PANF_BLOCKING | PANF_CANCEL | PANF_FRAME | flags);
		}
		//! Close the panel
		void Close() {if (isValid()) globPtr->close(id); }
		void ClearID() {id = 0;}
		//! Recreates the panel
		void Recreate ()
		{
			if (id)
			{
				Close();
				globPtr->destroy(id);
				id = 0;
			}
			id = globPtr->create(const_cast<char *>(mTitle.c_str()), globPtr);
		}
		/*! Recreates the panel with a new title
		 * @param _title a std:string with the title for the panel */
		void Recreate (const std::string _title)
		{
			if (id)
			{
				Close();
				globPtr->destroy(id);
				id = 0;
			}
			Create(_title);
		}
		/*! Opens and displays a NON-Modal panel, one opened without PANF_BLOCKING.
		 * Open is called inmediately and return's the control to handle. 
		 * The panel must be created first, as it's controls created, sized, positioned and initialized as well.
		 * @param block If 0, it returns when the event queue is empty, if it's EVNT_BLOCKING, it will not return until the 
		 * user has closed the panel manually.
		 * @return 0 if the panel is still open, or -1 if the user closed it.
		*/
		int Handle (int block) { return globPtr->open(id, block); }

		/*! Draws the panel
		 * After the panel is draw on the screen, it will call 
		 * your panel draw callbacks if they are set
		 * @param mode */
		void Draw(DrMode mode = DR_REFRESH) {	globPtr->draw(id, mode); }

		/*! Iterates the controls of the panel */
		LWControl *nextControl(LWControlID *ctl){ return globPtr->nextControl(id, ctl);}
		/*! @name Drawing
		 *  Drawing related functionality
		 */
		//@{
		//! Returns the draw functions for the panel
		LWDrawFuncs getDrawFuncs()
		{
			return LWDrawFuncs(globPtr->drawFuncs, id);
		}

		// Draw functions
		//! Draws a line
		void DrawLine(int color, int x1, int y1, int x2, int y2)
		{
			globPtr->drawFuncs->drawLine(id, color, x1, y1, x2, y2);
		}
		//! Draw a Border
		void DrawBorder(int indent, int x,int y,int w,int h = 0) const
		{
				globPtr->drawFuncs->drawBorder(id, indent, x, y, w, h);
		}

		//! Draw a resizing handle on the lower left corner
		void DrawResizeHandle()
		{
			int width = GetWidth();
			int height = GetHeight();
			DrawLine(LWP_WHITE, width-18, height-2, width-2,height-18 );
			DrawLine(LWP_WHITE, width-14, height-2, width-2,height-14 );
			DrawLine(LWP_WHITE, width-10, height-2, width-2,height-10 );
			DrawLine(LWP_WHITE, width-6, height-2, width-2,height-6 );

			DrawLine(LWP_BLACK, width-17, height-2, width-2,height-17 );
			DrawLine(LWP_BLACK, width-13, height-2, width-2,height-13 );
			DrawLine(LWP_BLACK, width-9, height-2, width-2,height-9 );
			DrawLine(LWP_BLACK, width-5, height-2, width-2,height-5 );
		}
		//@}

		//! Returns a pointer to the userdata
		void *user_data() {return globPtr->user_data;}

		//! Returns the global
		GlobalFunc *globalFun() {return globPtr->globalFun;}
		/*! @name Controls
		 *  Controls that can be created with LWPanels
		 */
		//@{
		//! Adds a series of mutually exclusive buttons
		/*!
		 * @image html panelchoiceh.jpg "A horizontal choice control"
		 * @image html panelchoicev.jpg "A vertical choice control"
		 * @param *title The label for the control
		 * @param **items Pointer to a NULL terminated array for the options. i.e: static char *choices[] = { "One", "Two", "Three", NULL };
		 * @param orientation The orientation for the control
		 * @return a pointer to the created LWControl
		 *
		 *  How to add a AddChoice
		@code
		 static char *choices[] = { "One", "Two", "Three", NULL };
		 control = Panel.AddChoice("PopUp", choices, true);
		 @endcode
		 */
		LWControl *AddChoice (const char *title, const char **items, bool orientation)
		{
			LWPanControlDesc desc;
			desc.type = LWT_CHOICE;
			desc.choice.vertical = orientation ? MX_HCHOICE : MX_VCHOICE;
			desc.choice.items=items;
			return addControl("ChoiceControl", &desc, title);
		}
		/*! Adds a pop up control
		 *  @ingroup LWPanelControls
		 *  @image html panelpopupc.jpg "A closed pop up"
		 *  @image html panelpopupo.jpg "The choices when the popup is open"
		 *  How to add a AddPopUp
		 @code
			static char *choices[] = { "One", "Two", "Three", NULL };
			control = Panel.AddPopUp("PopUp", choices, 50);
			@endcode
		 *  @param *title The label for the control
		 *  @param **items Pointer to a NULL terminated array for the options. 
		 *  @param orientation The orientation for the control
		*/
		LWControl *AddPopUp (const char *title, const char **items, int width = 0)
		{
			LWPanControlDesc desc;
			desc.type = LWT_CHOICE;
			desc.choice.vertical=width;
			desc.choice.items=items;
			return addControl("PopupControl", &desc, title);
		}
		/*! Adds a numeric control for double values
		 *  @ingroup LWPanelControls
		 *  @image html panelfloat.jpg "A numeric control for double values"
		 *  How to add a AddFloat
		 *  control = Panel.AddFloat("Double Value");
		 *  @param *title The label for the control
		 *  @param type Type of the control to allow this function be used by AddDistance for example */
		LWControl *AddFloat (const char *title, LWType type = LWT_FLOAT)
		{
			LWPanControlDesc desc;
			desc.type = type;
			return addControl("FloatControl", &desc, title);
		}
		/*! Adds a numeric control for double values with support for units and their conversion
		 *  @ingroup LWPanelControls
		 *  @image html paneldistance.jpg "A numeric control for double values with units"
		 *  How to add a AddDistance
		 *  control = Panel.AddDistance("Double Value");
		 *  @param *title The label for the control */
		LWControl *AddDistance (const char *title) { return AddFloat(title, LWT_DISTANCE); }
		/*! Adds a Read Only numeric control for double values
		 *  @ingroup LWPanelControls
		 *  @image html panelfloatinfo.jpg "A read only numeric control for double values"
		 *  How to add a AddFloatInfo
		 *  control = Panel.AddFloatInfo("Info Double Value");
		 *  @param *title The label for the control */
		LWControl *AddFloatInfo (const char *title)
		{
			LWPanControlDesc desc;
			desc.type = LWT_FLOAT;
			return addControl("FloatInfoControl", &desc, title);
		}
		/*! Adds a numeric control for double vector values
		 *  @ingroup LWPanelControls
		 *  @image html panelfloatvec.jpg "A numeric control for double vector values"
		 *  How to add a AddFloatVector
		 *  control = Panel.AddFloatVector("Double Vector Value");
		 *  @param *title The label for the control
		 *  @param type Type of the control to allow this function be used by AddDistanceVector for example */
		LWControl *AddFloatVector (const char *title, LWType type = LWT_VFLOAT)
		{
			LWPanControlDesc desc;
			desc.type = type;
			return addControl("FVecControl", &desc, title);
		}
		/*! Adds a numeric control for double vector values with support for units and their conversion
		 *  @ingroup LWPanelControls
		 *  @image html paneldistancevec.jpg "A numeric control for double vector values with units"
		 *  How to add a AddDistanceVector
		 *  control = Panel.AddDistanceVector("Double Vector Value");
		 *  @param *title The label for the control */
		LWControl *AddDistanceVector (const char *title) {return AddFloatVector(title, LWT_VDIST);}
		/*! Adds a Read Only numeric control for double vector values
		 *  @ingroup LWPanelControls
		 *  @image html panelfloatvecinfo.jpg "A read only numeric control for double vector values"
		 *  How to add a AddFloatVectorInfo
		 *  control = Panel.AddFloatVectorInfo("Info Double Vector Value");
		 *  @param *title The label for the control */
		LWControl *AddFloatVectorInfo (const char *title)
		{
			LWPanControlDesc desc;
			desc.type = LWT_VFLOAT;
			return addControl("FVecInfoControl", &desc, title);
		}
		//! Adds a pop up filled up with items present on the scene
		/*!
			 * @image html paneladditem.jpg "The different popups with their options open"
		 *  @param *title The label for the control
		 *  @param type The type of items to be listed. Defined on lwrender.h (line 22)
		 *  @param width The width of the control in pixels
		 */
		LWControl *AddItem (const char *title, LWItemType type, int width = 0)
		{
			LWPanControlDesc desc;
			desc.type=LWT_LWITEM;
			desc.lwitem.global=reinterpret_cast<void *>(lwpp::SuperGlobal);
			desc.lwitem.itemType=type;
			desc.lwitem.list=NULL;
			desc.lwitem.count=0;
			desc.lwitem.width=width;
			return addControl("LWListControl", &desc, title);
		}
		/*! Adds a pick item pop up control
		 *  @ingroup LWPanelControls
		 *  @image html panelpickitem.jpg "Pick Item pop up and example choices when the popup is open, the text over the open lists is the LWItemType used for each one"
		 *  How to add a pick item pop up
			control = Panel.AddPickItem("Pick Item", LWI_ANY, 80);
		 *  @param *title The label for the control
		 *  @param type The type of items to be listed. Defined on lwrender.h (line 22) 
		 *  @param width The width of the control in pixels */
		LWControl *AddPickItem (const char *title, LWItemType type, int width)
		{
			LWPanControlDesc desc;
			desc.type=LWT_LWITEM;
			desc.lwitem.global=reinterpret_cast<void *>(lwpp::SuperGlobal);
			desc.lwitem.itemType=type;
			desc.lwitem.list=NULL;
			desc.lwitem.count=0;
			desc.lwitem.width=width;
			return addControl("LWPickListControl", &desc, title);
		}
		/*! Adds a button control
		 *  @ingroup LWPanelControls
		 *  @image html panelbutton.jpg "A single button"
		 *  How to add a button
			control = Panel.AddButton("Button", 50);
		 *  @param *title The label for the control
		 *  @param width The width of the control in pixels */
		LWControl *AddButton (const char *title, int width = 0)
		{
			LWPanControlDesc desc;
			desc.type=LWT_INTEGER;
			if (width != 0)
			{
				desc.type=LWT_AREA;
				desc.area.width=width;
				desc.area.height=0;
			}
			return addControl("ButtonControl", &desc, title);
		}
		/*! Adds a boolean check control
		 *  @ingroup LWPanelControls
		 *  @image html panelboolean.jpg "A single boolean check"
		 *  How to add a button
			control = Panel.AddBoolean("Boolean", 50);
		 *  @param *title The label for the control
		 *  @param width The width of the control in pixels */
		LWControl *AddBoolean (const char *title, int width = 0)
		{
			LWPanControlDesc desc;
			desc.type=LWT_BOOLEAN;
			desc.area.width=width;
			desc.area.height=0;
			return addControl("BoolControl", &desc, title);
		}
		/*! Adds a boolean button control
		 *  @ingroup LWPanelControls
		 *  @image html panelbooleanbut.jpg "A single boolean button"
		 *  How to add a button
			control = Panel.AddBoolButton("Boolean Button", 100);
		 *  @param *title The label for the control
		 *  @param width The width of the control in pixels */
		LWControl *AddBoolButton (const char *title, int width = 0)
		{
			LWPanControlDesc desc;
			desc.type= (width == 0 ) ? LWT_BOOLEAN : LWT_AREA; // is this actually correct in the SDK macros?
			desc.area.width=width;
			desc.area.height=0;
			return addControl("BoolButtonControl", &desc, title);
		}

		/*! Adds a numeric control for integer values
		 *  @ingroup LWPanelControls
		 *  @image html panelinteger.jpg "A numeric control for integer values"
		 *  How to add a AddInteger
		 *  control = Panel.AddInteger("Interger Value", 80);
		 *  @param *title The label for the control
		 *  @param width The width of the control in pixels */
		LWControl *AddInteger (const char *title, int width = 0)
		{			
			LWPanControlDesc desc;
			desc.type= LWT_INTEGER;
			return addControl("NumControl", &desc, title);
		}
		/*! Adds a Read Only numeric control for integer values
		 *  @ingroup LWPanelControls
		 *  @image html panelintinfo.jpg "A read only numeric control for integer values"
		 *  How to add a AddIntegerInfo
		 *  control = Panel.AddIntegerInfo("Info Integer Value", 80);
		 *  @param *title The label for the control 
		 *  @param width The width of the control in pixels */
		LWControl *AddIntegerInfo (const char *title, int width = 0)
		{
			LWPanControlDesc desc;
			desc.type= LWT_INTEGER;
			return addControl("NumInfoControl", &desc, title);
		}

		/*! Adds a slider with an edit field, edit's field value can't be bigger than its max range.
		 *  @ingroup LWPanelControls
		 *  @image html panelslider.jpg "A slider control with a (min = 0, max = 100)"
		 *  How to add a AddSlider
		 *  control = Panel.AddSlider("Slider", 0, 100, 100);
		 *  @param *title The label for the control 
		 *  @param min The minimun value for the slider
		 *  @param max The maximun value for the slider
		 *  @param width The width of the control in pixels
		 *  @param type Type of the control to allow this function be used by AddVerticalSlider for example */
		LWControl *AddSlider (const char *title, int min, int max, int width, const char *type = "SliderControl")
		{
			LWPanControlDesc desc;
			desc.type=LWT_RANGE;
			desc.range.width=width;
			desc.range.min=min;
			desc.range.max=max;
			return addControl(type, &desc, title);
		}
		/*! Adds a vertical slider without edit field.
		 *  @ingroup LWPanelControls
		 *  @image html panelvslider.jpg "A vertical slider control with a (min = 0, max = 100). On the image it's value is 0."
		 *  How to add a AddVerticalSlider
		 *  control = Panel.AddVerticalSlider("Slider", 0, 100, 100);
		 *  @param *title The label for the control 
		 *  @param min The minimun value for the slider
		 *  @param max The maximun value for the slider
		 *  @param width The width of the control in pixels */
		LWControl *AddVerticalSlider (const char *title, int min, int max, int width)
		{
			return AddSlider(title, min, max, width, "VSlideControl");
		}
		/*! Adds a horizontal slider without edit field.
		 *  @ingroup LWPanelControls
		 *  @image html panelhslider.jpg "A horizontal slider control with a (min = 0, max = 100). On the image it's value is 0."
		 *  How to add a AddHorizontalSlider
		 *  control = Panel.AddHorizontalSlider("Slider", 0, 100, 100);
		 *  @param *title The label for the control 
		 *  @param min The minimun value for the slider
		 *  @param max The maximun value for the slider
		 *  @param width The width of the control in pixels */
		LWControl *AddHorizontalSlider (const char *title, int min, int max, int width)
		{
			return AddSlider(title, min, max, width, "HSlideControl");
		}
		/*! Adds a horizontal slider with an edit field, edit's field value CAN be bigger than its max range.
		 *  @ingroup LWPanelControls
		 *  @image html paneluslider.jpg "A unbounded horizontal slider control with a (min = 0, max = 100). On the image it's value is 0. Note the edit field Value is different to the slider."
		 *  How to add a AddUnboundSlider
		 *  control = Panel.AddUnboundSlider("Unbound Slider", 0, 100, 100);
		 *  @param *title The label for the control 
		 *  @param min The minimun value for the slider
		 *  @param max The maximun value for the slider
		 *  @param width The width of the control in pixels */
		LWControl *AddUnboundSlider (const char *title, int min, int max, int width)
		{
			return AddSlider(title, min, max, width, "UnboundSlideControl");
		}
		/*! Adds a mini slider with an edit field, edit's field value can't be bigger than its max range.
		 *  @ingroup LWPanelControls
		 *  @image html panelmslider.jpg "A mini slider control with a (min = 0, max = 100). On the image it's value is 0."
		 *  How to add a AddMiniSlider
		 *  control = Panel.AddMiniSlider("Mini Slider", 0, 100, 100);
		 *  @param *title The label for the control 
		 *  @param min The minimun value for the slider
		 *  @param max The maximun value for the slider
		 *  @param width The width of the "edit field" (not the control) in pixels */
		LWControl *AddMiniSlider (const char *title, int min, int max, int width)
		{
			return AddSlider(title, min, max, width, "MiniSliderControl");
		}

		/*! Adds a numeric control for integer vector values
		 *  @ingroup LWPanelControls
		 *  @image html panelintvec.jpg "A numeric control for integer vector values"
		 *  How to add a AddVector
		 *  control = Panel.AddVector("Integer Vector Value");
		 *  @param *title The label for the control
		 *  @param type Type of the control to allow this function be used by AddVectorInfo for example */
		LWControl *AddVector (const char *title, const char *type = "VecControl")
		{
			LWPanControlDesc desc;
			desc.type = LWT_VINT;
			return addControl(type, &desc, title);
		}
		/*! Adds a Read Only numeric control for integer vector values
		 *  @ingroup LWPanelControls
		 *  @image html panelintvecinfo.jpg "A read only numeric control for integer vector values"
		 *  How to add a AddVectorInfo
		 *  control = Panel.AddVectorInfo("Info Integer Vector Value");
		 *  @param *title The label for the control */
		LWControl *AddVectorInfo (const char *title) { return AddVector(title,"VecInfoControl"); }
		/*! Adds a RGB control for color values
		 *  @ingroup LWPanelControls
		 *  @image html panelrgbcolor.jpg "A RGB control for color values"
		 *  How to add a AddRGB
		 *  control = Panel.AddRGB("RGB Color");
		 *  @param *title The label for the control */
		LWControl *AddRGB (const char *title)	{	return AddVector(title,"RGBControl"); }
		/*! Adds a Mini RGB control for color values
		 *  @ingroup LWPanelControls
		 *  @image html panelminirgbcolor.jpg "A Mini RGB control for color values"
		 *  How to add a AddMiniRGB
		 *  control = Panel.AddMiniRGB("Mini RGB Color");
		 *  @param *title The label for the control */
		LWControl *AddMiniRGB (const char *title)	{	return AddVector(title,"MiniRGBControl"); }
		/*! Adds a Mini HSV control for color values
		 *  @ingroup LWPanelControls
		 *  @image html panelminihvscolor.jpg "A Mini HSV control for color values"
		 *  How to add a AddMiniHSV
		 *  control = Panel.AddMiniHSV("Mini HSV Color");
		 *  @param *title The label for the control */
		LWControl *AddMiniHSV (const char *title)	{	return AddVector(title,"MiniHSVControl"); }
		/*! Adds a RGB Vector control for color values
		 *  @ingroup LWPanelControls
		 *  @image html panelrgbveccolor.jpg "A RGB Vector control for color values"
		 *  How to add a AddRGBVector
		 *  control = Panel.AddRGBVector("RGB Vector Color");
		 *  @param *title The label for the control */
		LWControl *AddRGBVector (const char *title)	{	return AddVector(title,"RGBVecControl"); }
		/*! Adds an edit field for a string of text 
		 *  @ingroup LWPanelControls
		 *  @image html paneltfstring.jpg "An empty edit field for a string text. 25 characters long."
		 *  How to add a AddString
		 *  control = Panel.AddString("Text Field", 25);
		 *  @param *title The label for the control
		 *  @param width The width in characters of the control
		 *  @param type Type of the control to allow this function be used by AddStringInfo for example */
		LWControl *AddString (const char *title, int width, const char *type = "EditControl")
		{
			LWPanControlDesc desc;
			desc.type = LWT_STRING;
			desc.string.width=width;
			return addControl(type, &desc, title);
		}
		/*! Adds a Read Only field for a string of text 
		 *  @ingroup LWPanelControls
		 *  @image html paneltfstringinfo.jpg "An empty field for a string text. 25 characters long."
		 *  How to add a AddStringInfo
		 *  control = Panel.AddStringInfo("Info Text Field", 25);
		 *  @param *title The label for the control
		 *  @param width The width in characters of the control */
		LWControl *AddStringInfo (const char *title, int width)	{	return AddString(title, width, "InfoControl"); }
		/*! Adds a file edit field with an arrow button to open the OS requester to select files 
		 *  @ingroup LWPanelControls
		 *  @image html panelfile.jpg "A file edit field with a fake file name. 25 characters long."
		 *  How to add a AddFile
		 *  control = Panel.AddFile("File", 25);
		 *  control.Set<const char *>("C:\\temp.file");
		 *  @param *title The label for the control
		 *  @param width The width in characters of the control */
		LWControl *AddFile (const char *title, int width) { return AddString(title, width, "FileControl");}
		/*! Adds a file button
		 *  @ingroup LWPanelControls
		 *  @image html panelfilebutton.jpg "A file button."
		 *  How to add a AddFileButton
		 *  control = Panel.AddFileButton("File", 25);
		 *  @param *title The label for the control
		 *  @param width The width in pixels of the control */
		LWControl *AddFileButton (const char *title, int width)	{	return AddString(title, width, "FileButtonControl");}
		/*! Adds a Load File edit field with an arrow button to open the OS requester to select files 
		 *  @ingroup LWPanelControls
		 *  @image html panelloadfile.jpg "A Load file edit field with a fake file name. 15 characters long."
		 *  How to add a AddFile
		 *  control Panel.AddLoad("Load File", 15);
		 *  control.Set<const char *>("C:\\temp.file");
		 *  @param *title The label for the control
		 *  @param width The width in characters of the control */
		LWControl *AddLoad (const char *title, int width)	{	return AddString(title, width, "LoadControl");}
		/*! Adds a Load File button
		 *  @ingroup LWPanelControls
		 *  @image html panelloadfilebutton.jpg "A Load File button."
		 *  How to add a AddLoadButton
		 *  control = Panel.AddLoadButton("Load File", 65);
		 *  @param *title The label for the control
		 *  @param width The width in pixels of the control */
		LWControl *AddLoadButton (const char *title, int width)	{return AddString(title, width, "LoadButtonControl");}
		/*! Adds a save file edit field with an arrow button to open the OS requester to select files 
		 *  @ingroup LWPanelControls
		 *  @image html panelsavefile.jpg "A save file edit field with a fake file name. 25 characters long."
		 *  How to add a AddSave
		 *  control = Panel.AddSave("Save File", 25);
		 *  control.Set<const char *>("C:\\temp.file");
		 *  @param *title The label for the control
		 *  @param width The width in characters of the control */
		LWControl *AddSave (const char *title, int width)	{ return AddString(title, width, "SaveControl");}
		/*! Adds a Save File button
		 *  @ingroup LWPanelControls
		 *  @image html panelsavefilebutton.jpg "A Save File button."
		 *  How to add a AddSaveButton
		 *  control = Panel.AddSaveButton("Save File", 65);
		 *  @param *title The label for the control
		 *  @param width The width in pixels of the control */
		LWControl *AddSaveButton (const char *title, int width)	{return AddString(title, width, "SaveButtonControl");}
		/*! Adds a Directory file edit field with an arrow button to open the OS requester to select a directory
		 *  @ingroup LWPanelControls
		 *  @image html paneldir.jpg "A directory file edit field with a real dir name. 25 characters long."
		 *  How to add a AddDir
		 *  control = Panel.AddDir("Select Directory", 25);
		 *  control.Set<const char *>("C:\\temp");
		 *  @param *title The label for the control
		 *  @param width The width in characters of the control */
		LWControl *AddDir (const char *title, int width){return AddString(title, width, "DirControl");}
		/*! Adds a Select Directory button
		 *  @ingroup LWPanelControls
		 *  @image html paneldirbutton.jpg "A Select Directory button."
		 *  How to add a AddDirButton
		 *  control = Panel.AddDirButton("Select Directory", 65);
		 *  @param *title The label for the control
		 *  @param width The width in pixels of the control */
		LWControl *AddDirButton (const char *title, int width){return AddString(title, width, "DirButtonControl");}
		/*! Adds a field for static strings of text 
		 *  @ingroup LWPanelControls
		 *  @image html paneltext.jpg "Three static strings of text"
		 *  How to add a AddText
		 *  const char *statictext[] = { "...one line of text", "...two lines of text", "...three lines of text", NULL };
		 *  control = Panel.AddText("Static Text", statictext);
		 *  @param *title The label for the control
		 *  @param text NULL terminated array of static text strings */
		LWControl *AddText (const char *title, const char**text)
		{
			LWPanControlDesc desc;
			desc.type = LWT_TEXT;
			desc.text.text=text;
			return addControl("TextControl", &desc, title);
		}
		/*! Adds a rectagular Area with a border. The area can generate mouse clicks.
		 *  @ingroup LWPanelControls
		 *  @image html panelarea.jpg "An area of 75x50 pixels."
		 *  How to add a AddArea
		 *  control = Panel.AddArea("Area", 75, 50);
		 *  @param *title The label for the control
		 *  @param width The width in pixels of the control
		 *  @param height The height in pixels of the control
		 *  @param type Type of the control to allow this function be used by AddOpenGL for example */
		LWControl *AddArea (const char *title, int width, int height, const char *type = "AreaControl")
		{
			LWPanControlDesc desc;
			desc.type = LWT_AREA;
			desc.area.width=width;
			desc.area.height=height;
			return addControl(type, &desc, title);
		}
		/*! Adds a rectagular OpenGL area with a border. OpenGL commands can be used in this control, after set a callback for it.
		 *  @ingroup LWPanelControls
		 *  @image html panelopengl.jpg "An OpenGL area of 75x50 pixels."
		 *  How to add a AddOpenGL
		 *  control = Panel.AddOpenGL("OpenGL", 75, 50);
		 *  @param *title The label for the control
		 *  @param width The width in pixels of the control
		 *  @param height The height in pixels of the control */
		LWControl *AddOpenGL (const char *title, int width, int height)	{return AddArea(title, width, height, "OpenGLControl");}
		/*! Adds a rectagular drawing area with a border. The drawing area doesn't include the 1 pixel border. 
		 *  @ingroup LWPanelControls
		 *  @image html panelcanvas.jpg "An area of 75x50 pixels."
		 *  How to add a AddCanvas
		 *  control = Panel.AddCanvas("Canvas", 75, 50);
		 *  @param *title The label for the control
		 *  @param width The width in pixels of the control
		 *  @param height The height in pixels of the control
		 */
		LWControl *AddCanvas (const char *title, int width, int height)	{return AddArea(title, width, height, "CanvasControl");}
		/*! Adds a drag button (mini slider) WITHOUT an edit field.
		 *  @todo This control doesn't resize. So width and height are not neccesary
		 *  @ingroup LWPanelControls
		 *  @image html paneldragbutton.jpg "A drag button control. 10x10 pixels."
		 *  How to add a AddDragButton
		 *  control = Panel.AddDragButton("Drag Button", 10, 10);
		 *  @param *title The label for the control
		 *  @param width The width in pixels of the control
		 *  @param height The height in pixels of the control */
		LWControl *AddDragButton (const char *title, int width, int height){return AddArea(title, width, height, "DragButtonControl");}
		/*! Adds a vertical drag button (mini slider) WITHOUT an edit field.
		 *  @ingroup LWPanelControls
		 *  @image html panelvdragbutton.jpg "A vertical drag button control."
		 *  How to add a AddVDragButton
		 *  control = Panel.AddVDragButton("Vertical Drag Button");
		 *  @param *title The label for the control */
		LWControl *AddVDragButton (const char *title){return AddArea(title, 0, -1, "DragButtonControl");}
		/*! Adds a horizontal drag button (mini slider) WITHOUT an edit field.
		 *  @ingroup LWPanelControls
		 *  @image html panelhdragbutton.jpg "A horizontal drag button control."
		 *  How to add a AddHDragButton
		 *  control = Panel.AddHDragButton("Horizontal Drag Button");
		 *  @param *title The label for the control */
		LWControl *AddHDragButton (const char *title){return AddArea(title, -1, 0, "DragButtonControl");}
		/*! Adds a rectagular Drag Area with a border. The area can generate drag and mouse clicks.
		 *  @ingroup LWPanelControls
		 *  @image html paneldragarea.jpg "A drag area of 75x50 pixels."
		 *  How to add a AddDragArea
		 *  control = Panel.AddDragArea("Drag Area", 75, 50);
		 *  @param *title The label for the control
		 *  @param width The width in pixels of the control
		 *  @param height The height in pixels of the control */
		LWControl *AddDragArea (const char *title, int width, int height){return AddArea(title, width, height, "DragAreaControl");}
		/*! Adds a sized rectangle with a border.
		 *  @ingroup LWPanelControls
		 *  @todo This control doesn't have a label. So title is not neccesary
		 *  @image html panelborder.jpg "A border of 75x50 pixels."
		 *  How to add a AddBorder
		 *  control = Panel.AddBorder("Border", 75, 50);
		 *  @param *title The label for the control
		 *  @param width The width in pixels of the control
		 *  @param height The height in pixels of the control */
		LWControl *AddBorder (const char *title, int width, int height){return AddArea(title, width, height, "BorderControl");}
		LWControl *AddBorder(int width, int height) { return AddArea(0, width, height, "BorderControl"); }
		/*! Adds a sized tree with the channels present on the scene.
		 *  @ingroup LWPanelControls
		 *  @image html pannelchannels.jpg "A channels tree of 125x300 pixels."
		 *  How to add a AddChannel
		 *  control = Panel.AddChannel("Channels", 125, 300);
		 *  @param *title The label for the control
		 *  @param width The width in pixels of the control
		 *  @param height The height in pixels of the control */
		LWControl *AddChannel (const char *title, int width, int height){return AddArea(title, width, height, "ChannelControl");}
		/*! Adds a sized tree with the channels present on the scene.
		 *  @ingroup LWPanelControls
		 *  @todo This control doesn't have a label. So title is not neccesary. In fact no idea what it does.
		 *  @image html pannelpalette.jpg "A palette of 125x300 pixels."
		 *  How to add a AddPalette
		 *  control = Panel.AddPalette("Palette", 125, 300);
		 *  @param *title The label for the control
		 *  @param width The width in pixels of the control
		 *  @param height The height in pixels of the control */
		LWControl *AddPalette (const char *title, int width, int height)
		{
			LWPanControlDesc desc;
			desc.type = LWT_RANGE;
			desc.area.width=width;
			desc.area.height=height;
			return addControl("PaletteControl", &desc, title);
		}
		/*! Adds a mini slider with an edit field, for values as percentages.
		 *  @ingroup LWPanelControls
		 *  @image html panelpercent.jpg "A numeric control for double values"
		 *  How to add a AddPercent
		 *  control = Panel.AddPercent("Percent Value");
		 *  @param *title The label for the control */
		LWControl *AddPercent (const char *title)
		{
			LWPanControlDesc desc;
			desc.type = LWT_RANGE;
			desc.range.min=0;
			desc.range.max=100;
			return addControl("PercentControl", &desc, title);
		}
		/*! Adds a mini slider with an edit field, for values as angles.
		 *  Internally the value is stored as Radians.
		 *  @ingroup LWPanelControls
		 *  @image html panelangle.jpg "A numeric control for double values"
		 *  How to add a AddAngle
		 *  control = Panel.AddAngle("Angle Value");
		 *  @param *title The label for the control */
		LWControl *AddAngle (const char *title)
		{
			LWPanControlDesc desc;
			desc.type = LWT_RANGE;
			desc.range.min=0;
			desc.range.max=360;
			return addControl("AngleControl", &desc, title);
		}
		/*! Adds a customizable pop up control
		 *  The (*count) and *(*name) callbacks are used to fill up the
		 *  control instead of use static text for the options.
		 *  @ingroup LWPanelControls
		 *  @image html panelcustpopupc.jpg "A customized closed pop up"
		 *  @image html panelcustpopupo.jpg "The choices when the popup is open"
		 *  How to add a AddCustomPopup
		 *	@code
			static char *choices[] = { "One", "Two", "Three", NULL };

			static int custpopupCount(void *);
			static char * custpopupName(void *data, int index);
			int yourclass::custpopupCount(void *){	return 3; }
			char * yourclass::custpopupName(void *data, int index)
			{
				if ( index >= 0 && index < 3 )
					return choices[ index ];
				else
					return NULL;
			}
			// On yourclass::Options()
			control = Panel.AddCustomPopup("Custom PopUp", 50, &yourclass::custpopupCount, &yourclass::custpopupName);
		 *	@endcode
		 *  @param *title The label for the control
		 *  @param width The width in pixels of the control
		 *  @param (*count) Pointer to it's own count function. 
		 *  @param *(*name) Pointer to it's own name function.
		*/
		LWControl *AddCustomPopup (const char *title, int width, int (*count)(void *), const char *(*name)(void *, int))
		{
			LWPanControlDesc desc;
			desc.type = LWT_POPUP;
			desc.popup.width=width;
			desc.popup.nameFn=name;
			desc.popup.countFn=count;
			return addControl("CustomPopupControl", &desc, title);
		}
		/*! Adds a customizable pop up control
		 *  The (*count) and *(*name) callbacks are used to fill up the
		 *  control instead of use static text for the options.
		 *  @ingroup LWPanelControls
		 *  @image html panelcustpopupc.jpg "A customized closed pop up"
		 *  @image html panelcustpopupo.jpg "The choices when the popup is open"
		 *  How to add a AddCustomPopup
		 *	@code
			static char *choices[] = { "One", "Two", "Three", NULL };

			static int custpopupCount(void *);
			static char * custpopupName(void *data, int index);
			int yourclass::custpopupCount(void *){	return 3; }
			char * yourclass::custpopupName(void *data, int index)
			{
				if ( index >= 0 && index < 3 )
					return choices[ index ];
				else
					return NULL;
			}
			// On yourclass::Options()
			control = Panel.AddCustomPopup<yourclass>("Custom PopUp", 50);
		 *	@endcode
		 *  @param *title The label for the control
		 *  @param width The width in pixels of the control
		*/
		template<typename T>
		LWControl *AddCustomPopup (const char *title, int width)
		{
			LWPanControlDesc desc;
			desc.type = LWT_POPUP;
			desc.popup.width=width;
			desc.popup.nameFn=PanelControlAdaptor<T, GetPanelEventHandler>::cb_Name;
			desc.popup.countFn=PanelControlAdaptor<T, GetPanelEventHandler>::cb_Count;
			PanelControl control = addControl("CustomPopupControl", &desc, title);
			control.SetUserData(control.getControl());
			return control.getControl();
		}
		/*! Adds a scrolling pop up menu control. The label will be shown on the control itself.
		 *  @ingroup LWPanelControls
		 *  @image html panelpopdownc.jpg "A closed pop down"
		 *  @image html panelcustpopupo.jpg "The choices when the popup is open"
		 *  How to add a AddPopdown
		 *	const char *choices[] = { "One", "Two", "Three", NULL };
		 *	control = Panel.AddPopdown("Pop Down", choices);
		 *  @param *title The label for the control
		 *  @param **items Pointer to a NULL terminated array for the options. 
		 *  @param orientation The orientation for the control
		*/
		LWControl *AddPopdown (const char *title, const char **items)
		{
			LWPanControlDesc desc;
			desc.type = LWT_CHOICE;
			desc.choice.vertical=MX_POPUP;
			desc.choice.items=items;
			return addControl("PopDownControl", &desc, title);
		}
		/*! Adds a series of tabs.
		 *  @ingroup LWPanelControls
		 *  @image html paneltabs.jpg "A series of tabs"
		 *  How to add a AddTabs
		 *	const char *choices[] = { "One", "Two", "Three", NULL };
		 *	control = Panel.AddTabs("Tabs", choices);
		 *  @param *title The label for the control
		 *  @param **items Pointer to a NULL terminated array for the options. 
		*/
		LWControl *AddTabs (const char *title, const char **items)
		{
			LWPanControlDesc desc;
			desc.type = LWT_CHOICE;
			desc.choice.vertical=MX_HCHOICE;
			desc.choice.items=items;
			return addControl("TabChoiceControl", &desc, title);
		}
		/*! Adds an Xpanel embeddedon the LWPanel.
		 *  @ingroup LWPanelControls
		 *  @image html panelxpanel.jpg "An Xpanel containing an read-only text field."
		 *  How to add a XPanel
		 *	@code
			// On your header(yourclass.h) file:

			class yourclass: public lwpp::LWPanelMasterHandler,
									public lwpp::XPanelInterface
			{
			public:
				// Add the XPANEL variables that define your XPanel
				static LWXPanelHint xpHint[];	
				static LWXPanelControl xpCtrl[];
				static LWXPanelDataDesc xpDesc[];

				//Add a function to your class to get the XPanel
				LWXPanelID getXPanel();

				// Constructor & Destructor
				yourclass (void *priv, void *context, LWError *err);
				~yourclass ();
			protected:
				// The member for the xpanel control
				lwpp::PanelControl control;
			};

			// On your source (yourclass.cpp) file:

			// enum for the XPanel
			enum {	ID_STRG = 0x8001 };
			
			// XPanel controls
			LWXPanelControl yourclass::xpCtrl[] =
			{
				{ID_STRG, "WARNING", "sInfo"},
				{ 0 }
			};

			// XPanel controls descriptions
			LWXPanelDataDesc yourclass::xpDesc[] =
			{
				{ID_STRG, "WARNING", "string"},
				{ 0 }
			};
			// XPanel hint
			LWXPanelHint yourclass::xpHint[] =
			{
				XpBORDER( LWXPBDR_NONE ), // No border on the XPanel control
				XpEND
			};
			// The string that appears on the control inside the XPanel
			char yourclass::startInfo[] = "No elements added yet";

			//Constructor
			yourclass::yourclass( void *priv, void *context, LWError *err) :
				lwpp::LWPanelMasterHandler(priv, context, err),
			{
			}
			// Destructor
			yourclass::~yourclass(){ ; }
			
			LWXPanelID yourclass::getXPanel()
			{
				if (!LW_XPanel.isValid())
				{
					// Create the View for the XPanel
					CreateViewXPanel(xpCtrl,xpDesc,xpHint);
				}
				return LW_XPanel.getID();
			}

			// LWPANEL CREATION
			LWError yourclass::Options ()
			{
				// Creates the panel with title
				Panel.Create("Your Panel Title Here");

				// ADD THE CONTROLS
				control = Panel.AddXPanel("Xpanel", getXPanel());

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
		 *  @param *title The label for the control
		 *  @param xp 
		*/
		LWControl *AddXPanel (const char *title, LWXPanelID xp)
		{
			LWPanControlDesc desc;
			desc.type = LWT_XPANEL;
			desc.xpanel.xpan=xp;
			return addControl("XPanelControl", &desc, title);
		}

		/*! Adds a list of items with a scroll bar.
		 *  @ingroup LWPanelControls
		 *  @param *title The label for the control as a std::string
		 *  @param width The width in pixels of the control
		 *  @param (*count) Pointer to it's own count function. 
		 *  @param *(*name) Pointer to it's own name function.
		 *  @param items Number of visible items. 
		 *  @param topVisible The index of the item visible as the first item.  */
		LWControl *AddListbox (const std::string &title, int width, int (*count)(void *), const char *(*name)(void *, int), int items, int topVisible = 0)
		{
			LWPanControlDesc desc;
			desc.type = LWT_POPUP;
			desc.listbox.width=width;
			desc.listbox.nameFn=name;
			desc.listbox.countFn=count;
			desc.listbox.visItems=items;
			desc.listbox.top = topVisible;
			return addControl("ListBoxControl", &desc, title.c_str());
		}
		/*! Adds a list of items with a scroll bar.
		 *  @ingroup LWPanelControls
		 *  @param *title The label for the control as a std::string
		 *  @param width The width in pixels of the control
		 *  @param items Number of visible items. 
		 *  @param topVisible The index of the item visible as the first item. */
		template<typename T>
		LWControl *AddListbox (const std::string &title, int width, int items, int topVisible = 0)
		{
			LWPanControlDesc desc;
			desc.type = LWT_POPUP;
			desc.listbox.width=width;
			desc.listbox.nameFn=PanelControlAdaptor<T, GetPanelEventHandler>::cb_Name;
			desc.listbox.countFn=PanelControlAdaptor<T, GetPanelEventHandler>::cb_Count;
			desc.listbox.visItems=items;
			desc.listbox.top = topVisible;
			PanelControl control = addControl("ListBoxControl", &desc, title.c_str());
			control.SetUserData(control.getControl());
			return control.getControl();
		}
		template<typename T>
		void AddListbox (PanelControl &control, const std::string &title, int width, int items, int topVisible = 0)
		{
			LWPanControlDesc desc;
			desc.type = LWT_POPUP;
			desc.listbox.width=width;
			desc.listbox.nameFn=PanelControlAdaptor<T, GetPanelEventHandler>::cb_Name;
			desc.listbox.countFn=PanelControlAdaptor<T, GetPanelEventHandler>::cb_Count;
			desc.listbox.visItems=items;
			desc.listbox.top = topVisible;
			control = addControl("ListBoxControl", &desc, title.c_str());
			control.SetUserData(control.getControl());
		}
		template<typename T>
		void AddListbox (PanelControl &control, int width, int items, int topVisible = 0)
		{
			LWPanControlDesc desc;
			desc.type = LWT_POPUP;
			desc.listbox.width=width;
			desc.listbox.nameFn=PanelControlAdaptor<T, GetPanelEventHandler>::cb_Name;
			desc.listbox.countFn=PanelControlAdaptor<T, GetPanelEventHandler>::cb_Count;
			desc.listbox.visItems=items;
			desc.listbox.top = topVisible;
			control = addControl("ListBoxControl", &desc, 0);
			control.SetUserData(control.getControl());			
		}
		/*! Adds a list of items with a scroll bar.
		 *  @ingroup LWPanelControls
		 *  @image html panellistboxnotit.jpg "A templated List Box for 5 items without title. 150 pixels width."
		 *  How to add a AddListbox (follow the  AddListbox example and in Options()
		 *	control = Panel.AddListbox<yourclass>(150, 5, 0);
		 *  @param width The width in pixels of the control
		 *  @param items Number of visible items. 
		 *  @param topVisible The index of the item visible as the first item. */
		template<typename T>
		LWControl *AddListbox (int width, int items, int topVisible = 0)
		{
			LWPanControlDesc desc;
			desc.type = LWT_POPUP;
			desc.listbox.width=width;
			desc.listbox.nameFn=PanelControlAdaptor<T, GetPanelEventHandler>::cb_Name;
			desc.listbox.countFn=PanelControlAdaptor<T, GetPanelEventHandler>::cb_Count;
			desc.listbox.visItems=items;
			desc.listbox.top = topVisible;
			PanelControl control = addControl("ListBoxControl", &desc, 0);
			control.SetUserData(control.getControl());
			return control.getControl();
		}

		/*! Adds a multi-column list of items with a scroll bar.
		 *  @ingroup LWPanelControls

		 *  @param *title The label for the control
		 *  @param width The width in pixels of the control
		 *  @param items Number of visible items. 
		 *	@param topVisible The index of the item visible as the first item.
		 * @param *count Count function, count the number of items
		 * @param *name Name function, return the name of an item/column
		 *	@param *colwidth ColumnWidth funtion. Must return the pixel width of column i, up to a maximun of 10 columns. Returning a 0 indicates is the ending colum.
		 */
		LWControl *AddMultiListbox (const char *title, int width, int items, int topVisible,
																int (*count)(void *),
																const char *(*name)(void *, int, int),
																int (*colwidth)(void *, int i))
		{
			LWPanControlDesc desc;
			desc.type = LWT_MLIST;
			desc.multiList.width=width;
			desc.multiList.nameFn=name;
			desc.multiList.countFn=count;
			desc.multiList.visItems = items;
			desc.multiList.top = topVisible;
			desc.multiList.colWidth=colwidth;
			return addControl("MultiListBoxControl", &desc, title);
		}

		//! Adds a multi-column list of items with a scroll bar.
		/*!  @ingroup LWPanelControls
		 *  @param *title The label for the control
		 *  @param width The width in pixels of the control
		 *  @param items Number of visible items. 	
		 *  @image html panelmultilistbox.jpg "A Multi List Box, 2 columns, for 5 items. 200 pixels width."
		 *	@code
			// On your header(yourclass.h) file:

			class yourclass: public lwpp::LWPanelMasterHandler,
							 public lwpp::LWPanelMultiListControlCallBacks
			{
			public:
				// CALLBACKS
				// Templated MultilistBox Callbacks
				//! MultilistBox name 
				virtual const char* controlMultiListName(lwpp::PanelControl& lwcontrol, int index, int column);
				//! MultilistBox Column widths
				virtual int controlMultiListColumnWidth(lwpp::PanelControl& lwcontrol, int colindex)
				
				//! CONTROL EVENTS
				virtual void controlEvent(lwpp::PanelControl& lwcontrol);

				//! CONTROL COUNT CALLBACK
				int controlCount(lwpp::PanelControl& lwcontrol)

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
			int yourclass::controlCount(lwpp::PanelControl& lwcontrol)
			{ 
				if (lwcontrol == control)
				{
					return 3;
				}
				return 0;
			}

						
			// MULTILIST BOX NAME CALLBACK
			const char* yourclass::controlMultiListName(lwpp::PanelControl& lwcontrol, int index, int column)
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
			int yourclass::controlMultiListColumnWidth(lwpp::PanelControl& lwcontrol, int colindex)
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

				// SET THE PANEL USER DATA
				Panel.SetUserData(this);

				// ADD THE CONTROLS
				control = Panel.AddMultiListbox<yourclass>("Multi List Box", 200, 5, 0);
				
				// SET THE CALLBACKS
				control.SetEventCallback<yourclass>();	

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
		 *	control = Panel.AddMultiListbox<yourclass>("Multi List Box", 200, 5, 0);
		 */
		template<typename T>
		LWControl *AddMultiListbox (const char *title, int width, int items, int topVisible = 0)
		{
			LWPanControlDesc desc;
			desc.type = LWT_MLIST;
			desc.multiList.width=width;
			desc.multiList.visItems=items;
			desc.multiList.top=topVisible;			
			desc.multiList.nameFn=PanelControlAdaptor<T, GetPanelEventHandler>::cb_MultiListName;
			desc.multiList.countFn=PanelControlAdaptor<T, GetPanelEventHandler>::cb_Count;
			desc.multiList.colWidth=PanelControlAdaptor<T, GetPanelEventHandler>::cb_MultiListColumnWidth;
			PanelControl control = addControl("MultiListBoxControl", &desc, title);
			control.SetUserData(control.getControl());
			return control.getControl();
		}

		template<typename T>
		void AddMultiListbox (PanelControl &control, const char *title, int width, int items, int topVisible = 0)
		{
			LWPanControlDesc desc;
			desc.type = LWT_MLIST;
			desc.multiList.width=width;
			desc.multiList.visItems=items;
			desc.multiList.top=topVisible;			
			desc.multiList.nameFn=PanelControlAdaptor<T, GetPanelEventHandler>::cb_MultiListName;
			desc.multiList.countFn=PanelControlAdaptor<T, GetPanelEventHandler>::cb_Count;
			desc.multiList.colWidth=PanelControlAdaptor<T, GetPanelEventHandler>::cb_MultiListColumnWidth;
			control = addControl("MultiListBoxControl", &desc, title);
			control.SetUserData(control.getControl());			
		}

		//! Adds a Tree Control
		template<typename T>
		LWControl *AddTree (const char *title, int width, int height)
		{
			LWPanControlDesc desc;
			desc.type = LWT_TREE;
			desc.tree.width=width;
			desc.tree.height=height;
			desc.tree.infoFn=PanelControlAdaptor<T, GetPanelEventHandler>::cb_TreeInfo;
			desc.tree.countFn=PanelControlAdaptor<T, GetPanelEventHandler>::cb_TreeCount;
			desc.tree.leafFn=PanelControlAdaptor<T, GetPanelEventHandler>::cb_TreeChild;
			desc.tree.moveFn=0;
			PanelControl control = addControl("TreeControl", &desc, title);
			control.SetUserData(control.getControl());
			return control.getControl();
		}
		//! Adds a Tree Control
		LWControl *AddTree (const char *title, int width, int height,
												int   (*count)(void *data, void *node),
												void *(*leaf)(void *data, void *node, int i),
												const char *(*info)(void *data, void *node, int *nodeFlags),
												void  (*move)(void *data, void *node, void *parentNode, int i) = 0
											 )
		{
			LWPanControlDesc desc;
			desc.type = LWT_TREE;
			desc.tree.width=width;
			desc.tree.height=height;
			desc.tree.infoFn=info;
			desc.tree.countFn=count;
			desc.tree.leafFn=leaf;
			desc.tree.moveFn=move;
			return addControl("TreeControl", &desc, title);
		}
		//@}
		// Set
		void Set (void *p, pTag tag)
		{
			set(tag, p);
		}
		void Set (int i, pTag tag)
		{
			set(tag, &i);
		}

		// Get any integer tag
		int GetInt(pTag tag) const
		{
			int i;
			get(tag, &i);
			return i;
		}
		// Get any pointer
		void *GetPointer(pTag tag)
		{
			LWValue pval;
			get(tag, &pval);
			return pval.ptr.ptr;
		}
		void Set(pTag tag, void *p)
		{
			set(tag, p);
		}
		//! Get panel width in pixels
		int GetWidth() const {return GetInt(PAN_W);}
		//! Set panel width in pixels
		void SetWidth(int w){Set(w, PAN_W);}
		//! Get panel height in pixels
		int GetHeight() const {return GetInt(PAN_H);}
		//! Set panel height in pixels
		void SetHeight(int h){Set(h, PAN_H);}
		//! Get panel width in pixels
		int GetMinWidth() const { return GetInt(PAN_MIN_W); }
		//! Set panel width in pixels
		void SetMinWidth(int w) { Set(w, PAN_MIN_W); }
		//! Get panel height in pixels
		int GetMinHeight() const { return GetInt(PAN_MIN_H); }
		//! Set panel height in pixels
		void SetMinHeight(int h) { Set(h, PAN_MIN_H); }

		//! Get panel width in pixels
		int GetMaxWidth() const { return GetInt(PAN_MAX_W); }
		//! Set panel width in pixels
		void SetMaxWidth(int w) { Set(w, PAN_MAX_W); }
		//! Get panel height in pixels
		int GetMaxHeight() const { return GetInt(PAN_MAX_H); }
		//! Set panel height in pixels
		void SetMaxHeight(int h) { Set(h, PAN_MAX_H); }
		//! Get the panel qualifier keys 
		int GetQualifiers() const {return GetInt(PAN_QUALIFIERS);}
		//! Set the panel user data
		void SetUserData (void *d){set(PAN_USERDATA, d);}
		//! Bring the panel to the front
		void ToFront() {Set(1, PAN_TO_FRONT);}
		void SetTitle(std::string s) {Set(const_cast<char*>(s.c_str()), PAN_TITLE);}
		//! Get the panel user data
		void *GetUserData ()
		{
			void *ud;
			get(PAN_USERDATA, &ud);
			return ud;
		}
		void SetUserDraw (LWPanDrawHook d){set(PAN_USERDRAW, reinterpret_cast<void*>(d));}
		void SetUserKeys (LWPanKeyHook d) {set(PAN_USERKEYS, reinterpret_cast<void*>(d));}
		void SetUserKeyUp (LWPanKeyHook d) {set(PAN_USERKEYUPS, reinterpret_cast<void*>(d));}
		void SetUserActivate (LWPanHook d) {set(PAN_USERACTIVATE, reinterpret_cast<void*>(d));}
		void SetUserOpen (LWPanHook d) {set(PAN_USEROPEN, reinterpret_cast<void*>(d));}
		void SetUserClose (LWPanHook d) {set(PAN_USERCLOSE, reinterpret_cast<void*>(d));}
		void SetUserMouseMove (LWPanMouseHook d) {set(PAN_MOUSEMOVE, reinterpret_cast<void*>(d));}
		void SetUserMouseButton (LWPanMouseHook d) {set(PAN_MOUSEBUTTON, reinterpret_cast<void*>(d));}
		void SetUserMousewheel(LWPanMouseHook d) { set(PAN_MOUSEWHEEL, reinterpret_cast<void*>(d)); }
		void SetUserResize(LWPanResizeHook d) { set(PAN_USERRESIZE, reinterpret_cast<void*>(d)); }
		void SetConfigTag(unsigned int tag) { Set(tag, PAN_CONFIGTAG); }

		template <typename T>
		void SetCallbacks(T *host)
		{
			SetUserData(host);
			SetUserDraw(cb_PanDraw<T>);
			SetUserMouseButton(cb_PanMouseButton<T>);
			SetUserMouseMove(cb_PanMouseMove<T>);
			SetUserActivate(cb_PanActivate<T>);
			SetUserOpen(cb_PanOpen<T>);
			SetUserClose(cb_PanClose<T>);
			SetUserKeys(cb_PanKeyDown<T>);
			SetUserKeyUp(cb_PanKeyUp<T>);
			SetUserResize(cb_PanResize<T>);
			SetUserMousewheel(cb_PanMousewheel<T>);
		}

		// static callbacks
		//! Callback for panel draw
		template<typename T>	static void cb_PanDraw(LWPanelID id, void *userdata, DrMode DrawMode)
		{
				if (T *instance = static_cast<T *>(userdata))
				{
					instance->panelDraw(LWPanel(id), DrawMode);
				}
		}

		//! Callback for panel activation
		template<typename T>	static void cb_PanActivate(LWPanelID id, void *userdata);

		//! Callback for open panel
		template<typename T>	static void cb_PanOpen(LWPanelID id, void *userdata);

		//! Callback for panel close
		template<typename T>	static void cb_PanClose(LWPanelID id, void *userdata);

		//! Static callback for key down
		template<typename T> static int cb_PanKeyDown (LWPanelID id, void *userdata, LWDualKey key);

		//! Static callback for key up
		template<typename T>	static int cb_PanKeyUp (LWPanelID id, void *userdata, LWDualKey key);

		//! Static callback for mouse button
		template<typename T>	static void cb_PanMouseButton (LWPanelID id, void *userdata, int qualifiers, int x, int y);

		//! Static callback for mouse move
		template<typename T>	static void cb_PanMouseMove (LWPanelID id, void *userdata, int qualifiers, int x, int y);

		template<typename T>	static void cb_PanMousewheel(LWPanelID id, void *userdata, int qualifiers, int x, int y);
		
		template<typename T>  static void cb_PanResize (LWPanelID id,  void *userdata, int w, int h);
	};

	//! @ingroup Globals
	class LWRaster : public GlobalBase<LWRasterFuncs>
	{
		LWRasterID id;
		int lastX, lastY;
	public:
		LWRaster(int w, int h, int flags = 0) : id(0)
		{
			create(w, h, flags);
		}
		LWRaster() : id(0)
		{
			;
		}
		~LWRaster()
		{
			if (id)
			{
				globPtr->destroy(id);
				id = 0;
			}
		}
		void create(int w, int h, int flags = 0)
		{
			if (id) globPtr->destroy(id);
			id = globPtr->create(w, h, flags);
		}
		void drawPixel(int c, int x, int y)
		{
			globPtr->drawPixel(id, c, x, y);
		}
		void drawPixel(int r, int g, int b, int x, int y)
		{
			globPtr->drawRGBPixel(id, r, g, b, x, y);
		}
		void moveTo(int x, int y)
		{
			lastX = x; lastY = y;
		}
		void drawTo(int c, int x, int y)
		{
			drawLine(c, lastX, lastY, x, y);
			moveTo(x,y);
		}
		void drawLine(int c, int x1, int y1, int x2, int y2)
		{
			globPtr->drawLine(id, c, x1, y1, x2, y2);
		}
		void drawBox(int c, int x, int y, int w, int h)
		{
			globPtr->drawBox(id, c, x, y, w, h);
		}
		void drawBox(int r, int g, int b, int x, int y, int w, int h)
		{
			globPtr->drawRGBBox(id, r, g, b, x, y, w, h);
		}

		void drawRoundBox(int c, int x, int y, int w, int h, int bevel = 2);
		void drawBoxOutline(int c, int x, int y, int w, int h);
		void fillRoundBox(int c, int x, int y, int w, int h, int bevel = 2);

		void eraseBox(int x, int y, int w, int h)
		{
			globPtr->eraseBox(id, x, y, w, h);
		}
		void drawBorder(int indent, int x, int y, int w, int h = 0)
		{
			globPtr->drawBorder(id, indent, x, y, w, h);
		}
		void drawText(std::string s, int c, int x, int y)
		{
			globPtr->drawText(id, const_cast<char *>(s.c_str()), c, x, y);
		}
		void blitPanel(int srcx, int srcy, LWPanelID pan, int destx, int desty, int w, int h)
		{
			globPtr->blitPanel(id, srcx, srcy, pan, destx, desty, w, h);
		}
		void blitPanel(int srcx, int srcy, LWPanel &pan, int destx, int desty, int w, int h)
		{
			blitPanel(srcx, srcy, pan.GetID(), destx, desty, w, h);
		}
		void blitRaster(LWRasterID raster, int srcx, int srcy, int destx, int desty, int w, int h)
		{
			globPtr->blitRaster(id, srcx, srcy, raster, destx, desty, w, h);
		}
		void blitRaster(LWRaster &raster, int srcx, int srcy, int destx, int desty, int w, int h)
		{
			blitRaster(raster.id, srcx, srcy, destx, desty, w, h);
		}
	};
	
	//! @ingroup LWPanels
	//! Encapsulates virtual member functions that can be overriden by the actual Plugin to process LWPanels events
	class LWPanelCallbacks
	{
			friend class LWPanel;
		public:
			//! Handle Panel redraws
			virtual void panelDraw(const LWPanel&, DrMode ) {;}
			//! Called when a panel is activated by the user
			virtual void panelActivate(const LWPanel&) {;}
			//! Called when a panel is opened
			virtual void panelOpen(const LWPanel&) {;}
			//! Called when a panel is closed
			virtual void panelClose(const LWPanel&) {;}
			//! Called when a key has been pressed
			virtual int panelKeyDown(const LWPanel&, LWDualKey ) {return 0;}
			//! Called when a key has been released
			virtual int panelKeyUp(const LWPanel&, LWDualKey ) {return 0;}
			//! Called when a mouse button has been pressed
			virtual void panelMouseButton(const LWPanel&, int , int, int) {;}
			//! Called when the mouse has been moved
			virtual void panelMouseMove(const LWPanel&, int , int, int) {;}
			//! Called when a panel is resized
			virtual void panelResize(const LWPanel&, int w, int h) { ; }
			//! Called when the mouse wheel is rotated
			virtual void panelMousewheel(const LWPanel&, int, int, int) { ; }
	};

	/*
	 * Panel Callbacks
	 */

	//! Callback for panel activation
	template<typename T>
	void LWPanel::cb_PanActivate(LWPanelID id, void *userdata)
	{
		if (T *instance = static_cast<T *>(userdata))
		{
			instance->panelActivate(LWPanel(id));
		}
	}
	//! Callback for open panel
	template<typename T>
	void LWPanel::cb_PanOpen(LWPanelID id, void *userdata)
	{
		if (T *instance = static_cast<T *>(userdata))
		{
			instance->panelOpen(LWPanel(id));
		}
	}
	//! Callback for panel close
	template<typename T>
	void LWPanel::cb_PanClose(LWPanelID id, void *userdata)
	{
		if ( !userdata ) return;
		if (T *instance = static_cast<T *>(userdata))
		{
			instance->panelClose(LWPanel(id));
		}
	}
	//! Static callback for key down
	template<typename T>
	int LWPanel::cb_PanKeyDown (LWPanelID id, void *userdata, LWDualKey key)
	{
			if (T *instance = static_cast<T *>(userdata))
			{
				return instance->panelKeyDown(LWPanel(id), key);
			}
			return 0; // The keypress has not been processed
	}
	//! Callback for panel key up
	template<typename T>
	int LWPanel::cb_PanKeyUp (LWPanelID id, void *userdata, LWDualKey key)
	{
			if (T *instance = static_cast<T *>(userdata))
			{
				return instance->panelKeyUp(LWPanel(id),key);
			}
			return 0; // The keypress has not been processed
	}
	
	//! Callback for panel mouse button
	template<typename T>
	void LWPanel::cb_PanMouseButton (LWPanelID id, void *userdata, int qualifiers, int x, int y)
	{
			if (T *instance = static_cast<T *>(userdata))
			{
				return instance->panelMouseButton(LWPanel(id), qualifiers, x, y);
			}
	}
	//! Callback for panel mouse move
	template<typename T>
	void LWPanel::cb_PanMouseMove (LWPanelID id, void *userdata, int qualifiers, int x, int y)
	{
			if (T *instance = static_cast<T *>(userdata))
			{
				instance->panelMouseMove(LWPanel(id), qualifiers, x, y);
			}
	}
	//! Callback for panel resize
	template<typename T>
	void LWPanel::cb_PanResize (LWPanelID id,  void *userdata, int w, int h)
	{
			if (T *instance = static_cast<T *>(userdata))
			{
				instance->panelResize(LWPanel(id), w, h);
			}
	}
	//! Callback for mouse wheel
	template<typename T>
	void LWPanel::cb_PanMousewheel(LWPanelID id, void *userdata, int qualifiers, int x, int y)
	{
		if(T *instance = static_cast<T *>(userdata))
		{
			instance->panelMousewheel(LWPanel(id), qualifiers, x, y);
		}
	}


} // namespace lwpp

#endif // LWPP_PANEL_H
