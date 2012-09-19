#ifndef LWPP_MODELER_H
#define LWPP_MODELER_H

#include <lwmodeler.h>
#include <lwdyna.h>
#include <lwpp/global.h>
#include <lwpp/monitor.h>

namespace lwpp
{

  class ModelerState : protected GlobalBase<LWStateQueryFuncs>
  {
  public:

    //! Returns the number of data layers for the current object.
    int GetNumLayers()
    {
      return globPtr->numLayers();
    }

    //! Returns bits for the data layers included in the EltOpLayer selection.
    /*!
     * If bit i of the mask is set,
     * then layer i + 1 of the current object belongs to the set defined by the oplayer argument.
     * This function is provided primarily for backward compatibility.
     * New code should use the GetLayerList function,
     * which is designed for multiple objects and an unlimited number of layers. 
     */
    unsigned int GetLayerMask(EltOpLayer lyr)
    {
      return globPtr->layerMask(lyr);
    }

    //! Returns the name of the current default surface. 
    const char *GetSurface()
    {
      return globPtr->surface();
    }

    //! Returns the number of points in the layer selection. 
    /*!
     * If box isn't NULL, it is an array of six doubles that will
     * receive the bounding box of the points in the layer selection, in the order (x0, x1, y0, y1, z0, z1). 
     */
    unsigned int GetBoundingBox(EltOpLayer lyr, double *box=0)
    {
      return globPtr->bbox(lyr, box);
    }

    //! Returns a string containing layer numbers for the given EltOpLayer and object.
    /*!
     * The layer numbers in the string are separated by spaces,
     * with the highest numbered layer listed first.
     * The object name is its filename, or NULL for the current object
     */
    const char *GetLayerList(EltOpLayer lyr, const char *name)
    {
      return globPtr->layerList(lyr, name);
    }

    //! Returns the filename of the current object.
    /*!
     * If the geometry in the current layers hasn't been saved to a file yet,
     * this returns the reference name (the name that would be returned by the Object Functions refName function).
     * If no object has been loaded into Modeler, this returns NULL. 
     */
    const char *GetObjectName()
    {
      return globPtr->object();
    }

    //! Returns the state of a user interface setting.
    /*!
     *  The setting codes are
     * - <b>LWM_MODE_SELECTION</b> Returns the selection mode (points, polygons, volume) as an integer. 
     * - <b>LWM_MODE_SYMMETRY</b> Returns the state of the symmetry toggle.
     */
    int GetMode(int m)
    {
      return globPtr->mode(m);
    }

    //! Returns the name of the currently selected vertex map, and stores the LWID of the vmap in the second argument.
    /*!
     * The index can be one of the following: 
     * - LWM_VMAP_WEIGHT
     * - LWM_VMAP_TEXTURE
     * - LWM_VMAP_MORPH
     */
    const char *GetSelectedVMap(int i, LWID *id)
    {
      return globPtr->vmap(i, id);
    }

  };

  class DynaMonitor : protected GlobalBase<DynaMonitorFuncs>
  {
  public:
    LWMonitor *mon;
    DynaMonitor(const char *a, const char *b)
      : mon(0)
    {
      if (globPtr)
      {
        mon = globPtr->create(a, b);
      }
    }
    virtual ~DynaMonitor()
    {
      if (mon) globPtr->destroy(mon);
    }

    SimpleMonitor Get()
    {
      return SimpleMonitor(mon);
    }
  };


} // end namespace lwpp

#endif // LWPP_MODELER_H