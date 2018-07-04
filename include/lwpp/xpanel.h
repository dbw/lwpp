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
		lwpp::dostream dout;
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
		void clear (int r, int g, int b) const
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
			if (lwxpanel) globPtr->destroy(lwxpanel);
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

		void setUserData(void *data, unsigned int id = 0)
		{
			globPtr->setData(lwxpanel, id, data);
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
		auto plugin = xpan.getUserData<T *>(cid);
		if (plugin) return plugin->ButtonClick(cid);
		else
		{
			auto plugin = xpan.getUserData<T *>();
			if (plugin) return plugin->ButtonClick(cid);
		}
	}

	template <typename T>
	void LWXPanelPopCmdFunc(LWXPanelID panel, int cid, int cmdid)
	{
		lwpp::XPanel xpan(panel);
		auto plugin = xpan.getUserData<T *>(cid);
		if (plugin) return plugin->PopCommand(cid, cmdid);
		else
		{
			auto plugin = xpan.getUserData<T *>();
			if (plugin) return plugin->PopCommand(cid, cmdid);
		}
	}

	template <typename T>
	void LWXPanelControlDrawFunc(LWXPanelID panel, unsigned int cid, LWXPDrAreaID reg, int w, int h)
	{
		lwpp::XPanel xpan(panel);
		auto plugin = xpan.getUserData<T *>(cid);
		XPDrawArea area(reg, xpan.getDrawFuncs(), w, h);
		if (plugin) plugin->ControlDraw(cid, area);
		else
		{
			auto plugin = xpan.getUserData<T *>();
			if (plugin) plugin->ControlDraw(cid, area);
		}
	}
}
#endif // LWPP_XPANEL_H
