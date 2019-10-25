#ifndef LWPP_NODE_HANDLER_H
#define LWPP_NODE_HANDLER_H

#include "lwpp/plugin_handler.h"
#include "lwpp/nodes.h"
#include <lwpp/comring.h>

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
				// always update by default
				if (nevent == NIE_CONNECT) Update();
				if (nevent == NIE_DISCONNECT) Update();        
				if (nevent == NIE_INPUTNODEDESTROY) Update();
				return 1;
			}

		public:
			static int CB_NodeInputEvent(void *userData, NodeInputID nid, LWNodalEvent nevent, ConnectionType type)
			{
#ifdef _DEBUG
				dout << "CB_NodeInputEvent - userData: " << userData << "\n";
				//return 0;
#endif				
				NodeHandler *plugin = static_cast<NodeHandler *>(userData);
				return plugin->NodeInputEvent(nid, nevent, type);
			}
			/*!
			 * @name Inputs
			 * Create nodal inputs
			 */
			//! @{
			LWNodeInput *addInput(LWID vendor, LWID customType, const std::string name) const
			{
				return LWNode::addInput(vendor, customType, name, CB_NodeInputEvent);
			}
			LWNodeInput *addInput(ConnectionType type, const std::string name) const
			{
				return LWNode::addInput(type, name, CB_NodeInputEvent);
			}
			LWNodeInput *addColorInput(const std::string name = "Color") const
			{
				return LWNode::addColorInput(name, CB_NodeInputEvent);
			}
			LWNodeInput *addColourInput(const std::string name = "Colour") const
			{
				return addColorInput(name);
			}
			LWNodeInput *addScalarInput(const std::string name = "Scalar") const
			{
				return LWNode::addScalarInput(name, CB_NodeInputEvent);
			}
			LWNodeInput *addVectorInput(const std::string name = "Vector") const
			{
				return LWNode::addVectorInput(name, CB_NodeInputEvent);
			}
			LWNodeInput *addIntegerInput(const std::string name = "Integer") const
			{
				return LWNode::addIntegerInput(name, CB_NodeInputEvent);
			}
			LWNodeInput *addFunctionInput(const std::string name = "Function") const
			{
				return LWNode::addFunctionInput(name, CB_NodeInputEvent);
			}
			LWNodeInput *addBSDFInput(const std::string name = "Material") const
			{
				return LWNode::addBSDFInput(name, CB_NodeInputEvent);
			}
			LWNodeInput *addProjectionInput(const std::string name = "Projection") const
			{
				return LWNode::addProjectionInput(name, CB_NodeInputEvent);
			}
			//! @}

			/*!
			 * @name auto_Inputs
			 * Create nodal inputs handles by a std::auto_ptr
			 */
			//! @{
			auto_NodeInput autoColorInput(const std::string name = "Color") const
			{
				return auto_NodeInput(LWNode::addColorInput(name, CB_NodeInputEvent));
			}
			auto_NodeInput autoColourInput(const std::string name = "Colour") const
			{
				return auto_NodeInput(addColorInput(name));
			}
			auto_NodeInput autoScalarInput(const std::string name = "Scalar") const
			{
				return auto_NodeInput(LWNode::addScalarInput(name, CB_NodeInputEvent));
			}
			auto_NodeInput autoVectorInput(const std::string name = "Vector") const
			{
				return auto_NodeInput(LWNode::addVectorInput(name, CB_NodeInputEvent));
			}
			auto_NodeInput autoIntegerInput(const std::string name = "Integer") const
			{
				return auto_NodeInput(LWNode::addIntegerInput(name, CB_NodeInputEvent));
			}
			auto_NodeInput autoFunctionInput(const std::string name = "Function") const
			{
				return auto_NodeInput(LWNode::addFunctionInput(name, CB_NodeInputEvent));
			}
			auto_NodeInput autoBSDFInput(const std::string name = "Material") const
			{
				return auto_NodeInput(LWNode::addBSDFInput(name, CB_NodeInputEvent));
			}
			//! @}

			NodeHandler(void *priv, void *context, LWError *err) : InstanceHandler(priv, context, err, LWNODE_HCLASS)
			{
#ifdef _DEBUG
				dout << "NodeHandler() " << this << "\n";
#endif				
				Context = static_cast<NodeID>(context);
				LWNode::setID(Context);
			}
			virtual ~NodeHandler() {;}
			virtual void Evaluate(LWShadingGeometry *sg, NodeOutputID outID, NodeValue value)
			{
				UNUSED(sg);
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
			virtual unsigned int materialGL(LWNodeOGLMaterial* oglM)
			{
				return 0;
			}
	};


	//! @ingroup Adaptor
	template <class T, int Version>
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
			if ( version != Version ) return AFUNC_BADVERSION;
			try
			{
				lwpp::SetSuperGlobal(global);
				UNUSED(serverData);

				LWNodeHandler *plugin = static_cast<LWNodeHandler *>(inst);
				InstanceAdaptor<T>::Activate(plugin->inst);
				RenderAdaptor<T>::Activate(plugin->rend);
				ItemAdaptor<T>::Activate(plugin->item);
				plugin->evaluate = NodeAdaptor::evaluate;
				plugin->customPreview = NodeAdaptor::customPreview;
				plugin->flags = NodeAdaptor::flags;
				plugin->materialGL = NodeAdaptor::materialGL;
				return AFUNC_OK;			
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in NodeHandler::Activate():", e.what());
				return AFUNC_BADAPP;
			}
		}

		private:
		//! safe destroy that pauses VPR
		static void vprSafeDestroy (LWInstance instance)
		{
			try
			{
				lwpp::LimboComRing lcr;
				T *plugin = static_cast<T *>(instance);
				delete plugin;
			}
			catch (std::exception &e)
			{
				lwpp::LWMessage::Error("An exception occured in NodeAdaptor::vprSafeDestroy():", e.what());
			}
		}

		static void evaluate (LWInstance instance, LWShadingGeometry* sg, NodeOutputID outID, NodeValue value)
		{
			try
			{
				if (instance)
				{
					T *plugin = static_cast<T *>(instance);
					plugin->Evaluate(sg, outID, value);
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
		static unsigned int materialGL(LWInstance instance, LWNodeOGLMaterial* oglM)
		{
			try
			{
				T *plugin = static_cast<T *>(instance);
				return plugin->materialGL(oglM);
			}
			catch ( std::exception &e )
			{
				lwpp::LWMessage::Error("An exception occured in NodeHandler::materialGL():", e.what());
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
#ifdef _DEBUG
				dout << "XPanelNodeHandler() " << this << "\n";
#endif
				;
			}
			virtual ~XPanelNodeHandler() {;}
			virtual int NodeInputEvent ( NodeInputID nid, LWNodalEvent nevent, ConnectionType type)
			{
				LW_XPanel.ViewRefresh();				
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
	IMPLEMENT_XPANELADAPTOR(Node, LWNODECLASS_VERSION);
}

#endif // LWPP_NODE_HANDLER_H