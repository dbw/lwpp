#include <lwpp/global.h>
#include <lwpp/message.h>
#include <lwpp/lw_version.h>

#include <lwpanel.h>
#include <lwanimlod.h>
#include <lwanimsav.h>
#include <lwanimuv.h>
#include <lwcamera.h>
#include <lwchannel.h>
#include <lwcmdseq.h>
#include <lwcomring.h>
#include <lwcustobj.h>
#include <lwdialog.h>
#include <lwdisplay.h>
#include <lwdisplce.h>
#include <lwdopetrack.h>
#include <lwdyna.h>
#include <lwenvel.h>
#include <lwenviron.h>
#include <lwfilter.h>
#include <lwframbuf.h>
#include <lwgeneric.h>
#include <lwglobsrv.h>
#include <lwhandler.h>
#include <lwhost.h>
#include <lwimage.h>
#include <lwimageio.h>
#include <lwio.h>
#include <lwlaytool.h>
#include <lwlcore.h>
#include <lwlight.h>
#include <lwmaster.h>
#include <lwmath.h>
#include <lwmeshedt.h>
#include <lwmeshes.h>
#include <lwmodeler.h>
#include <lwmodtool.h>
#include <lwmodule.h>
#include <lwmonitor.h>
#include <lwmotion.h>
#include <lwmtutil.h>
#include <lwnodeeditor.h>
#include <lwnodes.h>
#include <lwobjimp.h>
#include <lwobjrep.h>
#include <lwpolygon.h>
#include <lwpreview.h>
#include <lwprtcl.h>
#include <lwrender.h>
#include <lwscenecv.h>
#include <lwsceneeditor.h>
#include <lwserver.h>
//#include <lwshader.h>
#include <lwshelf.h>
#include <lwsurf.h>
#include <lwsurfed.h>
#include <lwtexture.h>
#include <lwtool.h>
#include <lwtxtr.h>
#include <lwtxtred.h>
//#include <lwvolume.h>
#include <lwvparm.h>
#include <lwxpanel.h>
#include <lwtimer.h>
#include <lwbufferset.h>

namespace lwpp
{
#ifdef _DEBUG
	dostream dout;
#endif

	//! Initialize and store superglobal GlobalFunc
	GlobalFunc *SuperGlobal = 0;

	unsigned int LightWave::systemID = 0;
	unsigned int LightWave::productInfo = 0;

	/*!
	* Initialize the SuperGlobal, this is just to keep it clean
	*/
	void SetSuperGlobal(GlobalFunc *g)
	{
#ifdef _DEBUG
		//lwpp::dostream dout;
		//std::hex(dout);
		//dout << "SuperGlobal set: 0x" << g << "\n";
#endif
		SuperGlobal = g;
		static LWMessage lwm;
		// now set global version information...
		LightWave::Init();
	}

	void LightWave::Init()
	{
		//systemID = (unsigned int)(unsigned long) (SuperGlobal (LWPP_SYSTEMID_GLOBAL, GFUSE_TRANSIENT));
		systemID = (unsigned int) (unsigned long) (SuperGlobal (LWSYSTEMID_GLOBAL, GFUSE_ACQUIRE));
		productInfo = ( unsigned int ) (unsigned long) SuperGlobal( LWPRODUCTINFO_GLOBAL, GFUSE_TRANSIENT );
	}

	int LightWave::GetNumCores()
	{
#ifdef _MSWIN
		SYSTEM_INFO siSysInfo;
		GetSystemInfo(&siSysInfo);
		return siSysInfo.dwNumberOfProcessors;
#endif
#ifdef _MACOS
		return MPProcessors();
#endif
	}

		IMPLEMENT_GLOBAL(LWInstUpdate,				LWINSTUPDATE_GLOBAL)
		IMPLEMENT_GLOBAL(LWBackdropInfo,			LWBACKDROPINFO_GLOBAL)

		IMPLEMENT_GLOBAL(LWGlobalPool,				LWGLOBALPOOL_GLOBAL)
		IMPLEMENT_GLOBAL(LWInterfaceInfo,			LWINTERFACEINFO_GLOBAL)
		IMPLEMENT_GLOBAL(LWMessageFuncs,			LWMESSAGEFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWCameraInfo,				LWCAMERAINFO_GLOBAL)
		IMPLEMENT_GLOBAL(ContextMenuFuncs,			LWCONTEXTMENU_GLOBAL)
		IMPLEMENT_GLOBAL(LWFileActivateFunc,		LWFILEACTIVATEFUNC_GLOBAL)
		IMPLEMENT_GLOBAL(LWDirInfoFunc,				LWDIRINFOFUNC_GLOBAL)
		// IMPLEMENT_GLOBAL(LWFileTypeFunc)
		IMPLEMENT_GLOBAL(LWImageUtil,				LWIMAGEUTIL_GLOBAL)
		IMPLEMENT_GLOBAL(LWImageList,				LWIMAGELIST_GLOBAL)
		IMPLEMENT_GLOBAL(LWFileIOFuncs,				LWFILEIOFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWItemInfo,				LWITEMINFO_GLOBAL)
		IMPLEMENT_GLOBAL(LWLMonFuncs,				LWLMONFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWTimeInfo,				LWTIMEINFO_GLOBAL)

		IMPLEMENT_GLOBAL(LWNodeFuncs,				LWNODEFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWNodeInputFuncs,			LWNODEINPUTFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWNodeOutputFuncs,			LWNODEOUTPUTFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWNodeUtilityFuncs,		LWNODEUTILITYFUNCS_GLOBAL)

		IMPLEMENT_GLOBAL(LWNodeEditorFuncs,			LWNODEEDITORFUNCS_GLOBAL)

		IMPLEMENT_GLOBAL(LWObjectInfo,				LWOBJECTINFO_GLOBAL)
		IMPLEMENT_GLOBAL(LWLightInfo,				LWLIGHTINFO_GLOBAL)
		IMPLEMENT_GLOBAL(LWObjectFuncs,				LWOBJECTFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWPanelFuncs,				LWPANELFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWXPanelFuncs,				LWXPANELFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWSceneInfo,				LWSCENEINFO_GLOBAL)
		IMPLEMENT_GLOBAL(LWSurfaceFuncs,			LWSURFACEFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWSurfEdFuncs,				LWSURFEDFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWTextureFuncs,			LWTEXTUREFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWTxtrEdFuncs,				LWTXTREDFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWMTUtilFuncs,				LWMTUTILFUNCS_GLOBAL)

		IMPLEMENT_GLOBAL(LWVParmFuncs,				LWVPARMFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWChannelInfo,				LWCHANNELINFO_GLOBAL)
		IMPLEMENT_GLOBAL(LWEnvelopeFuncs,			LWENVELOPEFUNCS_GLOBAL)

		IMPLEMENT_GLOBAL(LWPreviewFuncs,			LWPREVIEWFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWComRing,					LWCOMRING_GLOBAL)
		IMPLEMENT_GLOBAL(LWServerInfo,				LWSERVERINFO_GLOBAL)

		IMPLEMENT_GLOBAL(LWRasterFuncs,				LWRASTERFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWTimer,					LWTIMER_GLOBAL)

		IMPLEMENT_GLOBAL(DynaMonitorFuncs,			LWDYNAMONITORFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWStateQueryFuncs,			LWSTATEQUERYFUNCS_GLOBAL)

		IMPLEMENT_GLOBAL(LWColorActivateFunc,		LWCOLORACTIVATEFUNC_GLOBAL)

		IMPLEMENT_GLOBAL(LWFilterGlobal, LWFILTERINFO_GLOBAL)
		IMPLEMENT_GLOBAL(LWAOVFuncs, LWAOVFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWBSDFFuncs, LWBSDFFUNCS_GLOBAL)
		IMPLEMENT_GLOBAL(LWBufferSetGlobal, LWBUFFERLIST_GLOBAL);
		IMPLEMENT_GLOBAL(LWLightEvaluationFuncs, LWLIGHTEVALUATIONFUNCS_GLOBAL);
}