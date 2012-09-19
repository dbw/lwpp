#ifndef LW95_H
#define LW95_H

#include <lwtypes.h>
#include <lwhandler.h>
#include <lwenvel.h>
#include <lwmeshes.h>

namespace LightWave95
{

typedef double		 LWRayCastFunc (const LWDVector position,
                    const LWDVector direction);

typedef double		 LWRayTraceFunc (const LWDVector position,
                     const LWDVector direction,
                     LWDVector color);

typedef double		 LWRayShadeFunc (const LWDVector position,
                     const LWDVector direction,
                     struct st_LWShaderAccess *shader_access);

typedef int		 LWIlluminateFunc (LWItemID light, const LWDVector position,
                       LWDVector direction, LWDVector color);

typedef int		 LWIlluminateNormalFunc (LWItemID light, const LWDVector position,
                       LWDVector direction, const LWDVector normal_vec, LWDVector color);

typedef int		 LightSampleFunc( void *data, LWItemID light, const LWDVector dir, const LWDVector color );
typedef double	 LWIlluminateSampleFunc( LWItemID light, const LWDVector pos, LWDVector dir,
                                     LightSampleFunc *sampler, void *data );

typedef double	 LWIlluminateSampleNormalFunc( LWItemID light, const LWDVector pos, LWDVector dir,
                                     const LWDVector normal_vec, LightSampleFunc *sampler, void *data );

typedef double		 LWRayTraceModeFunc (const LWDVector position,
                         const LWDVector direction,
                         LWDVector color,
                         const double eta,
                         const int rtmode );

typedef void*        RandomFloatData;
typedef float        LWRandomFloatFunc (RandomFloatData);

typedef struct st_LWShaderAccess {
	int		  sx, sy;
	double		  oPos[3], wPos[3];
	double		  gNorm[3];
	double		  spotSize;
	double		  raySource[3];
	double		  rayLength;
	double		  cosine;
	double		  oXfrm[9],  wXfrm[9];
	LWItemID	  objID;
	int		  polNum;

	double		  wNorm[3];
	double		  color[3];
	double		  luminous;
	double		  diffuse;
	double		  specular;
	double		  mirror;
	double		  transparency;
	double		  eta;
	double		  roughness;

	LWIlluminateFunc 	*illuminate;
	LWRayTraceFunc	 	*rayTrace;
	LWRayCastFunc	 	*rayCast;
	LWRayShadeFunc	 	*rayShade;

	int		  flags;
	int		  bounces;
	LWItemID	  sourceID;
	double		  wNorm0[3];
	double		  bumpHeight;
	double		  translucency;
	double		  colorHL;
	double		  colorFL;
	double		  addTransparency;
	double		  difSharpness;

	LWPntID		  verts[4];				// surrounding vertex IDs
	float		  weights[4];			// vertex weigths
	float	      vertsWPos[4][3];		// vertex world positions
	LWPolID		  polygon;				// polygon ID

	double		  replacement_percentage;
	double		  replacement_color[3]; 
	double		  reflectionBlur;
	double		  refractionBlur;

	LWRayTraceModeFunc	*rayTraceMode;
	double		  incomingEta;
	int		  ResourceContextIndex;
	double        diffuse_shade[3];
	double        specular_shade[3];
	double        refract_shade[3];
	double        reflect_shade[3];

	LWIlluminateSampleFunc	*illuminateSample;
	LWRandomFloatFunc       *randomFloat;

	LWIlluminateNormalFunc *illuminateNormal;
	LWIlluminateSampleNormalFunc *illuminateSampleNormal;

	double        subsx, subsy;
	double        bumpDropoff;

	LWDVector     rayDir;

} LWShaderAccess;

// The LWNodalAccess struct.
typedef struct LWNodalAccess_t {

	int			sx, sy;

	LWDVector		oPos, wPos;
	LWDVector		gNorm;
	LWDVector		wNorm;
	LWDVector		wNorm0;

	LWDVector		raySource;
	double			rayLength;

	double			spotSize;
	double			cosine;
	double			bumpHeight;

	double			oXfrm[9], wXfrm[9];

	LWIlluminateFunc		*illuminate;
	LWIlluminateSampleFunc	*illuminateSample;
	LWRayTraceFunc			*rayTrace;
	LWRayCastFunc			*rayCast;
	LWRayShadeFunc			*rayShade;
	LWRayTraceModeFunc		*rayTraceMode;

	int			bounces;

	LWItemID		sourceID;

	LWItemID		objID;
	int			polNum;

	LWPntID			verts[4];
	float			weights[4];
	float			vertsWPos[4][3];
	LWPolID			polygon;

	double			incomingEta;			// Incoming refraction index.
	int			ResourceContextIndex;	// Current CPU thread running.
	int			flags;

	LWRandomFloatFunc   *randomFloat;

	LWIlluminateNormalFunc *illuminateNormal;
	LWIlluminateSampleNormalFunc *illuminateSampleNormal;

	double        subsx, subsy;

	LWDVector     rayDir;

} LWNodalAccess;

} // LightWave95

#endif