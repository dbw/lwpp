#ifndef LWPP_NODEEDITOR_H
#define LWPP_NODEEDITOR_H

#include <lwnodeeditor.h>
#include <lwpp/nodes.h>
#include <lwpp/vparm.h>
#include <string>

namespace lwpp
{

	class RootNode
	{
	public:
		virtual LWError          init( NodeEditorID, int );
		virtual LWError          newTime( NodeEditorID, LWFrame, LWTime );
		virtual void             cleanup( NodeEditorID );
		virtual LWXPanelID       rootPanel( NodeEditorID ); 
		virtual LWError          rootOptions( NodeEditorID );
		virtual void             rootPreview( NodeEditorID, LWNodalAccess*, LWDVector );
		virtual void             rootCustomPreview( NodeEditorID, int, int );
	};

	class NodeEditor : protected lwpp::GlobalBase<LWNodeEditorFuncs>, public lwpp::Storeable, public NodeInputHelper
	{
	protected:
			LWRootNode mRootNode;
			NodeEditorID mId;
			bool do_destroy;

			LWNodeInput *addInput(ConnectionType type, const std::string name, NodeInputEvent* inp_event)
			{
				NodeInputID id = globPtr->addInput(mId, type, name.c_str(), inp_event);
			  return new LWNodeInput(id);
			}
		public:
			NodeEditor(NodeEditorID id = 0, bool destroy = false)
				: mId(id),
					do_destroy(destroy)
			{
				;
			}

			NodeEditor(const std::string rootName, const std::string panelTitle, RootNode* rn, NodePreviewType previewType = NPT_RENDER, bool destroy = true)
				: mId(0), 
				  do_destroy(destroy)
			{
				create(rootName, panelTitle, rn, previewType);
			}
			~NodeEditor()
			{
				destroy();
			}

			bool create( const std::string rootName, const std::string panelTitle, RootNode* rn,  NodePreviewType previewType = NPT_RENDER);

			void destroy()
			{
				if (mId && do_destroy)
				{
					globPtr->destroy(mId);
					mId = 0;
				}
			}

			NodeEditorID getId() const
			{
				return mId;
			}

			void rename( const std::string newName)
			{
				globPtr->rename(newName.c_str(), mId);
			}
			void setUpdateFunc(nodeEditorUpdateFunc* func)
			{
				globPtr->setUpdateFunc(mId, func);
			}
			LWError copy( NodeEditorID from)
			{
				return globPtr->copy(mId, from);
			}
			NodeEditor &operator=(const NodeEditor &from)
			{
				copy(from.getId());	
				return *this;
			}
			virtual LWError Load(const LoadState &ls )
			{
				return globPtr->load(mId, ls.getState());
			}
			virtual LWError Save(const SaveState &ss )
			{
				return globPtr->save(mId, ss.getState());
			}
			LWError init(int renderMode )
			{
				return globPtr->init(mId, renderMode);
			}
			LWError newTime( LWFrame frame, LWTime time)
			{
				return globPtr->newTime(mId, frame, time);
			}
			void cleanup()
			{
				globPtr->cleanup(mId);
			}
			void setEnvGroup(LWChanGroupID cId)
			{
				globPtr->setEnvGroup(mId, cId);
			}
			void setAutosize(nodeAutoSizeFunc* func)
			{
				globPtr->setAutosize(mId, func);
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
			NodeInputID getInputByName(const std::string name )
			{
				return globPtr->getInputByName(mId, name.c_str());
			}
			NodeInputID getInputByIndex(int idx)
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

			NodeID getRootNodeID()
			{
				return globPtr->getRootNodeID(mId);
			}
	};
}

#endif // LWPP_NODEEDITOR_H