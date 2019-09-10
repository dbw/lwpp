#include "lwpp/colour_management.h"
#include "lwpp/threads.h"

namespace lwpp
{    
  
  IMPLEMENT_GLOBAL(LWColorSpaceFuncs, LWPP_COLORSPACEFUNCS_GLOBAL);

  ColourManager::ColourManager(LWColorSpaceType _type, LWColourSpaceLayer layer, bool _supportLUT)
  : type(_type),
  mLayer(layer),
  toLinear(0),
  toColourSpace(0),
  useDefault(true), 
  ilt(0), 
  ilf(0),
  supportLUT(_supportLUT),
  Standalone(false)
  {
	  DBG_ENTER_FUNC;
    Update();
  }
  
  //! Default C'tor
  ColourManager::ColourManager(LWColourSpace _cspace, LWColourSpaceLayer layer)
  : cSpace(_cspace),
  mLayer(layer),
  toLinear(0),
  toColourSpace(0),
  useDefault(false), 
  ilt(0), 
  ilf(0),
  supportLUT(false),
  Standalone(true)
  {
	  DBG_ENTER_FUNC;
    Update();
  }
  
  ColourManager::ColourManager(ColourManager const& f)
  : cSpace(f.cSpace),
  mLayer(f.mLayer),
  toLinear(0),
  toColourSpace(0),
  useDefault(f.useDefault), 
  ilt(0), 
  ilf(0),
  supportLUT(f.supportLUT),
  Standalone(f.Standalone)
  {
	  DBG_ENTER_FUNC;
    Update();
  }  
  
  ColourManager& ColourManager::operator= (ColourManager const& f)
  {
    if (this != &f)  // Gracefully handle self assignment
    {  
      cSpace = f.cSpace;
      mLayer = f.mLayer;
      useDefault = f.useDefault;
      supportLUT = f.supportLUT;
      Standalone = f.Standalone;
      Update();
    }
    return *this;
  }
  
  bool ColourManager::operator!=(const ColourManager &comp) const
  {
    if (mLayer != comp.mLayer) return true;
    if (cSpace != comp.cSpace) return true;
    if (type != comp.type) return true;
    return false;
  }

  size_t ColourManager::popCount()
  {
    if (!available()) return 0;
    int showDefault = Standalone ? 0 : 1;
    if (supportLUT)
    {
      return globPtr->numberOfColorSpaces(mLayer) + 1 + showDefault; // Default first
    }
    else
    {
      return lwcs_sizeof + showDefault;
    }
  }

  const char *ColourManager::popName(int n)
  {
    if (!available()) return "";
    if (!Standalone)
    {
      if (n == 0) return "Default";
      --n;
    }
    if (supportLUT)
    {
      if (n == (globPtr->numberOfColorSpaces(mLayer) + 1)) return "(load table)";
    }
    return globPtr->nameOfColorSpaces(mLayer, n);
  }

  void ColourManager::Update()
  {

#ifdef _DEBUG
    //lwpp::dostream dout;
    //dout << this << "::Update() - useDefault: " << useDefault << std::endl; 
#endif 
	 updateGlobal();
    if (!available()) return;
    if (useDefault)
    {
      // get the default colour space for the current type
      char *cSpaceName = const_cast<char *>(globPtr->colorSpaceName(type));
#ifdef _DEBUG      
      //if (cSpaceName) dout << " type: " << type << std::endl; 
      //assert(cSpaceName != 0);
#endif
      if (cSpaceName)
      {
        cSpace = globPtr->nameToColorSpace(cSpaceName);
#ifdef _DEBUG
        //dout << "  ColourSpace name: \"" << cSpaceName << "\" for: " << type << std::endl; 
#endif 
      }
      // and get the LUT if appropriate
      fileLUT = const_cast<char *>(globPtr->colorSpaceFile(type));
    }
    // retrieve the function pointers
    toLinear = globPtr->getPixelConversionRGB(cSpace, lwcsc_colorspaceToLinear, &ilt);    
    toColourSpace = globPtr->getPixelConversionRGB(cSpace, lwcsc_linearToColorspace, &ilf); 
#ifdef _DEBUG
    //dout << "  ColourSpace: " << cSpace << std::endl; 
#endif     
  }

  int ColourManager::getIndex()
  {
    if (!Standalone)
    {
      if (useDefault) return 0;
      return globPtr->colorSpaceToIndex(mLayer, cSpace) + 1;
    }
    else
    {
      return globPtr->colorSpaceToIndex(mLayer, cSpace);
    }
  }
  void ColourManager::setIndex(int idx)
  {
    if (!Standalone)
    {
      useDefault = (idx == 0);
      --idx;
    }
    if (!useDefault)
    {
      cSpace = globPtr->indexToColorSpace(mLayer, idx);      
    }
    Update();
  }

  namespace
  {

    #define IO_VERS LWID_('V','E','R','S')  //!< Version
    #define IO_CSIX LWID_('C','S','I','X')  //!< Colour Space Index
    #define IO_CSNM LWID_('C','S','N','M')  //!< Colour Space Index
    #define IO_LUTF LWID_('L','U','T','F')  //!< LUT file

    static LWBlockIdent idroot[] =
    {
      {IO_VERS, "Version"}, // 0
      {IO_CSIX, "CSIndex"}, // 1
      {IO_CSNM, "CSName"}, // 2
      {IO_LUTF, "LUTFile"}, // 3
      0
    };
  }

  LWError ColourManager::Load(const LoadState &ls ) 
  {
    LWError err = 0;
    short version = -1;
    while (LWID id = ls.Find(idroot))
    {
      switch(id)
      {
      case IO_VERS:
        ls.read(version);
        if (version > 1)
        {
          err = "The lwpp::ColourManager settings require a newer version to read.";
        }
        useDefault = true;
        break;

      case IO_CSIX:
        cSpace = static_cast<LWColourSpace>(ls.readInt());
        useDefault = false;
        break;

      default:
        break;
      }
      ls.End();
    }
    Update();
    return err;
  }

  LWError ColourManager::Save(const SaveState &ss )
  {
    LWError err = 0;
    short version = 1;
    ss.WriteBlock(idroot[0], version);
    if (!useDefault)
    {
      int cSpaceIdx = cSpace;
	    ss.WriteBlock(idroot[1], cSpaceIdx);
    }
    return err;
  }
}