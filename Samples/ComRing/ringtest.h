#ifndef RINGTEST_H
#define RINGTEST_H

#include <lwpp/customobject_handler.h>
#include <lwpp/vparm.h>
#include <lwpp/message.h>
#include <lwpp/comring.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cmath>

#define RINGNAME  "ringtest_channel"

struct DummyData
{
	char    message[3][100];
	int     an_int;
	double  a_double;
	float   array_float[5];
};

	
// this is the structure that holds the data for the plug-in

class ringtest : public lwpp::CustomObjectHandler, public lwpp::comRingCommunicator
{
private:
	double  radius;

	LWItemID    me;
	LWItemID    target;

	double  circle_points_xz[36][3];
	double  circle_points_yz[36][3];
	double  circle_points_yx[36][3];

public:
	// Constructor & Destructor
	ringtest(void *priv, void *context, LWError *err);
	virtual ~ringtest();

	// this structure holds some 'dummy' data that we will send to the LScript
	// to decode and display to the user.
	typedef struct RingTest *RingTestID;
	
	// calculate a quarter of a circle, and mirror the points to
	// the other three quadrants
	void calcNewCircle();

	// The calback function for the ringevent
	virtual void RingEvent(void *portData,int eventCode,void *eventData);

	// Flags()
	virtual unsigned int Flags() { return LWCOF_OVERLAY; }

	/*
	======================================================================
	Evaluate()
	Handler callback.
	====================================================================== */

	virtual void Evaluate(lwpp::CustomObjAccess &coa);

};

#endif //RINGTEST_H