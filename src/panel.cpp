#include <lwpp/panel.h>

namespace lwpp
{

	void *getPanelHandler(LWControlID id)
	{

		if (id)
		{
			PanelControl control(id);
			LWPanel panel(control.PanelID());
			if (panel.GetID()) return (panel.GetUserData());
		}
		return nullptr;
	}

	void *getControlHandler(LWControlID id)
	{
		if (id)
		{
			PanelControl control(id);
			return (control.GetUserData());
		}
		return nullptr;
	}

	void LWRaster::drawRoundBox(int c, int x, int y, int w, int h, int bevel)
	{
		--w; --h;
		moveTo(   x+bevel  , y);
		drawTo(c, x+w-bevel, y);
		drawTo(c, x+w      , y+bevel);
		drawTo(c, x+w      , y+h-bevel);
		drawTo(c, x+w-bevel, y+h);
		drawTo(c, x+bevel  , y+h);
		drawTo(c, x        , y+h-bevel);
		drawTo(c, x        , y+bevel);
		drawTo(c, x+bevel  , y);
	}

	void LWRaster::drawBoxOutline(int c, int x, int y, int w, int h)
	{
		--w; --h;
		moveTo(   x  , y);
		drawTo(c, x+w, y);
		drawTo(c, x+w, y+h);
		drawTo(c, x  , y+h);
		drawTo(c, x  , y);
	}

	void LWRaster::fillRoundBox(int c, int x, int y, int w, int h, int bevel)
	{
		--w; --h;
		drawLine(c, x+bevel, y+bevel-1, x+w-bevel, y+bevel-1);
		drawBox(c, x+bevel-1, y+bevel, w, h-bevel-1);
		drawLine(c, x+bevel, y+h-1, x+w-bevel, y+h-1);
	}
}
