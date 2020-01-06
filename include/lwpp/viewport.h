#pragma once
#ifndef LWPP_VIEWPORT_H
#define LWPP_VIEWPORT_H

#include <lwpp/global.h>
#include "lwviewportinfo.h"

namespace lwpp
{
	//! @ingroup Globals
	/*!
	*  Returns information about one of the viewports in Layout
	*/
	class ViewportInfo : protected GlobalBase<LWViewportInfo>
	{
	protected:
		int mView = -2;
		double mHoverPos[2];
	public:
		ViewportInfo(int v = 0) : mView(v) {}
		void setView(int v) { mView = v; }
		int getView() const { return mView; }
		double getHoverX() const { return mHoverPos[0]; }
		double getHoverY() const { return mHoverPos[1]; }
		int getType() { return globPtr->type(mView); }
		void getViewPosition(LWDVector spot) { globPtr->pos(mView, spot); }
		lwpp::Point3d getViewPosition()
		{
			lwpp::Point3d spot;
			getViewPosition(spot.asLWVector());
			return spot;
		};
		unsigned int getFlags() { return globPtr->flags(mView); }
		void getRect(int *left, int *top, int *width, int *height) { return globPtr->rect(mView, left, top, width, height); }
		int getHoverIndex(double cursor_pos[2]) { return globPtr->hoverIndex(cursor_pos); }
		int getHoverIndex()
		{			
			return getHoverIndex(mHoverPos);
		}
		bool isHover()
		{
			return (getHoverIndex() == mView);
		}
		int getActivePreview() { return globPtr->getActivePreview(); }
		bool isActivePreview() { return getActivePreview() == mView; }
    LWItemID getViewItem() { return globPtr->viewItem(mView); }
		double getWorldSize(double pixels, LWDVector ref)
		{
			return globPtr->pixelSize(mView, pixels, ref);
		}
		int getHandleSize()
		{
			return globPtr->handleSize(mView);
		}
		int project(LWDVector w, double* x, double* y, double *z)
		{
			return globPtr->project(mView, w, x, y, z);
		}
		int project(lwpp::Point3d &p, double* x, double* y, double *z)
		{
			return globPtr->project(mView, p.asLWVector(), x, y, z);
		}
    int project(LWDVector p, double w[])
    {
      return project(p, &w[0], &w[1], &w[2]);
    }
    int project(lwpp::Point3d& p, double w[])
    {
      return project(p, &w[0], &w[1], &w[2]);
    }

		bool unProject(double winx, double winy, double winz, LWDVector world)
		{
			return (globPtr->unproject(mView, winx, winy, winz, world) == 1);
		}
		bool unProject(double winx, double winy, double winz, lwpp::Point3d &world)
		{			
			return unProject(winx, winy, winz, world.asLWVector());
		}

		void getXfrm(double mat[9])
		{
			return globPtr->xfrm(mView, mat);
		}

		bool getProjection(LWDMatrix4 proj, LWDMatrix4 iProj)
		{
			return (globPtr->projection(mView, proj, iProj) == 1);
		}

		bool getModelView(LWDMatrix4 proj, LWDMatrix4 iProj)
		{
			return (globPtr->modelview(mView, proj, iProj) == 1);
		}

		void getClip(double *hither, double* yon)
		{
			globPtr->clip(mView, hither, yon);
		}
	};
}

#endif
