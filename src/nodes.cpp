#include <lwpp/nodes.h>
#include <lwpp/global.h>
#include <lwpp/node_handler.h>

namespace lwpp
{
  void ShadingEvaluator::setValue(lwpp::NodeHandler *node, NodeValue value)
	{
		node->setValue(value, colour);
	}

	int IlluminationSampler( void *data, LWItemID light, const LWDVector dir, const LWDVector color )
	{
		ShadingEvaluator *sampler=static_cast<ShadingEvaluator *>(data);
		if (sampler)
		{
			return (*sampler)(light, dir, color);
		}
		return 0;
	}

  const char inFuncs[] = "NodeInputFuncs";
	const char inFuncs2[] = "NodeInputFuncs 2";

  IMPLEMENT_NAMED_GLOBAL(LWNodeInputFuncs, inFuncs)
  IMPLEMENT_NAMED_GLOBAL(LWNodeInputFuncs, inFuncs2)

  void LWNodeInput::init()
  {
    if (lwpp::LightWave::isAtLeast(9, 5))
    {
      globPtr = getTransGlobal<LWNodeInputFuncs>(inFuncs2);
    }
    else
    {
      globPtr = getTransGlobal<LWNodeInputFuncs>(inFuncs);
    }
  }
  
	LWNode::LWNode(NodeID _id)
	: id(_id)
	{
		if (lwpp::LightWave::isAtLeast(9, 5))
		{
			inF = getTransGlobal<LWNodeInputFuncs>(inFuncs2);
		}
		else
		{
			inF = getTransGlobal<LWNodeInputFuncs>(inFuncs);
		}
	}

  LWNode::~LWNode()
  {
    ;
  }
}
