#ifndef LWPP_NODES_H
#define LWPP_NODES_H

#include <lwpp/global.h>
#include <lwpp/vparm.h>
#include <lwpp/matrix4x4.h>
#include <lwnodes.h>
#include <memory>

namespace lwpp
{
	class NodeHandler;

	//! A base clasee for shaders that evaluate depending on light samples
	class ShadingEvaluator
	{
	protected:
		lwpp::Vector3d colour = { 0.0, 0.0, 0.0 };
	public:
		/*! Perform shading here
		 * This function is called once for every sample for the current light
		 * I suppose the return value is 1 if the spot is lit, 0 otherwise
		 */
		virtual void operator()(const double lightColour[3], float weight);
		virtual double brdf_pdf(const LWDVector lightDir) = 0;
		lwpp::Vector3d GetShading() const {return colour;}
		virtual void setValue(lwpp::NodeHandler *node, NodeValue value);
	};
	//! Creates a callback to sample illumination
	/*!
	 * Create a shading class derived from ShadingEvaluator
	 * Use the sampler:
	 * @code 

	 ShadingEvaluator shading; // if needed set variables needed by the evaluator
	 na->illuminateSample(lightid, na->wPos, dir, IlluminationSampler, &shading);

	 * @endcode
	 */
	//int IlluminationSampler( void *data, LWItemID light, const LWDVector dir, const double color[4] );
	
	extern const char inFuncs[];
	extern const char outFuncs[];

	//! Node Input
	/*!
	 * This class encapsulates a single node input
	 * @ingroup Globals
	 */
	class LWNodeInput // : protected GlobalBase<LWNodeInputFuncs>
	{
	private:
		static LWNodeInputFuncs *inF;
		NodeID mNodeID = 0;
		NodeInputID mID = 0;
		bool do_destroy = false;

		void init();
	public:
		LWNodeInput() { init(); }
		LWNodeInput(const NodeID _id, ConnectionType const type, const std::string name, NodeInputEvent* inp_event, const bool _destroy = true)
		: do_destroy(_destroy),
			mNodeID(_id)
		{
			init();
			mID = inF->create(mNodeID, type, name.c_str(), inp_event);
		}
		LWNodeInput(const NodeID _id, const std::string name, const LWID vendor, const LWID type, NodeInputEvent* inp_event, const bool _destroy = true)
			: do_destroy(_destroy),
			mNodeID(_id)
		{
			init();
			createCustom(name.c_str(), vendor, type, inp_event);
		}
		LWNodeInput(const NodeInputID _id, const bool _destroy = false)
		: do_destroy(_destroy),
			mID(_id)
		{
			init();
			mNodeID = node();
		}

		~LWNodeInput()
		{
			if(do_destroy) inF->destroy(mID);
			mID = 0;
		}

		void destroy()
		{
			if (mID) inF->destroy(mID);
		}

		NodeInputID getID() const {return mID;}

		int	evaluate(LWShadingGeometry *sg, NodeValue value)
		{
			return inF->evaluate(mID, sg, value);
		}

		int	evaluate(LWShadingGeometry *sg, LWDVector *value )
		{
			return evaluate(sg, static_cast<void *>(value));
		}

		int	evaluate(LWShadingGeometry *sg, double *value )
		{
			return evaluate(sg, static_cast<void *>(value));
		}

		int	evaluate(LWShadingGeometry *sg, lwpp::Vector3d &value )
		{
			return evaluate(sg, static_cast<void *>(value.asLWVector()));
		}

		int	evaluate(LWShadingGeometry *sg, int *value )
		{
			return evaluate(sg, static_cast<void *>(value));
		}

		int	evaluate(LWShadingGeometry *sg, bool *value )
		{
			int i = *value;
			int ret = evaluate(sg, &i);
			*value = (i != 0);
			return ret;
		}
		
		int	evaluate(LWShadingGeometry *sg, lwpp::VParm *vparm)
		{
			return evaluate(sg, static_cast<void *>(vparm->asLWVector()));
		}
		
		int	evaluateBSDF(LWShadingGeometry *sg, LWBSDF bsdf)
		{
			return evaluate(sg, static_cast<void *>(bsdf));
		}

		int	evaluate(LWShadingGeometry *sg, LWNodalProjection *projection)
		{
			return evaluate(sg, static_cast<void *>(projection));
		}
		/*
		template<class T>
		int evaluate(LWNodalAccess &na, T *value )
		{
			return evaluate(&na, value);
		}
		*/
		bool check(void)
		{
			return (inF->check(mID) != 0);
		}
		bool isConnected(void) {return check();}
		bool isID(NodeInputID id) const { return mID == id; }
		NodeID node(void) {return inF->node(mID);}
		void setNode(NodeID nID) { mNodeID = nID; }
		void disconnect (NodeID nid) 
		{
			inF->disconnect(mID);
		}
	 void disconnect() { inF->disconnect(mID); }

		NodeInputID next() {return inF->next(mID);}
		NodeInputID previous() {return inF->previous(mID);}
		const char *name()
		{
			return inF->name(mID);
		}
		void rename(const char *name)
		{
			inF->rename(mID, name);
		}
		ConnectionType type()
		{
			return inF->type(mID);
		}

		LWID vendorID() { return inF->vendorID(mID); }
		LWID typeID() { return inF->typeID(mID); }
		void createCustom(const char *name, LWID vendor, LWID type, NodeInputEvent* inp_event = nullptr)
		{
			if (mNodeID)
			{
				if (mID) inF->destroy(mID);
				mID = inF->createCustom(mNodeID, NOT_CUSTOM, name, inp_event, vendor, type);
			}
		}
		NodeOutputID connectedOutput() { return inF->connectedOutput(mID); }
	};

	typedef std::auto_ptr<LWNodeInput> auto_NodeInput; //!< Helper declaration for node member variables
	typedef std::unique_ptr<LWNodeInput> unique_NodeInput; //!< Helper declaration for node member variables
	typedef std::shared_ptr<LWNodeInput> shared_NodeInput; //!< Helper declaration for node member variables

		//! Node Input
	//! @ingroup Globals
	class LWNodeOutput // : protected GlobalBase<LWNodeOutputFuncs>
	{
	private:
		static LWNodeOutputFuncs *outF;
		NodeID mNodeID;
		NodeOutputID mID;
		bool do_destroy;

		void init();

	public:
		LWNodeOutput(NodeID _id, ConnectionType type, std::string name, bool _destroy = true)
		: do_destroy(_destroy),
			mNodeID(_id),
			mID(0)
		{
			init();
			mID = outF->create(mNodeID, type, name.c_str());
		}

		LWNodeOutput(NodeID _id, std::string name, LWID vendor, LWID type, bool _destroy = true)
			: do_destroy(_destroy),
			mNodeID(_id),
			mID(0)
		{
			init();
			createCustom(name.c_str(), vendor, type);
		}

		LWNodeOutput(NodeOutputID _id, bool _destroy = false)
			: do_destroy(_destroy),
			mNodeID(0),
			mID(_id)
		{
			init();
		}

		~LWNodeOutput()
		{
			if (do_destroy)	outF->destroy(mID);
		}
		void destroy()
		{
			if(mID) outF->destroy(mID);
		}
		NodeID node(void) {return outF->node(mID);}
		void setNode(NodeID nID) { mNodeID = nID; }
		NodeOutputID next(void) {return outF->next(mID);}
		NodeOutputID previous(void) {return outF->previous(mID);}
		NodeOutputID getID(void) const {return mID;}
		bool isID(const NodeOutputID _id) const {return mID == _id;}
		bool operator == (const NodeOutputID cmp)
		{
			return mID == cmp;
		}
		const char *name()
		{
			return outF->name(mID);
		}
		void rename(const char *name)
		{
			outF->rename(mID, name);
		}
		ConnectionType type()
		{
			return outF->type(mID);
		}
		LWID vendorID() { return outF->vendorID(mID); }
		LWID typeID() { return outF->typeID(mID); }
		void createCustom(const char *name, LWID vendor, LWID type)
		{
			if (mNodeID)
			{
				if (mID) outF->destroy(mID);
				mID = outF->createCustom(mNodeID, NOT_CUSTOM, name, vendor, type);
			}
		}
	};

	typedef std::auto_ptr<LWNodeOutput> auto_NodeOutput; //!< Helper declaration for node member variables
	typedef std::unique_ptr<LWNodeOutput> unique_NodeOutput; //!< Helper declaration for node member variables
	typedef std::shared_ptr<LWNodeOutput> shared_NodeOutput; //!< Helper declaration for node member variables
	class NodeInputHelper
	{

		virtual LWNodeInput *addInput(ConnectionType type, const std::string name, NodeInputEvent* inp_event) const = 0;
	public:
		/*! @name Node Inputs
		 *  Functions to add node inputs
		 */
		//@{
		LWNodeInput *addColorInput(const std::string name ="Color", NodeInputEvent* inp_event = 0) const
		{
			return addInput(NOT_RGB, name, inp_event);
		}
		LWNodeInput *addColourInput(const std::string name = "Colour", NodeInputEvent* inp_event = 0) const
		{
			return addColorInput(name, inp_event);
		}
		LWNodeInput *addRGBInput(const std::string name = "Colour", NodeInputEvent* inp_event = 0) const
		{
			return addColorInput(name, inp_event);
		}
		LWNodeInput *addScalarInput(const std::string name = "Scalar", NodeInputEvent* inp_event = 0) const
		{
			return addInput(NOT_SCALAR, name, inp_event);
		}
		LWNodeInput *addVectorInput(const std::string name = "Vector", NodeInputEvent* inp_event = 0) const
		{
			return addInput(NOT_VECTOR, name, inp_event);
		}
		LWNodeInput *addIntegerInput(const std::string name = "Integer", NodeInputEvent* inp_event = 0) const
		{
			return addInput(NOT_INTEGER, name, inp_event);
		}
		LWNodeInput *addFunctionInput(const std::string name = "Function", NodeInputEvent* inp_event = 0) const
		{
			return addInput(NOT_FUNCTION, name, inp_event);
		}
		LWNodeInput *addProjectionInput(const std::string name = "Projection", NodeInputEvent* inp_event = 0) const
		{
			return addInput(NOT_PROJECTION, name, inp_event);
		}
		LWNodeInput *addMatrix44Input(const std::string name = "Matrix", NodeInputEvent* inp_event = 0) const
		{
			return addInput(NOT_MATRIX44, name, inp_event);
		}
		LWNodeInput *addBSDFInput(const std::string name = "Material", NodeInputEvent* inp_event = 0) const
		{
			return addInput(NOT_BSDF, name, inp_event);
		}
		//@}
	};

	typedef std::auto_ptr<LWNodeInput> auto_NodeInput; //!< Helper declaration for node member variables
	typedef std::unique_ptr<LWNodeInput> unique_NodeInput; //!< Helper declaration for node member variables
	typedef std::shared_ptr<LWNodeInput> shared_NodeInput; //!< Helper declaration for node member variables

	//! Helper callbacks for node menus
	//! @ingroup Globals

	class NodeMenuBase
	{
	protected:
		LWNodeMenuFuncs mFuncs;
	public:
		virtual ~NodeMenuBase() {}
		virtual LWNodeMenuFuncs * getMenuFuncs() { return &mFuncs; }
	};

	template <typename T>
	class NodeMenu : public NodeMenuBase
	{
	public:
		virtual ~NodeMenu(){}
		static int countCB(NodeMenuInputType type, void* inputdata, void* userdata)
		{
			if (userdata)
			{
				auto plug = static_cast <T *>(userdata);
				return plug->NodeMenuCount(type, inputdata);
			}
			return 0;
		}
		static const char* nameCB(NodeMenuInputType type, void* inputdata, void* userdata, int item)
		{
			if (userdata)
			{
				auto plug = static_cast <T *>(userdata);
				return plug->NodeMenuName(type, inputdata, item);
			}
			return "";
		}
		static void eventCB(NodeMenuInputType type, void* inputdata, void* userdata, int item)
		{
			if (userdata)
			{
				auto plug = static_cast <T *>(userdata);
				plug->NodeMenuEvent(type, inputdata, item);
			}
		}

		NodeMenu(T *host)
		{
			mFuncs.userdata = host;
			mFuncs.countFn = countCB;
			mFuncs.nameFn = nameCB;
			mFuncs.eventFn = eventCB;
		}

		/*
		virtual int NodeMenuCount(NodeMenuInputType type, void* inputdata) = 0;
		virtual const char* NodeMenuName(NodeMenuInputType type, void* inputdata, int item) = 0;
		virtual void  NodeMenuEvent(NodeMenuInputType type, void* inputdata, int item) = 0;
		*/
	};

	//! LightWave Node
	//! @ingroup Globals
	class LWNode : private GlobalBase<LWNodeFuncs>, public NodeInputHelper
	{
	protected:
		NodeID id;
		static LWNodeInputFuncs *inF;
		static LWNodeOutputFuncs *outF;

		virtual LWNodeInput *addInput(ConnectionType type, const std::string name, NodeInputEvent* inp_event = 0) const
		{
		 return new LWNodeInput(id, type, name, inp_event);
		}
		// (NodeID _id, const std::string name, LWID vendor, LWID type, NodeInputEvent* inp_event, bool _destroy = true)
		virtual LWNodeInput *addInput(const LWID vendor, const LWID type, const std::string name, NodeInputEvent* inp_event = 0) const
		{
			return new LWNodeInput(id, name, vendor, type, inp_event);
		}
		void setValue(NodeValue val, void *v) { outF->setValue(val, v); }
		void disconnect(NodeInputID nid)
		{
			inF->disconnect(nid);
		}
	public:
		LWNode(NodeID _id = 0);
		virtual ~LWNode();

		void setID(NodeID _id) {id = _id;}
		NodeID getID(void) {return id;}

		const char *nodeName() const {return globPtr->nodeName(id);}
		const char *serverUserName() const {return globPtr->serverUserName(id);}
		LWChanGroupID chanGrp() const {return globPtr->chanGrp(id);}

		//! Set the GUI colours for this node
		void setNodeColor(int col[3]) {globPtr->setNodeColor(id, col);}
		//! Set the GUI colours for this node
		void setNodeColor(int r, int g, int b) {globPtr->setNodeColor3(id, r, g, b);}

		void setNodePreviewType(NodePreviewType type) {globPtr->setNodePreviewType(id, type);}

		//! Immediately update the preview for this node
		void UpdateNodePreview() {globPtr->UpdateNodePreview(id);}

		NodeInputID firstInput(void) {return inF->first(id);}
		int numInputs(void) {return inF->numInputs(id);}
		NodeInputID	inputByIndex (int n ) {return inF->byIndex(id, n);}
		int inputGetIndex (NodeInputID nid) 
		{      
			return inF->getIndex(nid);   
		}

		void disconnect (LWNodeInput *nid) 
		{
			inF->disconnect(nid->getID());
		}

		void setNodeMenuFuncs(LWNodeMenuFuncs *funcs)
		{
			globPtr->SetNodeMenuFuncs(id, funcs);
		}

		void setNodeMenuFuncs(NodeMenuBase &menu)
		{
			setNodeMenuFuncs(menu.getMenuFuncs());
		}

		/*! @name Outputs
		 *  Adding node outputs
		 */
		//@{
		LWNodeOutput *addOutput(LWID vendor, LWID customType, const std::string name)
		{
			return new LWNodeOutput(id, name, vendor, customType);
		}
		LWNodeOutput *addOutput(ConnectionType type, const std::string name)
		{
			return new LWNodeOutput(id, type, name);
		}
		LWNodeOutput *addColorOutput(const std::string name ="Color")
		{
			return addOutput(NOT_RGB, name);
		}
		LWNodeOutput *addColourOutput(const std::string name = "Colour")
		{
			return addColorOutput(name);
		}
		LWNodeOutput *addRGBOutput(const std::string name = "Colour")
		{
			return addColorOutput(name);
		}
		LWNodeOutput *addScalarOutput(const std::string name ="Scalar")
		{
			return addOutput(NOT_SCALAR, name);
		}
		LWNodeOutput *addVectorOutput(const std::string name ="Vector")
		{
			return addOutput(NOT_VECTOR, name);
		}
		LWNodeOutput *addIntegerOutput(const std::string name ="Integer")
		{
			return addOutput(NOT_INTEGER, name);
		}
		LWNodeOutput *addFunctionOutput(const std::string name ="Function")
		{
			return addOutput(NOT_FUNCTION, name);
		}
		LWNodeOutput *addProjectionOutput(const std::string name = "Projection")
		{
			return addOutput(NOT_PROJECTION, name);
		}
		LWNodeOutput *addMatrix44Output(const std::string name = "Matrix")
		{
			return addOutput(NOT_MATRIX44, name);
		}
		LWNodeOutput *addBSDFOutput(const std::string name = "Material")
		{
			return addOutput(NOT_BSDF, name);
		}
		// @}
		/*! @name auto_Outputs
		 *  Adding node outputs that are managed by std::auto_ptr
		 */
		//@{
		auto_NodeOutput autoColorOutput(const std::string name ="Color")
		{
			return auto_NodeOutput(addColorOutput(name));
		}
		auto_NodeOutput autoColourOutput(const std::string name = "Colour")
		{
			return auto_NodeOutput(addColorOutput(name));
		}
		auto_NodeOutput autoRGBOutput(const std::string name = "Colour")
		{
			return auto_NodeOutput(addColorOutput(name));
		}
		auto_NodeOutput autoScalarOutput(const std::string name ="Scalar")
		{
			return auto_NodeOutput(addScalarOutput(name));
		}
		auto_NodeOutput autoVectorOutput(const std::string name ="Vector")
		{
			return auto_NodeOutput(addVectorOutput(name));
		}
		auto_NodeOutput autoIntegerOutput(const std::string name ="Integer")
		{
			return auto_NodeOutput(addIntegerOutput(name));
		}
		auto_NodeOutput autoFunctionOutput(const std::string name ="Function")
		{
			return auto_NodeOutput(addFunctionOutput(name));
		}
		auto_NodeOutput autoProjectionOutput(const std::string name = "Projection")
		{
			return auto_NodeOutput(addProjectionOutput(name));
		}
		auto_NodeOutput autoMatrix44Output(const std::string name = "Matrix")
		{
			return auto_NodeOutput(addMatrix44Output(name));
		}
		auto_NodeOutput autoBSDFOutput(const std::string name = "Material")
		{
			return auto_NodeOutput(addBSDFOutput(name));
		}
		// @}

		NodeOutputID firstOutput(void) {return outF->first(id);}
		int numOutputs(void) const {return outF->numInputs(id);}
		NodeOutputID outputByIndex (int n ) const {return outF->byIndex(id, n);}
		int outputGetIndex (NodeOutputID nid) const
		{
			return outF->getIndex(nid);      
		}

		ConnectionType getType( NodeValue val) {return outF->getType(val);}
		/*! @name Returning values during the node evaluation
		 *  These functions can only be called in evaluate.
		 */
		//@{
		void setValue(NodeValue val, double *v)
		{
			setValue(val, static_cast<void *>(v));
		}
		void setValue(NodeValue val, LWDVector *v)
		{
			setValue(val, static_cast<void *>(v));
		}
		void setValue(NodeValue val, double a, double b, double c)
		{
			double v[3];
			v[0] = a;
			v[1] = b;
			v[2] = c;
			setValue(val, static_cast<void *>(v));
		}
		void setValue(NodeValue val, float v[3])
		{
			setValue(val, static_cast<double>(v[0]), static_cast<double>(v[1]), static_cast<double>(v[2]));
		}

		void setValue(NodeValue val, float v)
		{
			double d = v;
			setValue(val, &d);
		}

		void setValue(NodeValue val, double v)
		{
			setValue(val, &v);
		}

		void setValue(NodeValue val, int *v)
		{
			setValue(val, static_cast<void *>(v));
		}

		void setValue(NodeValue val, unsigned int *v)
		{
			unsigned int i = *v;
			setValue(val, static_cast<void *>(&i));
		}

		void setValue(NodeValue val, bool v)
		{
			int i = v;
			setValue(val, static_cast<void *>(&i));
		}

		void setValue(NodeValue val, Vector3d &v)
		{
			setValue(val, v.asLWVector());
		}
		//@}

		void *getValue( NodeValue val) {return outF->getValue(val);}
	};

	//! Utility class for nodes
	//! @ingroup Globals
	class LWNodeUtility : protected GlobalBase<LWNodeUtilityFuncs>
	{
	public:
		LWNodeUtility ()
		{;}
		static void Blend ( LWDVector result, LWDVector bg, LWDVector fg, double alpha, BlendingMode mode)
		{
			globPtr->Blend(result, bg, fg, alpha, mode);
		}
		static void	NodeAutosize( NodeID id, LWDVector scale, LWDVector position )
		{
			globPtr->NodeAutosize(id, scale, position);
		}

		static void PlanarMapping(const LWDVector pos, const LWDVector rot, const LWDVector scl,
															int axis, int world, int utiles, int vtiles, double uoffset, double voffset,
															const LWDMatrix4 refMatrix, const LWShadingGeometry* sg, LWNodalProjection* proj)
		{
			globPtr->planarMapping(pos, rot, scl, axis, world, utiles, vtiles, uoffset, voffset, refMatrix, sg, proj);
		}
        
        static void PlanarMapping(const lwpp::Vector3d &pos, const lwpp::Vector3d &rot, const lwpp::Vector3d &scl,
                                  int axis, int world, int utiles, int vtiles, double uoffset, double voffset,
                                  const lwpp::Matrix4x4d &refMatrix, const LWShadingGeometry* sg, LWNodalProjection* proj)
        {
            LWDMatrix4 mat;
            refMatrix.asLWMatrix(mat);
            return PlanarMapping(pos.asLWVector(), rot.asLWVector(), scl.asLWVector(),
                                 axis, world, utiles, vtiles, uoffset, voffset,
                                 mat, sg, proj);
        }
    };

	/*
	Various helper functions
	*/
	// Return a scalar from an input/vparm combo, can be used to initialise a variable
	double EvalScalarVInput(LWShadingGeometry *sg, lwpp::unique_NodeInput &lwni, lwpp::unique_VParm &vp);

	// Helps handling inputs with a matching vparm in DataGet()
	void *GetVInput(lwpp::unique_NodeInput &lwni, lwpp::unique_VParm &vp);

	extern const char *BlendModeStrings[];
	
/*
typedef struct	LWNodeUtilityFuncs_t {

	void			(*PlanarMapping)( LWDVector tPos, int axis, double *u, double *v );
	void			(*SphericalMapping)( LWDVector tPos, int axis, double *u, double *v );
	void			(*CylindricalMapping)( LWDVector tPos, int axis, double *u, double *v );
	void			(*CubicMapping)( LWDVector tPos, int axis, double *u, double *v );
					// The axis for cubic mapping is the major axis the surface normal points to.
	void			(*FrontMapping)( LWDVector tPos, double pixelAspect, double *u, double *v, double cameraMatrix[4][4] );
	void			(*ReflectionMapping)( double seamAngle, LWDVector direction, double *u, double *v );
	int			(*UVMapping)( const char* UVMapName, LWNodalAccess*, double *u, double *v );
					// Returns true if succesful.
	void		(*BuildCameraMatrix)( LWTime time, LWItemId cameraID, double cameraMatrix[4][4] );
					// Build the transformation matrix for front projection mapping to use.

} LWNodeUtilityFuncs;
*/

}

#endif // LWPP_NODES_H
