#include <lwpp/image.h>
#include <lwpp/xpanel.h>
#include <lwpp/comring.h>
#include <lwpp/colour_management.h>
#include <lwpp/file_request.h>
#include "lwmath.h"

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
		else if (n == 1) 
		{
				return "(load image)";
		}
    return globPtr->name(getNumImage(n - 2));
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
		return count + 2;	// add two for the "(none)" and (load) cases
  }

  void Image::GetPopUp(int i) 
  {
		if (i == 0)
		{
			SetID(0);
		}
		else if (i == 1)
		{
			// load image
			lwpp::DirInfo di(LWFTYPE_IMAGE);
			lwpp::FileRequest2 fr(FREQ_LOAD, "Load Image", LWFTYPE_IMAGE, "", "", LWFTYPE_IMAGE);
			if (!fr.Post())
			{
				SetID(0);
				return;
			}
			else
			{
				load(fr.getFullPath());
				return;
			}
		}
		else
		{
			SetID(getNumImage(i - 2));
		}
  }
  //! Set the value for a pop-up
  int Image::SetPopUp(void) 
  {
    if (isValid()) return getImageNum(id) + 2;
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
		if (reg == nullptr) return;
    lwpp::XPanel panel(pan);
    void *ud = panel.getUserData(cid);
    Image *img = static_cast<Image *>(ud);

		if (img == nullptr)
		{
			lwpp::XPDrawArea area(reg, w, h);
			area.clear(); // clear preview to black
			return;
		}
    img->drawXpanel(reg, w, h);
  }

  void Image::ControlZoom(LWXPanelID pan, unsigned int cid, int x, int y, int* rect, int clickcount)
  {
    if (pan == nullptr) return;
    lwpp::XPanel panel(pan);
    void* ud = panel.getUserData(cid);
    Image* img = static_cast<Image*>(ud);
    if (img != nullptr)
    {
      img->controlZoom( cid,  x,  y, rect, clickcount);
    }
  }

  /*! @todo add colourspace support

  */
  void Image::drawXpanel(LWXPDrAreaID reg, int w, int h)
  {
    lwpp::XPDrawArea area(reg, w, h);
    area.clear(); // clear preview to black
    if ( id == nullptr ) return;

    int width, height;
    size(width, height);

    float sx = (float)w / (float)width; // step size
    float sy = (float)h / (float)height;

    if ( mFlags.keepAspect )
    {
      float step = fmin(sx, sy); // smallest step
      sx = step;
      sy = step;
    }
    // scaled width and height
    const int sw = sx * width;
    const int sh = sy * height;

    const int x_offset = (w - sw) / 2;
    const int y_offset = (h - sh) / 2;

    ColourManager cm(lwpp::lwcst_viewer);

    const float pxStep = 1.0 / sx;
    const float pyStep = 1.0 / sy;
    float ix = 0.0;
    
    const bool displayAlpha = (hasAlpha() && mFlags.previewMode == PRV_RGBA);
    for ( int x = 0; x < sw; x++ )
    {
      float iy = 0.0;
      for ( int y = 0; y < sh; y++ )
      {                              
        LWBufferValue rgb[3];
        RGB(ix, iy, rgb);

         if (mFlags.previewMode > PRV_RGB)
         {
           float value = 0.f;
           switch (mFlags.previewMode)
           {
             case PRV_LUMA:
               value = 0.2126f * rgb[0] + 0.7152f * rgb[1] + 0.0722f * rgb[2];
               break;
             case PRV_R:
               value = rgb[0];
               break;
             case PRV_G:
               value = rgb[1];
               break;
             case PRV_B:
               value = rgb[2];
               break;
             case PRV_A:
               value = hasAlpha() ? alpha(ix, iy) : 0.0;
               break;
             default: 
               break;
           }
           rgb[0] = rgb[1] = rgb[2] = value;
         }
				if (mFlags.checkered && displayAlpha)
				{
					const float a = alpha(ix, iy);
					if (a < 1.0)
					{
						const int checkSize = 16.0;
						auto check = (x / checkSize + y / checkSize) % 2;
						auto col = check ? 0.1 : 0.2;
						col *= 1 - a;
						for (int i = 0; i < 3; ++i)
							rgb[i] += col;
					}
				}
				cm.convertToColourSpace(rgb);
        area.drawPixel(rgb, x + x_offset, y + y_offset);
        iy += pyStep;
      }
      ix += pxStep;
    }
  }

  void Image::drawXpanelZoom(LWXPDrAreaID reg, int w, int h, const unsigned int left, const unsigned int top,
                             const unsigned int right, const unsigned int bottom)
  {
    lwpp::XPDrawArea area(reg, w, h);
    area.clear(); // clear preview to black
    if (id == nullptr) return;

    int width, height;
    size(width, height);
    auto zoom_width = right - left+1;
    auto zoom_height = bottom - top+1;

    float sx = (float)w / (float)zoom_width; // step size
    float sy = (float)h / (float)zoom_height;

    if (mFlags.keepAspect)
    {
      float step = fmin(sx, sy); // smallest step
      sx = step;
      sy = step;
    }
    // scaled width and height
    const int sw = sx * zoom_width;
    const int sh = sy * zoom_height;

    const int x_offset = (w - sw) / 2;
    const int y_offset = (h - sh) / 2;

    ColourManager cm(lwpp::lwcst_viewer);

    const float pxStep = 1.0 / sx;
    const float pyStep = 1.0 / sy;
    float ix = left;
    const bool displayAlpha = (hasAlpha() && mFlags.previewMode == PRV_RGBA);
    for (int x = 0; x < sw; x++)
    {
      float iy = top;
      for (int y = 0; y < sh; y++)
      {
        LWBufferValue rgb[3];
        RGB(ix, iy, rgb);

        if (mFlags.previewMode > PRV_RGB)
        {
          float value = 0.0f;
          switch (mFlags.previewMode)
          {
            case PRV_LUMA:
              value = 0.2126f * rgb[0] + 0.7152f * rgb[1] + 0.0722f * rgb[2];
              break;
            case PRV_R:
              value = rgb[0];
              break;
            case PRV_G:
              value = rgb[1];
              break;
            case PRV_B:
              value = rgb[2];
              break;
            case PRV_A:
              value = hasAlpha() ? alpha(ix, iy) : 0.0;
              break;
            default:
              break;
          }
          rgb[0] = rgb[1] = rgb[2] = value;
        }
        if (mFlags.checkered && displayAlpha)
        {
          const float a = alpha(ix, iy);
          if (a < 1.0)
          {
            const int checkSize = 16.0;
            auto check = (x / checkSize + y / checkSize) % 2;
            auto col = check ? 0.1 : 0.2;
            col *= 1 - a;
            for (int i = 0; i < 3; ++i)
              rgb[i] += col;
          }
        }
        cm.convertToColourSpace(rgb);
        area.drawPixel(rgb, x + x_offset, y + y_offset);
        iy += pyStep;
      }
      ix += pxStep;
    }
  }

  void Image::drawNodePreview(NodeDraw& nd, int w, int h)
  {
    nd.drawBox(0,0,0, 0, 0, w, h);
    if (id == nullptr) return;

    int width, height;
    size(width, height);

    float sx = (float)w / (float)width; // step size
    float sy = (float)h / (float)height;

    if (mFlags.keepAspect)
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
    LWBufferValue rgb[3];
    bool displayAlpha = (hasAlpha() && mFlags.previewMode == PRV_RGBA);
    for (x = 0; x < sw; x++)
    {
      float iy = 0.0;
      for (y = 0; y < sh; y++)
      {
        RGB(ix, iy, rgb);

        if (mFlags.previewMode > PRV_RGB)
        {
          float value;
          switch (mFlags.previewMode)
          {
            case PRV_LUMA:
              value = 0.2126f * rgb[0] + 0.7152f * rgb[1] + 0.0722f * rgb[2];
              break;
            case PRV_R:
              value = rgb[0];
              break;
            case PRV_G:
              value = rgb[1];
              break;
            case PRV_B:
              value = rgb[2];
              break;
            case PRV_A:
              value = hasAlpha() ? alpha(ix, iy) : 0.0;
              break;
            default:
              break;
          }
          rgb[0] = rgb[1] = rgb[2] = value;
        }
        if (mFlags.checkered && displayAlpha)
        {
          float a = alpha(ix, iy);
          if (a < 1.0)
          {
            const int checkSize = 16.0;
            auto check = (x / checkSize + y / checkSize) % 2;
            auto col = check ? 0.1 : 0.2;
            col *= 1 - a;
            for (int i = 0; i < 3; ++i)
              rgb[i] += col;
          }
        }
        cm.convertToColourSpace(rgb);
        nd.drawPixel(rgb[0] * 255.0, rgb[1] * 255.0, rgb[2] * 255.0, x + x_offset, y + y_offset);
        iy += pyStep;
      }
      ix += pxStep;
    }
    nd.blit();
  }

	inline void normalize(LWDVector v)
	{
		auto mag = VLEN(v);
		VDIVS(v, mag);
	}

	lwpp::Vector3d ComputeBump(const LWNodalProjection &proj, double dtu, double dtv, double displace, double bumpStrength)
	{
		LWDVector rv = { 0.0, 0.0, 0.0 };

		if (dtu || dtv)
		{
			LWDVector dndu, dndv, nn;
			VCPY(dndu, proj.dPdu);
			VCPY(dndv, proj.dPdv);
			VCROSS(nn, dndu, dndv);
			normalize(nn);

			normalize(dndu);
			normalize(dndv);

			LWDVector d_dndu;
			d_dndu[0] = dndu[0] - dtu * nn[0] + displace * dndu[0];
			d_dndu[1] = dndu[1] - dtu * nn[1] + displace * dndu[1];
			d_dndu[2] = dndu[2] - dtu * nn[2] + displace * dndu[2];

			LWDVector d_dndv;
			d_dndv[0] = dndv[0] - dtv * nn[0] + displace * dndv[0];
			d_dndv[1] = dndv[1] - dtv * nn[1] + displace * dndv[1];
			d_dndv[2] = dndv[2] - dtv * nn[2] + displace * dndv[2];

			VCROSS(rv, d_dndu, d_dndv);
			VSCL(rv, -0.05 * displace * bumpStrength);
		}
		return lwpp::Vector3d(rv);
	}

	Vector3d Image::evaluateBumpGradient(const LWNodalProjection& proj,
                                       int pixelBlending, int useMip, double mipStrength, double bumpStrength,
                                       LWTextureWrap uWrap, LWTextureWrap vWrap) const
  {
    const double delta = 2.0;    // should be 0.5f;
		double baseTex[4], uTex[4], vTex[4];
		LWNodalProjection bProj = proj;

		auto du = std::abs(proj.dudx) + std::abs(proj.dudy);
		du *= delta; // should be 0.5f;
		if (du == 0) du = .0005f;

		auto dv = std::abs(proj.dvdx) + std::abs(proj.dvdy);
		dv *= delta; // should be 0.5f;
		if (dv == 0) dv = .0005f;

		// get three samples
		lwpp::Vector3d P(proj.P), dpdu(proj.dPdu), dpdv(proj.dPdv);

		baseTex[3] = evaluate(bProj, pixelBlending, useMip, mipStrength, uWrap, vWrap, baseTex);

		auto p = P + dpdu * du;
		for (int i = 0; i < 3; ++i)	bProj.P[i] = p[i];
		bProj.u = proj.u + du;
		uTex[3] = evaluate(bProj, pixelBlending, useMip, mipStrength, uWrap, vWrap, uTex);

		p = P + dpdv * dv;
		for (int i = 0; i < 3; ++i)	bProj.P[i] = p[i];
		bProj.v = proj.v + dv; bProj.u = proj.u;
		vTex[3] = evaluate(bProj, pixelBlending, useMip, mipStrength, uWrap, vWrap, vTex);

		auto displace = Colour2Luma(baseTex) * baseTex[3];
		auto udisplace = Colour2Luma(uTex) * uTex[3];
		auto vdisplace = Colour2Luma(vTex) * vTex[3];

		auto dtu = ((displace - udisplace) / du);
		auto dtv = ((displace - vdisplace) / dv);

		return lwpp::ComputeBump(proj, dtu, dtv, displace, bumpStrength);
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
