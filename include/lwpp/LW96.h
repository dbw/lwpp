#ifndef LW96_H
#define LW96_H

#include <lwtypes.h>
#include <lwhandler.h>
#include <lwenvel.h>
#include <lwmeshes.h>

namespace LightWave96
{
/*
enum
{
    LWBUF_SPECIAL,
    LWBUF_LUMINOUS,
    LWBUF_DIFFUSE,
    LWBUF_SPECULAR,
    LWBUF_MIRROR,
    LWBUF_TRANS,
    LWBUF_RAW_RED,
    LWBUF_RAW_GREEN,
    LWBUF_RAW_BLUE,
    LWBUF_SHADING,
    LWBUF_SHADOW,
    LWBUF_GEOMETRY,
    LWBUF_DEPTH,
    LWBUF_DIFFSHADE,
    LWBUF_SPECSHADE,
    LWBUF_MOTION_X,
    LWBUF_MOTION_Y,
    LWBUF_REFL_RED,
    LWBUF_REFL_GREEN,
    LWBUF_REFL_BLUE,
    LWBUF_DIFF_RED,
    LWBUF_DIFF_GREEN,
    LWBUF_DIFF_BLUE,
    LWBUF_SPEC_RED,
    LWBUF_SPEC_GREEN,
    LWBUF_SPEC_BLUE,
    LWBUF_BACKDROP_RED,
    LWBUF_BACKDROP_GREEN,
    LWBUF_BACKDROP_BLUE,
    LWBUF_PREEFFECT_RED,
    LWBUF_PREEFFECT_GREEN,
    LWBUF_PREEFFECT_BLUE,
    LWBUF_RED,
    LWBUF_GREEN,
    LWBUF_BLUE,
    LWBUF_ALPHA,
    LWBUF_REFR_RED,
    LWBUF_REFR_GREEN,
    LWBUF_REFR_BLUE,
    LWBUF_REFR_ALPHA,

    LWBUF_MAX_CHANNELS
};
*/

	typedef void* LWRenderData;

	typedef double		 LWRayCastFunc (const LWDVector position,	const LWDVector direction);

	typedef double		 LWRayTraceFunc (const LWDVector position,	const LWDVector direction,
		LWDVector color);

	typedef double		 LWRayShadeFunc (const LWDVector position, const LWDVector direction,	struct st_LWShaderAccess *shader_access);

	typedef int		 LWIlluminateFunc (LWItemID light, const LWDVector position,	LWDVector direction, LWDVector color);

	typedef int		 LWIlluminateNormalFunc (LWItemID light, LWRenderData rd, const LWDVector position,	LWDVector direction, const LWDVector normal_vec, LWDVector color);

	typedef int		 LightSampleFunc( void *data, LWItemID light, const LWDVector dir, const double color[4] );
	typedef double	 LWIlluminateSampleFunc( LWItemID light, const LWDVector pos, LWDVector dir, LightSampleFunc *sampler, void *data );

	typedef double	 LWIlluminateSampleNormalFunc( LWItemID light, LWRenderData rd, const LWDVector pos, const LWDVector normal_vec, LightSampleFunc *sampler, void *data );

	typedef void     LWBufferGetValFunc( LWRenderData rd, int type, int num, double* );
	typedef void     LWBufferSetValFunc( LWRenderData rd, int type, int num, double* );

#define RTMODE_DISTANCE		0
#define RTMODE_REFLECTION	1
#define RTMODE_REFRACTION	2
#define RTMODE_DISSOLVE		3
#define RTMODE_SHADOW		4
#define RTMODE_OCCLUSION	5
#define RTMODE_BACKDROP     6
#define RTMODE_OCCLUSION_DEPTH 7
#define RTFLAG_BACKSHADE    0x08000000
#define RTFLAG_BACKDROP     0x10000000
#define RTFLAG_OBJECT       0x20000000
#define RTFLAG_SAMPLEDRAY	0x40000000
#define RTFLAG_NOPOLYGON	0x80000000

	typedef double LWRayTraceModeFunc (const LWDVector position, const LWDVector direction,	LWDVector color, const double eta, const int rtmode );

	typedef void* LWRayData;

typedef struct st_LWRayTraceData
{
    // send
    LWDVector   rayStart;
    LWDVector   rayDir;
    LWDVector   backdrop; // Fill this if RTFLAG_BACKDROP is set.
    LWDVector   weight; // This color is used the weigh the contribution weight of the ray in the renderer.
    double      eta;
    int         flags;

    // receive
    LWRayData   ray; // This data needs to be filled by LWRayTraceData before calling LWRayTraceShade.
    double      len;
    LWDVector   result; // The result is filled with the backdrop color at LWRayTraceData if the ray doesn't hit anything.

} LWRayTraceData;

typedef void         LWRayTraceDataFunc( LWRenderData, LWRayTraceData* );
typedef void         LWRayTraceShadeFunc( LWRayTraceData* );

typedef void*        RandomFloatData;
typedef float        LWRandomFloatFunc (RandomFloatData);

#define LWRT_SHADOW	 (1<<0) // This flag is on if the incoming ray is a shadow ray
#define LWRT_PREVIEW (1<<1) // This flag is on if the spot is being rendered in a preview context, like the Viper.
#define LWRT_POLYSIDE (1<<2) // This flag is on if the incoming ray hit the back side of the polygon
#define LWRT_SAMPLEDRAY (1<<3) // This flag is on if the incoming ray was cast for sampling soft reflections, etc.
#define LWRT_PREPROCESS (1<<4) // This flag is on if the incoming ray is a radiosity preprocess ray.
#define LWRT_EXITING (1<<5) // This flag is on if the ray is going to exit to an undefined surface - Ie. vacuum/air, which needs to be handled by the material/shader.
#define LWRT_DOUBLESIDED (1<<6) // This flag is on if the surface is double sided.
#define LWRT_FRAME_PREPROCESS (1<<7) // This flag is on if the incoming ray is a frame preprocess ray.
#define LWRT_RADIOSITY (1<<8) // This flag is on if the incoming ray is a radiosity ray.


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
		RandomFloatData         randomData;
		LWRandomFloatFunc       *randomFloat;

		LWIlluminateNormalFunc *illuminateNormal;
		LWIlluminateSampleNormalFunc *illuminateSampleNormal;

		double        subsx, subsy;
		double        bumpDropoff;

		LWDVector     rayDir;

		LWRayTraceDataFunc  *rayTraceData;
		LWRayTraceShadeFunc *rayTraceShade;
		LWRenderData        render;

		LWBufferGetValFunc  *getVal;
		LWBufferSetValFunc  *setVal;

	} LWShaderAccess;

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

		RandomFloatData     randomData;
		LWRandomFloatFunc   *randomFloat;

		LWIlluminateNormalFunc *illuminateNormal;
		LWIlluminateSampleNormalFunc *illuminateSampleNormal;

		double        subsx, subsy;

		LWDVector     rayDir;

		LWRayTraceDataFunc  *rayTraceData;
		LWRayTraceShadeFunc *rayTraceShade;
		LWRenderData        render;

		LWBufferGetValFunc  *getVal;
		LWBufferSetValFunc  *setVal;

	} LWNodalAccess;

#define NF_TRANSP (1<<0) // This flag should be set if a material node might have transparency

} // LightWave95

#endif