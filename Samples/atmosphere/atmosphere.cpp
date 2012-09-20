/*
======================================================================
atmosphere.c

Volumetric fog.

Gregory Duquesne
Minor touch-up by Ernie Wright  16 Apr 00

Based on chapter 14 (K. Musgrave, L. Gritz, S. Worley) of TEXTURING
AND MODELING, 2nd ed., ISBN 0-12-228730-4.
====================================================================== */

/*
======================================================================

Mike Wolf 
David Ballesteros 

LWPP VERSION  24 March 2007

====================================================================== */

#include "atmosphere.h"
#include <lwpp/vector3d.h>
#include <lwpp/point3d.h>

ServerTagInfo atmosphereTags[] =
{
	{"LWPP Atmosphere Demo",SRVTAG_USERNAME},
	{"LWPP Atmosphere Demo Volumetric",SRVTAG_DESCRIPTION },
	{ NULL }
};


static LWXPanelControl ctrl_list[] = {
      { ID_MARCH, "Render Type",        "iPopChoice" },
      { ID_HI,    "Top",                "distance"   },
      { ID_LO,    "Bottom",             "distance"   },
      { ID_FA,    "Falloff",            "percent"    },
      { ID_DEN,   "Nominal Distance",   "distance"   },
      { ID_LUM,   "Luminosity",         "percent"    },
      { ID_OPA,   "Opacity",            "percent"    },
      { ID_BCK,   "Use Backdrop Color", "iBoolean"   },
      { ID_COL,   "Color",              "color"      },
      { ID_RES,   "Quality",            "iPopChoice" },
      { ID_TXTR,  "Use Texture",        "iBoolean"   },
      { ID_TXBT,  "Edit Texture",       "vButton"    },
      { 0 }
};

static LWXPanelDataDesc data_descrip[] = {
      { ID_MARCH, "Render Type",        "integer"    },
      { ID_HI,    "Top",                "distance"   },
      { ID_LO,    "Bottom",             "distance"   },
      { ID_FA,    "Falloff",            "percent"    },
      { ID_DEN,   "Nominal Distance",   "distance"   },
      { ID_LUM,   "Luminosity",         "percent"    },
      { ID_OPA,   "Opacity",            "percent"    },
      { ID_BCK,   "Use Backdrop Color", "integer"    },
      { ID_COL,   "Color",              "color"      },
      { ID_RES,   "Quality",            "integer"    },
      { ID_TXTR,  "Use Texture",        "integer"    },
      { ID_TXBT,  "Edit Texture",       NULL         },
      { 0 },
};

static char *quality[] = { "Very Low", "Low", "Medium", "Good", "Very Good", NULL };
static char *render[] = { "Fast Fog", "Ray Marcher", NULL };
static int bckMap[] = { 1, 0 };


Atmosphere::Atmosphere(void *priv, void *context, LWError *err) :
		lwpp::XPanelVolumetricHandler(priv, context, err),
		FogTextureEd( "FogTexture", TEF_ALL - TEF_USEBTN)
{
	// We initialize the values for the member variables here
	lo = -4.0;
	hi = 0.0;
	fa = 0.5;
	lum = 0.5;
	opa = 1.0;
	den = 1.0;
	res = 2;
	type = 1;
	bck  = 1;
	useTxtr = 0;
	txtr = 0;
	txedID = 0;
	march   = 0;
	VSET( col, 1.0 );


	// Originally : iteminfo     = global( LWITEMINFO_GLOBAL, GFUSE_TRANSIENT );
	iteminfo = (LWItemInfo *) lwpp::SuperGlobal( LWITEMINFO_GLOBAL, GFUSE_TRANSIENT );
   
	// Originally : txedf = global( LWTXTREDFUNCS_GLOBAL, GFUSE_TRANSIENT );
	txedf =  (LWTxtrEdFuncs *) lwpp::SuperGlobal( LWTXTREDFUNCS_GLOBAL, GFUSE_TRANSIENT );

	// Originally : txtrf = global( LWTEXTUREFUNCS_GLOBAL, GFUSE_TRANSIENT );
	txtrf = (LWTextureFuncs *) lwpp::SuperGlobal( LWTXTREDFUNCS_GLOBAL, GFUSE_TRANSIENT );

	// Originally : backdropinfo = global( LWBACKDROPINFO_GLOBAL, GFUSE_TRANSIENT );
	backdropinfo = (LWBackdropInfo *) lwpp::SuperGlobal( LWBACKDROPINFO_GLOBAL, GFUSE_TRANSIENT );



}

Atmosphere::~Atmosphere()
{
}


/*
======================================================================
GADD()

Calculate density and illumination at a point.  Called by the ray
marcher.
====================================================================== */

double Atmosphere::GADD( lwpp::VolumetricAccess &va, double pp[ 3 ], double y, double falloff,
   int use_lighting, int atmos_type, double li[ 3 ] )
{
   LWItemID light = iteminfo->first( LWI_LIGHT, NULL );
   double ilu[ 3 ], ldir[ 3 ];

   /* accumulate contributions from each light */

   if ( use_lighting && ( va.getFlags() & LWVEF_COLOR )) {
      VSET( li, 0.0 );
      while ( light ) {
         va.illuminate( light, pp, ldir, ilu );
         VADD( li, ilu );
         light = iteminfo->next( light );
      }
   }
   else
      VSET( li, 1.0 );

   /* return GADD (geometric atmospheric density distribution) */

   if ( atmos_type == 1 )
      return exp( -falloff * y );
   else
      return 1.0;
}


/*
======================================================================
evalTexture()

Evaluate the texture, which the ray marcher uses to modulate the
transparency.
====================================================================== */

double Atmosphere::evalTexture( LWMicropol *mp, double pos[ 3 ], double stride,
   double col[ 4 ], LWTextureID txtr )
{
   VCPY( mp->oPos, pos );
   VCPY( mp->wPos, pos );
   mp->spotSize = 0.3333 * stride;
   return txtrf->evaluate( txtr, mp, col );
}


/*
======================================================================
raymarchEval()

Find the ray's intersection with the fog layer and accumulate samples
along the ray.  Each sample has a color, an opacity, a size (called a
stride) and is identified by its position (dist) along the ray.  The
accumulation stops when the end of the intersection, or a max opacity,
or a max number of samples has been reached.
====================================================================== */

void Atmosphere::raymarchEval(lwpp::VolumetricAccess va)
{
   double density = 100.0, falloff = 10.0;
   double integstart = 0.0, integend = 100.0;
   double minstepsize = 0.01, maxstepsize = 100.0;
   double k = 200.0;
   double fogclr[ 3 ];
   int use_lighting = 1, atmos_type = type;

   double dtau, last_dtau, total_tau, rstep, tau, x, y, h, col[ 4 ], trans;
   int nsteps = 0;                  /* record number of integration steps */
   double li[ 3 ], last_li[ 3 ];
   LWVolumeSample sample;           /* differential color & opacity */
   // ORIGINALLY  LWDVector PP;
   lwpp::Point3D PP;
   LWMicropol mp;

   k *= res;
   falloff *= fa;
   density /= den;

   h = hi - lo;
   if ( h == 0 ) return;

   /* if using a texture, initialize the micropolygon */

   if ( useTxtr ) {
      memset( &mp, 0, sizeof( LWMicropol ));
      mp.gNorm[ 1 ] = mp.wNorm[ 1 ] = 1.0;
      mp.oAxis = mp.wAxis = 1;
      mp.oScl[ 0 ] = mp.oScl[ 1 ] = mp.oScl[ 2 ] = 1.0;
   }

   /* get the fog color */

   if ( bck )
   {
	   const double origin[3] = {va.getOrigin().x,va.getOrigin().y, va.getOrigin().z };
	   backdropinfo->backdrop( time, origin, fogclr );
   } else
   {
      VCPY( fogclr, this->col );
   }

   /* assume initially that the ray origin is in the fog */

   sample.dist = va.getNearClip();

   /* below the fog bottom, looking up */

   if ( va.getDirection().y > 0 && va.getOrigin().y < lo ) {
      y = ( lo - va.getOrigin().y );
      x = y / va.getDirection().y;
      sample.dist = sqrt( x * x + y * y );
   }

   /* above the fog top, looking down */

   if ( va.getDirection().y < 0 && va.getOrigin().y > hi ) {
      y = ( hi - va.getOrigin().y );
      x = y / va.getDirection().y;
      sample.dist = sqrt( x * x + y * y );
   }

   /* test if intersection within clipping range */

   if ( sample.dist > va.getFarClip() ) return;

   // ORIGINALLY
   // VADDS3( PP, va.getOrigin(), va.getDirection(), sample.dist );
   PP = ((lwpp::Point3D(va.getOrigin()) + lwpp::Vector3D(va.getDirection())) *  sample.dist);

   y = ( PP.y - lo ) / h;

   /* test if outside fog */

   if (( y > 1 && va.getDirection().y > 0 ) || ( y < 0 && va.getDirection().y < 0 ))
      return;

   dtau = density * GADD( va, PP.asLWDVector(), y, falloff, use_lighting, atmos_type, li );
   rstep = 1.0 / ( k * dtau + 0.001 );
   sample.stride = sample.dist * MAX( MIN( CLAMP( rstep, minstepsize, maxstepsize ),
      va.getFarClip() - sample.dist ), 0.0005 );

   ++nsteps;
   total_tau = 0;

   while (( sample.dist <= va.getFarClip() )
      && ( total_tau * opa < 300 ) && ( nsteps < 200 )) {

      last_dtau = dtau;
      VCPY( last_li, li );
      // ORIGINALLY 
	  // VADDS3( PP, va.getOrigin(), va.getDirection(), sample.dist );

      y = ( upcomp( PP.asLWDVector() ) - lo ) / h;

      /* test if outside fog */

      if (( y > 1 && va.getDirection().y > 0 ) || ( y < 0 && va.getDirection().y < 0 ))
         return;

      dtau = density * GADD( va, PP.asLWDVector(), y, falloff, use_lighting, atmos_type, li );
      if ( !useTxtr ) {
         trans = 0;
         VSCL3( col, fogclr, lum );
      }
      else {
         trans = evalTexture( &mp, PP.asLWDVector(), sample.stride, col, txtr );
         VSCL( col, lum );
      }

      if ( trans != 1.0 ) {
         tau = 0.5 * sample.stride * ( dtau * ( 1.0 - trans ) + last_dtau );
         total_tau += tau;

         /* color */

         sample.color[ 0 ] = 0.5 * col[ 0 ] * sample.stride
            * ( li[ 0 ] * dtau + last_li[ 0 ] * last_dtau );
         sample.color[ 1 ] = 0.5 * col[ 1 ] * sample.stride
            * ( li[ 1 ] * dtau + last_li[ 1 ] * last_dtau );
         sample.color[ 2 ] = 0.5 * col[ 2 ] * sample.stride
            * ( li[ 2 ] * dtau + last_li[ 2 ] * last_dtau );

         /* opacity */

         VSET( sample.opacity, tau * opa );

         /* add volumetric sample to the ray */

         va.addSample( &sample );
      }

      /* new stride = f(dist) */

      rstep = 1.0 / ( k * dtau + 0.001 );
      sample.stride = sample.dist * MAX( MIN( CLAMP( rstep, minstepsize, maxstepsize ),
         va.getFarClip() - sample.dist ), 0.0005 );
      sample.dist += sample.stride;
      nsteps += 1;
   }
}


/*
======================================================================
analyticEval()

The analytic evaluation is different from the ray marching.  What it
does is just evaluate an overall fog effect for the ray based on the
analytical solution of the integration of an exponential density
distribution (d = exp(-y)).  The result is saved as a single sample of
size 0 and opacity 1.0. The same technique can be used to represent
surface samples with the volumetric engine.  By combining the overall
fog effect (=fog amount) and the background color, we can evaluate
this fog in a way similar to LW's standard fog.
====================================================================== */

void Atmosphere::analyticEval(lwpp::VolumetricAccess va)
{
   LWVolumeSample sample;                   /* differential color & opacity */
   double density = 100.f, falloff = 200.f;
   double tau = 0, fog = 0, x = 0, y = 0, yo = 0, ye = 0, lo = 0, le = 0, h = 0 , nearClip = 0, fogclr[ 3 ];

   lwpp::Vector3D dir;


   /* skip evaluation if this is a shadow ray */

   if ( !( va.getFlags() & LWVEF_COLOR )) return;

   h = hi - lo;
   if ( h == 0 ) return;

   falloff = 200 * fa;
   density /= den;
   le = lo = nearClip = 0.0001;

   /* find ray/fog intersection */

   y = va.getOrigin().y;
   if ( va.getDirection().y > 0 ) {  /* looking up */
      if ( y > hi )
         return;

      else if ( y > lo && y < hi ) {
         y = ( hi - va.getOrigin().y );
         x = y / va.getDirection().y;

         ye = hi;
         le = sqrt( x * x + y * y );

         lo = nearClip;
         yo = va.getOrigin().y + lo * va.getDirection().y;
         sample.dist = lo;
      }

      else if ( y < lo ) {
         y = ( lo - va.getOrigin().y );
         x = y / va.getDirection().y;

         lo = sqrt( x * x + y * y );
         yo = va.getOrigin().y + lo * va.getDirection().y;

         y = ( hi - va.getOrigin().y );
         x = y / va.getDirection().y;

         ye = hi;
         le = sqrt( x * x + y * y );

         sample.dist = lo;
      }
   }
   else
   if ( va.getDirection().y < 0 ) {  /* looking down */
      if ( y < lo )
         return;

      else if ( y > lo && y < hi ) {
         y = ( lo - va.getOrigin().y );
         x = y / va.getDirection().y;

         ye = lo;
         le = sqrt( x * x + y * y );

         lo = nearClip;
         yo = va.getOrigin().y + lo * va.getDirection().y;
      }

      else if ( y > hi ) {
         y = ( hi - va.getOrigin().y );
         x = y / va.getDirection().y;

         lo = sqrt( x * x + y * y );
         yo = va.getOrigin().y + lo * va.getDirection().y;

         y = ( lo - va.getOrigin().y );
         x = y / va.getDirection().y;

         ye = lo;
         le = sqrt( x * x + y * y );
      }
   }
   else
       return;

   if ( lo > va.getFarClip() )
      return;

   /* evaluate tau = optical distance */

   sample.dist = 0.9999 * va.getFarClip();
   if ( le > va.getFarClip() ) {
      le = va.getFarClip();
      ye = va.getOrigin().y + le * va.getDirection().y;
   }

   yo = ( yo - lo ) / h;
   ye = ( ye - lo ) / h;
   if ( fabs( va.getDirection().y ) > 0.001 ) {
      tau = exp( -( yo + va.getDirection().y * ( le - lo ))) - exp( -yo );
      tau *= -( density * ( le - lo )) / ( falloff * va.getDirection().y );
   }
   else {
      dir = va.getDirection();
      if ( dir.y > 0 )
         dir.y = 0.001;
      else
         dir.y = -0.001;

      tau = exp( -( yo + dir.y * ( le - lo ))) - exp( -yo );
      tau *= -( density * ( le - lo )) / ( falloff * dir.y );
   }

   /* convert tau to the overall normalised fog amount along that ray */

   tau -= density * ( le - lo ) * exp( -falloff );
   if ( tau > 0 )
      fog = 1 - exp( -tau );  /* R -> [0,1].... 0 = no fog , 1 = max fog */
   else
      fog = 1;

   fog *= opa;
   CLAMP( fog, 0, 1 );

   /* Now we compute the final color value in a way similar to LW's fog:
        final_color = (1 - fog) * backColor + fog * fogColor * luminosity
      backColor = va.rayColor  is the color viewed from the ray (ie LW's
      render).  The result is set a sample of size 0, and opacity = 1.0
      which means that this sample is opaque (null sized samples are
      treated as surface samples). */

   sample.stride = 0;
   VSET( sample.opacity, 1.0 );
   if ( bck )
   {
	  const double direction[3] = {va.getDirection().x,va.getDirection().y, va.getDirection().z };
      backdropinfo->backdrop( time, direction, fogclr );
   } else
   {
	   VCPY( fogclr, col );
   }

   sample.color[ 0 ] = ( 1 - fog ) * va.getRayColorR() + fog * fogclr[ 0 ] * lum;
   sample.color[ 1 ] = ( 1 - fog ) * va.getRayColorG() + fog * fogclr[ 1 ] * lum;
   sample.color[ 2 ] = ( 1 - fog ) * va.getRayColorB() + fog * fogclr[ 2 ] * lum;

   va.addSample( &sample );
}

void *  Atmosphere::DataGet (unsigned int vid)
{
	switch ( vid ) {
		case ID_MARCH:  
		  return &march;
		case ID_HI:     
		  return &hi;
		case ID_LO:     
		  return &lo;
		case ID_FA:     
		  return &fa;
		case ID_DEN:    
		  return &den;
		case ID_LUM:
		  return &lum;
		case ID_OPA:    
		  return &opa;
		case ID_BCK:    
		  return &bck;
		case ID_COL:    
		  return &col;
		case ID_RES:    
		  return &res;
		case ID_TXTR:
		  return &useTxtr;
		default:
		 return NULL;
	}

}

LWXPRefreshCode  Atmosphere::DataSet (unsigned int vid, void *value)
{

	double *d = ( double * ) value;
	int *i = ( int * ) value;

	switch ( vid ) {
		case ID_MARCH:  
			march = *i;
			return LWXPRC_DFLT;
		case ID_HI:
			hi = *d;
			return LWXPRC_DFLT;
		case ID_LO:
			lo = *d;
			return LWXPRC_DFLT;
		case ID_FA:
			fa = *d;
			return LWXPRC_DFLT;
		case ID_LUM:
			lum = *d;
			return LWXPRC_DFLT;
		case ID_OPA:
			opa = *d;
			return LWXPRC_DFLT;
		case ID_DEN:
			den = *d;
			return LWXPRC_DFLT;
		case ID_BCK:
			bck = *i;
			return LWXPRC_DFLT;
		case ID_RES:
			res = *i;
			return LWXPRC_DFLT;
		case ID_TXTR:
			useTxtr = *i;
			return LWXPRC_DFLT;
		case ID_COL:
			VCPY( col, d );
			return LWXPRC_DFLT;
		default:
			return LWXPRC_NONE;
	}
}


/*
======================================================================
PanelDestroyNotify()

XPanels callback.  Called after the panel has been destroyed.
====================================================================== */

void Atmosphere::PanelDestroyNotify(void)
{

	if ( txedID )
	{
         txedf->unsubscribe( txedID );
         txedID = NULL;
  }
}


/*
======================================================================
ButtonClick()

XPanels callback.  Called when the user presses the "Edit Texture"
button.
====================================================================== */

void Atmosphere::ButtonClick(unsigned int cid)
{
	UNUSED(cid);
	if (!FogTexture.getID())
	{
		FogTexture.create(TRT_COLOR, "FogTexture");
	}
	FogTextureEd.open(FogTexture.getID());
}


/*
======================================================================
Interface()

XPanels Interface.  Called when the user presses the "Options"
button.
====================================================================== */
int Atmosphere::Interface( int version, LWInterface *local, void *serverData )
{
	UNUSED(serverData);
	UNUSED(version);

	if ( !txedf ) return AFUNC_BADGLOBAL;
	
	LWXPanelHint hint[] = {
		XpLABEL( 0, "VFog Settings" ),
		XpBUTNOTIFY( ID_TXBT, XPanelInterface::LWXPanelBtnClickFunc ),
		XpSTRLIST( ID_RES, quality ),
		XpSTRLIST( ID_MARCH, render ),

		XpGROUP_( ID_GGRP ),
			XpH( ID_HI ), XpH( ID_LO ), XpH( ID_FA ), XpH( ID_DEN ), XpEND,
		XpGROUP_( ID_SGRP ),
			XpH( ID_LUM ), XpH( ID_OPA ), XpH( ID_BCK ), XpH( ID_COL ), XpEND,
		XpGROUP_( ID_RGRP ),
			XpH( ID_RES ), XpH( ID_TXTR ), XpH( ID_TXBT ), XpEND,

		XpENABLE_( ID_MARCH ),
			XpH( ID_RGRP ), XpEND,
		XpENABLE_MAP_( ID_BCK, bckMap ),
			XpH( ID_COL ), XpEND,

		XpMIN( ID_LUM, 0 ), XpMAX( ID_LUM, 1 ),
		XpMIN( ID_OPA, 0 ), XpMAX( ID_OPA, 1 ),
		XpMIN( ID_FA, 0 ),  XpMAX( ID_FA, 1 ),

		XpEND
	}; // end hint[]
		
	CreateViewXPanel (ctrl_list, data_descrip, hint);

	// FINE because the callback it is in our class
	LW_XPanel.setUserData(this, ID_TXBT);

	local->panel = LW_XPanel.getID();

	local->options = NULL;
	local->command = NULL;

	return AFUNC_OK;
}


/*
======================================================================
Describe()

Return a one-line text description of the instance data.

No need, classes may call SetDescription(const char *) or SetDescription(std::string) at any time to change this, anything else is handled by lwpp

====================================================================== */

/*
const char * atmosphere::Describe()
{
   sprintf( desc, "Bottom: %2f Top: %2f", lo, hi );
   return desc;
}
*/



/*
======================================================================
NewTime()

Called at the start of each frame.
====================================================================== */

LWError Atmosphere::NewTime( LWFrame f, LWTime t )
{
	UNUSED(f);
	time = t;
	return NULL;
}

/*
======================================================================
Evaluate()
====================================================================== */

double Atmosphere::Evaluate(lwpp::VolumetricAccess voa)
{
   if ( march )
      raymarchEval(voa);
   else
      analyticEval(voa);

   return 1.0;
}


lwpp::XPanelVolumetricAdaptor<Atmosphere> atmosphereInstance("Atmosphere LWPP", atmosphereTags);
