#ifndef LWPP_LIGHTINFO_H
#define LWPP_LIGHTINFO_H

#include <lwpp/wrapper.h>
#include <lwpp/item.h>
namespace lwpp
{
  /*
void		(*ambient)      (LWTime, LWDVector color);
    int		(*shadowType)   (LWItemID);
    void		(*coneAngles)   (LWItemID, LWTime, double *radius, double *edge);
    LWImageID	(*projImage)    (LWItemID);
    int		(*shadMapSize)  (LWItemID);
    double		(*shadMapAngle) (LWItemID, LWTime);
    double		(*shadMapFuzz)  (LWItemID, LWTime);
    int		(*quality)	(LWItemID, LWTime);
        void            (*shadowColor)  (LWItemID, LWTime, LWDVector color);
  */

  //! Wrapper for LWObjectInfo
  //! @ingroup Globals
  /*!
   * Extends a LWItem by adding light specific functionality
   */
  class LWLight  : protected GlobalBase<LWLightInfo>, public LWItem
  {
  public:
    //! Constructor
    /*!
    * @param item the LWItemID of the item to retrieve information from.
    */
    LWLight(LWItemID item = 0)
      : LWItem (item, LWI_LIGHT)
    {
      ;
    }
    void setLight(LWItemID item) {SetID(item);}

    int getType() {return globPtr->type(GetID());}

    void getColour(LWTime time, LWDVector colour) {globPtr->color(GetID(), time, colour);}

    void getRawColour(LWTime time, LWDVector colour) {globPtr->rawColor(GetID(), time, colour);}

    double getIntensity(LWTime time) {return globPtr->intensity(GetID(), time);}

    double getRange(LWTime time) {return globPtr->range(GetID(), time);}

    int getFalloff() {return globPtr->falloff(GetID());}	

    unsigned int getFlags() {return globPtr->flags(GetID());}

    bool hasFalloff()
    {
      return ((getFlags() & LWLFL_LIMITED_RANGE) && (getFalloff() != LWLFALL_OFF));
    }
  };
}
#endif //LWPP_LIGHTINFO_H