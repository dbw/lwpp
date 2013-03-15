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
  const char inFuncs3[] = "NodeInputFuncs 3";

  IMPLEMENT_NAMED_GLOBAL(LWNodeInputFuncs, inFuncs)
  IMPLEMENT_NAMED_GLOBAL(LWNodeInputFuncs, inFuncs2)
  IMPLEMENT_NAMED_GLOBAL(LWNodeInputFuncs, inFuncs3)

  void LWNodeInput::init()
  {
    if (lwpp::LightWave::isAtLeast(11, 0))
    {
      inF = getTransGlobal<LWNodeInputFuncs>(inFuncs3);
    }
    if (lwpp::LightWave::isAtLeast(9, 5))
    {
      inF = getTransGlobal<LWNodeInputFuncs>(inFuncs2);
    }
    else
    {
      inF = getTransGlobal<LWNodeInputFuncs>(inFuncs);
    }
  }
  
	LWNode::LWNode(NodeID _id)
	: id(_id)
	{
    if (lwpp::LightWave::isAtLeast(11, 0))
    {
      inF = getTransGlobal<LWNodeInputFuncs>(inFuncs3);
    }
		else if (lwpp::LightWave::isAtLeast(9, 5))
		{
			inF = getTransGlobal<LWNodeInputFuncs>(inFuncs2);
		}
		else
		{
			inF = getTransGlobal<LWNodeInputFuncs>(inFuncs);
		}

    if (lwpp::LightWave::isAtLeast(11, 0))
    {
      outF = getTransGlobal<LWNodeOutputFuncs>(outFuncs2);
    }
    else
		{
			outF = getTransGlobal<LWNodeOutputFuncs>(outFuncs);
		}
	}

  LWNode::~LWNode()
  {
    ;
  }

  const char outFuncs[] = "NodeOutputFuncs";
	const char outFuncs2[] = "NodeOutputFuncs 2";

  IMPLEMENT_NAMED_GLOBAL(LWNodeOutputFuncs, outFuncs)
  IMPLEMENT_NAMED_GLOBAL(LWNodeOutputFuncs, outFuncs2)

  void LWNodeOutput::init()
  {
    if (lwpp::LightWave::isAtLeast(11, 0))
    {
      outF = getTransGlobal<LWNodeOutputFuncs>(outFuncs2);
    }
    else
		{
			outF = getTransGlobal<LWNodeOutputFuncs>(outFuncs);
		}
  }

  LWNodeInputFuncs *LWNodeInput::inF = 0;
  LWNodeOutputFuncs *LWNodeOutput::outF = 0;

  LWNodeInputFuncs *LWNode::inF = 0;
  LWNodeOutputFuncs *LWNode::outF = 0;

}

