#ifndef LWPP_CAMINFO_H
#define LWPP_CAMINFO_H

#include <lwpp/global.h>
#include <lwrender.h>
#include <lwpp/lw_version.h>
#include <lwpp/math.h>

namespace lwpp
{
	//! @ingroup Globals
	/*!
	*  Returns information about one of the existing cameras on the loaded scene
	*/
	class CameraInfo : protected GlobalBase<LWCameraInfo>
	{
	private:
		LWItemID camera;
	public:
		//! Constructor
		CameraInfo(LWItemID _camera = LWITEM_NULL)
			: camera(_camera)	{;}

		//! Returns the LWItemID of the camera
		LWItemID getID() const {return camera;}

		/*! Changes the LWItemID of the camera to a new one so the class can be used with more than one camera
		*   @param _camera ID of the new Camera to use
		*/
		void setID(LWItemID _camera)             {camera = _camera;}

		/*! Returns the zoom factor of the camera at an specified time
		*   @param time Specified time
		*/
		double zoomFactor (LWTime time) {return globPtr->zoomFactor(camera, time);}

		/*! Returns the focal length of the camera at an specified time
		*   @param time Specified time
		*/
		double focalLength (LWTime time) {return globPtr->focalLength(camera, time);}

		/*! Returns the focal distance of the camera at an specified time
		*   @param time Specified time
		*/
		double focalDistance (LWTime time) {return globPtr->focalDistance(camera, time);}

		/*! Returns the fstop number of the camera at an specified time
		*   @param time Specified time
		*/
		double fStop (LWTime time) {return globPtr->fStop(camera, time);}

		/*! Returns the blur length of the camera at an specified time
		*   If the usingGlobalMBR is true the value returned by this the one specified in the Render Globals panel
		*   @param time Specified time
		*/
		double blurLength (LWTime time) {return globPtr->blurLength(camera, time);}

		/*! Returns the Field Of View, Horizontal and Vertical angles of the camera at an specified time
		*   The values are specified in radians.
		*   @param time Specified time
		*   @param *hfov Pointer to double where horizontal FOV would be stored
		*   @param *vfov Pointer to double where vertical FOV would be stored
		*/
		void   fovAngles  (LWTime time, double *hfov, double *vfov) {globPtr->fovAngles (camera, time, hfov, vfov);}

		//! Returns the flags of the camera 
		unsigned int flags ()  {return globPtr->flags(camera);}

		//! Returns a boolean value indicating if the camera has Stero rendering active or not
		bool isStereo ()  {return (flags() & LWCAMF_STEREO);}

#ifndef LWCAMF_EYE
#define LWCAMF_EYE          (1<<9) /* In Stereo Rendering, This Indicates Which Eye Is Being Processed. */
#endif
		bool isRightEye ()  {return (flags() & LWCAMF_EYE) != 0;}

		//! Returns a boolean value indicating if the camera has a Limited Region active
		bool hasLimitedRegion ()  {return (flags() & LWCAMF_LIMITED_REGION) != 0;}

		//! Returns a boolean value indicating if the camera has a Mask active
		bool hasMask ()  {return (flags() & LWCAMF_MASK) != 0;}

		//! Returns a boolean value indicating if the camera has Depth of Field active
		bool hasDOF ()  {return (flags() & LWCAMF_DOF) != 0;}

		/*! Returns a boolean value indicating if the camera has Particle Blur active
		*  If the usingGlobalMBR is true the value returned by this about LWCAMF_PARTICLE_BLUR flag is the one specified in the Render Globals panel
		*/
		bool hasParticleBlur()  {return (flags() & LWCAMF_PARTICLE_BLUR) != 0;}

		//! Returns a boolean value indicating if the camera use a Limited Region without borders
		bool hasNoLRBorders ()  {return (flags() & LWCAMF_LR_NOBORDERS) != 0;}

		/*! Returns a string indicating which fiel is beign used by the camera
		*  If the usingGlobal is true the value returned by this about LWCAMF_FIELD flag is the one specified in the Render Globals panel
		*/
		const char *whichField ()  {return (flags() & LWCAMF_FIELD) ? "Even/Upper First" : "Odd/Lower First";}

		//! Returns a boolean value indicating that the camera is using a Camera Plugin
		bool usePlugin ()  {return (flags() & LWCAMF_USECAMTYPE) != 0;}

		/*! Returns a boolean value indicating if the camera supports Depth of Field
		*   If the camera has the DOF active must be checked with hasDOF();
		*/
		bool supportsDOF () {return (flags() & LWCAMF_SUPPORTS_DOF) != 0;}

		/*! Returns the width and height resolution in pixels that the camera will use during rendering
		*   @param *w Pointer to int where the horizontal resolution would be stored
		*   @param *h Pointer to int where the vertical resolution would be stored
		*/
		void   resolution(int *w, int *h) {globPtr->resolution(camera, w, h);}

		/*! Returns the pixel aspect ratio: Image Width / Image Height at an specified time
		*   If the returned value is greater than 1.0, the pixels are short wide.
		*   If the returned value is less than 1.0, the pixels are tall thin.
		*   If the usingGlobal is true the value returned by this is the pixelAspect specified in the Render Globals panel
		*   @param time Specified time
		*/
		double pixelAspect(LWTime time) {return globPtr->pixelAspect(camera, time);}

		/*! Returns a double value indicating the Eye Separation
		*   If the camera has the Stereo active must be checked with isStereo();
		*   @param time Specified time
		*/
		double separation(LWTime time) {return globPtr->separation(camera, time);}

		/*! Returns the measure in pixels of the limits for the limited region area.
		*   The extents are returned in four integer values that must be passed to it X0, Y0, X1, Y1
		*   @param *x0 Pointer to int where the left position in pixels would be stored
		*   @param *y0 Pointer to int where the top position in pixels would be stored
		*   @param *x1 Pointer to int where the right position in pixels would be stored
		*   @param *y1 Pointer to int where the bottom position in pixels would be stored
		*/
		void regionLimits(int *x0, int *y0, int *x1, int *y1) const {globPtr->regionLimits(camera, x0, y0, x1, y1);}

		/*! Returns the measure in pixels for the Mask of the camera.
		*   The extents are returned in four integer values that must be passed to it X0, Y0, X1, Y1
		*   If the camera has a Mask active must be checked with hasMask();
		*   @param *x0 Pointer to int where the left limit in pixels would be stored
		*   @param *y0 Pointer to int where the top limit in pixels would be stored
		*   @param *x1 Pointer to int where the right limit in pixels would be stored
		*   @param *y1 Pointer to int where the bottom limit in pixels would be stored
		*/
		void maskLimits(int *x0, int *y0, int *x1, int *y1) const {globPtr->maskLimits(camera, x0, y0, x1, y1);}

		/*! Returns the color of the camera mask
		*   If the camera has a Mask active must be checked with hasMask();
		*   @param *color Pointer to LWDVector where color of the mask would be stored
		*/
		void maskColor(LWDVector color) {globPtr->maskColor(camera, color);}

		/*! Returns a value indicating the type of MotionBlur that the camera has
		*   If the usingGlobal is true the value returned by this is the one specified in the Render Globals panel
		*   The possible returned values are
		*	LWCAMMB_OFF
		*	LWCAMMB_NORMAL
		*	LWCAMMB_DITHERED
		*/
		unsigned int motionBlur() {return globPtr->motionBlur(camera);}

		/*! Returns a value indicating the type of Field Rendering the camera has
		*   If the usingGlobal is true the value returned by this is the one specified in the Render Globals panel
		*   The possible returned values are
		*	LWCAMFIELDR_OFF
		*	LWCAMFIELDR_EVENFIRST
		*	LWCAMFIELDR_ODDFIRST
		*/
		unsigned int fieldRendering() {return globPtr->fieldRendering(camera);}

		/*! Returns a boolean value indicating if the camera DO NOT has Field Rendering
		*   If the usingGlobal is true the value returned by this is the one specified in the Render Globals panel
		*/
		bool hasNotFields() { return (this->fieldRendering() ==  LWCAMFIELDR_OFF) == 1;}

		/*! Returns a boolean value indicating if the camera has  Even First Field Rendering
		*   If the usingGlobal is true the value returned by this is the one specified in the Render Globals panel
		*/
		bool evenFirst() { return (this->fieldRendering() == LWCAMFIELDR_EVENFIRST) == 1;}

		/*! Returns a boolean value indicating if the camera has  Odd First Field Rendering
		*   If the usingGlobal is true the value returned by this is the one specified in the Render Globals panel
		*/
		bool oddFirst() { return (this->fieldRendering() == LWCAMFIELDR_ODDFIRST) == 1;}

		/*! Returns the iris position of the camera at an specified time and antialiasing pass?
		*   Usually this value is used on DOF and represent a sample point in the aperture of the canera.
		*   Has a range equal to: -1: lower height of aperture, 1: height of aperture
		*   The pas number can go from 0 to SceneInfo.maxSamplesPerPixel - 1.
		*   If there is a sample point, it returns 1, 0 if not exists a sample point.
		*   @param time Specified time
		*   @param pass Number of antialiasing pass
		*   @param *irisx Pointer to float where the x coordinate of the iris in the aperture would be stored. A value beteen -1..1
		*   @param *irisy Pointer to float where the y coordinate of the iris in the aperture would be stored. A value beteen -1..1
		*/
		int irisPos( LWTime time, int pass, float *irisx, float *irisy){return globPtr->irisPos(camera, time, pass, irisx, irisy);}

		//! Returns a boolean value indicating if the camera use the global settings in the Render Globals panel
		bool usingGlobalResolution ()  {return (globPtr->usingGlobalResolution( camera ) == 1);}

		//! Returns a boolean value indicating if the camera use the Motion Blur global settings in the Render Globals panel
		bool usingGlobalBlur ()  {return (globPtr->usingGlobalBlur( camera ) == 1);}

		//! Returns a boolean value indicating if the camera use the Mask global settings in the Render Globals panel
		bool usingGlobalMask ()  {return (globPtr->usingGlobalMask( camera ) == 1);}

		/*! Returns a value indicating the number of MotionBlur passes for the camera
		*   If the usingGlobal is true the value returned by this is the one specified in the Render Globals panel
		*   @param time Specified time
		*/
		int motionBlurPasses(LWTime time) {return globPtr->motionBlurPasses(camera, time);}

		/*! Returns a value indicating the Shutter Efficiency for the camera
		*   It is expresed as a decimal value: 0 = 0%, 0.5 = 50%, 1 = 100%
		*   If the usingGlobalBlur is true the value returned by this is the one specified in the Render Globals panel
		*   @param time Specified time
		*/
		double shutterEfficiency( LWTime time ) {return globPtr->shutterEfficiency(camera, time);}

		//! Returns the noiseSampler of the camera
		unsigned int noiseSampler() {return globPtr->noiseSampler(camera);}

		/*! Returns a value indicating size in meters of the Film Size for the camera
		*   If the usingGlobal is true the value returned by this is the one specified in the Render Globals panel
		*   @param *width Pointer to double where the horizontal film size would be stored.
		*   @param *height Pointer to double where the vertical film size would be stored.
		*/
		void filmSize (double *width, double *height) {globPtr->filmSize(camera, width, height);}

		/*! Returns a boolean value indicating if the structure with the timing information for the camera and
		*   a sepecified frame frame has been correctly filled. The frame value is a frame number, not a time value.
		*   Refer to the SDK docs for a complete explanation
		*   @param frame Frame number.
		*   @param *timingInfo Pointer to LWFrameTimingInfo struct.
		*/
		bool frameTiming (LWFrame frame, LWFrameTimingInfo *timingInfo) {return globPtr->frameTiming (camera, frame, timingInfo) ? true : false;}

		/*! Returns a value indicating the Antialiasing level for the camera
		*   @param time Specified time
		*/
		unsigned int antiAliasing ( LWTime time ) { return globPtr->antiAliasing (camera, time);}

		/*! Returns a value indicating the Oversampling number for the camera
		*   @param time Specified time
		*/
		double  overSampling ( LWTime time) { return globPtr->overSampling(camera, time);}

		/*! Fills the number of sides and it's rotation (in radians) of the diaphragm for the camera.
		*   @param time Specified time
		*   @param *sides Pointer to int where number of sides of the diaphragm would be stored.
		*   @param *rotation Pointer to double where the rotation of the diaphragm would be stored.
		*/
		void  diaphragm( LWTime time, int *sides, double* rotation ) { globPtr->diaphragm(camera, time, sides, rotation);}

	};

	//added evaluator
	//Usage, CameraEvaluator myEvaluator(LWItemID cameraID);
	//then myEvaluator.Evaluate(imagex,imagey,CameraRay*);
	class CameraEvaluator : protected GlobalBase<LWCameraEvaluationFuncs>
	{
	private:
		LWCameraEvaluatorID cameraEvaluator;

		bool init;
		bool newframe;
		bool cleanup;
	public:
		LWCameraRay camRay;
		//Simple constructor, loads first camera
		CameraEvaluator() : init(false), newframe(false), cleanup(false), cameraEvaluator(0)
		{
			ItemInfo myItem;
			Create(myItem.first(LWI_CAMERA, NULL));
		}
		//Define which camera you want to use
		CameraEvaluator(LWItemID _camera) : init(false), newframe(false), cleanup(false), cameraEvaluator(0)
		{
			Create(_camera);
		}
        CameraEvaluator(CameraInfo _caminfo) : init(false), newframe(false), cleanup(false), cameraEvaluator(0)
		{
			Create(_caminfo.getID());
		}
		~CameraEvaluator()
		{
			if(!cleanup)CleanUp();
			Destroy();
		}
		//Create evaluator
		void Create(LWItemID cameraID)
		{
			if(cameraEvaluator) Destroy();
			cameraEvaluator = globPtr->create(cameraID);
			init=false; newframe=false; cleanup=false;
		}

		void Destroy()
		{
			if(!cleanup) CleanUp();
			globPtr->destroy(cameraEvaluator);
		}

		LWError Init(int mode)
		{
			init=true;
			return globPtr->init(cameraEvaluator, mode);
		}

		void CleanUp()
		{
			cleanup=true;
			globPtr->cleanUp(cameraEvaluator);
		}

		LWError NewTime(LWFrame frame, LWTime time)
		{
			return globPtr->newTime(cameraEvaluator, frame, time);
		}

		int Preview(double lpx, double lpy, LWDMatrix4 projection)
		{
			return globPtr->preview(cameraEvaluator,  lpx, lpy, projection);
		}
		LWError NewFrame()
		{
			if(!init) Init(LWINIT_RENDER);
			newframe=true;
			return globPtr->newFrame(cameraEvaluator);
		}

		/*	Evaluation Functions
			Overloaded so you can use simple or complex calls
		*/
		/* Full Evaluation function */
		int Evaluate(double fpx, double fpy, double lpx, double lpy, double fractime, LWCameraRay* mycamray)
		{
			if(!init) Init(LWINIT_RENDER);
			if(!newframe) NewFrame();
			int retValue = globPtr->evaluate(cameraEvaluator,fpx,fpy,lpx,lpy,fractime,mycamray);
			camRay = *mycamray;
			return retValue;
		}
		/* Simple, non DOF lens, takes image position and time only */
		inline int Evaluate(double fpx, double fpy, double fractime)
		{
			return Evaluate(fpx, fpy, 0.0, 0.0, fractime, &camRay);
		}
	};
}
#endif // LWPP_CAMINFO_H