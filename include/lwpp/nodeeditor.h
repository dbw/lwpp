#ifndef LWPP_NODEEDITOR_H
#define LWPP_NODEEDITOR_H

#include <lwnodeeditor.h>
#include <lwpp/nodes.h>
#include <lwpp/vparm.h>
#include <string>

namespace lwpp
{
	class NodeEditor : protected lwpp::GlobalBase<LWNodeEditorFuncs>, public lwpp::Storeable
	{
	protected:
		lwpp::GlobalBase<LWNodeInputFuncs> nodeInFuncs;
		lwpp::GlobalBase<LWNodeOutputFuncs> nodeOutFuncs;

		NodeEditorID mId = nullptr;
		bool do_destroy = false;

		NodeInputID addInput(ConnectionType type, const std::string name, NodeInputEvent* inp_event = nullptr) const
		{
			return globPtr->addInput(mId, type, name.c_str(), inp_event);
		}

		NodeOutputID addOutput(ConnectionType type, const std::string name) const
		{
			return globPtr->addOutput(mId, type, name.c_str());
		}

		void create(const char *name, const char *title, LWRootNode *root, LWInputNode *input, void *userData, NodeEditorID parent = nullptr)
		{
			destroy();
			mId = globPtr->create(name, title, root, input, userData, parent);
		}

		/* Node Editor */

	public:
		NodeEditor(NodeEditorID id = 0, bool destroy = false)
			: mId(id),
			do_destroy(destroy)
		{
			;
		}

		NodeEditor(const std::string rootName, const std::string panelTitle, LWRootNode *root, LWInputNode *input, void *userData, NodeEditorID parent = nullptr, bool destroy = true)
			: mId(0),
			do_destroy(destroy)
		{
			create(rootName.c_str(), panelTitle.c_str(), root, input, userData, parent);
#ifdef _DEBUG
			lwpp::dostream dout;
			dout << "Created NodeEditor : " << mId << "\n";
#endif
		}

		NodeEditor(const NodeEditor &from)
			: mId(from.mId), do_destroy(from.do_destroy)
		{}

		virtual ~NodeEditor()
		{
			destroy();
		}

		virtual int NodeEditorInputEvent(NodeInputID nid, LWNodalEvent nevent, ConnectionType type)
		{
			return 1;
		}

		void setAutoDestroy(bool val) { do_destroy = val; }
		
		void destroy( bool force = false)
		{
			if (do_destroy || force)
			{
				if (mId)
				{
#ifdef _DEBUG
					lwpp::dostream dout;
					dout << "Destroying NodeEditor : " <<mId << "\n";
#endif
					globPtr->destroy(mId);
				}
			}
			mId = 0;
		}

		NodeEditorID getId() const
		{
			return mId;
		}

		void rename(const std::string newName)
		{
			globPtr->rename(newName.c_str(), mId);
		}
		void setUpdateFunc(nodeEditorUpdateFunc* func = nullptr)
		{
			globPtr->setUpdateFunc(mId, func);
		}
		LWError copy(NodeEditorID from)
		{
			return globPtr->copy(mId, from);
		}
		NodeEditor &operator=(const NodeEditor &from)
		{
			copy(from.getId());
			return *this;
		}

		virtual LWError Load(const LoadState &ls)
		{
			return globPtr->load(mId, ls.getState());
		}
		virtual LWError Save(const SaveState &ss)
		{
			return globPtr->save(mId, ss.getState());
		}

		LWError init(int renderMode)
		{
			return globPtr->init(mId, renderMode);
		}
		LWError newTime(LWFrame frame, LWTime time)
		{
			return globPtr->newTime(mId, frame, time);
		}
		void cleanup()
		{
			globPtr->cleanup(mId);
		}

		NodeInputID addInput(ConnectionType type, const char*name, NodeInputEvent*event)
		{
			return globPtr->addInput(mId, type, name, event);
		}
		void remInput(NodeInputID node)
		{
			return globPtr->remInput(mId, node);
		}

		NodeOutputID addOutput(ConnectionType type, const char*name)
		{
			return globPtr->addOutput(mId, type, name);
		}
		void remOutput(NodeOutputID node)
		{
			return globPtr->remOutput(mId, node);
		}

		void setEnvGroup(LWChanGroupID cId)
		{
			globPtr->setEnvGroup(mId, cId);
		}
		void setAutosize(nodeAutoSizeFunc* func, double data[3][2])
		{
			globPtr->setAutosize(mId, func, data);
		}
		void setUserData(void * data)
		{
			globPtr->setUserData(mId, data);
		}
		template <typename T>
		T *getUserData()
		{
			return static_cast<T *>(globPtr->getUserData(mId));
		}
		const int numberOfNodes() const
		{
			return globPtr->numberOfNodes(mId);
		}
		int numInputs() const
		{
			return globPtr->numInputs(mId);
		}
		NodeInputID getInputByName(const std::string name)
		{
			return globPtr->getInputByName(mId, name.c_str());
		}
		NodeInputID getInputByIndex(int idx) const
		{
			return globPtr->getInputByIndex(mId, idx);
		}
		const unsigned short getState() const
		{
			return globPtr->getState(mId);
		}
		void setState(unsigned short int state)
		{
			return globPtr->setState(mId, state);
		}
		bool OpenNodeEditor()
		{
			return (globPtr->OpenNodeEditor(mId) != 0);
		}
		bool isOpenNodeEditor()
		{
			return (globPtr->isOpenNodeEditor(mId) != 0);
		}
		NodeID getRootNodeID() const
		{
			return globPtr->getRootNodeID(mId);
		}
		NodeID getInputNodeID() const
		{
			return globPtr->getInputNodeID(mId);
		}
		NodeEditorID getEditorID(NodeID node) const
		{
			return globPtr->getEditorID(node);
		}
		void fromNode(NodeID node)
		{	
			destroy();
			do_destroy = false;
			mId = globPtr->getEditorID(node);
		}
		NodeEditor getEditor(NodeID node)
		{
			return NodeEditor(globPtr->getEditorID(node));
		}
		NodeEditor const getParentEditor()
		{
			return NodeEditor(globPtr->getParentEditorID(mId));
		}
		NodeEditorID const getParentEditorID()
		{
			return globPtr->getParentEditorID(mId);
		}
		LWError connect(NodeOutputID out, NodeInputID in)
		{
			return globPtr->connect(out, in);
		}
		NodeID addNode(const char *server_name)
		{
			return globPtr->addNode(mId, server_name);
		}
		void destroyNode(NodeID node)
		{
			globPtr->destroyNode(node);
		}
		void setXY(NodeID node, int x, int y)
		{
			globPtr->setXY(node, x, y);
		}
		void setContext(const char *context)
		{
			globPtr->setContext(mId, context);
		}
		void setContext(const std::string &context)
		{
			setContext(context.c_str());
		}
		const char *getContext()
		{
			return globPtr->getContext(mId);
		}
		const LWItemID* usedItems()
		{
			return globPtr->usedItems(mId);
		}
		void reset()
		{
			globPtr->reset(mId);
		}
	};

	template <typename T>
	int CB_NodeGraphInputEvent(void *userData, NodeInputID nid, LWNodalEvent nevent, ConnectionType type)
	{
		T *plugin = static_cast<T *>(userData);
		return plugin->NodeEditorInputEvent(nid, nevent, type);
	}

	template <typename T>
	class RootNodeAdaptor
	{
	private:
		static T *getUserData(NodeEditorID id)
		{
			NodeEditor ne(id);
			return ne.getUserData<T>();
		}
	public:
		static LWError CB_rootInit(NodeEditorID id, int renderMode)
		{
			if (auto rn = getUserData(id))
			{
				return rn->rootInit(renderMode);
			}
			return 0;
		}
		static LWError CB_rootNewTime(NodeEditorID id, LWFrame frame, LWTime time)
		{
			if (auto rn = getUserData(id))
			{
				return rn->rootNewTime(frame, time);
			}
			return 0;
		}
		static void CB_rootCleanup(NodeEditorID id)
		{
			if (auto rn = getUserData(id))
			{
				rn->rootCleanup();
			}
		}
		static LWXPanelID CB_rootPanel(NodeEditorID id)
		{
			if (auto rn = getUserData(id))
			{
				return rn->rootPanel();
			}
			return 0;
		}
		static LWError CB_rootOptions(NodeEditorID id)
		{
			if (auto rn = getUserData(id))
			{
				return rn->rootOptions();
			}
			return 0;
		}
		static void CB_rootPreview(NodeEditorID id, LWShadingGeometry *sg, LWDVector val)
		{
			if (auto rn = getUserData(id))
			{
				return rn->rootPreview(sg, val);
			}
		}
		static void CB_rootCustomPreview(NodeEditorID id, int width, int height)
		{
			if (auto rn = getUserData(id))
			{
				return rn->rootCustomPreview(width, height);
			}
		}
	};

	template <typename T>
	class InputNodeAdaptor
	{
	private:
		static T *getUserData(NodeEditorID id)
		{
			NodeEditor ne(id);
			return ne.getUserData<T>();
		}
	public:
		static LWError CB_inputInit(NodeEditorID id, int renderMode)
		{
			if (auto rn = getUserData(id))
			{
				return rn->inputInit(renderMode);
			}
			return 0;
		}
		static LWError CB_inputNewTime(NodeEditorID id, LWFrame frame, LWTime time)
		{
			if (auto rn = getUserData(id))
			{
				return rn->inputNewTime(frame, time);
			}
			return 0;
		}
		static void CB_inputCleanup(NodeEditorID id)
		{
			if (T *rn = getUserData(id))
			{
				rn->inputCleanup();
			}
		}
		static void CB_inputEvaluate(NodeEditorID id, LWShadingGeometry* sg, NodeOutputID no, NodeValue val)
		{
			if (auto rn = getUserData(id))
			{
				rn->inputEvaluate(sg, no, val);
			}
		}
		static LWXPanelID CB_inputPanel(NodeEditorID id)
		{
			if (auto rn = getUserData(id))
			{
				return rn->inputPanel();
			}
			return 0;
		}
		static LWError CB_inputOptions(NodeEditorID id)
		{
			if (auto rn = getUserData(id))
			{
				return rn->inputOptions();
			}
			return 0;
		}
	};

  template <typename T>
	class RootNode
	{
	public:
		LWRootNode mRootNode;
		virtual LWError          rootInit(int){ return 0; }
		virtual LWError          rootNewTime(LWFrame, LWTime) { return 0; }
		virtual void             rootCleanup() { ; }
		virtual LWXPanelID       rootPanel() {return 0;};
		virtual LWError          rootOptions() { return 0; };
		virtual void             rootPreview(LWShadingGeometry*, LWDVector) { ; }
		virtual void             rootCustomPreview(int, int) { ; }
		LWRootNode* getRootNode(){ return &mRootNode; }
		RootNode()
		{
			mRootNode.previewType = NPT_OFF;
			mRootNode.init = &RootNodeAdaptor<T>::CB_rootInit;
			mRootNode.newTime = &RootNodeAdaptor<T>::CB_rootNewTime;
			mRootNode.cleanup = nullptr;
			mRootNode.options = nullptr;
			mRootNode.preview = nullptr;
			mRootNode.customPreview = nullptr;
			mRootNode.panel = &RootNodeAdaptor<T>::CB_rootPanel;
		}
		virtual ~RootNode() {}
	};

	template <typename T>
	class InputNode
	{		
	public:
		LWInputNode mInputNode;
		virtual LWError inputInit(int) { return 0; };
		virtual LWError inputNewTime(LWFrame, LWTime) { return 0; };
		virtual void inputCleanup() { ; }
		virtual void inputEvaluate(LWShadingGeometry*, NodeOutputID, NodeValue) { ; }
		virtual LWXPanelID inputPanel() { return 0; };
		virtual LWError inputOptions() { return 0; };
		LWInputNode* getInputNode(){ return &mInputNode; }
		InputNode()
		{
			mInputNode.init = &InputNodeAdaptor<T>::CB_inputInit;
			mInputNode.cleanup = nullptr;
			mInputNode.panel = nullptr;
			mInputNode.options = nullptr;
			mInputNode.evaluate = &InputNodeAdaptor<T>::CB_inputEvaluate;
			mInputNode.newTime = &InputNodeAdaptor<InputNode>::CB_inputNewTime;
			//mInputNode.cleanup = &InputNodeAdaptor<InputNode>::CB_inputCleanup;
			mInputNode.panel = &InputNodeAdaptor<T>::CB_inputPanel;
			//mInputNode.options = &InputNodeAdaptor<InputNode>::CB_inputOptions;
		}
		virtual ~InputNode() {}
	};

}



#endif // LWPP_NODEEDITOR_H