#include <lwpp/nodeeditor.h>

namespace lwpp
{

	LWError          RootNode::init( NodeEditorID, int ) {return 0;}
	LWError          RootNode::newTime( NodeEditorID, LWFrame, LWTime ) {return 0;}
	void             RootNode::cleanup( NodeEditorID ) {;}
	LWXPanelID       RootNode::rootPanel( NodeEditorID ) {return 0;}
	LWError          RootNode::rootOptions( NodeEditorID ) {return 0;}
	void             RootNode::rootPreview( NodeEditorID, LWNodalAccess*, LWDVector ){;}
	void             RootNode::rootCustomPreview( NodeEditorID, int, int ){;}

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
		static LWError CB_init( NodeEditorID id, int renderMode)
		{
			if (T *rn = getUserData(id))
			{
				return rn->init(id, renderMode);
			}
			return 0;
		}
		static LWError CB_newTime( NodeEditorID id, LWFrame frame, LWTime time)
		{
			if (T *rn = getUserData(id))
			{
				return rn->newTime(id, frame, time);
			}
			return 0;
		}
		static void CB_cleanup( NodeEditorID id )
		{
			if (T *rn = getUserData(id))
			{
				rn->cleanup(id);
			}
		}
		static LWXPanelID CB_rootPanel( NodeEditorID id )
		{
			if (T *rn = getUserData(id))
			{
				return rn->rootPanel(id);
			}
			return 0;
		}
		static LWError CB_rootOptions( NodeEditorID id )
		{
			if (T *rn = getUserData(id))
			{
				return rn->rootOptions(id);
			}
			return 0;
		}
		static void CB_rootPreview( NodeEditorID id, LWNodalAccess *na, LWDVector val)
		{
			if (T *rn = getUserData(id))
			{
				return rn->rootPreview(id, na, val);
			}
		}
		static void CB_rootCustomPreview( NodeEditorID id, int width, int height)
		{
			if (T *rn = getUserData(id))
			{
				return rn->rootCustomPreview(id, width, height);
			}
		}
	};

	bool NodeEditor::create( const std::string rootName, const std::string panelTitle, RootNode* rn, NodePreviewType previewType)
	{
		destroy();
		mRootNode.previewType			  = previewType;
		mRootNode.init							= &RootNodeAdaptor<RootNode>::CB_init;
		mRootNode.newTime						= &RootNodeAdaptor<RootNode>::CB_newTime;
		mRootNode.cleanup						= &RootNodeAdaptor<RootNode>::CB_cleanup;
		mRootNode.rootPanel					= &RootNodeAdaptor<RootNode>::CB_rootPanel;
		mRootNode.rootOptions				= &RootNodeAdaptor<RootNode>::CB_rootOptions;
		mRootNode.rootPreview				= &RootNodeAdaptor<RootNode>::CB_rootPreview;
		mRootNode.rootCustomPreview = &RootNodeAdaptor<RootNode>::CB_rootCustomPreview;
		mId = globPtr->create(rootName.c_str(), panelTitle.c_str(), &mRootNode, rn);
		return (mId != 0);
	}
}