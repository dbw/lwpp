//! Compatbility file for LW 11
/*!
* This makes some of the new functionality in LW11 optionally available for projects
* compiled against an older SDK
*/

#ifndef LW11_COMPAT_H
#define LW11_COMPAT_H

#ifdef LW11_COMPAT

#include <lwhandler.h>
#include <lwrender.h>
#include <lwlight.h>

#define LWLIGHT11_VERSION 2

namespace LW11
{

  typedef void*        LWSamplerState;
  typedef float        LWGetSamplerRangeFunc (LWSamplerState, unsigned int*, unsigned int*);
  typedef void         LWGetSample2DFunc (LWSamplerState, unsigned int, float*); // float* must point to 2 floats.
  typedef float        LWGetSample1DFunc (LWSamplerState, unsigned int);

  typedef struct st_LWLightAccess
  {
    LWItemID lightID;

    LWDVector worldPos;

    LWDVector toWorld[3];
    LWDVector toLight[3];

    unsigned int maxIlluminations;

    LWRayCastFunc*      rayCast;
    RandomFloatData     randomData;
    LWRandomFloatFunc*  randomFloat;

    unsigned int bounces;
    unsigned int flags;

    LWDVector surfaceNormal;

    LWSamplerState          samplerState;
    LWGetSamplerRangeFunc   *getSamplerRange;
    LWGetSample2DFunc       *getSample2D;
    LWGetSample1DFunc       *getSample1D;

  } LWLightAccess;

  typedef struct st_LWLightHandler
  {
    LWInstanceFuncs* inst;
    LWItemFuncs* item;
    LWRenderFuncs* rend;
    unsigned int (*flags)      (LWInstance);
    LWError      (*newFrame)   (LWInstance, const LWFrameInfo* frameinfo, unsigned int* maxilluminations, const LWLightAccess* lightaccess);
    int          (*evaluate)   (LWInstance, const LWDVector spot, double fractime, LWIllumination illumination[], const LWLightAccess* lightaccess);
    unsigned int (*getPhotons) (LWInstance, unsigned int maxphotons, LWPhoton photons[], const LWLightAccess* lightaccess);
    unsigned int (*getRayIlluminations) (LWInstance, LWDVector raystart, LWDVector raydir, double farclip, unsigned int maxrayilluminations, LWRayIllumination rayilluminations[], const LWLightAccess* lightaccess);
    const LWPreviewLight* (*preview) (LWInstance);
  } LWLightHandler;

} // LW11

#endif LW11_COMPAT

#endif //LW11_COMPAT_H