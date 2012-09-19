/*!
* @file
* @brief colous management functionality as added in LW 10.0
* @note this is a modified version of the header as supplied with LW 10.0. The original header is not included
* 
*/

#ifndef LWPP_COLORSPACE_H
#define LWPP_COLORSPACE_H

#include <lwpp/storeable.h>
#include <lwpp/wrapper.h>

namespace lwpp
{

#define LWPP_COLORSPACEFUNCS_GLOBAL "LWColorSpace"

  enum LWColourSpace {
    lwcs_linear = 0,                /*!< LightWave linear color space.      */
    lwcs_sRGB,                      /*!< Standard RGB color space.          */
    lwcs_rec709,                    /*!< Recommendation BT.709, HDTV.       */
    lwcs_cineon,                    /*!< Eastman Kodak Co.                  */
    lwcs_ciexyz,                    /*!< CIE XYZ.                           */
    lwcs_sizeof
  };

  enum LWColorSpaceType {
    lwcst_viewer = 0,               /*!< Viewer color space.                */
    lwcst_surface_color,            /*!< Surface       color space.         */
    lwcst_light_color, 
    lwcst_palette_files,            /*!< Palette files color space.         */
    lwcst_8bit_files,               /*!< 8 bit   files color space.         */
    lwcst_float_files,              /*!< Float   files color space.         */
    lwcst_alpha_files,              /*!< Alpha   files color space.         */
    lwcst_output,                   /*!< Output  files color space.         */
    lwcst_output_alpha,             /*!< Alpha   files color space.         */
    lwcst_sizeof
  };

  enum LWColourSpaceConversion {
    lwcsc_colorspaceToLinear = 0,   /*!< Convert from non-linear to linear. */
    lwcsc_linearToColorspace,       /*!< Convert from linear to non-linear. */
    lwcsc_sizeof
  };

  enum LWColourSpaceLayer {
    lwcsl_RGB = 0,                  /*!< RGB   Channel.                     */
    lwcsl_Alpha,                    /*!< Alpha Channel.                     */
    lwcsl_sizeof
  };

  struct st_lwimagelookup;
  /**
  *      struct st_lwimagelookup *
  *
  *      Is a forward reference to the internal lw instance structure ( defined internally ).
  *      This is used instead of a void * pointer.
  */

  typedef void LWPixelConversionRGBFunc(   struct st_lwimagelookup *, float *, float * ); /*!< Conversion function prototype. */
  typedef void LWPixelConversionAlphaFunc( struct st_lwimagelookup *, float *, float * ); /*!< Conversion function prototype. */

  /**
  *      Color space functions.
  *
  *      Color spaces file references are save in the scene file if they are used,
  *      in the viewer, color space file defaults or in a LWO from the image viewer.
  */

  typedef struct st_lwcolorspacefuncs {

    /* These function are used to get the color space of a loaded converter or load one from the disk. */

    LWColourSpace            (*nameToColorSpace       )( char * );               /*!< Color space name. */
    LWColourSpace            (*loadPixelLookupTable   )( char * );               /*!< File name.        */

    /* These functions are used to get the pixel conversion functions. */

    LWPixelConversionRGBFunc   *(*getPixelConversionRGB  )( LWColourSpace, LWColourSpaceConversion, struct st_lwimagelookup ** );
    LWPixelConversionAlphaFunc *(*getPixelConversionAlpha)( LWColourSpace, LWColourSpaceConversion, struct st_lwimagelookup ** );

    /* These functions are used to get color space names and file names, based on type. */

    const char             *(*colorSpaceName         )( LWColorSpaceType );    /*!< Gets the name of the current color space of the type selected. */
    const char             *(*colorSpaceFile         )( LWColorSpaceType );    /*!< Gets the file of the current color space of the type selected. */

    /* These functions are used in pop-ups. */

    int                     (*numberOfColorSpaces    )( LWColourSpaceLayer );    /*!< Gets the number of loaded color spaces in the layer.           */
    const char             *(*nameOfColorSpaces      )( LWColourSpaceLayer, int);/*!< Gets the name of the color space in the layer.                 */
    LWColourSpace            (*indexToColorSpace      )( LWColourSpaceLayer, int);/*!< Used to convert from indexes of pop-ups to color spaces.       */
    int                     (*colorSpaceToIndex      )( LWColourSpaceLayer, LWColourSpace );

    /* This functions is to change the currently selected color space. */

    int                     (*setColorSpace          )( LWColorSpaceType, LWColourSpace );

  } LWColorSpaceFuncs;

  
  
  class ColourManager : public PopUpCallback, public Storeable, public GlobalBase<LWColorSpaceFuncs>
  {
    LWPixelConversionRGBFunc *toLinear, *toColourSpace;
    st_lwimagelookup *ilt, *ilf;
    char *fileLUT;
    LWColorSpaceType type;
    LWColourSpace cSpace;
    LWColourSpaceLayer mLayer;    
    bool useDefault;
    bool Standalone;
    bool supportLUT;
  protected:
    //! Actual worker function to be supplied by the deriving class
    /*!
     * @return The number of items
     */
    virtual size_t popCount();
    //! Actual worker function to be supplied by the deriving class
    /*!
     * @param n A item index
     * @return The name of the item
     */
    virtual const char *popName(int n);
  public:
    //! Create a new manager that depends on a LWColorSpaceType defined by LW and optionally uses that setting as a default
    ColourManager(LWColorSpaceType _type, LWColourSpaceLayer layer = lwcsl_RGB, bool _supportLUT = false);

    //! Default C'tor, creates a standalone colour manager
    ColourManager(LWColourSpace _cspace = lwcs_linear, LWColourSpaceLayer layer = lwcsl_RGB);
    
    //! Copy C'tor
    ColourManager(ColourManager const& f);
    
    ColourManager& operator= (ColourManager const& f);

    bool operator!=(const ColourManager &comp) const;

    void Update();

    void convertToLinear(float *from, float *to)
    {
      if (toLinear)
      {
        toLinear(ilt, from, to);
      }
    }
    void convertToColourSpace(float *from, float *to)
    {
      if (toColourSpace)
      {
        toColourSpace(ilf, from, to);
      }
    }
    
    void convertToLinear(float *from)
    {
      if (toLinear)
      {
        toLinear(ilt, from, from);
      }
    }
    void convertToColourSpace(float *from)
    {
      if (toColourSpace)
      {
        toColourSpace(ilf, from, from);
      }
    }    
    
    float convertToLinear(float from)
    {
      if (toLinear)
      {
        float rgb[]= {from, 0.0, 0.0};
        toLinear(ilt, rgb, rgb);
        return rgb[0];
      }
      return from;
    }
    float convertToColourSpace(float from)
    {
      if (toColourSpace)
      {
        float rgb[]= {from, 0.0, 0.0};
        toColourSpace(ilf, rgb, rgb);
        return rgb[0];
      }
      return from;
    }
    
    int getIndex();

    void setIndex(int idx);

    virtual LWError Load(const LoadState &ls );

    virtual LWError Save(const SaveState &ss );
    
    LWColourSpace getColourSpace() const {return cSpace;}

  };

}
#endif // LWPP_COLORPSPACE_H
