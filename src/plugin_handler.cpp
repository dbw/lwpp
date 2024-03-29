#include <lwpp/plugin_handler.h>

namespace lwpp 
{

	void XPanelInterface::LWXPanelPopCmdFunc (LWXPanelID panel, int cid, int cmd)
	{
		XPanel xpan(panel);
		XPanelInterface *plugin = static_cast<XPanelInterface *>(xpan.getUserData());
		if (plugin)
			plugin->PopCommand(cid, cmd);
	}

	void XPanelInterface::LWXPanelControlDrawFunc ( LWXPanelID panel, unsigned int cid, LWXPDrAreaID reg, int w, int h )
	{
		XPanel xpan(panel);
		XPanelInterface *plugin = static_cast<XPanelInterface *>(xpan.getUserData());
		XPDrawArea area(reg, xpan.getDrawFuncs(), w, h);
		if (plugin)
			plugin->ControlDraw(cid, area);
	}
	void XPanelInterface::LWXPanelControlZoomFunc ( LWXPanelID panel, unsigned int cid, int x, int y, int *region, int clickcount )
	{
		XPanel xpan(panel);
		XPanelInterface *plugin = static_cast<XPanelInterface *>(xpan.getUserData(cid));
		if (plugin)
			plugin->ControlZoom(cid, x, y, region, clickcount);
	}

	void XPanelInterface::LWXPanelZoomFunc(LWXPanelID panel, unsigned int cid, int x, int y, int* region, int clickcount)
	{
		XPanel xpan(panel);
		XPanelInterface* plugin = static_cast<XPanelInterface*>(xpan.getUserData());
		if (plugin)
			plugin->ControlZoom(cid, x, y, region, clickcount);
	}

	//! Put data into a XPanel View
	void *XPanelInterface::CB_DataGet(LWInstance inst, unsigned int vid ) 
	{
		XPanelInterface *plugin = static_cast<XPanelInterface *>(inst);
		if (plugin)
			return plugin->DataGet(vid);
		return 0;
	}
	//! Retrieve data fom a XPanel View
	LWXPRefreshCode XPanelInterface::CB_DataSet(LWInstance inst,unsigned int vid, void *value)
	{
		XPanelInterface *plugin = static_cast<XPanelInterface *>(inst);
		if (plugin)
			return plugin->DataSet(vid, value);
		return LWXPRC_NONE;
	}
	//! Callback for XPanel button Clicks
	void XPanelInterface::LWXPanelBtnClickFunc (LWXPanelID panel, int cid)
	{
		XPanel xpan(panel);
		XPanelInterface *plugin = static_cast<XPanelInterface *>(xpan.getUserData());
		if (plugin)
			plugin->ButtonClick(cid);
	}

	LWXPRefreshCode XPanelInterface::LWXPanelChangeNotifyFunc (LWXPanelID panel, unsigned int cid, unsigned int vid, int event_type)
	{
		XPanel xpan(panel);
		XPanelInterface *plugin = static_cast<XPanelInterface *>(xpan.getUserData());
		if (plugin) 
			return plugin->ChangeNotify(panel, cid, vid, event_type);
		return LWXPRC_NONE;
	}

	void XPanelInterface::LWXPanelDestroyNotifyFunc(void *data)
	{
		XPanelInterface *plugin = static_cast<XPanelInterface *>(data);
		if (plugin) 
			plugin->PanelDestroyNotify();
	}

	void XPanelInterface::CreateViewXPanel(lwpp::DynamicControlData &controls, lwpp::DynamicHints &hints, bool do_destroy)
	{
		CreateViewXPanel(this, controls.getControl(), controls.getDataDesc(), hints.get(), do_destroy);
	}
	void XPanelInterface::CreateViewXPanel(void *host, lwpp::DynamicControlData &controls, lwpp::DynamicHints &hints, bool do_destroy)
	{
		CreateViewXPanel(host, controls.getControl(), controls.getDataDesc(), hints.get(), do_destroy);
	}
	void XPanelInterface::CreateViewXPanel(LWXPanelControl *controls, LWXPanelDataDesc *desc, LWXPanelHint *hints, bool do_destroy)
	{
		CreateViewXPanel(this, controls, desc, hints, do_destroy);
	}
	void XPanelInterface::CreateViewXPanel(void *host, LWXPanelControl *controls, LWXPanelDataDesc *desc, LWXPanelHint *hints, bool do_destroy)
	{
		GetXPanel().Create(LWXP_VIEW, controls, do_destroy); // Don't destroy by default
		GetXPanel().Describe(desc, CB_DataGet, CB_DataSet);

		if (hints) GetXPanel().Hint(hints);

		static LWXPanelHint default_hint[] =
		{
			XpCHGNOTIFY(LWXPanelChangeNotifyFunc),
			XpEND
		};
		GetXPanel().Hint(default_hint);
		static LWXPanelHint destroy_hint[] =
		{
			XpDESTROYNOTIFY(LWXPanelDestroyNotifyFunc),
			XpEND
		};
		if (do_destroy)
		{
			GetXPanel().Hint(destroy_hint);
		}		
		GetXPanel().ViewInst(host);
		GetXPanel().setUserData(host);
	}
}