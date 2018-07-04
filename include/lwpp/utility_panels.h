#pragma once

#ifndef LWPP_UTILITY_PANELS_H
#define LWPP_UTILITY_PANELS_H

#include <lwpp/panel_sizer.h>
#include <lwpp/lwpanel_handler.h>

namespace lwpp
{
	/*!
	* Different Pre-fab panels
	*/
	//! @ingroup LWPanels
	//! An About panel
	class AboutPanel : private LWPanel
	{
	private:
		const char *image;
		int width;
		int height;
		LWRaster mRaster;
		PanelControl canvas;
		void draw(LWControlID control, DrMode drawmode);
	public:
		AboutPanel(const std::string title, const char**message, const char *_image = 0, int w = 0, int h = 0);
		~AboutPanel();
		void Open();
		static void UserDraw(LWControlID control, void *userdata, DrMode drawmode);
	};

	//! @ingroup LWPanels
	//! A Log panel
	class LogPanel : private LWPanel, protected LWPanelListCallBacks, public std::vector<std::string>, public LWPanelCallbacks
	{
		PanelControl listbox;
		int w, h;
		lwpp::HorizontalSizer mainSizer;
		lwpp::VerticalSizer vSizer;
	public:
		virtual size_t controlCount(PanelControl);
		virtual const char* controlName(PanelControl, int index);
	public:
		LogPanel(const std::string &title, int width = 400, int height = 10)
			: LWPanel(title), w(width), h(height)
		{
			;
		}
		void Open(const std::string &name);
		void panelResize(const lwpp::LWPanel& pan, int w, int h);
	};
}
#endif // LWPP_UTILITY_PANELS_H
