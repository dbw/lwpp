//--------------------------------------------------------------------------
// ComRing Interaction Example                                   by Bob Hood
//
// This CustomObject plug-in is a tool used by the RingTest LScript.  It
// displays a circular area around the camera object to which it is applied,
// and then adjusts that area in real time as messages are sent to it from
// the LScript using the ComRing.
//
// As part of the "example" nature of this project, a chunk of data is
// sent to the LScript for decoding whenever this plug-in is activated.
//

/*
======================================================================

Mike Wolf 
David Ballesteros 

LWPP VERSION  26 May 2007

====================================================================== */

#include "ringtest.h"

ringtest::ringtest(void *priv, void *context, LWError *err)
: lwpp::CustomObjectHandler(priv, context, err),
	target(0),
	radius(1.0)

{
	SetDescription("ComRing Test");

	me = Context;
	calcNewCircle();

	// We attach the plugin to the Communicator Ring
	ringAttach(RINGNAME);
	DummyData	dd;
  // test the caller's event function.  the ringtest.ls
  // script will display these values to the user when
  // they are decoded.

  memset(&dd,0,sizeof(DummyData));
  strcpy(dd.message[0],"Greetings!");
  strcpy(dd.message[1],"Salutations!");
  strcpy(dd.message[2],"Felicitations!");

  dd.an_int = 23876;
  dd.a_double = -5.49821;

  dd.array_float[0] = 1.1111f;
  dd.array_float[1] = 2.2222f;
  dd.array_float[2] = 3.3333f;
  dd.array_float[3] = 4.4444f;
  dd.array_float[4] = 5.5555f;

  // Sent a message to the Communicator Ring
	ringMessage(RINGNAME,1,(void *)&dd);
}

ringtest::~ringtest()
{
	ringDetach(RINGNAME);
}

// calculate a quarter of a circle, and mirror the points to
// the other three quadrants
void ringtest::calcNewCircle()
{
  double angle = 0.0;

  for(int x = 0; x < 9; x++)
  {
		double rad_angle = angle * 1.74532925199433E-002;

		circle_points_xz[x][0] = radius * sin(rad_angle);      // x
		circle_points_xz[x][1] = 0.0;
		circle_points_xz[x][2] = radius * cos(rad_angle);      // z

		circle_points_yz[x][0] = 0.0;
		circle_points_yz[x][1] = radius * sin(rad_angle);
		circle_points_yz[x][2] = radius * cos(rad_angle);

		circle_points_yx[x][0] = radius * sin(rad_angle);
		circle_points_yx[x][1] = radius * cos(rad_angle);
		circle_points_yx[x][2] = 0.0;

		rad_angle = (angle + 90.0) * 1.74532925199433E-002;

		circle_points_xz[x + 9][0] = radius * sin(rad_angle);
		circle_points_xz[x + 9][1] = 0.0;
		circle_points_xz[x + 9][2] = radius * cos(rad_angle);

		circle_points_yz[x + 9][0] = 0.0;
		circle_points_yz[x + 9][1] = radius * sin(rad_angle);
		circle_points_yz[x + 9][2] = radius * cos(rad_angle);

		circle_points_yx[x + 9][0] = radius * sin(rad_angle);
		circle_points_yx[x + 9][1] = radius * cos(rad_angle);
		circle_points_yx[x + 9][2] = 0.0;

		rad_angle = (angle + 180.0) * 1.74532925199433E-002;

		circle_points_xz[x + 18][0] = radius * sin(rad_angle);
		circle_points_xz[x + 18][1] = 0.0;
		circle_points_xz[x + 18][2] = radius * cos(rad_angle);

		circle_points_yz[x + 18][0] = 0.0;
		circle_points_yz[x + 18][1] = radius * sin(rad_angle);
		circle_points_yz[x + 18][2] = radius * cos(rad_angle);

		circle_points_yx[x + 18][0] = radius * sin(rad_angle);
		circle_points_yx[x + 18][1] = radius * cos(rad_angle);
		circle_points_yx[x + 18][2] = 0.0;

		rad_angle = (angle + 270.0) * 1.74532925199433E-002;

		circle_points_xz[x + 27][0] = radius * sin(rad_angle);
		circle_points_xz[x + 27][1] = 0.0;
		circle_points_xz[x + 27][2] = radius * cos(rad_angle);

		circle_points_yz[x + 27][0] = 0.0;
		circle_points_yz[x + 27][1] = radius * sin(rad_angle);
		circle_points_yz[x + 27][2] = radius * cos(rad_angle);

		circle_points_yx[x + 27][0] = radius * sin(rad_angle);
		circle_points_yx[x + 27][1] = radius * cos(rad_angle);
		circle_points_yx[x + 27][2] = 0.0;

		angle += 10.0;
  }
}

// as events are generated on our particular ComRing ("ringtest_channel", created
// by the controlling LScript), we will receive notification here in real-time.
// because it is in real-time -- i.e., we are holding onto the CPU time -- we must
// process here as quickly as possible.

void ringtest::RingEvent(void *portData,int eventCode,void *eventData)
{
	double          *ddata;
	int             *idata;
	DummyData       *dd;

	if(eventCode == (int)me)
	{
		ddata = (double *)eventData;
		radius = *ddata;
		// calculate a new circle
		calcNewCircle();
	}
	else if(eventCode == 1)     // target selection
	{
		idata = (int *) eventData;
		target = (LWItemID) *idata;
	}
	else if(eventCode == 2)     // more complex test data from the LScript
	{
		dd = (DummyData *)eventData;

		// you can examine the data here
		dd = 0;
	}
}

void ringtest::Evaluate(lwpp::CustomObjAccess &coa)
{
	float rgba[4] = {0.0f, 1.0f, 0.0f, 0.5f};

	coa.SetPattern(LWLPAT_SOLID);
	coa.SetColor(rgba);

	double center[3] = {0.0, 0.0, 0.0};
	for(int x = 0; x < 35; x++)
	{
			coa.DrawTriangle(center, circle_points_xz[x], circle_points_xz[x + 1],	LWCSYS_OBJECT);
	}

	coa.DrawTriangle (center,	circle_points_xz[35],	circle_points_xz[0],LWCSYS_OBJECT);
}

static ServerTagInfo comringTags[] =
{
	{"ComRingTest",SRVTAG_USERNAME},
	{"ComRingTest",SRVTAG_DESCRIPTION },
	{ NULL }
};

lwpp::CustomObjectAdaptor<ringtest> ringtestInstance("ComRingTest", comringTags);
