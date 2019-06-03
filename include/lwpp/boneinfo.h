#ifndef LWPP_BONEINFO_H
#define LWPP_BONEINFO_H

#include <lwpp/wrapper.h>
#include <lwpp/item.h>
namespace lwpp
{

  //! Wrapper for LWObjectInfo
  //! @ingroup Globals
  /*!
   * Extends a LWItem by adding light specific functionality
   */
  class Bone  : protected GlobalBase<LWBoneInfo>, public LWItem
  {
  public:
    //! Constructor
    /*!
    * @param item the LWItemID of the item to retrieve information from.
    */
		Bone(LWItemID item = 0)
      : LWItem (item, LWI_BONE)
    {
      ;
    }

		Bone(const LWItem &item)
			: LWItem(item)
		{
			;
		}

		double getStrength() { return globPtr->strength(GetID()); }
		double getFalloff() { return globPtr->falloff(GetID()); }
		double getTwist() { return globPtr->twist(GetID()); }
		const char *getWeightMap() { return globPtr->weightMap(GetID()); }
		unsigned int getFlags() { return globPtr->flags(GetID()); }
		void getRestParam(LWItemParam param, LWDVector rest) { globPtr->restParam(GetID(), param, rest); }
		double getRestLength() { return globPtr->restLength(GetID()); }
		void getLimits(double *inner, double *outer) { globPtr->limits(GetID(), inner, outer); }
		int getType() { return globPtr->type(GetID()); }
  };
}
#endif //LWPP_BONEINFO_H