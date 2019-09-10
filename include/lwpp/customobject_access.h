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
		void SetColorCC(const float rgba[4]) { access->setColorCC(access->dispData, (float *)(rgba)); }
		void SetColorCC(const float r, float g, float b, float a)
		{
			float rgba[4] = { r, g, b, a };
			SetColorCC(rgba);
		}
		void SetPattern (int lpat) {access->setPattern(access->dispData, lpat);}
		void SetTexture (int size, unsigned char *image) {access->setTexture(access->dispData, size, image);}
		void SetUVs (double uv0[2], double uv1[2], double uv2[2], double uv3[2]) {access->setUVs(access->dispData, uv0, uv1, uv2, uv3);}
		void SetUVs (double uv[4][2]) {SetUVs(uv[0], uv[1], uv[2], uv[3]);}
		void DrawPoint (double pos[3], int csys) {access->point(access->dispData, pos, csys);}
		void DrawLine (double from[3], double to[3], int csys) {access->line(access->dispData, from, to, csys);}
		void DrawTriangle (double v1[3], double v2[3], double v3[3], int csys) {access->triangle(access->dispData, v1, v2, v3, csys);}
		void DrawQuad (double v1[3], double v2[3], double v3[3], double v4[3], int csys) {access->quad(access->dispData, v1, v2, v3, v4, csys);}
		void DrawQuad (double v[4][3], int csys) {DrawQuad(v[0], v[1], v[2], v[3], csys);}
		void DrawCircle (double center[3], double radius, int csys) {access->circle(access->dispData, center, radius, csys);}
		void DrawCircle(double center[3], int csys, double radius, int rsys) { access->circle2(access->dispData, center, csys, radius, rsys); }
		void DrawDisk(double center[3], double radius, int csys) { access->disk(access->dispData, center, radius, csys); }
		void DrawDisk(double center[3], int csys, double radius, int rsys) { access->disk2(access->dispData, center, csys, radius, rsys); }

		void DrawText (double center[3], const char *text, int just, int csys) {access->text(access->dispData, center, text, just, csys);}
		void DrawText(double center[3], const std::string text, int just, int csys) { DrawText(center, text.c_str(), just, csys); }
		Point3d GetViewPos() {return Point3d(access->viewPos);}
		Vector3d GetViewDir() {return Vector3d(access->viewDir);}
		void SetCoordinateSystem(LWItemID item) {access->setCSysItem(access->dispData, item);}

		void DrawPolygon(unsigned int numv, double v[][3], int csys)
		{
			access->polygon(access->dispData, numv, v, csys);
		}
		void DrawPolygon(unsigned int numv, unsigned int verts[], double v[][3], int csys)
		{
			access->polyIndexed(access->dispData, numv, verts, v, csys);
		}
		void SetDrawMode(unsigned int mode) {access->setDrawMode(access->dispData, mode);}
		int MeasureText (const char *text /* language encoded */, int *width, int *height, int *offset_y)
		{
			return access->measureText(access->dispData, text, width, height, offset_y);
		}
		int MeasureText(const std::string &text /* language encoded */, int *width, int *height, int *offset_y)
		{
			return MeasureText(text.c_str(), width, height, offset_y);
		}
		void SetPart(unsigned int part)
		{
			access->setPart(access->dispData, part);
		}
		void SetThickness (float point_size, float line_width)
		{
			access->setThickness(access->dispData, point_size, line_width);
		}
		void GetColour(unsigned int idx, float rgba[4])
		{
			access->getColor(access->dispData, idx, rgba);
		}
		void Push(LWDMatrix4 mat)
		{
			access->pushMatrix(access->dispData, mat);
		}
		void Pop()
		{
			access->popMatrix(access->dispData);
		}
		void LineStrip(unsigned int numv, double p[][3], int csys)
		{
			access->lineStrip(access->dispData, numv, p, csys);
		}
		void LineStrip(unsigned int numv, double *p, int csys)
		{
			access->lineStrip(access->dispData, numv, (double (*)[3])p, csys);
		}
		void LineLoop(unsigned int numv, double p[][3], int csys)
		{
			access->lineLoop(access->dispData, numv, p, csys);
		}
	};

} // end namespace

#endif // CUSTOMOBJECT_ACCESS_H