#ifndef LWPP_SCENEINFO_H
#define LWPP_SCENEINFO_H

#include <lwpp/item.h>
#include <lwpp/matrix4x4.h>
#include <lwpp/camerainfo.h>
//#include <lwrender.h>

namespace lwpp
{
  //! @ingroup Globals
  class TimeInfo : public GlobalBase<LWTimeInfo>
  {
  public:
    TimeInfo() {	;	}
    LWTime Time() const
    {
      if (globPtr) return globPtr->time;
      else return 0;
    }
    LWFrame Frame() const
    {
      if (globPtr) return globPtr->frame;
      else return 0;
    }
  };

  inline LWFrame GetFrame()
  {
    TimeInfo ti;
    return ti.Frame();
  }

  inline LWTime GetTime()
  {
    TimeInfo ti;
    return ti.Time();
  }

  //! Provides information about the currently loaded scene
  /*!
   *	@ingroup Globals
   */
  class SceneInfo  : protected GlobalBase<LWSceneInfo>
  {
    static const char *filterNames[];
  public:
    //! Constructor
    SceneInfo() {	;	}

    //! Check if we have access to the global
    bool isValid() const {return globPtr != 0;}
    //! Returns the loaded scene name
    const char *name() const {return globPtr->name;}

    //! Returns the name of the file scene
    const char *filename() const {return globPtr->filename;}

    //! Returns the total number of points in the scene
    int numPoints() const {return globPtr->numPoints;}

    //! Returns the total number of polygons in the scene
    int numPolygons() const {return globPtr->numPolygons;}

    //! Returns the render type selected for the scene: LWRTYPE_WIRE, LWRTYPE_QUICK, LWRTYPE_REALISTIC
    int renderType() const {return globPtr->renderType;}

    /*!
    *  Returns the render options activated for the scene as flags:
    *	 - LWROPT_SHADOWTRACE
    *	 - LWROPT_REFLECTTRACE
    *	 - LWROPT_REFRACTTRACE
    *	 - LWROPT_FIELDS
    *	 - LWROPT_EVENFIELDS
    *	 - LWROPT_MOTIONBLUR
    *	 - LWROPT_DEPTHOFFIELD
    *	 - LWROPT_LIMITEDREGION
    *	 - LWROPT_PARTICLEBLUR
    *	 - LWROPT_ENHANCEDAA
    *	 - LWROPT_SAVEANIM
    *	 - LWROPT_SAVERGB
    *	 - LWROPT_SAVEALPHA
    *
    *  LWROPT_EVENFIELDS is only ON if Field Rendering is on, and the first line of the output image is from the field that comes first in time.
    */
    int renderOpts() const {return globPtr->renderOpts;}

    //! Returns the render start frame of the scene. This is not the same start frame as for the previews.
    LWFrame frameStart() const {return globPtr->frameStart;}

    //! Returns the render end frame of the scene. This is not the same end frame as for the previews.
    LWFrame frameEnd() const {return globPtr->frameEnd;}

    //! Returns the size of the render step frame of the scene. This is not the same end frame as for the previews.
    LWFrame frameStep() const {return globPtr->frameStep;}

    //! Returns the playback speed of the scene in frames / second.

    double framesPerSecond() const {return globPtr->framesPerSecond;}

    //! Returns the width of the image to render in pixels.
    int frameWidth() const {return globPtr->frameWidth;}

    //! Returns the height of the image to render in pixels.
    int frameHeight() const {return globPtr->frameHeight;}
    /*! Returns the pixel aspect ratio: Image Width / Image Height.
    *   If the returned value is greater than 1.0, the pixels are short wide.
    *   If the returned value is less than 1.0, the pixels are tall thin.
    */
    double pixelAspect() const {return globPtr->pixelAspect;}
    /*! Returns the limit for the minimum number of samples per pixel
    *   This can not be exactly computed due to the different render techniques, antialiasing modes and
    *	  adaptive sampling. But this gives a range based on the current render options.
    */
    int minSamplesPerPixel() const {return globPtr->minSamplesPerPixel;}
    /*! Returns the limit for the maximum number of samples per pixel
    *   This can not be exactly computed due to the different render techniques, antialiasing modes and
    *	  adaptive sampling. But this gives a range based on the current render options.
    */
    int maxSamplesPerPixel() const {return globPtr->maxSamplesPerPixel;}
    /*! Returns the measure in pixels of the limited region area.
    *   The extents are returned as a pointer to a array where limreg[0] = X0, limreg[1] = Y0, limreg[2] = X1, limreg[3] = Y1
    */
    int *limitedRegion() const {return globPtr->limitedRegion;}
    //! Returns maximum recursion depth for raytracing
    int recursionDepth() const {return globPtr->recursionDepth;}
    /*!
    * renderCamera(LWTime time) - Returns the render camera ID in the scene that will render the frame at the time specified
    */
    LWItemID renderCamera(LWTime time) const {return globPtr->renderCamera(time);}
    /*!
    * renderCamera - Returns the actual render camera ID at the current scene time
    */
    LWItemID  renderCamera ()
    {
      return renderCamera(GetTime());
    }
    //! Returns the number of threads used during the rendering
    int numThreads() const {return globPtr->numThreads;}
    //! Returns the name of the current animation file
    const char *animFilename() const {return globPtr->animFilename;}
    //! Returns the actual RGB prefix (i.e: tga, jpg,etc...) for the saved image file
    const char *RGBPrefix() const {return globPtr->RGBPrefix;}
    //! Returns the actual Alpha prefix (i.e: tga, jpg,etc...) for the saved alpha file
    const char *alphaPrefix() const {return globPtr->alphaPrefix;}
    /*!
    * antialiasing - Returns the actual level of antialiasing for the render camera
    *				 A value of cero means antialiasing is OFF
    *				 A value of 1 (lowest) to 4 (extreme) indicate the increasing antialiasing settings
    *				 A value of 100 to 114. Use PLD 1 = 100, PLD 2 = 101, PLD 3 = 102, ... and PLD 15 =  114)
    */
    int antialiasing() const {return globPtr->antialiasing;}
    //! Returns a boolean value indicating if Enhanced Antialiasing is active or not
    bool enhancedAA() const {return globPtr->enhancedAA ? true : false;}
    //! Returns a boolean value indicating if Adaptive Sampling is active or not
    const char *getAAName() const
    {
      static std::string ret;
      ret.clear();
      int aa = antialiasing();

      if (aa >= 100)
      {
        char buffer[256];
        sprintf(buffer,"PLD %i",aa - 99);
        ret = buffer;
      }
      else
      {
        if ((aa >= 0) && (aa < 5) && enhancedAA())	ret = "Enhanced ";
        switch (aa)
        {
          case 0:	ret =  "none"; break;
          case 1: ret += "Low"; break;
          case 2: ret += "Medium"; break;
          case 3: ret += "High"; break;
          case 4: ret += "Extreme"; break;
          default: ret += "unknown"; break;
        }
      }
      if (adaptiveSampling())
      {
        char buffer[256];
        sprintf(buffer," - AS (%f)", adaptiveThreshold());
        ret += buffer;
      }
      return ret.c_str();
    }
    bool adaptiveSampling (void) const {return globPtr->adaptiveSampling ? true : false;}
    //! Returns the adaptive sampling threshold, only valid if adaptive sampling is activated
    float adaptiveThreshold(void) const {return globPtr->adaptiveThreshold;}
    //! Returns a boolean value indicating if Soft Filter is active or not
    bool softfilter  (void) const {
      // we check the FIRST bit
      return (globPtr->filter | 0x0001) ? true : false;
    }

    // Enumeration for the filter names
    enum FilterTypes {
      CLASSIC,
      BOX, BOX_SHARP, BOX_SOFT,
      GAUSSIAN, GAUSSIAN_SHARP, GAUSSIAN_SOFT,
      MITCHELL, MITCHELL_SHARP, MITCHELL_SOFT,
      LANCZOS, LANCZOS_SHARP, LANCZOS_SOFT, UNKNOWN
    };

    //! Returns an integer value indicating the reconstruction filter selected for the current render camera
    /*!
    *	Example:
    *   lwpp::SceneInfo si;
    *   if (si.reconstructionFilter == SceneInfo::CLASSIC) {... };
    */
    FilterTypes reconstructionFilter () const
    {
      return static_cast<FilterTypes>((globPtr->filter >> 1) & 0x001F); // remove soft filter bit and mask out lower 5 bits
    }

    //! Returns a string indicating the reconstruction filter selected for the current render camera
    const char *reconstructionFilterName () const
    {
      FilterTypes filter = reconstructionFilter();
      if ((filter >= 0) && (filter < UNKNOWN))
      {
        return filterNames[filter];
      }
      else
      {
        return "unknown";
      }
     }

    // Returns a boolean value indicating if the area lights are double sided or not
    //bool dblSidedAreaLights(void) const {return globPtr->dblSidedAreaLights ? true : false;}


    double getCausticsIntensity() const  { return globPtr->causticsIntensity; }
    int getCausticsAccuracy() const { return globPtr->causticsAccuracy; }
    int getCausticsSoftness() const { return globPtr->causticsSoftness; }
    int getRadiosityFlags() const { return globPtr->radiosityFlags; }
    double getDynamicsRangeMin() const  { return globPtr->dynamicsRangeMin; }
    double getDynamicsRangeMax() const  { return globPtr->dynamicsRangeMax; }
    double getSaturation() const  { return globPtr->saturation; }

    LWRENDERINGMODE GetRenderMode() const { return (LWRENDERINGMODE) globPtr->rendermode; }
    /*
    int         pixelFilterForceMT;
    int         has_multithreaded_filters;     
    int         useZminmax;
    double      zmin;
    double      zmax;
    int         calculateallnormals;  
    */

		unsigned int lightSamples() const { return globPtr->lightsamples; }
		unsigned int reflectionSamples() const { return globPtr->reflectionsamples; }
		unsigned int refractionSamples() const { return globPtr->refractionsamples; }
		unsigned int scatteringSamples() const { return globPtr->scatteringsamples; }

  };

  // Supporting functions
  //! Generate a world to screen transformation matrix
  /*!
   * @param[in] mID LWItem of the camera
   * @param[in] time The time at which to generate the matrix
   * @return A 4x4 transformation matrix
   */
  Matrix4x4d getWorld2Screen(LWItemID id, LWTime time);

  //! Return the world to Screen transformation matrix for the current camera at the time specified
  Matrix4x4d getWorld2Screen(LWTime time);

  Matrix4x4d getWorld2Screen(LWItem cid, LWTime time);

  //! Transformation from world to screen coordinates.
  bool World2Screen(lwpp::Point3d pt, double scoord[2], lwpp::Matrix4x4d &CameraTransform, double frameAspect);

  //! Transformation from world to screen coordinates.
  bool World2Screen(lwpp::Vector3d dir, double scoord[2], lwpp::Matrix4x4d &CameraTransform, double frameAspect);

  //! Transformation from world to screen coordinates.
  /*!
  * requires SceneInfo
  * @param[in] pt Point in world coordinates
  * @param[out] scoord screen coordinates in fractional pixels
  * @param[in] camID ID of the camera
  * @param[in] time LWTime
  * @param[in] pixelAspect The pixel aspect of the camera
  * @return true if successfull, false if called from within Modeler
  */
  bool World2Screen(lwpp::Point3d pt, double scoord[2], LWItemID camID, LWTime time, double pixelAspect);

  //! Transformation from world to screen coordinates.
  /*!
  * requires SceneInfo
  * @param[in] pt Point in world coordinates
  * @param[out] scoord screen coordinates ind fractional pixels
  * @param[in] time LWTime
  * @return true if successfull, false if called from within Modeler
  */
  bool World2Screen(lwpp::Point3d pt, double scoord[2], LWTime time);

}

#endif // LWPP_SCENEINFO_H