#ifndef CUSTOMOBJECT_ACCESS_H
#define CUSTOMOBJECT_ACCESS_H

#include <lwcustobj.h>

namespace lwpp
{
	class CustomObjAccess
	{
	protected:
		const LWCustomObjAccess *access;
	public:
		CustomObjAccess(const LWCustomObjAccess *acc) : access(acc) {;}
		int View() {return access->view;}
		int Flags() {return access->flags;}
		bool isSelected() {return (Flags() & LWCOFL_SELECTED) != 0;}
		bool isPicking() {return (Flags() & LWCOFL_PICKING) != 0;}
		void SetColor (float rgba[4]) {access->setColor(access->dispData, rgba);}
		void SetColor (float r, float g, float b, float a) 
		{
			float rgba[4] = {r, g, b, a};
			SetColor(rgba);
		}
		void SetPattern (int lpat) {access->setPattern(access->dispData, lpat);}
		void SetTexture (int size, unsigned char *image) {access->setTexture(access->dispData, size, image);}
		void SetUVs (double uv0[2], double uv1[2], double uv2[2], double uv3[2]) {access->setUVs(access->dispData, uv0, uv1, uv2, uv3);}
		void SetUVs (double uv[4][2]) {access->setUVs(access->dispData, uv[0], uv[1], uv[2], uv[3]);}
		void DrawPoint (double pos[3], int csys) {access->point(access->dispData, pos, csys);}
		void DrawLine (double from[3], double to[3], int csys) {access->line(access->dispData, from, to, csys);}
		void DrawTriangle (double v1[3], double v2[3], double v3[3], int csys) {access->triangle(access->dispData, v1, v2, v3, csys);}
		void DrawQuad (double v1[3], double v2[3], double v3[3], double v4[3], int csys) {access->quad(access->dispData, v1, v2, v3, v4, csys);}
		void DrawQuad (double v[4][3], int csys) {access->quad(access->dispData, v[0], v[1], v[2], v[3], csys);}
		void DrawCircle (double center[3], double radius, int csys) {access->circle(access->dispData, center, radius, csys);}
		void DrawText (double center[3], const char *text, int just, int csys) {access->text(access->dispData, center, text, just, csys);}
		Point3d GetViewPos() {return Point3d(access->viewPos);}
		Vector3d GetViewDir() {return Vector3d(access->viewDir);}
		void	SetCoordinateSystem(LWItemID item) {access->setCSysItem(access->dispData, item);}

		void DrawPolygon(unsigned int numv, double v[][3], int csys)
		{
			access->polygon(access->dispData, numv, v, csys);
		}
		void DrawPolygon(unsigned int numv, unsigned int verts[], double v[][3], int csys)
		{
			access->polyIndexed(access->dispData, numv, verts, v, csys);
		}
		void SetDrawMode(unsigned int mode) {access->setDrawMode(access->dispData, mode);}
		void DrawDisk(double p[3], double r, int csys)
		{
			access->disk(access->dispData, p, r, csys);
		}
#if LW_SDK >= 96
		// Todo, add part number
#endif

	};

} // end namespace

#endif // CUSTOMOBJECT_ACCESS_H