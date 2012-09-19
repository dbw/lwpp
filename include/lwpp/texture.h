/*!
 * @file
 * @brief
 * @version $Id: volumetric_handler.h 61 2007-10-12 12:39:39Z dballesg $
 */

#ifndef LWPP_TEXTURE_H
#define LWPP_TEXTURE_H

#include <lwpp/global.h>
#include <lwpp/vector3d.h>
#include <lwtxtr.h>

namespace lwpp
{
	class TextureLayer;
	//! A Texture is a set of TextureLayer that drive a parameter 
	/*!
	*  Give access to the Texture engine to create Image, Gradient or Procedural Textures
	*  that can be used even for modulate parameters on plugins.
  *  @ingroup Globals
	*/
	class Texture : protected GlobalBase<LWTextureFuncs>
	{
	private:
		LWTextureID txtr_id;
	public:
		//! Constructor
		Texture( LWTextureID _txtr_id = 0) : txtr_id(_txtr_id) {;}

		//! Returns the ID of the texture
		LWTextureID getID() const { return txtr_id; }

		// CONTEXTS

		/*! The texture context is used to populate and support the Input Parameter menu for gradient layers in the Texture Editor.
		*	This function creates a texture context. The Texture Editor uses the callbacks in the paramfuncs argument to get the value of the parameters.
		*   @param lwtxtparamfunc A LWTxtrParamFuncs structure that contains the callback to input parameters for the texture.
		*
		*	LWTxtrParamFuncs has the members:
		*		double (*paramEvaluate)(LWTxtrParamDesc *pdesc, int paramnum, LWMicropol *lwmicropol, gParamData userdata);
		*			Returns the value of the parameter.
		*			pdesc is a description that you pass to contextAddParam.
		*			paramnum is the index order in wich the parameter was created. 0 is reserved for Previous Layer, so your Input Parameters
		*			will start as 1, 2, etc... In the order you created them
		*			lwmicropol is a micropolygon passed to the texture
		*			userdata is your own data returned from your paramTime callback
		*		gParamData (*paramTime)(void *userdata, LWTxtrParamDesc *pdesc, int paramnum, LWTime time, LWFrame frame);
		*			An initialization function for the parameter. Is called before paramEvaluate to realize pre calculations for it.
		*			userdata is your own data returned from your userdata function
		*			paramnum is the index order in wich the parameter was created. 0 is reserved for Previous Layer, so your Input Parameters
		*			will start as 1, 2, etc... In the order you created them
		*			time is the time in seconds
		*			frame is the time in frames
		*		void (*paramCleanup) (LWTxtrParamDesc *, int paramnum, gParamData userdata);
		*			Frees any resources allocated in paramTime
		*			pdesc is a description that you pass to contextAddParam.
		*			paramnum is the index order in wich the parameter was created. 0 is reserved for Previous Layer, so your Input Parameters
		*			will start as 1, 2, etc... In the order you created them
		*			userdata is your own data returned with userdata function
		*/
		LWTxtrContextID contextCreate (LWTxtrParamFuncs lwtxtparamfunc) { return globPtr->contextCreate (lwtxtparamfunc); }
		//! Free the resources allocated by the previous function contextCreate.
    /*!
    * The context may only be destroyed if
		* all the textures using it has been destroyed first.
		* @param lwtxtrcontextid The context of the texture. You obtain this with the previous function contextCreate.
		*/
		void contextDestroy (LWTxtrContextID lwtxtrcontextid){ globPtr->contextDestroy (lwtxtrcontextid); }
		//! Add a parameter to the context.
    /*!
     * For Gradients this is the way to add an Input Parameter for them. The added parameter
		 * will appear on the list of Input Parameters for the Gradient.
		 * @param lwtxtrcontextid The context of the texture. You obtain this with the previous function contextCreate.
		 */
		void contextAddParam (LWTxtrContextID lwtxtrcontextid, LWTxtrParamDesc pdesc) { globPtr->contextAddParam (lwtxtrcontextid, pdesc); }

		// HANDLERS CALLS
		//! Creates a Texture.
    /*!
		*	will appear on the list of Input Parameters for the Gradient.
		*	@param returntype Is the type of value returned by the texture. It can be TRT_VECTOR, TRT_COLOR, TRT_PERCENT, TRT_SCALAR, TRT_DISPLACEMENT
		*	@param name This is the name of the texture that will appear on the Interface.
		* @param lwtxtrcontextid The id of the texture. It can contain one or more texture layers. You obtain this with the previous function contextCreate.
		*	Pass NULL if you do not want to create any Input Parameter.
		*	@param userdata is your own data returned with userdata function
		*/
		void create (int returntype, const char *name, LWTxtrContextID lwtxtrcontextid = 0, void *userdata = 0)
		{
			txtr_id = globPtr->create (returntype, name, lwtxtrcontextid, &userdata);
		}

		//! Destroy the texture created with create.
		void destroy ( ){ globPtr->destroy(txtr_id);}

		//! Copy a texture.
    /*!
		*	@param to ID of the destiny texture TO copy
		*	@param from ID of the source texture to copy from
		*/
		void copy(LWTextureID to, LWTextureID from){ globPtr->copy(to, from);}
		//! This allows do any pre calculation needed, that can depend on time
    /*!
		*	@param time is the time in seconds.
		*	@param frame is the time in frames.
		*/
		void newtime (LWTime time, LWFrame frame){ globPtr->newtime(txtr_id, time, frame);}

		//! You call this when the calculations for the texture are finished, after the last frame has been rendered for example.
		void cleanup ( ){ globPtr->cleanup(txtr_id);}

		//! Read the texture from a file.
		void load( const LWLoadState *loadstate ) { globPtr->load(txtr_id, loadstate);}

		//! Write the texture to a file.
		void save( const LWSaveState *savestate ) { globPtr->save(txtr_id, savestate);}

		//! Evaluates the texture.
    /*!
     * The double value returned is the alpha of the texture.
		 *	@param lwmp is a LWMicropol structure that must be initializated.
		 *	@param value is where the resulting value of the texture is returned.
		 */
		double evaluate (LWMicropolID lwmp, double *value) { return globPtr->evaluate(txtr_id, lwmp, value);}

		// TEXTURE DATA

		//! Creates a group that will contain the Envelopes created for parameters of the texture.
    /*!
		 *	@param txtr_group_id A LWChanGroupID of the envelopes group.
		 */
		void setEnvGroup (LWChanGroupID txtr_group_id);

 		/*! @name Layer Management
		 *  Access to the TextureLayer within the Texture
     */
    //@{

		//! Adds a new layer to the texture.
    /*!
		*	@param type Is the type of Layer to create. Can be TLT_IMAGE (Image Layer), TLT_PROC (Procedural Layer), TLT_GRAD (Gradient Layer)
		*/
		TextureLayer layerAdd (int type);
		//! Returns the ID of the first layer in the texture. This ID can be passed to functions the return the Layer data.
		TextureLayer firstLayer();
		//! Returns the ID of the last layer in the texture.  This ID can be passed to functions the return the Layer data.
		TextureLayer lastLayer();
		//! Returns the ID of the next layer in the texture, this is the Layer ID of the layer after the ID passed as parameter.
    /*!
		*	This ID can be passed to functions the return the Layer data.
		*/
		TextureLayer nextLayer(TextureLayer layer);

		LWTLayerID layerAddID (int type) { return globPtr->layerAdd (txtr_id, type); }

		//! Returns the ID of the first layer in the texture. This ID can be passed to functions the return the Layer data.
		LWTLayerID firstLayerID( ) { return globPtr->firstLayer(txtr_id); }

		//! Returns the ID of the last layer in the texture.  This ID can be passed to functions the return the Layer data.
		LWTLayerID lastLayerID( ) { return globPtr->lastLayer(txtr_id); }

		//! Returns the ID of the next layer in the texture, this is the Layer ID of the layer after the ID passed as parameter.
    /*!
		*	This ID can be passed to functions the return the Layer data.
		*/
		LWTLayerID nextLayerID(LWTLayerID lid) {	return globPtr->nextLayer(txtr_id, lid); }

    // @}

		//! Returns name of the texture.
		const char * name() { return globPtr->name(txtr_id); }

		//! Returns the type of value returned by the texture.
    /*!
		*	@return the type of value returned by the texture. It can be TRT_VECTOR, TRT_COLOR, TRT_PERCENT, TRT_SCALAR, TRT_DISPLACEMENT
		*/
		int getDataType () { return globPtr->type(txtr_id); }

		//! Returns the context of the texture. Is like the information of the first three parameter of the create function.
		LWTxtrContextID context (void) { return globPtr->context(txtr_id); }

	};

	// LAYERS

	/*! Wrapper for Texture Layers
	*   @ingroup Globals
	*
	*  Give access to the Texture engine to create Image, Gradient or Procedural Layers.
	*/
	class TextureLayer : public GlobalBase<LWTextureFuncs>
	{
	private:
		LWTLayerID id;
	public:
		TextureLayer(LWTLayerID _id = 0) : id(_id) {;}

		LWTLayerID getID() const {return id;}
		bool isValid () const {return id != 0;}

		//! Gets the Texture ID containing this layer.
		LWTextureID getTextureID() { return globPtr->texture(id); }

		//! Gets the type of the Layer.
		int getType() { return globPtr->layerType(id); }

		//! Set the type of the Layer.
    /*!
		 *	@param type Is the type of Layer to create. Can be TLT_IMAGE (Image Layer), TLT_PROC (Procedural Layer), TLT_GRAD (Gradient Layer)
		 */
		void setType(int type) { globPtr->layerSetType(id, type); }

 		/*! @name Setting Parameters
		 *  Changing paramaters for a TextureLayer
     */
    //@{

		//! Sets a parameter on a Layer.
    /*!
		*	@param tag Is the parameter of the Layer.
		*	@param data Is the data for the parameter.
		*/
		template <typename T>
		int setParam (int tag, T &data) { return globPtr->setParam(id, tag, &data); }

		//! Sets a parameter on a Layer.
    /*!
		*	@param tag Is the parameter of the Layer.
		*	@param data Is an integer value to set the parameter.
		*/
		int setParam (int tag, int data) { return globPtr->setParam(id, tag, &data); }

		//! Sets a parameter on a Layer.
    /*!
		*	@param tag Is the parameter of the Layer.
		*	@param data Is a pointer double value to set the parameter.
		*/
		int setParam (int tag, double data) { return globPtr->setParam(id, tag, &data); }

		//! Sets a parameter on a Layer.
    /*!
		*	@param tag Is the parameter of the Layer.
		*	@param data Is a double value to set the parameter.
		*/
		int setParam (int tag, double data[]) { return globPtr->setParam(id, tag, data); }

    //! Sets the position of a Layer.
    /*!
		*	@param position Is the double[3] with the new position.
		*/
		int setPosition (double position[3]) { return setParam(TXTAG_POSI, position); }

		//! Sets the rotation angles of a Layer.
    /*!
		*	@param rotation a double[3] with the new rotation angles as hpb.
		*/
		int setRotation (double rotation[3]) { return setParam(TXTAG_ROTA, rotation); }

		//! Sets the size of a Layer.
    /*!
		*	@param size Is the double[3] with the new size.
		*/
		int setSize (double size[3]) { return setParam(TXTAG_SIZE, size); }

		//! Sets the falloff of a Layer. It is an amout per unit of distance.
    /*!
		*	@param falloff Is the double[3] with the new fallof.
		*/
		int setFalloff (double falloff[3]) { return setParam(TXTAG_FALL, falloff); }

		//! Sets the Projection of an Image Layer.
    /*!
		*	@param projection is an int with the new projection. The values can accept are
		*   TXPRJ_PLANAR, TXPRJ_CYLINDRICAL, TXPRJ_SPHERICAL,
		*   TXPRJ_CUBIC, TXPRJ_FRONT, TXPRJ_UVMAP
		*/
		int setProjection (int projection) { return setParam(TXTAG_PROJ, projection); }

		//! Sets the Axis of projection of the Layer.
    /*!
		*	@param axis is an int with the new axis. The values can accept are ???????????????
		*/

		int setAxis (int axis) { return setParam(TXTAG_AXIS, axis); }

		//! Sets the Width Wrap amount of the Layer.
    /*!
		*	@param widthwrap is an double with the new width wrap.
		*/
		int setWidthWrap(double widthwrap) { return setParam(TXTAG_WWRP, widthwrap); }

		//! Sets the Height Wrap amount of the Layer.
    /*!
		*	@param heightwrap is an double with the new width wrap.
		*/
		int setHeightWrap (double heightwrap) { return setParam(TXTAG_WWRP, heightwrap); }

		//! Sets the Coordinate System amount of the Layer.
    /*!
		*	@param coordsys is an int with the new Coordinate System.
		*   0 for Object Coordinates. 1 for World Coordinates.
		*/
		int setCoordSystem (int coordsys) { return setParam(TXTAG_COORD, coordsys); }

		//! Sets the Image ID for a Image Layer.
    /*!
		*	@param imageID is an LWImageID with the new Image ID.
		*/
		int setImageID (LWImageID  imageID) { return setParam(TXTAG_IMAGE, imageID); }

		//! Sets the VMAP used for example on a UV Map Texture or Weight Map texture.
    /*!
		*	@param vmapID is an VMapID  with the new VMAP ID. It's value is the opaque pointer
		*   returned by the MeshEditOp pointVSet function or the mesh info pntVLookup function.
		*/
		int setVMapID (LWID  vmapID) { return setParam(TXTAG_VMAP, vmapID); }

		//! Sets the ID of the Reference Object used to drive origin, rotation and size of the Texture.
    /*!
		*	@param refObjID is an LWItemID with the new Item ID.
		*/
		int setReferenceItem (LWItemID refObjID) { return setParam(TXTAG_ROBJ, refObjID); }

		//! Sets the Layer opacity.
    /*!
		*	@param layerOpacity is an double with the new Layer opacity.
		*/
		int setLayerOpacity (double layerOpacity) { return setParam(TXTAG_OPAC, layerOpacity); }

		//! Turns on the antialiasing of  the Layer.
    /*!
		*	@param antialiasing is a bool value. True activates it, FALSE deactivates it.
		*/
		int setAntialiasing (bool antialiasing)
		{
			return setParam(TXTAG_AA, antialiasing ? 1 : 0);
		}

		//! sets the Texture Antialias Threshold. 
    /*!
     * The values of the texture will be antialiased only
		 *	if adjacent values are greater thant this threshold.
		 *	@param aaThreshold is an double with the new  Antialiasing threshold.
		 */
		int setAAThreshold (double aaThreshold) { return getParam(TXTAG_AAVAL, &aaThreshold); }

		//! Turns on the Pixel Blending.
    /*!
		*	@param pixelblending
		*/
		int setPixelBlending (bool pixelblending)
		{
			return setParam(TXTAG_PIXBLEND, pixelblending ? 1: 0);
		}

		//! Sets the Width Wrap behavior of the Layer.
    /*!
		*	@param widthwrap is an int pointer with the new width wrap behaviour. It accepts
		*    TXRPT_RESET, TXRPT_REPEAT, TXRPT_MIRROR, TXRPT_EDGE
		*/
		int setWidthWrapBehavior(int widthwrap) { return setParam(TXTAG_WREPEAT, widthwrap); }

		//! Sets the Height Wrap behavior of the Layer.
    /*!
		*	@param heightwrap is an int pointer with the new height wrap behaviour. It accepts
		*    TXRPT_RESET, TXRPT_REPEAT, TXRPT_MIRROR, TXRPT_EDGE
		*/

		int setHeightWrapBehavior (int heightwrap) { return setParam(TXTAG_HREPEAT, heightwrap); }

		//! Activates the Texture.
		bool activate(  ) {
			return (setParam(TXTAG_ACTIVE, 1) == 1) ? true: false;
		}

		//! DeActivates the Texture.
		bool deActivate(  ) {
			return (setParam(TXTAG_ACTIVE, 0) == 1) ? true: false;
		}

		//! Inverts the texture.
		bool invert(  ) {
			return (setParam(TXTAG_INVERT, 1) == 1) ? true: false;
		}

		//! Sets the blending mode of the layer.
    /*!
		*	@param blendmode is an int with the new blending mode. It can accept
		*   TXBLN_NORMAL, TXBLN_SUBTRACT, TXBLN_DIFFERENCE, TXBLN_MULTIPLY, TXBLN_DIVIDE, TXBLN_ALPHA, TXBLN_DISPLACE, TXBLN_ADD
		*/
		int setBlendingMode (int blendmode) { return getParam(TXTAG_BLEND, &blendmode); }

		//! Sets the Input Parameter index.
    /*!
		*	@param inputparameterindex is an int with the new index of the Input Parameter. It's value must be between 0 and 15.
		*/
		int setInputParameterIndex (int inputparameterindex) { return setParam(TXTAG_INPUT , inputparameterindex); }

		//! Sets the Anim UV Name.
    /*!
		*	@param auvName is an char pointer with the new name of the AnimUV.
		*/
		int setAuvName (char *auvName) { return setParam(TXTAG_AUVN , auvName); }

		//! Sets the Anim UV User.
    /*!
		*	@param auvUser is an char pointer with the new user of the AnimUV.
		*/
		int setAuvUser (char *auvUser) { return setParam(TXTAG_AUVU , auvUser); }

		//! Sets the Anim UV Plugin Options Array Byte Count.
    /*!
		*	@param auvArrByteCount is an int with the new byte count of the AnimUV.
		*/

		int setAuvArrayByteCount (int *auvArrByteCount) { return setParam(TXTAG_AUVC , auvArrByteCount); }

		//! Sets the Anim UV Plugin Options Array.
    /*!
		*	@param auvplugopts is an char pointer with the new plugin options AnimUV.
		*/
		int setAuvPluginOpts (char *auvplugopts) { return setParam(TXTAG_AUVO , auvplugopts); }

    //@}

    /*! @name Getting Paramaters
		 *  Retrieving parameters of a texture layer
     */
    //@{

		//! Gets a parameter from a Layer.
    /*!
		*	@param tag Is the parameter of the Layer.
		*	@param data Is the data to get from the parameter.
		*/
		int getParam (int tag, void *data) { return globPtr->getParam(id, tag, data); }

		template <typename T>
		T getParam (int tag) {
			T d;
			getParam(tag, &d);
			return d;
		}

		//! Gets the position of a Layer.
    /*!
		*	@param position Is the double[3] where to store the position.
		*/
		int getPosition (double position[3]) { return getParam(TXTAG_POSI, position); }

		//! Gets the rotation of a Layer.
    /*!
		*	@param rotation Is the double[3] where to store the rotation.
		*/

		int getRotation (double rotation[3]) { return getParam(TXTAG_ROTA, rotation); }
		//! Gets the size of a Layer.
    /*!
		*	@param size Is the double[3] where to store the size.
		*/

		int getSize (double size[3]) { return getParam(TXTAG_SIZE, size); }
		//! Gets the falloff of a Layer. It is an amout per unit of distance.
    /*!
		*	@param falloff Is the double[3] where to store the falloff.
		*/
		int getFalloff (double falloff[3]) { return getParam(TXTAG_FALL, falloff); }

		//! Gets the Projection of an Image Layer.
    /*!
		*	@param projection is an int pointer where to store the projection. The values can store are TXPRJ_PLANAR, TXPRJ_CYLINDRICAL,
		*   TXPRJ_SPHERICAL, TXPRJ_CUBIC, TXPRJ_FRONT, TXPRJ_UVMAP
		*/
		int getProjection (int *projection) { return getParam(TXTAG_PROJ, projection); }

		//! Gets the Axis of projection of the Layer.
    /*!
		*	@param axis is an int pointer where to store the axis. The values can store are ???????????????
		*/
		int getAxis (int *axis) { return getParam(TXTAG_AXIS, axis); }

		//! Gets the Width Wrap amount of the Layer.
		double getWidthWrap() {	return getParam<double>(TXTAG_WWRP); }

		//! Gets the Height Wrap amount of the Layer.
		double getHeightWrap() { return getParam<double>(TXTAG_HWRP); }

		//! Gets the Coordinate System amount of the Layer.
    /*!
		*	@param coordsys is an int pointer where to store the Coordinate System.
		*   0 for Object Coordinates. 1 for World Coordinates.
		*/
		int getCoordSystem (int *coordsys) { return getParam(TXTAG_COORD, coordsys); }

		//! Gets the Image ID for a Image Layer.
    /*!
		*	@param imageID is an LWImageID pointer where to store the Image ID.
		*/
		int getImageID (LWImageID  *imageID) { return getParam(TXTAG_IMAGE, imageID); }

		//! Gets the VMAP used for example on a UV Map Texture or Weight Map texture.
    /*!
		*	@param vmapID is an VMapID pointer where to store the VMAP ID. It's value is the opaque pointer
		*   returned by the MeshEditOp pointVSet function or the mesh info pntVLookup function.
		*/
		int getVMapID (LWID  *vmapID) { return getParam(TXTAG_VMAP, &vmapID); }

		//! Gets the ID of the Reference Object used to drive origin, rotation and size of the Texture.
    /*!
		*	@param refObjID is an LWItemID pointer where to store the Item ID.
		*/
		int getReferenceItem (LWItemID *refObjID) { return getParam(TXTAG_ROBJ, refObjID); }

		//! Gets the Layer opacity.
    /*!
		*	@param layerOpacity is an double pointer where to store the Layer opacity.
		*/

		int getLayerOpacity (double *layerOpacity) { return getParam(TXTAG_OPAC, layerOpacity); }
		//! Tells if the texture is antialias is enabled for the Texture.
    /*!
		*   TODO: Check the ones that returns boolean
		*/
		bool isAntialiased ( void )
		{
			int *isAA;
			getParam(TXTAG_AA, isAA);
			return (isAA != 0);
		}

		//! Gets the Texture Antialias Threshold.
    /*!
    * The values of the texture will be antialiased only
		*	if adjacent values they are greater thant this threshold.
		*	@param aaThreshold is an double pointer where to store the Texture Antialias Threshold.
		*/
		int getAAThreshold (double *aaThreshold) { return getParam(TXTAG_AAVAL, aaThreshold); }

		//! Check if the texture has Pixel Blending enabled for the Texture.
		bool hasPixelBlending (  )
		{
			int hasPB;
			getParam(TXTAG_PIXBLEND, &hasPB);
			return (hasPB != 0);
		}

		//! Gets the Width Wrap behavior of the Layer.
    /*!
		*	@param widthwrap is pointer to store the width wrap. It can return
		*    TXRPT_RESET, TXRPT_REPEAT, TXRPT_MIRROR, TXRPT_EDGE
		*/
		int getWidthWrapBehavior(int *widthwrap) { return getParam(TXTAG_WREPEAT, widthwrap); }

		//! Gets the Height Wrap behavior of the Layer.
    /*!
		*	@param heightwrap is pointer to the height wrap behaviour. It can return
		*    TXRPT_RESET, TXRPT_REPEAT, TXRPT_MIRROR, TXRPT_EDGE
		*/

		int getHeightWrapBehavior (int *heightwrap) { return getParam(TXTAG_HREPEAT, heightwrap); }
		//! Returns a boolean. true if the texture is active, false if not.
		bool isActive( )
		{
			int isAct;
			getParam(TXTAG_ACTIVE, &isAct);
			return ( isAct != 0);
		}

		//! Returns a boolean. true if the texture is inverted, false if not.
		bool isInverted( )
		{
			int isInv;
			getParam(TXTAG_INVERT, &isInv);
			return (isInv != 0);
		}

		//! Returns the blending mode of the layer.
    /*!
		*	@param blendmode is an int pointer where to store the blending mode. It can return
		*   TXBLN_NORMAL, TXBLN_SUBTRACT, TXBLN_DIFFERENCE, TXBLN_MULTIPLY, TXBLN_DIVIDE, TXBLN_ALPHA, TXBLN_DISPLACE, TXBLN_ADD
		*/
		int blendingmode (int *blendmode) { return getParam(TXTAG_BLEND, blendmode); }

		//! Returns Input Parameter index.
    /*!
		*	@param inputparameterindex is an int pointer where to store the index of the Input Parameter. It's value must be between 0 and 15.
		*/
		int inputParameterIndex (int *inputparameterindex) { return getParam(TXTAG_INPUT , inputparameterindex); }

		//! Returns Anim UV Name.
    /*!
		*	@param auvName is an char pointer where to store the name of the AnimUV.
		*/
		int auvName (char *auvName) { return getParam(TXTAG_AUVN , auvName); }

		//! Returns Anim UV User.
    /*!
		*	@param auvUser is an char pointer where to store the user of the AnimUV.
		*/
		int auvUser (char *auvUser) { return getParam(TXTAG_AUVU , auvUser); }

		//! Returns the Anim UV Plugin Options Array Byte Count.
    /*!
		*	@param auvArrByteCount is an int pointer where to store the array byte count of the AnimUV plugin.
		*/
		int auvArrayByteCount (int *auvArrByteCount) { return getParam(TXTAG_AUVC , auvArrByteCount); }

		//! Returns the Anim UV Plugin Options Array.
    /*!
		*	@param auvplugopts is an char pointer where to store the plugin options array of the AnimUV.
		*/
		int auvPluginOpts (char *auvplugopts) { return getParam(TXTAG_AUVO , auvplugopts); }

		//! Returns UV Coordinates
    /*! 
    * Returns UV Coordinates for a given position for Layers that use one of the implicit imaage mapping projections
		* like planar, cubic, cylindrical, or spherical). If the texture uses an explicit UV mapping, the UV coordinates
		* an be obtained directly from the vertex map through mesh info or MeshEditOp functions.
		*	@param wAxis is an int pointer where to store the dominant axis in WORLD coordinates for a cubic mapping.
		*	X = 0, Y = 1, Z=2
		*	This is usually chosen as the polygon normal component that's largest in absolute value.
		*	For projections other than cubic, these arguments are ignored.
		*	@param oAxis is an int pointer where to store the dominant axis in OBJECT coordinates for a cubic mapping.
		*	X = 0, Y = 1, Z=2
		*	This is usually chosen as the polygon normal component that's largest in absolute value.
		*	For projections other than cubic, these arguments are ignored.
		*	@param oPos is an double pointer to an array that specify the position for which the UV should be returned
		*   in OBJECT coordinates for a cubic mapping.
		*	@param wPos is an double pointer to an array that specify the position for which the UV should be returned
		*   in WORLD coordinates for a cubic mapping.
		*	@param uv is an double pointer to an array where to store the UV Coordinates.
		*/
		void evaluateUV( int wAxis, int oAxis, double oPos[3], double wPos[3], double uv[2])
		{
			globPtr->evaluateUV( id, wAxis, oAxis, oPos, wPos, uv);
		}

    // @}

		//! Provides access to a noise function.
    /*!
		*	@param pos is an array pointer where to store the noise returned value.
		*/
		double noise( double pos[3] )
		{
			return globPtr->noise(pos);
		}

		//! Returns the group associated with the LWTLayerID.
		LWChanGroupID getChannelGroup( void ) { return globPtr->layerEnvGroup(id); }

		bool nextLayer()
		{
			id = globPtr->nextLayer(getTextureID(), id);
			return isValid();
		}
	};

} // namespace lwpp

#endif // LWPP_TEXTURE_H