#pragma once
#ifndef LWPP_VIEWPORT_H
#define LWPP_VIEWPORT_H

#include <lwpp/global.h>
#include "lwviewportinfo.h"

namespace lwpp
{
	//! @ingroup Globals
	/*!
	*  Returns information about one of the existing cameras on the loaded scene
	*/
	class ViewportInfo : protected GlobalBase<LWViewportInfo>
	{
	protected:
		int view = -2;
	public:
		ViewportInfo(int v = 0) : view(v) {}
		void setView(int v) { view = v; }
		int getView() const { return view; }
		int getType() { return globPtr->type(view); }
		unsigned int getFlags() { return globPtr->flags(view); }
		void getRect(int *left, int *top, int *width, int *height) { return globPtr->rect(view, left, top, width, height); }
		int getHoverIndex(double cursor_pos[2]) { return globPtr->hoverIndex(cursor_pos); }
		bool isHover()
		{
			double pos[2];
			return (getHoverIndex(pos) == view);
		}
		int getActivePreview() { return globPtr->getActivePreview(); }
		bool isActivePreview() { return getActivePreview() == view; }
		double getWorldSize(double pixels, LWDVector ref)
		{
			return globPtr->pixelSize(view, pixels, ref);
		}
		int getHandleSize()
		{
			return globPtr->handleSize(view);
		}		
	};
}

#endif