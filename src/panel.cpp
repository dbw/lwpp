#include <lwpp/panel.h>

namespace lwpp
{
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

	// About 
	void AboutPanel::draw(LWControlID, DrMode)
	{
		// get the control position and size
		int xs = canvas.HotX();
		int cw = canvas.HotW();
		int ch = canvas.HotH();
		int ys = canvas.HotY();
		LWDrawFuncs df = getDrawFuncs();
		df.drawBox(255,255,255, xs, ys, cw, ch); // clear background
    // center the image
		xs += (cw - width) / 2;
		ys += (ch - height) / 2;
    mRaster.blitPanel(0, 0, GetID(), xs, ys, width, height); 
	}


	void AboutPanel::UserDraw( LWControlID control, void *userdata, DrMode drawmode )
	{
		if (userdata)
		{
			AboutPanel *mypanel = static_cast<AboutPanel *>(userdata);
			mypanel->draw(control, drawmode);
		}
	}	

	AboutPanel::AboutPanel(const std::string title, const char**message, const unsigned char *_image, int w, int h)
		: LWPanel(title), image(_image), width(w), height(h)
	{
    if (image)
    {
      mRaster.create(w, h);
      for(int y = 0; y < h; ++y)
      {
        int index = (height - y - 1) * w * 3;
        for(int x = 0; x < w; ++x)
        {
          mRaster.drawPixel(image[index+2], image[index+1],	image[index], x, y);
          index += 3;
        }
      }
			canvas = AddCanvas("", width, height);
			AddText("", message);
			int cw = GetWidth();
			canvas.setWidth(cw);
			canvas.MoveX(((GetWidth() - cw) / 2) + 6);
			canvas.SetUserData(this);
			canvas.SetUserDraw(&AboutPanel::UserDraw);
      SetUserData(this);
		}
		else
		{
			AddText("", message);
		}
	}

  AboutPanel::~AboutPanel()
  {
    ;
  }

	void AboutPanel::Open()
	{
		LWPanel::Open(PANF_BLOCKING);
	}

	/*
	 * Log Panel
	 */

	int LogPanel::controlCount(PanelControl&)
	{
		return static_cast<int>(this->size());
	}

	const char *LogPanel::controlName(PanelControl&, int index)
	{
		return this->at(index).c_str();
	}

	void LogPanel::Open(const std::string &name)
	{
		SetUserData(this);
		listbox = AddListbox<LogPanel> (name, w, h);
		LWPanel::Open(PANF_BLOCKING | PANF_FRAME);
	}
}