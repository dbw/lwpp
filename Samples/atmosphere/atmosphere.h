#ifndef ATMOSPHERE_H
#define ATMOSPHERE_H

#include <lwpp/volumetric_handler.h>
#include <lwpp/texture.h>
#include <lwpp/texture_editor.h>

#include <lwtxtr.h>
#include <lwtxtred.h>
#include <lwxpanel.h>
#include <lwmath.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>


#define upcomp(v) (v[1])

/* user interface stuff ----- */

enum { ID_MARCH = 0x8001, ID_HI, ID_LO, ID_FA, ID_LUM, ID_OPA, ID_DEN, ID_BCK,
	   ID_COL, ID_RES, ID_TXTR, ID_TXBT, ID_RGRP, ID_GGRP, ID_SGRP };

class Atmosphere: public lwpp::XPanelVolumetricHandler
{

private:
	
	/* some globals, we should use the lwpp versions of those */
	lwpp::TextureEditor FogTextureEd;
	lwpp::Texture FogTexture;

	LWTextureFuncs *txtrf;
	LWTxtrEdFuncs *txedf;
	LWBackdropInfo *backdropinfo;
	LWItemInfo *iteminfo;

public:

	
	double       lo, hi, fa, lum, opa, den, col[ 3 ];
	int          type, res, useTxtr, bck, march;
	char         desc[ 100 ];
	LWTextureID  txtr;
	LWTECltID    txedID;
	LWTime       time;
	

	// Constructor & Destructor
	Atmosphere(void *priv, void *context, LWError *err);
	~Atmosphere();
	

	// NO neded anymore and unsigned long has been replaced by int on LW 9.2
	// void * ui_get( unsigned long vid );
	// int ui_set( unsigned long vid, void *value );

	// this are the ones that replace them on the wrapper
	void *  DataGet (unsigned int vid);
	LWXPRefreshCode  DataSet (unsigned int vid, void *value);

	// Originally
	// static void button_event( LWXPanelID panel, unsigned long cid )
	// We changed the name to override the ButtonClick in plugin_handler.h of the lwpp
	virtual void ButtonClick(unsigned int cid);

	// Originally 
	// XCALL_( static void ) Destroy( AtmosphereData *dat )
	// We changed the name to override the PanelDestroyNotify in plugin_handler.h of the lwpp
	virtual void PanelDestroyNotify(void);
	//int Atmosphere_UI( int version, GlobalFunc *global, LWInterface *local, void *serverData );

	// this needs to be: (see plugin_handler.h -> XPanelInterface::Interface )
	virtual int Interface( int version, LWInterface *local, void *serverData );
	
	LWError NewTime( LWFrame f, LWTime t );
	double Evaluate(lwpp::VolumetricAccess voa);

	// Ray marching calculations
	void raymarchEval(lwpp::VolumetricAccess va);
	// Overall fog effect calculations similar to LW's standard fog 
	void analyticEval(lwpp::VolumetricAccess va);

	double evalTexture( LWMicropol *mp, double pos[ 3 ], double stride, double col[ 4 ], LWTextureID txtr );
	double GADD( lwpp::VolumetricAccess &va, double pp[ 3 ], double y, double falloff, int use_lighting, int atmos_type, double li[ 3 ] );

};


#endif // ATMOSPHERE_H
