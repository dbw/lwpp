/*!
* @file
*/
#ifndef LWPP_MOD_COMMAND_H
#define LWPP_MOD_COMMAND_H

#include <lwpp/global.h>

#include <lwmodeler.h>
#include <lwcmdseq.h>

namespace lwpp
{

	class PointScanner
	{
	public:
		static EDError doScan (void*p, const EDPointInfo* info) 
		{
			PointScanner *host = static_cast<PointScanner *>(p);
			return host->Scan(info);
		}
		virtual EDError Scan(const EDPointInfo* info) {return EDERR_NONE;}
	};

	class FastPointScanner
	{
	public:
		static EDError doScan (void*p, LWPntID id) 
		{
			FastPointScanner *host = static_cast<FastPointScanner *>(p);
			return host->Scan(id);
		}
		virtual EDError Scan(LWPntID id) {return EDERR_NONE;}
	};

	class EdgeScanner
	{
	public:
		static EDError doScan (void*p, const EDEdgeInfo* info) 
		{
			EdgeScanner *host = static_cast<EdgeScanner *>(p);
			return host->Scan(info);
		}
		virtual EDError Scan(const EDEdgeInfo* info) {return EDERR_NONE;}
	};

	class FastEdgeScanner
	{
	public:
		static EDError doScan (void*p, LWEdgeID id) 
		{
			FastEdgeScanner *host = static_cast<FastEdgeScanner *>(p);
			return host->Scan(id);
		}
		virtual EDError Scan(LWEdgeID id) {return EDERR_NONE;}
	};

	class PolyScanner
	{
	public:
		static EDError doScan (void*p, const EDPolygonInfo* info) 
		{
			PolyScanner *host = static_cast<PolyScanner *>(p);
			return host->Scan(info);
		}
		virtual EDError Scan(const EDPolygonInfo* info) {return EDERR_NONE;}
	};

	class FastPolyScanner
	{
	public:
		static EDError doScan (void*p, LWPolID id) 
		{
			FastPolyScanner *host = static_cast<FastPolyScanner *>(p);
			return host->Scan(id);
		}
		virtual EDError Scan(LWPolID id) {return EDERR_NONE;}
	};

	class MeshEditOperator
	{
	protected:
		const MeshEditOp *me;
	public:
		MeshEditOperator(const MeshEditOp *_me = 0)
			: me(_me)
		{}

		MeshEditOperator(const MeshEditOp &_me)
			: me(&_me)
		{}

		void SetMeshEditOp(const MeshEditOp *_me)
		{
			me = _me;
		}

		MeshEditOperator& operator=(const MeshEditOperator &from)
		{
			if (this != &from)
			{
				me = from.me;
			}
		}
		int getLayer() {assert(me!=0); return me->layerNum;}
		//! Call this when the edit is complete. 
		/*!
		* As changes are made during an edit, they are buffered through Modeler's undo mechanism,
		* so they are not reflected in the data until done is called, and not at all if done sets the error argument. 
		*
		* In general, if one of the edit functions returns an error, you'll pass that error to done.
		* If you just want the edit to stop or be discarded,
		* possibly because the user pressed the Cancel button in a progress monitor,
		* you'll pass EDERR_USERABORT.
		* If an error occurs in your plug-in, you'll pass EDERR_NOMEMORY (for memory allocation errors)
		* or EDERR_BADARGS (for everything else).
		* And if the edit was successful, you'll use EDERR_NONE.
		*
		* The selset argument tells Modeler how you want the selection to appear
		* to the user after the edit has been applied.
		* It contains flags combined using bitwise-or, and can include the following.
		* - <b>EDSELM_CLEARCURRENT</b> Deselect elements that were selected when the edit began. 
		* - <b>EDSELM_SELECTNEW</b> Select elements created by the edit. 
		* - <b>EDSELM_FORCEVRTS</b> Force selection of newly created vertices. 
		* - <b>EDSELM_FORCEPOLS</b> Force selection of newly created polygons. 
		*
		* A value of 0 leaves all directly selected elements selected after the edit.
		* The CLEARCURRENT and SELECTNEW flags are polite hints;
		* they won't override selection settings made by the user.
		* The force flags will always force direct selection of the points or polygons created by the edit. 
		*/
		void Done (EDError err, int selm)
		{
			assert(me!=0);
			if(me) me->done(me->state, err, selm);
		}

		int GetPointCount (EltOpLayer lyr, int mode)
		{
			assert(me!=0);
			if(me) return me->pointCount(me->state, lyr, mode);
			return 0;
		}

		int GetPolyCount (EltOpLayer lyr, int mode)
		{
			assert(me!=0);
			if(me) return me->polyCount(me->state, lyr, mode);
			return 0;
		}

		EDError Scan(PointScanner &scanner, EltOpLayer lyr)
		{
			assert(me!=0);
			if(me) return me->pointScan (me->state, &PointScanner::doScan, &scanner, lyr);
			return 0;
		}

		EDError Scan(FastPointScanner &scanner, EltOpLayer lyr, bool selOnly)
		{
			assert(me!=0);
			if(me) return me->fastPointScan (me->state, &FastPointScanner::doScan, &scanner, lyr, selOnly ? 1 : 0);
			return 0;
		}

		EDError Scan(PolyScanner &scanner, EltOpLayer lyr)
		{
			assert(me!=0);
			if(me) return me->polyScan (me->state, &PolyScanner::doScan, &scanner, lyr);
			return 0;
		}

		EDError Scan(FastPolyScanner &scanner, EltOpLayer lyr, bool selOnly)
		{
			assert(me!=0);
			if(me) return me->fastPolyScan (me->state, &FastPolyScanner::doScan, &scanner, lyr, selOnly ? 1 : 0);
			return 0;
		}

		EDPointInfo *GetInfo (LWPntID id)
		{
			assert(me!=0);
			if(me) return me->pointInfo (me->state, id);
			return 0;
		}

		EDPolygonInfo *GetInfo (LWPolID id)
		{
			assert(me!=0);
			if (me) return me->polyInfo (me->state, id);
			return 0;
		}

		bool GetPolyNormal(LWPolID id, double n[3])
		{
			assert(me!=0);
			if (me)
			{
				return (me->polyNormal(me->state, id, n) != 0);
			}
			return false;
		}

		LWPntID AddPoint(double *xyz)
		{
			assert(me!=0);
			if (me) return me->addPoint(me->state, xyz);
			return 0;
		}

		LWPolID AddFace(const char *surf, int numPnt, const LWPntID *pnts)
		{
			assert(me!=0);
			if (me) return me->addFace(me->state, surf, numPnt, pnts);
			return 0;
		}

		LWPolID AddCurve(const char *surf, int numPnt, const LWPntID *pnts, int flags)
		{
			assert(me!=0);
			if (me) return me->addCurve(me->state, surf, numPnt, pnts, flags);
			return 0;
		}

		EDError AddQuad(LWPntID p1, LWPntID p2, LWPntID p3, LWPntID p4)
		{
			assert(me!=0);
			if (me) return me->addQuad(me->state, p1, p2, p3, p4);
			return 0;
		}

		EDError AddTri(LWPntID p1, LWPntID p2, LWPntID p3)
		{
			assert(me!=0);
			if (me) return me->addTri(me->state, p1, p2, p3);
			return 0;
		}

		EDError AddPatch(int nr, int nc, int lr, int lc, EDBoundCv *r0, EDBoundCv *r1, EDBoundCv *c0, EDBoundCv *c1)
		{
			assert(me!=0);
			if (me) return me->addPatch(me->state, nr, nc, lr, lc, r0, r1, c0, c1);
			return 0;
		}

		EDError Remove (LWPntID id)
		{
			assert(me!=0);
			if (me) return me->remPoint (me->state, id);
			return 0;
		}

		EDError Remove (LWPolID id)
		{
			assert(me!=0);
			if (me) return me->remPoly (me->state, id);
			return 0;
		}

		EDError Move (LWPntID id, const double *delta)
		{
			assert(me!=0);
			if (me) return me->pntMove (me->state, id, delta);
			return 0;
		}

		//! Change the surface assigned to a polygon. 
		EDError SetSurface (LWPolID id, const char *name)
		{
			assert(me!=0);
			if (me) return me->polSurf (me->state, id, name);
			return 0;
		}

		//! Replace the point list of a polygon. 
		EDError ReplacePolyPoints (LWPolID id, int n, const LWPntID *p)
		{
			assert(me!=0);
			if (me) return me->polPnts (me->state, id, n, p);
			return 0;
		}

		//! Set polygon flags.
		/*!
		* The mask contains 1 bits for each flag you want to change,
		* and the value contains the new flag settings (0 or 1 for each 1 bit in the mask).
		*
		* Currently, the flags that can be changed are the EDPF_START and EDPF_END flags for curves. 
		*/
		EDError SetPolyFlags (LWPolID id, int mask, int value)
		{
			assert(me!=0);
			if (me) return me->polFlag (me->state, id, mask, value);
			return 0;
		}

		//! Add a polygon tag to a polygon, or change an existing tag.
		/*!
		* If the tag type is LWPTAG_SURF, the tag is the surface name.
		*
		* If the tag type is LWPTAG_PART, the tag is the part (or group) name.
		*
		* For anything other than surface tags, passing a NULL tag will remove an existing tag of the specified type. 
		*/
		EDError SetPolTag (LWPolID id, LWID type, const char *name = 0)
		{
			assert(me!=0);
			if (me) return me->polTag (me->state, id, type, name);
			return 0;
		}

		//! Add a vmap vector to a point. 
		/*!
		* The vmap type can be one of the following, or something else.
		* - LWVMAP_PICK - selection set
		* - LWVMAP_WGHT - weight map
		* - LWVMAP_MNVW - subpatch weight map
		* - LWVMAP_TXUV - texture UV coordinates
		* - LWVMAP_MORF - relative vertex displacement (morph)
		* - LWVMAP_SPOT - absolute vertex displacement (morph)
		* - LWVMAP_RGB, LWVMAP_RGBA - vertex color 
		*
		* Pass a NULL val_array to remove a vmap vector from the point.
		*/
		EDError SetVMap (LWPntID id, LWID vmap, const char *c = 0, int i = 0, float *f = 0)
		{
			assert(me!=0);
			if (me) return me->pntVMap (me->state, id, vmap, c, i, f);
			return 0;
		}

		//! Create a polygon. 
		/*!
		* If a template polygon is supplied, Modeler copies the polygon tags for the new polygon from the template.
		*
		* If the surface name is NULL, the surface will be that of the template,
		* or the current default surface if the template is NULL.
		* The vertices of the new polygon are listed in clockwise order,
		* and the normal will be inferred from the first, second and last vertex. 
		*/
		LWPolID AddPoly (LWID type, int numPnt, const LWPntID *pnt, LWPolID tmpl = 0, const char *surf = 0 )
		{
			assert(me!=0);
			if (me) return me->addPoly (me->state, type, tmpl, surf, numPnt, pnt);
			return 0;
		}

		//! Create an "interpolated" point. 
		/*!
		* The new point's vmap values are calculated as a weighted average of the
		* vmaps of the points in the points array.
		* If pos is NULL, the position is also computed as a weighted average.
		* If the weight array is NULL, the averaging over the point list is uniform.
		* The weights are renormalized to sum to 1.0. 
		*/
		LWPntID AddIPoint (double *xyz, int numPnt, const LWPntID *ids, const double *wt = 0)
		{
			assert(me!=0);
			if (me) return me->addIPnt (me->state, xyz, numPnt, ids, wt);
			return 0;
		}
		LWPntID AddIPoint (int numPnt, const LWPntID *ids, const double *wt = 0)
		{
			return AddIPoint(0, numPnt, ids, wt);
		}

		//! Set the texture UV for a point or polygon you're about to create.
		/*!
		* If a texture map is selected in Modeler's interface,
		* the UVs will be assigned to that map as points or polygons are created.
		* You'll typically want to give the user the option of whether or not to create UVs for new points and polygons. 
		*
		* When creating points, pass initUV an array of two floats and then
		* call any of the functions that create a point.
		* The two floats will be used as the U and V for the point,
		* after which the initUV state will be cleared so that subsequent
		* points have no UV unless the function is called again.
		*
		* To initialize per-polygon, or discontinuous, UVs,
		* call initUV with a pointer to 2n floats before creating a polygon with n vertices.
		* For each vertex, if the point's continuous UV value is different from the UV in the array,
		* then a polygon-specific UV is set for the vertex.
		* If the point has no continuous UV, then the continuous value for the point is set to the polygon UV. 
		*
		* Any combination of these two methods can be used to assign UVs to new data.
		* If only polygon UVs are specified, continuous UVs will still be created where polygons share UV values.
		* Alternately, plug-ins can assign UVs to points and only specify polygon UVs along seam polygons. 
		*/
		EDError InitUV (float *uv)
		{
			assert(me!=0);
			if (me) return me->initUV (me->state, uv);
			return 0;
		}

		//! Select a vmap for reading vectors.
		/*!
		* Returns an opaque pointer that can be used to select
		* the same vmap in later calls to this function.
		* The first time this is called for a given vmap,
		* the pointer can be NULL, and Modeler will locate and
		* select the vmap using the type and name arguments
		*/
		void *Select (void *vmapID, LWID vmaptype, const char *vmapname)
		{
			assert(me!=0);
			if (me) return me->pointVSet (me->state, vmapID, vmaptype, vmapname);
			return 0;
		}
		void *Select (LWID vmaptype, const char *vmapname)
		{
			return Select(0, vmaptype, vmapname);
		}
		//! Read the vmap vector for a point. 
		/*!
		* The vector is read from the vmap selected by a previous call to pointVSet.
		* If the point is mapped (has a vmap value in the selected vmap),
		* the val array is filled with the vmap vector for the point, and pointVGet returns true.
		* If you don't already know the dimension of the vmap
		* (the number of values per point, and therefore the required size of the val array),
		* you can use the scene objects global to find out. 
		*
		* See also pointVPGet and pointVEval.
		* pointVGet is equivalent to reading values from a VMAP chunk in an object file.
		* It returns the continuous, or per-point, vector.
		* For the raw discontinuous, or per-polygon-vertex value,
		* use pointVPGet, and for the combined value accounting for both sources, use pointVEval.
		*/
		bool ReadVMap (LWPntID point, float *val)
		{
			assert(me!=0);
			if (me) return (me->pointVGet (me->state, point, val) != 0);
			return 0;
		}

		//! Returns a tag string associated with the polygon.
		/*!
		* For the LWPTAG_SURF tag type, the surface name is returned. 
		*/
		const char *GetPolyTag (LWPolID polygon, LWID tagtype)
		{
			assert(me!=0);
			if (me) return me->polyTag (me->state, polygon, tagtype);
			return "";
		}

		//! Set the selection state of a point.
		/*!
		* These can only be called during OPSEL_MODIFY mesh edits.
		* The element is selected if setsel is true and deselected if it's false
		*/
		EDError Select (LWPntID pnt, bool sel)
		{
			assert(me!=0);
			if (me) return me->pntSelect (me->state, pnt, sel ? 1 : 0);
			return 0;
		}

		//! Set the selection state of a polygon.
		/*!
		* These can only be called during OPSEL_MODIFY mesh edits.
		* The element is selected if setsel is true and deselected if it's false
		*/
		EDError Select (LWPolID pol, bool sel)
		{
			assert(me!=0);
			if (me) return me->polSelect (me->state, pol, sel ? 1 : 0);
			return 0;
		}

		//! Read the vmap vector for a polygon vertex.
		/*!
		*  This is like pointVGet, but it returns the discontinuous vmap value,
		* equivalent to reading entries in a VMAD chunk. 
		*/
		bool ReadVMap (LWPntID point, LWPolID pol, float *val)
		{
			assert(me!=0);
			if (me) return (me->pointVPGet (me->state, point, pol, val) != 0);
			return 0;
		}

		//! Read the vmap vector for a point, accounting for both continuous and discontinuous values. 
		/*!
		* Generally, if a discontinuous value exists for the point, that value will be returned.  Otherwise the continuous value is used. 
		*/
		bool EvaluateVMap (LWPntID point, LWPolID pol, float *val)
		{
			assert(me!=0);
			if (me) return (me->pointVEval (me->state, point, pol, val) != 0);
			return 0;
		}
		//! Add a discontinuous vmap vector to a polygon vertex.
		/*!
		* This is the vector returned by ReadVMap (LWPntID point, LWPolID pol, float *val).
		* See pntVMap for a partial list of vmap types. 
		*/
		EDError AddDiscoVMap (LWPntID point, LWPolID pol, LWID type, const char *name, int dim, float *val)
		{
			assert(me!=0);
			if (me) return me->pntVPMap (me->state, point, pol, type, name, dim, val);
			return 0;
		}

		/*
		LWPntID             (*edgePoint1)       (EDStateRef, LWEdgeID);
		LWPntID             (*edgePoint2)       (EDStateRef, LWEdgeID);
		LWEdgeID            (*edgeFromPoints)   (EDStateRef, LWPntID, LWPntID);
		void                (*edgeFlip)         (EDStateRef, LWEdgeID);
		*/
		int GetEdgeCount (EltOpLayer lyr, int mode)
		{
			assert(me!=0);
			if (me) return me->edgeCount (me->state, lyr, mode);
			return 0;
		}

		EDError Scan(EdgeScanner &scanner, EltOpLayer lyr)
		{
			assert(me!=0);
			if(me) return me->edgeScan (me->state, &EdgeScanner::doScan, &scanner, lyr);
			return 0;
		}

		EDError Scan(FastEdgeScanner &scanner, EltOpLayer lyr, bool selOnly)
		{
			assert(me!=0);
			if(me) return me->fastEdgeScan (me->state, &FastEdgeScanner::doScan, &scanner, lyr, selOnly ? 1 : 0);
			return 0;
		}

		EDEdgeInfo *GetInfo (LWEdgeID id)
		{
			assert(me!=0);
			if (me) return me->edgeInfo (me->state, id);
			return 0;
		}

		//! Set the selection state of an edge.
		/*!
		* These can only be called during OPSEL_MODIFY mesh edits.
		* The element is selected if setsel is true and deselected if it's false
		*/
		EDError Select (LWEdgeID edge, bool sel)
		{
			assert(me!=0);
			if (me) return me->edgeSelect (me->state, edge, sel ? 1 : 0);
			return 0;
		}

		int GetEdgePolys (LWEdgeID edge, const LWPolID**pols)
		{
			assert(me!=0);
			if (me) return me->edgePolys (me->state, edge, pols);
			return 0;
		}

		LWPntID GetEdgePoint1 (LWEdgeID edge)
		{
			assert(me!=0);
			if (me) return me->edgePoint1 (me->state, edge);
			return 0;
		}

		LWPntID GetEdgePoint2 (LWEdgeID edge)
		{
			assert(me!=0);
			if (me) return me->edgePoint2 (me->state, edge);
			return 0;
		}

		//! Returns the edge between p1 and p2 if such an edge exists.
		/*!
		* Otherwise, the function returns NULL. 
		*/ 
		LWEdgeID GetEdge (LWPntID p1, LWPntID p2)
		{
			assert(me!=0);
			if (me) return me->edgeFromPoints (me->state, p1, p2);
			return 0;
		}

		void FlipEdge (LWEdgeID edge)
		{
			assert(me!=0);
			if (me) me->edgeFlip (me->state, edge);
		}

		//! Copies the position of the specified point into pos
		void GetPointPosition (LWPntID pnt, double pos[3])
		{
			assert(me!=0);
			if (me) me->pointPos (me->state, pnt, pos);
		}

		//! Returns the edges surrounding the specified point. 
		int GetPointEdges (LWPntID pnt, const LWEdgeID **edges)
		{
			assert(me!=0);
			if (me) return me->pointEdges (me->state, pnt, edges);
			return 0;
		}

		unsigned int GetPolyType (LWPolID pol)
		{
			assert(me!=0);
			if (me) return me->polyType (me->state, pol);
			return 0;
		}

		int GetPolyPoints (LWPolID pol, const LWPntID **pnts)
		{
			assert(me!=0);
			if (me) return me->polyPoints (me->state, pol, pnts);
			return 0;
		}

		int GetFlags (LWPntID pnt)
		{
			assert(me!=0);
			if (me) return me->pointFlags (me->state, pnt);
			return 0;
		}

		int GetFlags (LWEdgeID edg)
		{
			assert(me!=0);
			if (me) return me->edgeFlags (me->state, edg);
			return 0;
		}

		int GetFlags (LWPolID pol)
		{
			assert(me!=0);
			if (me) return me->polyFlags (me->state, pol);
			return 0;
		}

		void *GetData (LWPntID pnt)
		{
			assert(me!=0);
			if (me) return me->pointData (me->state, pnt);
			return 0;
		}

		void *GetData (LWEdgeID edg)
		{
			assert(me!=0);
			if (me) return me->edgeData (me->state, edg);
			return 0;
		}

		void *GetData (LWPolID pol)
		{
			assert(me!=0);
			if (me) return me->polyData (me->state, pol);
			return 0;
		}

		int GetLayer (LWPntID pnt)
		{
			assert(me!=0);
			if (me) return me->pointLayer (me->state, pnt);
			return 0;
		}

		int GetLayer (LWEdgeID edg)
		{
			assert(me!=0);
			if (me) return me->edgeLayer (me->state, edg);
			return 0;
		}

		int GetLayer (LWPolID pol)
		{
			assert(me!=0);
			if (me) return me->polyLayer (me->state, pol);
			return 0;
		}

		int SetLayer (int lyr)
		{
			assert(me!=0);
			if (me) return me->setLayer (me->state, lyr);
			return 0;
		}
		int AdvanceLayer ()
		{
			assert(me!=0);
			if (me) return me->advanceLayer (me->state);
			return 0;
		}

		int PointNew (LWPntID pnt)
		{
			assert(me!=0);
			if (me) return me->pointNew (me->state, pnt);
			return 0;
		}

		int EdgeNew (LWEdgeID edg)
		{
			assert(me!=0);
			if (me) return me->edgeNew (me->state, edg);
			return 0;
		}

		int PolyNew (LWPolID pol)
		{
			assert(me!=0);
			if (me) return me->polyNew (me->state, pol);
			return 0;
		}

		/*
		// new in Lightwave 9.6 - LWMESHEDIT_VERSION 6
		void *               (*vMapSelect)      (EDStateRef es, const char* name, LWID type, int dim);
		int                  (*vMapExists)      (EDStateRef es, const char* name, LWID type);
		unsigned int         (*vMapGetDimension)(EDStateRef es);
		void                 (*vMapSet)         (EDStateRef es, LWPntID point_id, LWPolID polygon_id, const float *value);
		void                 (*vMapSetIdeal)    (EDStateRef es, LWPntID point_id, LWPolID polygon_id, const float *value);
		void                 (*vMapRename)      (EDStateRef es, const char *new_name);
		void                 (*vMapRemove)      (EDStateRef es);
		*/

	};


	class ModelerCommand : public MeshEditOperator
	{
		LWModCommand *mc;

		MeshEditBegin*        editBegin()
		{
			assert(mc!=0);
			return mc->editBegin;
		}
		MeshEditBegin2*       editBegin2()
		{
			assert(mc!=0);
			return mc->editBegin2;
		}

	protected:
		int version;
		const char *GetArgument()
		{
			assert(mc!=0);
			return mc->argument;
		}
		LWCommandCode Lookup (const char *cmdName)
		{
			assert(mc!=0);
			return mc->lookup(mc->data, cmdName);
		}
		int  Execute        (LWCommandCode cmd, int argc, const DynaValue *argv, EltOpSelect op, DynaValue *result)
		{
			assert(mc!=0);
			return mc->execute(mc->data, cmd, argc, argv, op, result);
		}
		int  Evaluate         (const char *command)
		{
			assert(mc!=0);
			return mc->evaluate(mc->data, command);
		}
		void BeginUndoGroup() 
		{
			assert(mc!=0);
			mc->undoGroupBegin();
		}
		void EndUndoGroup()
		{
			assert(mc!=0);
			mc->undoGroupEnd();
		}

		void EditBegin (int pntBuf, int polBuf, EltOpSelect opsel)
		{
			assert(mc!=0);
			SetMeshEditOp(mc->editBegin(pntBuf, polBuf, opsel));
		}

		void EditBegin (int pntBuf, int edgeBuf, int polBuf, EltOpSelect opsel)
		{
			assert(mc!=0);
			return SetMeshEditOp(mc->editBegin2(pntBuf, edgeBuf, polBuf, opsel));
		}
	public: 
		ModelerCommand(int _version, LWModCommand *_mc)
			: mc(_mc), version(_version)
		{
		}

		virtual int Activate (void* p) = 0;

	};

	//! @ingroup Adaptor
	template <typename T>
	class ModelerCommandAdaptor
	{
	public:
		ModelerCommandAdaptor(const char *name, ServerTagInfo *tags = 0)
		{
			LWServer::AddPlugin(LWMODCOMMAND_CLASS, name, Activate, tags);
		}
		//! Set static callbacks for the LightWave instance handler
		static int Activate ( int version, GlobalFunc* global, LWInstance inst, void* serverData)
		{
			LWModCommand *local = static_cast<LWModCommand *>(inst);
			lwpp::SetSuperGlobal(global);
			T plugin(version, local);
			return plugin.Activate(serverData);
		}
	};

} // end namespace lwpp

#endif // LWPP_MOD_COMMAND_H