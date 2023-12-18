#include <lwpp/nodes.h>
#include <lwpp/global.h>
#include <lwpp/node_handler.h>

namespace lwpp
{
	const char *BlendModeStrings[] =
	{
		"Normal", "Additive", "Subtractive", "Multiply", "Screen",
		"Darken", "Lighten", "Difference", "Negative", "Colour Dodge", "Colour Burn",
		"Red", "Green", "Blue",
		0
	};

  void ShadingEvaluator::setValue(lwpp::NodeHandler *node, NodeValue value)
	{
		node->setValue(value, colour);
	}

	void ShadingEvaluator::operator()(const double lightColour[3], float weight)
	{
		colour += lwpp::Vector3d(lightColour) * weight;
	}

	/*
	int IlluminationSampler( void *data, LWItemID light, const LWDVector dir, const double color[4])
	{
		ShadingEvaluator *sampler=static_cast<ShadingEvaluator *>(data);
		if (sampler)
		{
			return (*sampler)(light, dir, color, 1.0f);
		}
		return 0;
	}
	*/

  const char inFuncs[] = LWNODEINPUTFUNCS_GLOBAL;
  const char outFuncs[] = LWNODEOUTPUTFUNCS_GLOBAL;

  IMPLEMENT_NAMED_GLOBAL(LWNodeInputFuncs, inFuncs)
  IMPLEMENT_NAMED_GLOBAL(LWNodeOutputFuncs, outFuncs)
  
  void LWNodeInput::init()
  {
      //if (inF == nullptr) inF = getTransGlobal<LWNodeInputFuncs>(inFuncs);
  }
  
	LWNode::LWNode(NodeID _id)
	: id(_id)
	{
		//if (inF == nullptr) inF = getTransGlobal<LWNodeInputFuncs>(inFuncs);
		//if (outF == nullptr) outF = getTransGlobal<LWNodeOutputFuncs>(outFuncs);
	}

  LWNode::~LWNode()
  {
    ;
  }

  
  void LWNodeOutput::init()
  {
		//if (outF == nullptr) outF = getTransGlobal<LWNodeOutputFuncs>(outFuncs);
  }

  //LWNodeInputFuncs *LWNodeInput::inF = 0;
  //LWNodeOutputFuncs *LWNodeOutput::outF = 0;

  //LWNodeInputFuncs *LWNode::inF = 0;
  //LWNodeOutputFuncs *LWNode::outF = 0;

  double EvalScalarVInput(LWShadingGeometry *sg, const lwpp::unique_NodeInput &lwni, const lwpp::unique_VParm &vp)
  {
    double v = vp->GetValue();
    if (lwni) lwni->evaluate(sg, &v);
    return v;
  }

  lwpp::Vector3d EvalVectorVInput(LWShadingGeometry* sg, const lwpp::unique_NodeInput& lwni, const lwpp::unique_VParm& vp)
  {
    lwpp::Vector3d v = *(vp);
    if (lwni) lwni->evaluate(sg, v);
    return v;
  }

  void *GetVInput(lwpp::unique_NodeInput &lwni, lwpp::unique_VParm &vp)
  {     
    if (!lwni) return vp->ID();
    return (lwni->isConnected()) ? 0 : vp->ID();
  }

}

