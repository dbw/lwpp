#ifndef LWPP_NODE_HANDLER_H
#define LWPP_NODE_HANDLER_H

#include "lwpp/plugin_handler.h"
#include "lwpp/nodes.h"

namespace lwpp
{
	//! @ingroup Handler
	class NodeHandler : public InstanceHandler, public virtual ItemHandler, public virtual RenderHandler, public LWNode
	{
		protected:
			NodeID Context;
			virtual int NodeInputEvent ( NodeInputID nid, LWNodalEvent nevent, ConnectionType type)
			{
				UNUSED(type);
				UNUSED(nid);
				if (nevent == NIE_CONNECT) Update();
				if (nevent == NIE_INPUTNODEDESTROY) Update();
				return 1;
			}

			static int CB_NodeInputEvent ( void *userData, NodeInputID nid, LWNodalEvent nevent, ConnectionType type)
			{
				NodeHandler *plugin = static_cast<NodeHandler *>(userData);
				return plugin->NodeInputEvent ( nid, nevent, type);
			}

		public:
      /*!
       * @name Inputs
       * Create nodal inputs
       */
      //! @{
			LWNodeInput *addColorInput(const std::string name = "Color")
			{
				return LWNode::addColorInput(name, CB_NodeInputEvent);
			}
			LWNodeInput *addColourInput(const std::string name = "Colour")
			{
				return addColorInput(name);
			}
			LWNodeInput *addScalarInput(const std::string name = "Scalar")
			{
				return LWNode::addScalarInput(name, CB_NodeInputEvent);
			}
			LWNodeInput *addVectorInput(const std::string name = "Vector")
			{
				return LWNode::addVectorInput(name, CB_NodeInputEvent);
			}
			LWNodeInput *addIntegerInput(const std::string name = "Integer")
			{
				return LWNode::addIntegerInput(name, CB_NodeInputEvent);
			}
			LWNodeInput *addFunctionInput(const std::string name = "Function")
			{
				return LWNode::addFunctionInput(name, CB_NodeInputEvent);
			}
			LWNodeInput *addMaterialInput(const std::string name = "Material") 
			{
				return LWNode::addMaterialInput(name, CB_NodeInputEvent);
			}
      //! @}

      /*!
       * @name auto_Inputs
       * Create nodal inputs handles by a std::auto_ptr
       */
      //! @{
			auto_NodeInput autoColorInput(const std::string name = "Color")
			{
				return auto_NodeInput(LWNode::addColorInput(name, CB_NodeInputEvent));
			}
			auto_NodeInput autoColourInput(const std::string name = "Colour")
			{
				return auto_NodeInput(addColorInput(name));
			}
			auto_NodeInput autoScalarInput(const std::string name = "Scalar")
			{
				return auto_NodeInput(LWNode::addScalarInput(name, CB_NodeInputEvent));
			}
			auto_NodeInput autoVectorInput(const std::string name = "Vector")
			{
				return auto_NodeInput(LWNode::addVectorInput(name, CB_NodeInputEvent));
			}
			auto_NodeInput autoIntegerInput(const std::string name = "Integer")
			{
				return auto_NodeInput(LWNode::addIntegerInput(name, CB_NodeInputEvent));
			}
			auto_NodeInput autoFunctionInput(const std::string name = "Function")
			{
				return auto_NodeInput(LWNode::addFunctionInput(name, CB_NodeInputEvent));
			}
			auto_NodeInput autoMaterialInput(const std::string name = "Material") 
			{
				return auto_NodeInput(LWNode::addMaterialInput(name, CB_NodeInputEvent));
			}
      //! @}

			NodeHandler(void *priv, void *context, LWError *err) : InstanceHandler(priv, context, err, LWNODE_HCLASS)
			{
				Context = static_cast<NodeID>(context);
				LWNode::setID(Context);
			}
			virtual ~NodeHandler() {;}
			virtual void Evaluate(LWNodalAccess* na, NodeOutputID outID, NodeValue value)
			{
				UNUSED(na);
				UNUSED(outID);
				UNUSED(value);
			}
			virtual void CustomPreview(int width, int height )
			{
				UNUSED(width);
				UNUSED(height);
			}
			virtual unsigned int Flags()
			{
				return 0;
			}
	};

	namespace LWSDK95_1
	{
		// Node flags. Returned by LWNodeHandler->flags
#define NF_TRANSP (1<<0) // This flag should be set if a material node might have transparency

		// Node handler activation.
		typedef struct st_LWNodeHandler {
			LWInstanceFuncs *inst;
			LWItemFuncs		*item;
			LWRenderFuncs   *rend;
			void			(*evaluate)( LWInstance, LWNodalAccess*, NodeOutputID, NodeValue );
			// Evaluation function receives the LWNodalAccess structure.
			// NodeOutputID is the output belonging to this node, and which is being currently asked the value from.
			// NodeValue is the value you need to set with the output functions when setting a value for this evaluation.

			void			(*customPreview)( LWInstance, int width, int height );
			// customPreview is called when the node has NPT_CUSTOM preview type set.

			unsigned int    (*flags)( LWInstance );
		} LWNodeHandler;
	}

	//! @ingroup Adaptor
	template <class T, int minVersion, int maxVersion>
	class NodeAdaptor : public InstanceAdaptor<T>, public ItemAdaptor<T>, public RenderAdaptor<T>
	{
		public:
		NodeAdaptor(const char *name, ServerTagInfo *tags)
		{
			LWServer::AddPlugin(LWNODE_HCLASS, name, Activate, tags);
		}
		//! Set static callbacks for the LightWave item handler
		static int Activate(int version, GlobalFunc *global, LWInstance inst, void *serverData)
		{
			if ( version > maxVersion ) return AFUNC_BADVERSION;
			if ( version < minVersion ) return AFUNC_BADVERSION;
			try
			{
				lwpp::SetSuperGlobal(global);
				UNUSED(serverData);
				if (version == 1)
				{
					LWNodeHandler *plugin = static_cast<LWNodeHandler *>(inst);
					InstanceAdaptor<T>::Activate(plugin->inst);
					RenderAdaptor<T>::Activate(plugin->rend);
					ItemAdaptor<T>::Activate(plugin->item);
					plugin->evaluate = NodeAdaptor::evaluate;
					plugin->customPreview = NodeAdaptor::customPreview;
					return AFUNC_OK;
				}
				else if (version == 2)
				{
					LWSDK95_1::LWNodeHandler *plugin = static_cast<LWSDK95_1::LWNodeHandler *>(inst);
					InstanceAdaptor<T>::Activate(plugin->inst);
					RenderAdaptor<T>::Activate(plugin->rend);
					ItemAdaptor<T>::Activate(plugin->item);
					plugin->evaluate = NodeAdaptor::evaluate;
					plugin->customPreview = NodeAdaptor::customPreview;
					plugin->flags = NodeAdaptor::flags;
					return AFUNC_OK;
				}
				return AFUNC_BADVERSION;
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in NodeHandler::Activate():", e.what());
				return AFUNC_BADAPP;
			}
		}

		private:
		static void evaluate (LWInstance instance, LWNodalAccess* na, NodeOutputID outID, NodeValue value)
		{
			try
			{
				if (instance)
				{
					T *plugin = static_cast<T *>(instance);
					plugin->Evaluate(na, outID, value);
				}
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in NodeHandler::evaluate():", e.what());
			}
		}

		static void customPreview (LWInstance instance, int width, int height )
		{
			try
			{
				T *plugin = static_cast<T *>(instance);
				plugin->CustomPreview(width, height);
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in NodeHandler::customPreview():", e.what());
			}
		}
		static unsigned int flags (LWInstance instance)
		{
			try
			{
				T *plugin = static_cast<T *>(instance);
				return plugin->Flags();
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in NodeHandler::Flags():", e.what());
				return 0;
			}
		}
	};

	//! @ingroup XPanelHandler
	class XPanelNodeHandler : public NodeHandler, public XPanelInterface
	{
		public:
			XPanelNodeHandler(void *priv, void *context, LWError *err) : NodeHandler(priv, context, err)
			{
				;
			}
			virtual ~XPanelNodeHandler() {;}
			virtual int NodeInputEvent ( NodeInputID nid, LWNodalEvent nevent, ConnectionType type)
			{
				if (LW_XPanel.isValid()) LW_XPanel.ViewRefresh();
				return NodeHandler::NodeInputEvent(nid, nevent, type);
			}
			virtual void ChangeNotify (LWXPanelID , unsigned int , unsigned int , int event_type)
			{
				if ( ( event_type == LWXPEVENT_VALUE ) || ( event_type == LWXPEVENT_HIT ) )
				{
					Update();
				}
				else if (event_type == LWXPEVENT_TRACK)
				{
					UpdateNodePreview();
				}
			}

	};

	//! @ingroup XPanelAdaptor
	IMPLEMENT_XPANELADAPTOR(Node, LWPP_NODE_VERSION, LWPP_NODE_VERSION);
}

#endif // LWPP_NODE_HANDLER_H