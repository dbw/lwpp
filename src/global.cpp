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
#include <lwshader.h>
#include <lwshelf.h>
#include <lwsurf.h>
#include <lwsurfed.h>
#include <lwtexture.h>
#include <lwtool.h>
#include <lwtxtr.h>
#include <lwtxtred.h>
#include <lwvolume.h>
#include <lwvparm.h>
#include <lwxpanel.h>
#include <lwtimer.h>

namespace lwpp
{
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
		lwpp::dostream dout;
		std::hex(dout);
		dout << "SuperGlobal set: 0x" << g << "\n";
#endif
		SuperGlobal = g;
		static LWMessage lwm;
		// now set global version information...
		LightWave::Init();
	}

	void LightWave::Init()
	{
		//systemID = (unsigned int)(unsigned long) (SuperGlobal (LWPP_SYSTEMID_GLOBAL, GFUSE_TRANSIENT));
    systemID = (unsigned int) (unsigned long) (SuperGlobal (LWPP_SYSTEMID_GLOBAL, GFUSE_ACQUIRE));
		productInfo = ( unsigned int ) (unsigned long) SuperGlobal( LWPP_PRODUCTINFO_GLOBAL, GFUSE_TRANSIENT );
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

	IMPLEMENT_GLOBAL(LWInstUpdate,				LWPP_INSTUPDATE_GLOBAL)
	IMPLEMENT_GLOBAL(LWBackdropInfo,			LWPP_BACKDROPINFO_GLOBAL)

	IMPLEMENT_GLOBAL(LWGlobalPool,				LWPP_GLOBALPOOL_GLOBAL)
	IMPLEMENT_GLOBAL(LWInterfaceInfo,			LWPP_INTERFACEINFO_GLOBAL)
	IMPLEMENT_GLOBAL(LWMessageFuncs,			LWPP_MESSAGEFUNCS_GLOBAL)
	IMPLEMENT_GLOBAL(LWCameraInfo,				LWPP_CAMERAINFO_GLOBAL)
	IMPLEMENT_GLOBAL(ContextMenuFuncs,		LWPP_CONTEXTMENU_GLOBAL)
	IMPLEMENT_GLOBAL(LWFileActivateFunc,	LWPP_FILEACTIVATEFUNC_GLOBAL)
	IMPLEMENT_GLOBAL(LWDirInfoFunc,				LWPP_DIRINFOFUNC_GLOBAL)
	// IMPLEMENT_GLOBAL(LWFileTypeFunc)
	IMPLEMENT_GLOBAL(LWImageUtil,					LWPP_IMAGEUTIL_GLOBAL)
	IMPLEMENT_GLOBAL(LWImageList,					LWPP_IMAGELIST_GLOBAL)
	IMPLEMENT_GLOBAL(LWFileIOFuncs,				LWPP_FILEIOFUNCS_GLOBAL)
	IMPLEMENT_GLOBAL(LWItemInfo,					LWPP_ITEMINFO_GLOBAL)
	IMPLEMENT_GLOBAL(LWLMonFuncs,					LWPP_LMONFUNCS_GLOBAL)
	IMPLEMENT_GLOBAL(LWTimeInfo,					LWPP_TIMEINFO_GLOBAL)

	IMPLEMENT_GLOBAL(LWNodeFuncs,					LWPP_NODEFUNCS_GLOBAL)
	IMPLEMENT_GLOBAL(LWNodeInputFuncs,		LWPP_NODEINPUTFUNCS_GLOBAL)
	IMPLEMENT_GLOBAL(LWNodeOutputFuncs,		LWPP_NODEOUTPUTFUNCS_GLOBAL)
	IMPLEMENT_GLOBAL(LWNodeUtilityFuncs,	LWPP_NODEUTILITYFUNCS_GLOBAL)

	IMPLEMENT_GLOBAL(LWNodeEditorFuncs,		LWPP_NODEEDITORFUNCS_GLOBAL)

	IMPLEMENT_GLOBAL(LWObjectInfo,				LWPP_OBJECTINFO_GLOBAL)
  IMPLEMENT_GLOBAL(LWLightInfo,				  LWPP_LIGHTINFO_GLOBAL)
	IMPLEMENT_GLOBAL(LWObjectFuncs,				LWPP_OBJECTFUNCS_GLOBAL)
	IMPLEMENT_GLOBAL(LWPanelFuncs,				LWPP_PANELFUNCS_GLOBAL)
	IMPLEMENT_GLOBAL(LWXPanelFuncs,				LWPP_XPANELFUNCS_GLOBAL)
	IMPLEMENT_GLOBAL(LWSceneInfo,					LWPP_SCENEINFO_GLOBAL)
	IMPLEMENT_GLOBAL(LWSurfaceFuncs,			LWPP_SURFACEFUNCS_GLOBAL)
	IMPLEMENT_GLOBAL(LWTextureFuncs,			LWPP_TEXTUREFUNCS_GLOBAL)
	IMPLEMENT_GLOBAL(LWTxtrEdFuncs,				LWPP_TXTREDFUNCS_GLOBAL)
	IMPLEMENT_GLOBAL(LWMTUtilFuncs,				LWPP_MTUTILFUNCS_GLOBAL)

	IMPLEMENT_GLOBAL(LWVParmFuncs,				LWPP_VPARMFUNCS_GLOBAL)
	IMPLEMENT_GLOBAL(LWChannelInfo,				LWPP_CHANNELINFO_GLOBAL)
	IMPLEMENT_GLOBAL(LWEnvelopeFuncs,			LWPP_ENVELOPEFUNCS_GLOBAL)

	IMPLEMENT_GLOBAL(LWPreviewFuncs,			LWPP_PREVIEWFUNCS_GLOBAL)
	IMPLEMENT_GLOBAL(LWComRing,						LWPP_COMRING_GLOBAL)
	IMPLEMENT_GLOBAL(LWServerInfo,				LWPP_SERVERINFO_GLOBAL)

	IMPLEMENT_GLOBAL(LWRasterFuncs,				LWPP_RASTERFUNCS_GLOBAL)
	IMPLEMENT_GLOBAL(LWTimer,							LWTIMER_GLOBAL)

  IMPLEMENT_GLOBAL(DynaMonitorFuncs,		LWDYNAMONITORFUNCS_GLOBAL)
  IMPLEMENT_GLOBAL(LWStateQueryFuncs,		LWSTATEQUERYFUNCS_GLOBAL)

  IMPLEMENT_GLOBAL(LWColorActivateFunc,		LWPP_COLORACTIVATEFUNC_GLOBAL)

  IMPLEMENT_GLOBAL(lwsdk10::LWServerInfo,		LWSERVERINFO_GLOBAL)
  
}