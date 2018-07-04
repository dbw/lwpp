#include <lwpp/utility_panels.h>
namespace lwpp
{
	// About 
	void AboutPanel::draw(LWControlID, DrMode)
	{
		// get the control position and size
		int xs = canvas.HotX();
		int cw = canvas.HotW();
		int ch = canvas.HotH();
		int ys = canvas.HotY();
		LWDrawFuncs df = getDrawFuncs();
		df.drawBox(255, 255, 255, xs, ys, cw, ch); // clear background
																							 // center the image
		xs += (cw - width) / 2;
		ys += (ch - height) / 2;
		mRaster.blitPanel(0, 0, GetID(), xs, ys, width, height);
	}


	void AboutPanel::UserDraw(LWControlID control, void *userdata, DrMode drawmode)
	{
		if (userdata)
		{
			AboutPanel *mypanel = static_cast<AboutPanel *>(userdata);
			mypanel->draw(control, drawmode);
		}
	}

	AboutPanel::AboutPanel(const std::string title, const char**message, const char *_image, int w, int h)
		: LWPanel(title), image(_image), width(w), height(h)
	{
		if (image)
		{
			mRaster.create(w, h);
			for (int y = 0; y < h; ++y)
			{
				int index = (height - y - 1) * w * 3;
				for (int x = 0; x < w; ++x)
				{
					mRaster.drawPixel(image[index + 2], image[index + 1], image[index], x, y);
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

	size_t LogPanel::controlCount(PanelControl)
	{
		return static_cast<int>(this->size());
	}

	const char *LogPanel::controlName(PanelControl, int index)
	{
		return this->at(index).c_str();
	}

	void LogPanel::Open(const std::string &name)
	{
		SetUserData(this);
		listbox = AddListbox<LogPanel>(name, w, h);
		vSizer.Add(listbox, lwpp::STRETCH_VERTICAL | lwpp::STRETCH_HORIZONTAL);
		mainSizer.Add(&vSizer);
		lwpp::Margin margin(0, 0, 0, 16);
		mainSizer.setMargin(margin);
		SetCallbacks(this);
		LWPanel::Open(PANF_BLOCKING | PANF_RESIZE | PANF_FRAME);
		int minW, minH;
		mainSizer.getMinSize(minW, minH);
		SetMinHeight(minH);
		SetMinWidth(minW);
		SetHeight(minH);
		SetWidth(minW);

	}
	void LogPanel::panelResize(const lwpp::LWPanel & pan, int w, int h)
	{
		mainSizer.Layout(4, 0, w - 8, h - 8 - 28, 0);
	}
}