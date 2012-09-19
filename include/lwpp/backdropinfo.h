#ifndef LWPP_BACKDROPINFO_H
#define LWPP_BACKDROPINFO_H

#include <lwpp/wrapper.h>
#include <lwrender.h>

namespace lwpp
{
	//! Wrapper for LWBackdropInfo
	class BackdropInfo  : protected GlobalBase<LWBackdropInfo>
	{
	    public:
	    //! Constructor
        BackdropInfo(){};
        //! Methods
        void getBackdrop (LWTime t, const double ray[3], double color[3])
        { 
            return globPtr->backdrop(t, ray, color);
        }
  	    int getType(){return globPtr->type;}
        bool isSolid(){return (globPtr->type==LWBACK_SOLID);}
        void getColor(LWTime t,double zenith[3], double sky[3],double ground[3], double nadir[3])
  	    {
  		    return globPtr->color(t,zenith,sky,ground,nadir);
  	    }
        void getSqueeze(LWTime t, double *sky, double *ground)
        {
            return globPtr->squeeze(t, sky, ground);
        }
    };
}
#endif //LWPP_BACKDROPINFO_H