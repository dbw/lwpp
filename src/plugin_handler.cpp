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

	void XPanelInterface::LWXPanelControlDrawFunc    ( LWXPanelID panel, unsigned int cid, LWXPDrAreaID reg, int w, int h )
	{
		XPanel xpan(panel);
		XPanelInterface *plugin = static_cast<XPanelInterface *>(xpan.getUserData());
		XPDrawArea area(reg, xpan.getDrawFuncs(), w, h);
		if (plugin)
			plugin->ControlDraw(cid, area);
	}
	void XPanelInterface::LWXPanelControlZoomFunc    ( LWXPanelID panel, unsigned int cid, int x, int y, int *region, int clickcount )
	{
		XPanel xpan(panel);
		XPanelInterface *plugin = static_cast<XPanelInterface *>(xpan.getUserData(cid));
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

	void XPanelInterface::LWXPanelChangeNotifyFunc (LWXPanelID panel, unsigned int cid, unsigned int vid, int event_type)
	{
		XPanel xpan(panel);
		XPanelInterface *plugin = static_cast<XPanelInterface *>(xpan.getUserData());
		if (plugin) 
			plugin->ChangeNotify(panel, cid, vid, event_type);
	}

	void XPanelInterface::LWXPanelDestroyNotifyFunc(void *data)
	{
		XPanelInterface *plugin = static_cast<XPanelInterface *>(data);
		if (plugin) 
			plugin->PanelDestroyNotify();
	}

	void XPanelInterface::CreateViewXPanel(LWXPanelControl *controls, LWXPanelDataDesc *desc, LWXPanelHint *hints, bool do_destroy)
	{
		CreateViewXPanel(this, controls, desc, hints, do_destroy);
	}
	void XPanelInterface::CreateViewXPanel(void *host, LWXPanelControl *controls, LWXPanelDataDesc *desc, LWXPanelHint *hints, bool do_destroy)
	{
		LW_XPanel.Create(LWXP_VIEW, controls, do_destroy); // Don't destroy by default
		if (hints) LW_XPanel.Hint(hints);

		LWXPanelHint default_hint[] =
		{
			XpCHGNOTIFY(LWXPanelChangeNotifyFunc),
			XpEND
		};
		LW_XPanel.Hint(default_hint);
		LWXPanelHint destroy_hint[] =
		{
			XpDESTROYNOTIFY(LWXPanelDestroyNotifyFunc),
			XpEND
		};
		if (do_destroy)
		{
			LW_XPanel.Hint(destroy_hint);
		}
		LW_XPanel.Describe(desc, CB_DataGet, CB_DataSet);
		LW_XPanel.ViewInst(host);
		LW_XPanel.setUserData(host);
	}
}