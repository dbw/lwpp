#ifndef LWPP_NODES_H
#define LWPP_NODES_H

#include <lwpp/global.h>
#include <lwpp/vparm.h>
#include <lwnodes.h>

namespace lwpp
{
	class NodeHandler;

	//! A base clasee for shaders that evaluate depending on light samples
	class ShadingEvaluator
	{
	protected:
		lwpp::Vector3d colour;
	public:
		/*! Perform shading here
		 * This function is called once for every sample for the current light
	   * I suppose the return value is 1 if the spot is lit, 0 otherwise
		 */
		virtual int operator()(LWItemID light, const LWDVector lightDir, const LWDVector lightColour ) = 0;
		lwpp::Vector3d GetShading() const {return colour;}
		void setValue(lwpp::NodeHandler *node, NodeValue value);
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
	int IlluminationSampler( void *data, LWItemID light, const LWDVector dir, const LWDVector color );
  
	extern const char inFuncs[];
	extern const char inFuncs2[];
  extern const char inFuncs3[];
  

	//! Node Input
	/*!
   * This class encapsulates a single node input
   * @ingroup Globals
   */
	class LWNodeInput // : protected GlobalBase<LWNodeInputFuncs>
	{
	private:
		static LWNodeInputFuncs *inF;
		NodeID node_id;
		NodeInputID id;
		bool do_destroy;

		int	evaluate(LWNodalAccess*na, void* value )
		{
			return inF->evaluate(id, na, value);
		}
		void init();
	public:
		LWNodeInput(NodeID _id, ConnectionType type, const std::string name, NodeInputEvent* inp_event, bool _destroy = true)
		: do_destroy(_destroy),
			node_id(_id)
		{
			init();
			id = inF->create(node_id, type, name.c_str(), inp_event);
		}
		LWNodeInput(NodeInputID _id, bool _destroy = false)
		: do_destroy(_destroy),
			node_id(0),
			id(_id)
		{
			init();
		}

		~LWNodeInput()
		{
			if (do_destroy) inF->destroy(node_id, id);
		}

		NodeInputID getID(void) const {return id;}

		int	evaluate(LWNodalAccess*na, LWDVector *value )
		{
			return evaluate(na, static_cast<void *>(value));
		}

		int	evaluate(LWNodalAccess*na, double *value )
		{
			return evaluate(na, static_cast<void *>(value));
		}

		int	evaluate(LWNodalAccess*na, lwpp::Vector3d &value )
		{
			return evaluate(na, static_cast<void *>(value.asLWVector()));
		}

		int	evaluate(LWNodalAccess*na, int *value )
		{
			return evaluate(na, static_cast<void *>(value));
		}

		int	evaluate(LWNodalAccess*na, bool *value )
		{
			int i = *value;
			int ret = evaluate(na, &i);
			*value = (i != 0);
			return ret;
		}

		int	evaluate(LWNodalAccess*na, lwpp::VParm *vparm)
		{
			return evaluate(na, static_cast<void *>(vparm->asLWVector()));
		}

		int	evaluate(LWNodalAccess*na, LWNodalMaterial *mat)
		{
			return evaluate(na, static_cast<void *>(mat));
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
			return (inF->check(id) != 0);
		}
		bool isConnected(void) {return check();}
		NodeID	node(void) {return inF->node(id);}
		void disconnect (NodeID nid) {inF->disconnect(nid, id);}
		NodeInputID next() {return inF->next(id);}
		NodeInputID previous() {return inF->previous(id);}

	};

  typedef std::auto_ptr<LWNodeInput> auto_NodeInput; //!< Helper declaration for node member variables

	extern const char outFuncs[];
	extern const char outFuncs2[];

		//! Node Input
	//! @ingroup Globals
	class LWNodeOutput // : protected GlobalBase<LWNodeOutputFuncs>
	{
	private:
    static LWNodeOutputFuncs *outF;
		NodeID node_id;
		NodeOutputID id;
		bool do_destroy;

    void init();

	public:
		LWNodeOutput(NodeID _id, ConnectionType type, std::string name, bool _destroy = true)
		: do_destroy(_destroy),
			node_id(_id),
			id(0)
		{
      init();
			id = outF->create(node_id, type, name.c_str());
		}

		~LWNodeOutput()
		{
			if (do_destroy) outF->destroy(node_id, id);
		}
		NodeID node(void) {return outF->node(id);}
		NodeOutputID next(void) {return outF->next(id);}
		NodeOutputID previous(void) {return outF->previous(id);}
		NodeOutputID getID(void) const {return id;}
		bool isID(const NodeOutputID _id) const {return id == _id;}
    bool operator == (const NodeOutputID cmp)
    {
      return id == cmp;
    }
	};

  typedef std::auto_ptr<LWNodeOutput> auto_NodeOutput; //!< Helper declaration for node member variables

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
			return addInput(NOT_COLOR, name, inp_event);
		}
		LWNodeInput *addColourInput(const std::string name = "Colour", NodeInputEvent* inp_event = 0) const
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
		LWNodeInput *addMaterialInput(const std::string name = "Material", NodeInputEvent* inp_event = 0) const
		{
			return addInput(NOT_MATERIAL, name, inp_event);
		}
    //@}
	};

	//! LightWave Node
	//! @ingroup Globals
	class LWNode : private GlobalBase<LWNodeFuncs>, public NodeInputHelper
	{
	private:
		NodeID id;
		static LWNodeInputFuncs *inF;
		static LWNodeOutputFuncs *outF;

		virtual LWNodeInput *addInput(ConnectionType type, const std::string name, NodeInputEvent* inp_event) const
		{
		 return new LWNodeInput(id, type, name.c_str(), inp_event);
		}

		LWNodeOutput *addOutput(ConnectionType type, const std::string name)
		{
			return new LWNodeOutput(id, type, name.c_str());
		}
		void setValue( NodeValue val, void *v) {outF->setValue(val, v);}
		void disconnect (NodeInputID nid) {inF->disconnect(id, nid);}

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
		int inputGetIndex (NodeInputID nid) {return inF->getIndex(id, nid);}

		void disconnect (LWNodeInput *nid) {inF->disconnect(id, nid->getID());}

    /*! @name Outputs
		 *  Adding node outputs
     */
    //@{
		LWNodeOutput *addColorOutput(const std::string name ="Color")
		{
			return addOutput(NOT_COLOR, name);
		}
		LWNodeOutput *addColourOutput(const std::string name = "Colour")
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
		LWNodeOutput *addMaterialOutput(const std::string name = "Material")
		{
			return addOutput(NOT_MATERIAL, name);
		}
    // @}
    /*! @name auto_Outputs
		 *  Adding node outputs that are managed by std::auto_ptr
     */
    //@{
		auto_NodeOutput autoColorOutput(const std::string name ="Color")
		{
			return auto_NodeOutput(addOutput(NOT_COLOR, name));
		}
		auto_NodeOutput autoColourOutput(const std::string name = "Colour")
		{
			return auto_NodeOutput(addColorOutput(name));
		}
		auto_NodeOutput autoScalarOutput(const std::string name ="Scalar")
		{
			return auto_NodeOutput(addOutput(NOT_SCALAR, name));
		}
		auto_NodeOutput autoVectorOutput(const std::string name ="Vector")
		{
			return auto_NodeOutput(addOutput(NOT_VECTOR, name));
		}
		auto_NodeOutput autoIntegerOutput(const std::string name ="Integer")
		{
			return auto_NodeOutput(addOutput(NOT_INTEGER, name));
		}
		auto_NodeOutput autoFunctionOutput(const std::string name ="Function")
		{
			return auto_NodeOutput(addOutput(NOT_FUNCTION, name));
		}
		auto_NodeOutput autoMaterialOutput(const std::string name = "Material")
		{
			return auto_NodeOutput(addOutput(NOT_MATERIAL, name));
		}
    // @}

		NodeOutputID firstOutput(void) {return outF->first(id);}
		int numOutputs(void) {return outF->numInputs(id);}
		NodeOutputID outputByIndex (int n ) {return outF->byIndex(id, n);}
		int outputGetIndex (NodeOutputID nid) {return outF->getIndex(id, nid);}

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

		void setValue(NodeValue val, LWNodalMaterial *material)
		{
			setValue(val, static_cast<void *>(material));
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
	};

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