#pragma once
       
#ifndef LWPP_COMPINFO_H
#define LWPP_COMPINFO_H

#include <lwpp/wrapper.h>
#include <lwrender.h>
#include <lwmath.h>

namespace lwpp
{
  //! Wrapper for LWCompInfo
  class CompInfo : protected GlobalBase<LWCompInfo>
  {
  public:
    //! Methods
    LWImageID getBackground() const  { return globPtr->bg; }
    LWImageID getForeground() const { return globPtr->fg; }
    LWImageID getForegroundAlpha() const { return globPtr->fgAlpha; }
#if (LW_VERSION > 2018)
    int getMode() const { return globPtr->mode; }
    void getColour(LWDVector col) const { VCPY(col, globPtr->color); }
    void getColor(LWDVector col) const { getColour(col); }
#endif
  };
}

#endif //LWPP_COMPINFO_H


