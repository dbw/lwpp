#include <lwpp/image.h>
#include <lwpp/xpanel.h>
#include <lwpp/comring.h>
#include <lwpp/colour_management.h>

#ifndef nullptr
#define nullptr 0
#endif

namespace lwpp
{
  int ImageUtil::findSaver(std::string name)
  {
    for (int i = 0; i < saverCount(); ++i)
    {
      if (name == saverName(i)) return i;
    }
    return -1; // not found
  }

  size_t ImageUtil::GetStride(int type)
  {
    size_t stride = 1;
    switch (type)
    {
    case LWIMTYP_RGB24:
      stride = 3;
      break;

    case LWIMTYP_RGBA32:
    case LWIMTYP_GREYFP:
      stride = 4;
      break;

    case LWIMTYP_RGBFP:
      stride = 4*3;
      break;

    case LWIMTYP_RGBAFP:
      stride = 4*4;
      break;

    case LWIMTYP_GREY8:
    case LWIMTYP_INDEX8:
      break;
    default:
      return 0;
      break;
    }
    return stride;
  }

  std::string ImageUtil::getSaverExtension(int n)
  {
    const char *name = saverName(n);
    std::string ext;
    if (name)
    {	
      std::string src(name);

      std::string::size_type start = src.find_last_of("(");
      std::string::size_type end = src.find_last_of(")");
      if ((start != std::string::npos) && (end != std::string::npos))
      {
        // remove extension including the previous separator
        ext = src.substr(start + 1, end-start-1);
        ext.erase(ext.find_last_of("."), 1);
      }
    }
    return ext;			
  }

  /*
  * class Image
  */ 
  const char *Image::popName(int n)
  {
    if (n == 0)
      return "(none)";
    else
      return globPtr->name(getNumImage(n - 1));
  }

  size_t Image::popCount(void) 
  {
    int count = 0;
    LWImageID _id = globPtr->first();
    while ( _id != 0)
    {
      count++;
      _id = globPtr->next(_id);
    }
    return count + 1;	// add one for the "(none)" case
  }

  void Image::GetPopUp(int i) 
  {
    if (i == 0)
    {
      SetID(0);
    }
    else
    {
      SetID(getNumImage(i-1));
    }
  }
  //! Set the value for a pop-up
  int Image::SetPopUp(void) 
  {
    if (isValid()) return getImageNum(id) + 1;
    return 0;
  }
  LWImageID Image::getNumImage(int n)
  {
    int count = 0;
    LWImageID _id;
    for (_id = globPtr->first(); count < n; count++, _id = globPtr->next(_id))
      ;
    return _id;
  }
  int Image::getImageNum(LWImageID i)
  {
    int count = 0;
    LWImageID _id = globPtr->first();
    while ( _id != 0)
    {
      if (_id == i) return count;
      count++;
      _id = globPtr->next(_id);
    }
    return 0; // image not found
  }
  
  void Image::DrawImage(LWXPanelID pan, unsigned int cid, LWXPDrAreaID reg, int w, int h)
  {
      if ( pan == nullptr ) return;
    lwpp::XPanel panel(pan);
    void *ud = panel.getUserData(cid);
    Image *img = static_cast<Image *>(ud);
    if ( img == nullptr ) return;
    if ( reg == nullptr ) return;
    img->drawXpanel(reg, w, h);
  }

  /*! @todo add colourspace support

  */
  void Image::drawXpanel(LWXPDrAreaID reg, int w, int h)
  {
    lwpp::XPDrawArea area(reg, w, h);
    area.drawBox(0, 0, 0, 0, 0, w, h); // clear preview to black
    if ( id == nullptr ) return;

    int width, height;
    size(width, height);

    float sx = (float)w / (float)width; // step size
    float sy = (float)h / (float)height;

    if ( mKeepAspect )
    {
      float step = fmin(sx, sy); // smallest step
      sx = step;
      sy = step;
    }
    // scaled width and height
    int sw = sx * width;
    int sh = sy * height;

    int x_offset = (w - sw) / 2;
    int y_offset = (h - sh) / 2;

    ColourManager cm(lwpp::lwcst_viewer);

    int x, y;
    float pxStep = 1.0 / sx;
    float pyStep = 1.0 / sy;
    float ix = 0.0;
    for ( x = 0; x < sw; x++ )
    {
      float iy = 0.0;
      for ( y = 0; y < sh; y++ )
      {
        LWBufferValue rgb[3];
        RGB(ix, iy, rgb);
        cm.convertToColourSpace(rgb);
        area.drawPixel(rgb, x + x_offset, y + y_offset);
        iy += pyStep;
      }
      ix += pxStep;
    }
  }


  namespace 
  {
    class ImageEventComring
    {
      static int usageCount;
      static comRingCommunicator *crc;

    public:
      static void ImageEvent(int eventCode, LWImageID id)
      {
        crc->ringMessage(IMAGECHANGE_COMRING, eventCode, id);
      }
      static void Register()
      {
        GlobalBase<LWImageList> il;
        if (il.available())
        {
          if (usageCount == 0)
          {
            crc = new comRingCommunicator();
            il->changeEvent(ImageEventComring::ImageEvent, LWERC_REGISTER);
          }
          ++usageCount;
        }
      }
      static void Unregister()
      {
        GlobalBase<LWImageList> il;
        if (il.available())
        {
          --usageCount;            
          if (usageCount == 0)
          {
            delete crc;
            crc = 0;
            il->changeEvent(ImageEventComring::ImageEvent, LWERC_UNREGISTER);
          }
        }
      }

    };
    int ImageEventComring::usageCount = 0;
    comRingCommunicator *ImageEventComring::crc = 0;
    static ImageEventComring iec;
  }

  void RegisterImageChangeComRing() {iec.Register();}
  void UnregisterImageChangeComRing()  {iec.Unregister();}
}