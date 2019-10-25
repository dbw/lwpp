/*!
 * @file
 * @brief Wrapper for LWXPanelFuncs
 */
#ifndef LWPP_XPANEL_H
#define LWPP_XPANEL_H
#include <lwxpanel.h>
#include <lwpp/global.h>

namespace lwpp
{
//! defines for the various XPanel controls
#define XPC_STRING "string"
#define XPC_INTEGER "integer"
#define XPC_FLOAT "float"
#define XPC_BUTTON "vButton"
#define XPC_BOOLEAN "iBoolean"
#define XPC_SLIDER "iSlider"
#define XPC_SLIDER_TEXT "iSliderText"
#define XPC_CHOICE "iChoice"
#define XPC_AXIS "axis"
#define XPC_POPCHOICE "iPopChoice"
#define XPC_POPCMD "vPopCmd"
#define XPC_PRESET_TEST "vPresetText"
#define XPC_FILENAME "sFileName"
#define XPC_CHOICE_XF "iChoTransform"
#define XPC_POPCHOICE_XF "iPopTransform"
#define XPC_THUMBNAIL "dThumbnail"
#define XPC_INFO "sInfo"
#define XPC_SHEET "sheet"

	//! various XPanel Data types for float numbers
#define XPC_FLOAT3 "float3"
#define XPC_FLOAT_ENV "float-env"
#define XPC_FLOAT3_ENV "float3-env"

#define XPC_DISTANCE "distance"
#define XPC_DISTANCE3 "distance3"
#define XPC_DISTANCE_ENV "distance-env"
#define XPC_DISTANCE3_ENV "distance3-env"

#define XPC_PERCENT "percent"
#define XPC_PERCENT3 "percent3"
#define XPC_PERCENT_ENV "percent-env"
#define XPC_PERCENT3_ENV "percent3-env"

#define XPC_ANGLE "angle"
#define XPC_ANGLE3 "angle3"
#define XPC_ANGLE_ENV "angle-env"
#define XPC_ANGLE3_ENV "angle3-env"

#define XPC_COLOUR "color"
#define XPC_COLOUR_ENV "color-env"

#define XPC_COLOR XPC_COLOUR
#define XPC_COLOR_ENV XPC_COLOUR_ENV

#define XPC_TIME "time"
#define XPC_TIME_ENV "time-env"


  //! Use any RGB value as a LW pen colour
  #define LW_COLOUR(red, green, blue) (0x01000000 | (red<<16) | (blue<<8) | green)
DEFINE_GLOBAL(LWXPanelFuncs)
 
	//! Wrapper for LWXPanelFuncs
	class XPanelFuncs : public GlobalBase<LWXPanelFuncs>
	{
	public:
		XPanelFuncs()
		{
			;
		}
		static LWXPanelID Create (int type, LWXPanelControl *ctrlDesc = 0)
		{
			return globPtr->create(type, ctrlDesc);
		}
		void Destroy (LWXPanelID lwxpanel)
		{
			if (lwxpanel)  globPtr->destroy(lwxpanel);
		}
		static void Describe(LWXPanelID lwxpanel, LWXPanelDataDesc *ddesc, LWXPanelGetFunc *getf = 0, LWXPanelSetFunc *setf = 0)
		{
			globPtr->describe(lwxpanel, ddesc, getf, setf);
		}
		static void Hint(LWXPanelID lwxpanel,LWXPanelHint *hint, unsigned int ctl = 0)
		{
			globPtr->hint(lwxpanel, ctl, hint);
		}
		static int Post(LWXPanelID lwxpanel)
		{
#ifdef _DEBUG
		std::hex(dout);
		dout << "Opening XPanel: " << lwxpanel << " (Post)\n";	
#endif
			return globPtr->post(lwxpanel);
		}
		static int Open(LWXPanelID lwxpanel)
		{
			return globPtr->open(lwxpanel);
		}
		static void ViewInst(LWXPanelID lwxpanel,void *inst)
		{
			globPtr->viewInst(lwxpanel, inst);
		}
		static void ViewRefresh(LWXPanelID lwxpanel)
		{
			globPtr->viewRefresh(lwxpanel);
		}
		// user data
		//! Get user data
		/*!
		 * @param lwxpanel LWXPanelID
		 * @param mID ID of the control or 0 for the panel
		 * @return The user data
		 */
		static void *getUserData(LWXPanelID lwxpanel,unsigned int id = 0)
		{
			return globPtr->getData(lwxpanel, id);
		}
		//! Set user data
		/*!
		 * @param lwxpanel LWXPanelID
		 * @param *data The user data
		 * @param mID ID of the control or 0 for the panel
		 */
		static void setUserData(LWXPanelID lwxpanel, void *data, unsigned int id = 0)
		{
			globPtr->setData(lwxpanel, id, data);
		}

		// Forms
		static void *formGet(LWXPanelID lwxpanel,unsigned int id)
		{
			return globPtr->formGet(lwxpanel, id);
		}
		static char *getFormString (LWXPanelID lwxpanel,unsigned int id)
		{
			return (char *) formGet(lwxpanel, id);
		}
		static int getFormInt (LWXPanelID lwxpanel,unsigned int id)
		{
			return *((int *) formGet(lwxpanel, id));
		}
		static float getFormFloat (LWXPanelID lwxpanel,unsigned int id)
		{
			return *((float *) formGet(lwxpanel, id));
		}
		static void formSet(LWXPanelID lwxpanel,unsigned int id, void* data)
		{
			globPtr->formSet(lwxpanel, id, data);
		}
		static void setForm(LWXPanelID lwxpanel,unsigned int id, int data)
		{
			formSet(lwxpanel, id, &data);
		}
		static void setForm(LWXPanelID lwxpanel,unsigned int id, float data)
		{
			formSet(lwxpanel, id, &data);
		}
		static void setForm(LWXPanelID lwxpanel,unsigned int id, char *data)
		{
			formSet(lwxpanel, id, (void *)data);
		}
		static void setForm(LWXPanelID lwxpanel,unsigned int id, const char *data)
		{
			formSet(lwxpanel, id, (void *)data);
		}
		static int getVersion() 
		{
			return globPtr->version;
		}
		static LWXPDrawFuncs *getDrawFuncs() 
		{
			return globPtr->drawf;
		}
	};

	class XPDrawArea
	{
	protected:
		LWXPDrAreaID mAreaId;
		LWXPDrawFuncs *mDrawFuncs;
		int mWidth, mHeight;
	public:
		XPDrawArea(LWXPDrAreaID _id, int w, int h) 
			: mAreaId(_id), mWidth(w), mHeight(h)
		{
			XPanelFuncs xpf;
			mDrawFuncs = xpf.getDrawFuncs();
		}
		XPDrawArea(LWXPDrAreaID _id, LWXPDrawFuncs *_df, int w, int h)
			: mAreaId(_id), mDrawFuncs(_df), mWidth(w), mHeight(h)
		{
			;
		}

		int getWidth() const {return mWidth;}
		int getHeight() const {return mHeight;}

		void drawPixel (int c, int x, int y ) const
		{
			mDrawFuncs->drawPixel(mAreaId, c, x, y);
		}
		void drawPixel (int r, int g, int b, int x, int y ) const
		{
			mDrawFuncs->drawRGBPixel(mAreaId, r, g, b, x, y);
		}
		void drawPixel (float r, float g, float b, int x, int y ) const
		{
			mDrawFuncs->drawRGBPixel(mAreaId, (int)(r * 255.0f), (int)(g * 255.0f), (int)(b * 255.0f), x, y);
		}
    void drawPixel(float rgb[3], int x, int y) const
    {
      drawPixel( rgb[0], rgb[1], rgb[2], x, y);
    }
		void drawLine (int c, int x, int y, int x2, int y2 ) const
		{
			mDrawFuncs->drawLine(mAreaId, c, x, y, x2, y2);
		}
		void drawBox (int c, int x, int y, int w, int h ) const
		{
			mDrawFuncs->drawBox(mAreaId, c, x, y, w, h);
		}
		void drawBox (int r, int g, int b, int x, int y, int w, int h ) const
		{
			mDrawFuncs->drawRGBBox(mAreaId, r, g, b, x, y, w, h);
		}
		void clear (int r = 0, int g = 0, int b = 0) const
		{
			drawBox(r, g, b, 0, 0, getWidth(), getHeight());
		}
		void clear (int c) const
		{
			drawBox(c, 0, 0, getWidth(), getHeight());
		}
		void drawBorder (int indent, int x, int y, int w, int h ) const
		{
			mDrawFuncs->drawBorder(mAreaId, indent, x, y, w, h);
		}

		int textWidth (const char *s ) const
		{
			return mDrawFuncs->textWidth(mAreaId, const_cast<char *>(s));
		}
		int textHeight (const char *s ) const
		{
			return mDrawFuncs->textHeight(mAreaId, const_cast<char *>(s));
		}
		void drawText (const char *s, int c, int x, int y) const
		{
			mDrawFuncs->drawText(mAreaId, const_cast<char *>(s), c, x, y);
		}
		LWXPDrAreaID getID() {return mAreaId;}
	};

	//! Wrapper for a LWXPanel
	class XPanel : protected XPanelFuncs
	{
	private:
    LWXPanelID lwxpanel = nullptr;
		bool destroy_on_exit = false;
	protected:
		void formSet(unsigned int id, void* data)
		{
			globPtr->formSet(lwxpanel, id, data);
		}
		void *formGet(unsigned int id)
		{
			return globPtr->formGet(lwxpanel, id);
		}
	public:
		void Create (int type, LWXPanelControl *ctrlDesc = nullptr, bool auto_close = true)
		{
			lwxpanel = globPtr->create(type, ctrlDesc);
			destroy_on_exit = auto_close;
		}
		void Destroy ()
		{
			if (destroy_on_exit && lwxpanel) globPtr->destroy(lwxpanel);
			lwxpanel = 0;
		}
		//! Initialize the class from a created panel
		/*!
		 * @note This will <b>not</b> destroy the panel if the class is destroyed by default
		 */
		XPanel(LWXPanelID panID = 0, bool auto_close = false) : lwxpanel(panID), destroy_on_exit(auto_close)
		{
			;
		}
		//! Initialize the class by creating a panel
		/*!
		 * @note This <b>will</b> destroy the panel if the class is destroyed by default
		 */
		XPanel(int type, LWXPanelControl *ctrlDesc = nullptr, bool auto_close = true) : lwxpanel(0), destroy_on_exit(auto_close)
		{
			Create(type, ctrlDesc, auto_close);
		}
		~XPanel()
		{
			LWPP_DBG_ENTER_FUNC;
		  if (destroy_on_exit) Destroy();
		}
		void DestroyOnExit(const bool destroy)
		{
			destroy_on_exit = destroy;
		}
		bool isValid()
		{
			return (lwxpanel != 0);
		}
		void Describe(LWXPanelDataDesc *ddesc = nullptr, LWXPanelGetFunc *getf = 0, LWXPanelSetFunc *setf = 0)
		{
			if (lwxpanel)	globPtr->describe(lwxpanel, ddesc, getf, setf);
		}
		void Hint(LWXPanelHint *hint, unsigned int ctl = 0)
		{
			if (lwxpanel)	globPtr->hint(lwxpanel, ctl, hint);
		}
		LWXPanelID getID() const
		{
			return lwxpanel;
		}
		void setID(LWXPanelID id)
		{
			lwxpanel = id;
		}
		int Post()
		{
#ifdef _DEBUG
		//lwpp::dostream dout;
		//std::hex(dout);
		//dout << "Opening XPanel: " << lwxpanel << " (Post)\n";	
#endif
			return globPtr->post(lwxpanel);
		}
		int Open()
		{
			return globPtr->open(lwxpanel);
		}
		void ViewInst(void *inst)
		{
			globPtr->viewInst(lwxpanel, inst);
		}
		void ViewRefresh()
		{
			if (lwxpanel) globPtr->viewRefresh(lwxpanel);
		}
		void ViewRebuild()
		{
			if (lwxpanel) globPtr->viewRebuild(lwxpanel);
		}
		static void ViewRefresh(LWXPanelID panel)
		{
			if (panel) globPtr->viewRefresh(panel);
		}
		void setAutoDestroy(bool close)
		{
			destroy_on_exit = close;
		}

		void DestroyNotify()
		{
			destroy_on_exit = false;
			setID(0);
		}
		// user data
		void *getUserData(unsigned int id = 0)
		{
			return globPtr->getData(lwxpanel, id);
		}
		template <typename T>
		T getUserData(unsigned int id = 0)
		{
			return static_cast<T >(globPtr->getData(lwxpanel, id));
		}

		template <typename T>
		T getFallbackUserData(unsigned int id)
		{
			auto ret = static_cast<T>(globPtr->getData(lwxpanel, id));
			if (ret != nullptr) return ret;

			return static_cast<T>(globPtr->getData(lwxpanel, 0));			
		}

		void setUserData(void *data, unsigned int id = 0)
		{
			globPtr->setData(lwxpanel, id, data);
		}
        
    void setUserData(const char* data, unsigned int id = 0)
    {
      setUserData((void*)data, id);
    }

		void clearUserData(unsigned int id = 0)
		{
			setUserData((void *)nullptr, id);
		}
        
		// Forms

		inline void getForm (unsigned int id, void *&d)
		{
			d = formGet(id);
		}
		inline void getForm (unsigned int id, char *&d)
		{
			d = (char *) formGet(id);
		}
		inline void getForm (unsigned int id, std::string &d)
		{
			d = (char *) formGet(id);
		}
		inline void getForm (unsigned int id, int &d)
		{
			int *i = (int *) formGet(id);
			d = *i;
		}
		inline void getForm (unsigned int id, unsigned int &d)
		{
			d = *((unsigned int *) formGet(id));
		}
		inline void getForm (unsigned int id, bool &d)
		{
			int l = *((int *) formGet(id));
			d = (l != 0);
		}
		inline void getForm (unsigned int id, float &d)
		{
			d = *((float *) formGet(id));
		}
		inline void getForm (unsigned int id, double &d)
		{
			d = *((double *) formGet(id));
		}
		// Set
		void setForm(unsigned int id, void *data)
		{
			formSet(id, data);
		}
		void setForm(unsigned int id, bool data)
		{
			int l = static_cast<int>(data);
			formSet(id, &l);
		}
		void setForm(unsigned int id, int data)
		{
			formSet(id, &data);
		}
		void setForm(unsigned int id, unsigned int data)
		{
			formSet(id, &data);
		}
		void setForm(unsigned int id, float data)
		{
			formSet(id, &data);
		}
		void setForm(unsigned int id, double data)
		{
			formSet(id, &data);
		}
		void setForm(unsigned int id, char *data)
		{
			formSet(id, (void *)data);
		}
		void setForm(unsigned int id, const char *data)
		{
			formSet(id, (void *)data);
		}
		void setForm(unsigned int id, std::string data)
		{
			formSet(id, (void *)data.c_str());
		}

		static int getVersion() 
		{
			return globPtr->version;
		}
		static LWXPDrawFuncs *getDrawFuncs() 
		{
			return globPtr->drawf;
		}
		static void DestroyNotifyCB(void *panData)
		{
			if (panData)
			{
				auto panel = static_cast<XPanel *>(panData);
				panel->DestroyNotify();
			}
		}
	}; 

		//! Base for classes that can handle XPanel view data
	class XPanelView
	{
		public:
			//! Put data into a XPanel View
			virtual void *DataGet(unsigned int vid ) = 0;
			//! Retrieve data fom a XPanel View
			virtual LWXPRefreshCode DataSet(unsigned int vid, void *value) = 0;
			virtual void ChangeNotify(LWXPanelID, unsigned int, unsigned int, int) {}
			virtual void ButtonClick(unsigned int) {}
			virtual void PopCommand(int cid, int cmdid) {}
	};

	// Template functions for callbacks

	template <typename T>
	void LWXPanelBtnClickFunc (LWXPanelID panel, int cid)
	{
		lwpp::XPanel xpan(panel);
		auto plugin = xpan.getFallbackUserData<T *>(cid);
		if (plugin) plugin->ButtonClick(cid);
	}

	template <typename T>
	void LWXPanelPopCmdFunc(LWXPanelID panel, int cid, int cmdid)
	{
		lwpp::XPanel xpan(panel);
		auto plugin = xpan.getFallbackUserData<T *>(cid);
		if (plugin) plugin->PopCommand(cid, cmdid);
	}

	template <typename T>
	void LWXPanelControlDrawFunc(LWXPanelID panel, unsigned int cid, LWXPDrAreaID reg, int w, int h)
	{
		lwpp::XPanel xpan(panel);
		auto plugin = xpan.getFallbackUserData<T *>(cid);		
		if (plugin)
		{
			XPDrawArea area(reg, xpan.getDrawFuncs(), w, h);
			plugin->ControlDraw(cid, area);
		}
	}

	namespace LW2019
	{
		typedef struct st_LWXPanelSheetColumnInfo
		{
			const char *title;
			float width;
			int justification;
			int valueIndex;
		} LWXPanelSheetColumnInfo;

		typedef struct st_LWXPanelSheetLayoutInfo
		{
			LWXPanelSheetColumnInfo *column_info_array;
			float row_height;
			int rows_visible;
		} LWXPanelSheetLayoutInfo, *LWXPanelSheetLayoutInfoRef;

		typedef struct st_LWXPanelSheetFuncs
		{
			int version; // current version is: 2

			// fill the column description for a specific index
			void(*getColumn)(LWXPanelID pan, unsigned int cid, int column_index, LWXPanelSheetColumn *column); // 1+

			// return the text to display for a given item and column. (NULL item (-1 index) is the header item).
			/* Justification is specified in the row/column text */
			const char * /* rich text utf-8 */ (*getText)(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item, int column_index); // 1+

			// Is the item expanded. (n/a for title item)
			unsigned int(*getIsExpanded)(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item); // 1+

			// return the number of sub-items of a given item.
			unsigned int(*getItemCount)(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item); // 1+

			// return the sub-item given its parent item and child index.
			LWXPanelSheetItemRef(*getSubitem)(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef parent_item, int child_index); // 1+

			// return the type of item for a given parent and child index.
			unsigned int(*getSubtype)(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item, int child_index); // 1+

			// respond to an item changing its parent and/or relative child index.
			void(*handleMove)(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item, LWXPanelSheetItemRef new_parent_item, int new_child_index); // 1+

			// set the expand state for an item (0:collpsed 1:expanded)
			void(*handleExpandState)(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item, int expanded); // 1+

			// respond to a change in the selection state of an item
			// (0:not selected; 1:selected; 2:toggled)
			int(*handleSelectItem)(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item, int state); // 1+

			// respond to the start of multi-selection sequence
			void(*handleSelectStart)(LWXPanelID pan, unsigned int cid, int state); // 1+

			// respond to the end of a multi-selection sequence
			void(*handleSelectEnd)(LWXPanelID pan, unsigned int cid); // 1+

			// respond to the vertical scroll
			void(*handleVScroll)(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef top_item); // 1+

			// respond to user input (events) occurring for a specific item and column index
			// header uses item of NULL.
			// return 1 if the input resulted in changes.
			int(*handleEvent)(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item, int column_index, LWXPanelSheetEvent *event); // 1+

			// respond to a new primary item being selected
			void(*handleSelectPrimaryItem)(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item); // 2+

			// respond to sheet layout changes
			void(*handleLayout)(LWXPanelID pan, unsigned int cid, LWXPanelSheetLayoutInfoRef layout); // 2+

		} LWXPanelSheetFuncs;
	}

	template <typename T>
	class xSheetClient
	{
		typedef T *itemRef;
	public:
		virtual void getSheetColumn(unsigned int cid, int column_index, LWXPanelSheetColumn *column) {}
		virtual const char * /* rich text utf-8 */ getSheetText(unsigned int cid, itemRef item, int column_index) { return ""; }
		virtual bool getSheetIsExpanded(unsigned int cid, itemRef item) { return false; }
		virtual unsigned int getSheetItemCount(unsigned int cid, itemRef item) { return 0; }
		virtual itemRef getSheetSubitem(unsigned int cid, itemRef parent_item, int child_index) { return nullptr; }
		virtual unsigned int getSheetSubtype(unsigned int cid, itemRef item, int child_index) { return 0; }
		virtual void handleSheetMove(unsigned int cid, itemRef item, itemRef new_parent_item, int new_child_index) {}
		virtual void handleSheetExpandState(unsigned int cid, itemRef item, bool expanded) {}
		// (0:not selected; 1:selected; 2:toggled)
		virtual int handleSheetSelectItem(unsigned int cid, itemRef item, int state) { return 0; }
		virtual void handleSheetSelectStart(unsigned int cid, int state) {}
		virtual void handleSheetSelectEnd(unsigned int cid) {}
		virtual void handleSheetVScroll(unsigned int cid, itemRef top_item) {}
		// return true if a change occured
		virtual bool handleSheetEvent(unsigned int cid, itemRef item, int column_index, LWXPanelSheetEvent *event) { return 0; }
		virtual void handleSelectPrimaryItem(unsigned int cid, LWXPanelSheetItemRef item) { return; }
		virtual void handleLayout(unsigned int cid, LW2019::LWXPanelSheetLayoutInfoRef layout) { return; }
	};

 	template<typename T, typename S>
	class XSheetServer
	{
		static T *getHost(LWXPanelID pan, unsigned int cid)
		{
			lwpp::XPanel xpan(pan);
			auto host = xpan.getUserData<T *>(cid);
			if (host) return host;
			return xpan.getUserData<T *>();
		}
		static void getColumn_CB(LWXPanelID pan, unsigned int cid, int column_index, LWXPanelSheetColumn *column)
		{
      LWPP_DBG_ENTER_FUNC;
			if (auto plugin = getHost(pan, cid))
				plugin->getSheetColumn(cid, column_index, column);			
		}

		static const char * getText_CB(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item, int column_index)
		{
      LWPP_DBG_ENTER_FUNC;
			if (auto plugin = getHost(pan, cid))
			{
				return plugin->getSheetText(cid, static_cast<S *>(item), column_index);
			}
			return "";
		}

		static unsigned int getIsExpanded_CB(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item)
		{
      LWPP_DBG_ENTER_FUNC;
			if (auto plugin = getHost(pan, cid))
			{
				return plugin->getSheetIsExpanded(cid, static_cast<S *>(item));
			}
			return 0;
		}

		static unsigned int getItemCount_CB(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item)
		{
      LWPP_DBG_ENTER_FUNC;
			if (auto plugin = getHost(pan, cid))
			{
				return plugin->getSheetItemCount(cid, static_cast<S *>(item));
			}
			return 0;
		}

		static LWXPanelSheetItemRef getSubitem_CB (LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef parent_item, int child_index)
		{
      LWPP_DBG_ENTER_FUNC;
			if (auto plugin = getHost(pan, cid))
			{
				return plugin->getSheetSubitem(cid, static_cast<S *>(parent_item), child_index);
			}
			return nullptr;
		}

		static unsigned int getSubtype_CB(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item, int child_index)
		{
      LWPP_DBG_ENTER_FUNC;
			if (auto plugin = getHost(pan, cid))
			{
				return plugin->getSheetSubtype(cid, static_cast<S *>(item), child_index);
			}
			return 0;
		}

		static void handleMove_CB(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item, LWXPanelSheetItemRef new_parent_item, int new_child_index)
		{
      LWPP_DBG_ENTER_FUNC;
			if (auto plugin = getHost(pan, cid))			
				 plugin->handleSheetMove(cid, static_cast<S *>(item), static_cast<S *>(new_parent_item), new_child_index);
		}

		static void handleExpandState_CB(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item, int expanded)
		{
      LWPP_DBG_ENTER_FUNC;
			if (auto plugin = getHost(pan, cid))
				plugin->handleSheetExpandState(cid, static_cast<S *>(item), (expanded == 1));
		}

		static int handleSelectItem_CB(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item, int state)
		{
      LWPP_DBG_ENTER_FUNC;
			if (auto plugin = getHost(pan, cid))
			{
				return plugin->handleSheetSelectItem(cid, static_cast<S *>(item), state);
			}
			return 0;
		}

		static void handleSelectStart_CB(LWXPanelID pan, unsigned int cid, int state)
		{
      LWPP_DBG_ENTER_FUNC;
			if (auto plugin = getHost(pan, cid))
				plugin->handleSheetSelectStart(cid, state);
		}

		static void handleSelectEnd_CB(LWXPanelID pan, unsigned int cid)
		{
      LWPP_DBG_ENTER_FUNC;
			if (auto plugin = getHost(pan, cid))
				plugin->handleSheetSelectEnd(cid);
		}

		static void handleSelectVScroll_CB(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef top_item)
		{
      LWPP_DBG_ENTER_FUNC;
			if (auto plugin = getHost(pan, cid))
				plugin->handleSheetVScroll(cid, static_cast<S *>(top_item));
		}

		static int handleEvent_CB(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item, int column_index, LWXPanelSheetEvent *event)
		{
      LWPP_DBG_ENTER_FUNC;
			if (auto plugin = getHost(pan, cid))
			{
				return plugin->handleSheetEvent(cid, static_cast<S *>(item), column_index, event);
			}
			return 0;
		}

		static void handleSelectPrimaryItem_CB(LWXPanelID pan, unsigned int cid, LWXPanelSheetItemRef item)
		{
      LWPP_DBG_ENTER_FUNC;
			if (auto plugin = getHost(pan, cid))			
				plugin->handleSelectPrimaryItem(cid, static_cast<S *>(item));
		}

		// respond to sheet layout changes
		static void handleLayout_CB(LWXPanelID pan, unsigned int cid, LW2019::LWXPanelSheetLayoutInfoRef layout)
		{
      LWPP_DBG_ENTER_FUNC;
			if (auto plugin = getHost(pan, cid))
				plugin->handleLayout(cid, layout);
		}
    
    static LW2019::LWXPanelSheetFuncs sheetFuncs; // this needs to be static to be used in static XPanel Hints
  
public:
		void *getSheetFuncs()
    { 
      LWPP_DBG_ENTER_FUNC;
      sheetFuncs.version = 1;
      sheetFuncs.getColumn = getColumn_CB;
      sheetFuncs.getText = getText_CB;
      sheetFuncs.getIsExpanded = getIsExpanded_CB;
      sheetFuncs.getItemCount = getItemCount_CB;
      sheetFuncs.getSubitem = getSubitem_CB;
      sheetFuncs.getSubtype = getSubtype_CB;
      sheetFuncs.handleMove = handleMove_CB;
      sheetFuncs.handleExpandState = handleExpandState_CB;
      sheetFuncs.handleSelectItem = handleSelectItem_CB;
      sheetFuncs.handleSelectStart = handleSelectStart_CB;
      sheetFuncs.handleSelectEnd = handleSelectEnd_CB;
      sheetFuncs.handleVScroll = handleSelectVScroll_CB;
      sheetFuncs.handleEvent = handleEvent_CB;
      sheetFuncs.handleSelectPrimaryItem = handleSelectPrimaryItem_CB;
      sheetFuncs.handleLayout = handleLayout_CB;
			if (lwpp::LightWave::isAtLeastYear(2019, 0)) sheetFuncs.version = 2;
      dout << "sheetFuncs: " << &sheetFuncs << "\n";
			return &sheetFuncs; 
		}
	};  

  template<typename T, typename S>
  LW2019::LWXPanelSheetFuncs XSheetServer<T, S>::sheetFuncs;

}
#endif // LWPP_XPANEL_H
