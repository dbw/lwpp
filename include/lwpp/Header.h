#pragma once

#ifndef LWPP_DOPETRACK_H
#define LWPP_DOPETRACK_H

#include "lwdopetrack.h"
#include <lwpp/wrapper.h>
#include <lwpp/item.h>
namespace lwpp
{	
	class Dopetrack : protected GlobalBase<LWLightInfo>, public LWItem
	{
	public:
		//! Constructor
		/*!
		* @param item the LWItemID of the item to retrieve information from.
		*/
		LWLight(LWItemID item = 0)
			: LWItem(item, LWI_LIGHT)
		{
			;
		}

		LWLight(const LWItem& item)
			: LWItem(item)
		{
			;
		}
		void setLight(LWItemID item) { SetID(item); }

		int getType() { return globPtr->type(GetID()); }

		void getColour(LWTime time, LWDVector colour) { globPtr->color(GetID(), time, colour); }

		void getRawColour(LWTime time, LWDVector colour) { globPtr->rawColor(GetID(), time, colour); }

		double getIntensity(LWTime time) { return globPtr->intensity(GetID(), time); }

		double getQuality(LWTime time) { return globPtr->quality(GetID(), time); }

		double getRange(LWTime time) { return globPtr->range(GetID(), time); }

		int getFalloff() { return globPtr->falloff(GetID()); }

		unsigned int getFlags() { return globPtr->flags(GetID()); }

		bool hasFalloff()
		{
			return ((getFlags() & LWLFL_LIMITED_RANGE) && (getFalloff() != LWLFALL_OFF));
		}
	};
}
#endif //LWPP_DOPETRACK_H
