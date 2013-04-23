/*!
 * @file
 * Compatibility layer for various version of LightWave. Currently static and determined during compile time.
 */

#ifndef LW_VERSION
#define LW_VERSION

#define LWPP_COMMANDFUNC_GLOBAL "LW Command Interface"

#ifndef LW_SDK
#define LW_SDK 75 // base version of the LW SDK to use
#endif

#ifdef LW_SDK
	#if LW_SDK == 75

		// globals
		#define LWPP_TEXTUREFUNCS_GLOBAL		"Texture Functions 3"
		#define LWPP_TXTREDFUNCS_GLOBAL			"Texture Editor 2"

		#define LWPP_DIRINFOFUNC_GLOBAL			"Directory Info"
		#define LWPP_FILETYPEFUNC_GLOBAL		"File Type Pattern"
		#define LWPP_FILEACTIVATEFUNC_GLOBAL	"File Request 2"
		#define LWPP_MESSAGEFUNCS_GLOBAL		"Info Messages 2"
		#define LWPP_LMONFUNCS_GLOBAL			"LWLMonFuncs"
		#define LWPP_OBJECTINFO_GLOBAL			"LW Object Info 4"
		#define LWPP_OBJECTFUNCS_GLOBAL			"Scene Objects 4"
		#define LWPP_INTERFACEINFO_GLOBAL		"LW Interface Info 5"

		#define LWPP_PANEL_SERVICES_NAME		"LWPanelServices"
		#define LWPP_PANELFUNCS_GLOBAL			"LWPanelServices"
		#define LWPP_RASTERFUNCS_GLOBAL			"RasterServices"
		#define LWPP_CONTEXTMENU_GLOBAL			"ContextMenuServices"
		#define LWPP_XPANELFUNCS_GLOBAL         "LWXPanel"
		#define LWPP_LMONFUNCS_GLOBAL			"LWLMonFuncs"
		#define LWPP_PANELS_API_VERSION			19

		#define LWPP_ITEMINFO_GLOBAL			"LW Item Info 3"
		#define LWPP_BONEINFO_GLOBAL			"LW Bone Info 3"
		#define LWPP_LIGHTINFO_GLOBAL			"LW Light Info 3"
		#define LWPP_CAMERAINFO_GLOBAL			"LW Camera Info 2"
		#define LWPP_SCENEINFO_GLOBAL			"LW Scene Info 3"
		#define LWPP_TIMEINFO_GLOBAL			"LW Time Info"
		#define LWPP_COMPINFO_GLOBAL			"LW Compositing Info"
		#define LWPP_BACKDROPINFO_GLOBAL		"LW Backdrop Info 2"
		#define LWPP_FOGINFO_GLOBAL				"LW Fog Info"

		#define LWPP_VIEWPORTINFO_GLOBAL		"LW Viewport Info"
		#define LWPP_GLOBALPOOL_RENDER_GLOBAL	"Global Render Memory"
		#define LWPP_GLOBALPOOL_GLOBAL			"Global Memory"
		#define LWPP_PREVIEWFUNCS_GLOBAL		"Preview Functions"

		#define LWPP_COMRING_GLOBAL				"LW Communication Ring"
		#define LWPP_SERVERINFO_GLOBAL			"LW Server Info"

		#define LWPP_SURFACEFUNCS_GLOBAL		"Surface Functions 3"

		#define LWPP_SYSTEMID_GLOBAL			"System ID"
		#define LWPP_PRODUCTINFO_GLOBAL			"Product Info"
		#define LWPP_MTUTILFUNCS_GLOBAL			"MultiThreading Utilities"

		#define LWPP_MTUTILFUNCS_GLOBAL_9		"MultiThreading Utilities"

		#define LWPP_VPARMFUNCS_GLOBAL			"LWVParmFuncs 2"
		#define LWPP_ENVELOPEFUNCS_GLOBAL		"Animation Envelopes"
		#define LWPP_CHANNELINFO_GLOBAL			"Channel Info 2"
		#define LWPP_XPANELFUNCS_GLOBAL			"LWXPanel"

		#define LWPP_INSTUPDATE_GLOBAL			"Instance Update"

		#define LWPP_FILEIOFUNCS_GLOBAL			"File IO"

		#define LWPP_IMAGELIST_GLOBAL			"LW Image List 3"
		#define LWPP_IMAGEUTIL_GLOBAL			"Image Utility"

		#define LWPP_PREVIEWFUNCS_GLOBAL		"Preview Functions"

		// added in LW 9.0 - only load in LW 9.0+ as well

		#define	LWPP_NODEFUNCS_GLOBAL			"NodeFuncs"
		#define	LWPP_NODEINPUTFUNCS_GLOBAL		"NodeInputFuncs"
		#define	LWPP_NODEOUTPUTFUNCS_GLOBAL		"NodeOutputFuncs"
		#define LWPP_NODEUTILITYFUNCS_GLOBAL	"NodeUtilityFuncs"
		#define LWPP_NODEDRAWFUNCS_GLOBAL		"NodeDrawFuncs"
		#define LWPP_NODEEDITORFUNCS_GLOBAL		"NodeEditorFuncs"

		#define LWPP_RASTERFUNCS_GLOBAL			"RasterServices"

		// classes
		#define LWPP_IMAGELOADER_VERSION		3
		#define LWPP_IMAGESAVER_VERSION			2
		#define LWPP_MASTER_VERSION				4
		#define LWPP_TEXTURE_VERSION			5
		#define LWPP_SHADER_VERSION				4
		#define LWPP_IMAGEFILTER_VERSION		4
		#define LWPP_PIXELFILTER_VERSION 6
		#define LWPP_DISPLACEMENT_VERSION		5
		#define LWPP_TEXTURE_VERSION			5
		#define LWPP_SHADER_VERSION				4
		#define LWPP_CUSTOMOBJECT_VERSION		4
		#define LWPP_NODE_VERSION 				1
		#define LWPP_MASTER_VERSION 4

	#elif LW_SDK == 93

// globals
		#define LWPP_TEXTUREFUNCS_GLOBAL			"Texture Functions 3"
		#define LWPP_TXTREDFUNCS_GLOBAL				"Texture Editor 2"

		#define LWPP_DIRINFOFUNC_GLOBAL				"Directory Info"
		#define LWPP_FILETYPEFUNC_GLOBAL			"File Type Pattern"
		#define LWPP_FILEACTIVATEFUNC_GLOBAL		"File Request 2"
		#define LWPP_MESSAGEFUNCS_GLOBAL			"Info Messages 2"
		#define LWPP_LMONFUNCS_GLOBAL				"LWLMonFuncs"
		#define LWPP_OBJECTINFO_GLOBAL				"LW Object Info 4"
		#define LWPP_OBJECTFUNCS_GLOBAL				"Scene Objects 4"
		#define LWPP_INTERFACEINFO_GLOBAL			"LW Interface Info 5"

		#define LWPP_PANEL_SERVICES_NAME			"LWPanelServices"
		#define LWPP_PANELFUNCS_GLOBAL				"LWPanelServices"
		#define LWPP_RASTERFUNCS_GLOBAL				"RasterServices"
		#define LWPP_CONTEXTMENU_GLOBAL				"ContextMenuServices"
		#define LWPP_XPANELFUNCS_GLOBAL			    "LWXPanel"
		#define LWPP_LMONFUNCS_GLOBAL				"LWLMonFuncs"
		#define LWPP_PANELS_API_VERSION				19

		//updated from 3 to 4
		#define LWPP_ITEMINFO_GLOBAL				"LW Item Info 4"
		#define LWPP_BONEINFO_GLOBAL				"LW Bone Info 3"
		#define LWPP_LIGHTINFO_GLOBAL				"LW Light Info 3"

		//Updated LWCam info from 2 to 5
		#define LWPP_CAMERAINFO_GLOBAL				"LW Camera Info 5"

		//Added evaluator funcs
		#define LWPP_CAMERAEVALUATIONFUNCS_GLOBAL	"LW Camera Evaluation Funcs 2"

		//Updated from 3 to 4
		#define LWPP_SCENEINFO_GLOBAL				"LW Scene Info 4"
		#define LWPP_TIMEINFO_GLOBAL				"LW Time Info"
		#define LWPP_COMPINFO_GLOBAL				"LW Compositing Info"
		#define LWPP_BACKDROPINFO_GLOBAL			"LW Backdrop Info 2"
		#define LWPP_FOGINFO_GLOBAL					"LW Fog Info"

		#define LWPP_VIEWPORTINFO_GLOBAL			"LW Viewport Info"
		#define LWPP_GLOBALPOOL_RENDER_GLOBAL		"Global Render Memory"
		#define LWPP_GLOBALPOOL_GLOBAL				"Global Memory"
		#define LWPP_PREVIEWFUNCS_GLOBAL			"Preview Functions"

		#define LWPP_COMRING_GLOBAL					"LW Communication Ring"
		#define LWPP_SERVERINFO_GLOBAL				"LW Server Info"

		#define LWPP_SURFACEFUNCS_GLOBAL			"Surface Functions 3"

		#define LWPP_SYSTEMID_GLOBAL				"System ID"
		#define LWPP_PRODUCTINFO_GLOBAL				"Product Info"
		#define LWPP_MTUTILFUNCS_GLOBAL				"MultiThreading Utilities"

		#define LWPP_MTUTILFUNCS_GLOBAL_9			"MultiThreading Utilities"

		#define LWPP_VPARMFUNCS_GLOBAL				"LWVParmFuncs 2"
		#define LWPP_ENVELOPEFUNCS_GLOBAL			"Animation Envelopes"
		#define LWPP_CHANNELINFO_GLOBAL				"Channel Info 2"
		#define LWPP_XPANELFUNCS_GLOBAL				"LWXPanel"

		#define LWPP_INSTUPDATE_GLOBAL				"Instance Update"

		#define LWPP_FILEIOFUNCS_GLOBAL				"File IO"

		#define LWPP_IMAGELIST_GLOBAL				"LW Image List 3"
		#define LWPP_IMAGEUTIL_GLOBAL				"Image Utility"

		#define LWPP_PREVIEWFUNCS_GLOBAL			"Preview Functions"

		// added in LW 9.0 - only load in LW 9.0+ as well

		#define	LWPP_NODEFUNCS_GLOBAL				"NodeFuncs"
		#define	LWPP_NODEINPUTFUNCS_GLOBAL			"NodeInputFuncs"
		#define	LWPP_NODEOUTPUTFUNCS_GLOBAL			"NodeOutputFuncs"
		#define LWPP_NODEUTILITYFUNCS_GLOBAL		"NodeUtilityFuncs"
		#define LWPP_NODEDRAWFUNCS_GLOBAL			"NodeDrawFuncs"
		#define LWPP_RASTERFUNCS_GLOBAL				"RasterServices"

		// classes
		#define LWPP_IMAGELOADER_VERSION			2
		#define LWPP_IMAGESAVER_VERSION				2
		#define LWPP_MASTER_VERSION					4
		#define LWPP_TEXTURE_VERSION				5
		#define LWPP_SHADER_VERSION					4
		#define LWPP_IMAGEFILTER_VERSION			4
		#define LWPP_TEXTURE_VERSION				5
		#define LWPP_SHADER_VERSION					4
		#define LWPP_PIXELFILTER_VERSION			6
		#define LWPP_DISPLACEMENT_VERSION			5
		#define LWPP_CUSTOMOBJECT_VERSION			5
		#define LWPP_NODE_VERSION 					2
    #define LWPP_MASTER_VERSION 4

	#elif LW_SDK == 95

// globals
		#define LWPP_TEXTUREFUNCS_GLOBAL			"Texture Functions 3"
		#define LWPP_TXTREDFUNCS_GLOBAL				"Texture Editor 2"

		#define LWPP_DIRINFOFUNC_GLOBAL				"Directory Info"
		#define LWPP_FILETYPEFUNC_GLOBAL			"File Type Pattern"
		#define LWPP_FILEACTIVATEFUNC_GLOBAL		"File Request 2"
		#define LWPP_MESSAGEFUNCS_GLOBAL			"Info Messages 2"
		#define LWPP_LMONFUNCS_GLOBAL				"LWLMonFuncs"
		#define LWPP_OBJECTINFO_GLOBAL				"LW Object Info 4"
		#define LWPP_OBJECTFUNCS_GLOBAL				"Scene Objects 4"
		#define LWPP_INTERFACEINFO_GLOBAL			"LW Interface Info 5"

		#define LWPP_PANEL_SERVICES_NAME			"LWPanelServices"
		#define LWPP_PANELFUNCS_GLOBAL				"LWPanelServices"
		#define LWPP_RASTERFUNCS_GLOBAL				"RasterServices"
		#define LWPP_CONTEXTMENU_GLOBAL				"ContextMenuServices"
		#define LWPP_XPANELFUNCS_GLOBAL				"LWXPanel"
		#define LWPP_LMONFUNCS_GLOBAL				"LWLMonFuncs"
		#define LWPP_PANELS_API_VERSION				19

		//updated from 3 to 4
		#define LWPP_ITEMINFO_GLOBAL				"LW Item Info 4"
		#define LWPP_BONEINFO_GLOBAL				"LW Bone Info 3"
		#define LWPP_LIGHTINFO_GLOBAL				"LW Light Info 3"

		//Updated LWCam info from 2 to 5
		#define LWPP_CAMERAINFO_GLOBAL				"LW Camera Info 5"

		//Added evaluator funcs
		#define LWPP_CAMERAEVALUATIONFUNCS_GLOBAL	"LW Camera Evaluation Funcs 2"

		//Updated from 3 to 4
		#define LWPP_SCENEINFO_GLOBAL				"LW Scene Info 4"
		#define LWPP_TIMEINFO_GLOBAL				"LW Time Info"
		#define LWPP_COMPINFO_GLOBAL				"LW Compositing Info"
		#define LWPP_BACKDROPINFO_GLOBAL			"LW Backdrop Info 2"
		#define LWPP_FOGINFO_GLOBAL					"LW Fog Info"

		#define LWPP_VIEWPORTINFO_GLOBAL			"LW Viewport Info"
		#define LWPP_GLOBALPOOL_RENDER_GLOBAL		"Global Render Memory"
		#define LWPP_GLOBALPOOL_GLOBAL				"Global Memory"
		#define LWPP_PREVIEWFUNCS_GLOBAL			"Preview Functions"

		#define LWPP_COMRING_GLOBAL					"LW Communication Ring"
		#define LWPP_SERVERINFO_GLOBAL				"LW Server Info"
		#define LWPP_SURFACEFUNCS_GLOBAL			"Surface Functions 3"

		#define LWPP_SYSTEMID_GLOBAL				"System ID"
		#define LWPP_PRODUCTINFO_GLOBAL				"Product Info"
		#define LWPP_MTUTILFUNCS_GLOBAL				"MultiThreading Utilities"

		#define LWPP_MTUTILFUNCS_GLOBAL_9			"MultiThreading Utilities"

		#define LWPP_VPARMFUNCS_GLOBAL				"LWVParmFuncs 2"
		#define LWPP_ENVELOPEFUNCS_GLOBAL			"Animation Envelopes"
		#define LWPP_CHANNELINFO_GLOBAL				"Channel Info 2"
		#define LWPP_XPANELFUNCS_GLOBAL				"LWXPanel"

		#define LWPP_INSTUPDATE_GLOBAL				"Instance Update"

		#define LWPP_FILEIOFUNCS_GLOBAL				"File IO"

		#define LWPP_IMAGELIST_GLOBAL				"LW Image List 5"
		#define LWPP_IMAGEUTIL_GLOBAL				"Image Utility 3"

		#define LWPP_PREVIEWFUNCS_GLOBAL			"Preview Functions"
		#define LWPP_COLORACTIVATEFUNC_GLOBAL "Color Picker"

		// added in LW 9.0 - only load in LW 9.0+ as well

		#define	LWPP_NODEFUNCS_GLOBAL				"NodeFuncs"
		#define	LWPP_NODEINPUTFUNCS_GLOBAL			"NodeInputFuncs"
		#define	LWPP_NODEOUTPUTFUNCS_GLOBAL			"NodeOutputFuncs"
		#define LWPP_NODEUTILITYFUNCS_GLOBAL		"NodeUtilityFuncs"
		#define LWPP_NODEDRAWFUNCS_GLOBAL			"NodeDrawFuncs"
	 	#define LWPP_NODEEDITORFUNCS_GLOBAL			"NodeEditorFuncs"
		#define LWPP_RASTERFUNCS_GLOBAL				"RasterServices"
		#define LWPP_PARTICLE_SERVICES_GLOBAL		"Particle Services"

		#define LWPP_LIGHTEVALUATIONFUNCS "LW Light Evaluation Funcs"

		// classes
		#define LWPP_COLORPICK_VERSION 4
		#define LWPP_IMAGELOADER_VERSION			3
		#define LWPP_IMAGESAVER_VERSION				3
		#define LWPP_MASTER_VERSION					4
		#define LWPP_TEXTURE_VERSION				5
		#define LWPP_SHADER_VERSION					4
		#define LWPP_IMAGEFILTER_VERSION			4
		#define LWPP_TEXTURE_VERSION				5
		#define LWPP_SHADER_VERSION					4
		#define LWPP_PIXELFILTER_VERSION 6
		#define LWPP_DISPLACEMENT_VERSION			5
		#define LWPP_CUSTOMOBJECT_VERSION			5
		#define LWPP_VOLUMETRIC_VERSION				7
		#define LWPP_NODE_VERSION 					2
    #define LWPP_MASTER_VERSION 4

    #define LWPP_LIGHT_VERSION 1

	#elif LW_SDK == 96

// globals
		#define LWPP_TEXTUREFUNCS_GLOBAL			"Texture Functions 3"
		#define LWPP_TXTREDFUNCS_GLOBAL				"Texture Editor 2"

		#define LWPP_DIRINFOFUNC_GLOBAL				"Directory Info"
		#define LWPP_FILETYPEFUNC_GLOBAL			"File Type Pattern"
		#define LWPP_FILEACTIVATEFUNC_GLOBAL		"File Request 2"
		#define LWPP_MESSAGEFUNCS_GLOBAL			"Info Messages 2"
		#define LWPP_LMONFUNCS_GLOBAL				"LWLMonFuncs"
		#define LWPP_OBJECTINFO_GLOBAL				"LW Object Info 4"
		#define LWPP_OBJECTFUNCS_GLOBAL				"Scene Objects 4"
		#define LWPP_INTERFACEINFO_GLOBAL			"LW Interface Info 5"

		#define LWPP_PANEL_SERVICES_NAME			"LWPanelServices"
		#define LWPP_PANELFUNCS_GLOBAL				"LWPanelServices"
		#define LWPP_RASTERFUNCS_GLOBAL				"RasterServices"
		#define LWPP_CONTEXTMENU_GLOBAL				"ContextMenuServices"
		#define LWPP_XPANELFUNCS_GLOBAL				"LWXPanel"
		#define LWPP_LMONFUNCS_GLOBAL				"LWLMonFuncs"
    #define LWPP_COLORACTIVATEFUNC_GLOBAL "Color Picker"

		#define LWPP_PANELS_API_VERSION				20

		//updated from 3 to 4
		#define LWPP_ITEMINFO_GLOBAL				"LW Item Info 4"
		#define LWPP_BONEINFO_GLOBAL				"LW Bone Info 3"
		#define LWPP_LIGHTINFO_GLOBAL				"LW Light Info 3"

		//Updated LWCam info from 2 to 5
		#define LWPP_CAMERAINFO_GLOBAL				"LW Camera Info 5"

		//Added evaluator funcs
		#define LWPP_CAMERAEVALUATIONFUNCS_GLOBAL	"LW Camera Evaluation Funcs 2"

		//Updated from 3 to 4
		#define LWPP_SCENEINFO_GLOBAL				"LW Scene Info 4"
		#define LWPP_TIMEINFO_GLOBAL				"LW Time Info"
		#define LWPP_COMPINFO_GLOBAL				"LW Compositing Info"
		#define LWPP_BACKDROPINFO_GLOBAL			"LW Backdrop Info 2"
		#define LWPP_FOGINFO_GLOBAL					"LW Fog Info"

		#define LWPP_VIEWPORTINFO_GLOBAL			"LW Viewport Info"
		#define LWPP_GLOBALPOOL_RENDER_GLOBAL		"Global Render Memory"
		#define LWPP_GLOBALPOOL_GLOBAL				"Global Memory"
		#define LWPP_PREVIEWFUNCS_GLOBAL			"Preview Functions"

		#define LWPP_COMRING_GLOBAL					"LW Communication Ring"
		#define LWPP_SERVERINFO_GLOBAL				"LW Server Info"
		#define LWPP_SURFACEFUNCS_GLOBAL			"Surface Functions 3"

		#define LWPP_SYSTEMID_GLOBAL				"System ID"
		#define LWPP_PRODUCTINFO_GLOBAL				"Product Info"
		#define LWPP_MTUTILFUNCS_GLOBAL				"MultiThreading Utilities"

		#define LWPP_MTUTILFUNCS_GLOBAL_9			"MultiThreading Utilities"

		#define LWPP_VPARMFUNCS_GLOBAL				"LWVParmFuncs 2"
		#define LWPP_ENVELOPEFUNCS_GLOBAL			"Animation Envelopes"
		#define LWPP_CHANNELINFO_GLOBAL				"Channel Info 2"
		#define LWPP_XPANELFUNCS_GLOBAL				"LWXPanel"

		#define LWPP_INSTUPDATE_GLOBAL				"Instance Update"

		#define LWPP_FILEIOFUNCS_GLOBAL				"File IO"

		#define LWPP_IMAGELIST_GLOBAL				"LW Image List 3"
		#define LWPP_IMAGEUTIL_GLOBAL				"Image Utility"

		// added in LW 9.0 - only load in LW 9.0+ as well

		#define	LWPP_NODEFUNCS_GLOBAL				"NodeFuncs"
		#define	LWPP_NODEINPUTFUNCS_GLOBAL			"NodeInputFuncs"
		#define	LWPP_NODEOUTPUTFUNCS_GLOBAL			"NodeOutputFuncs"
		#define LWPP_NODEUTILITYFUNCS_GLOBAL		"NodeUtilityFuncs"
		#define LWPP_NODEDRAWFUNCS_GLOBAL			"NodeDrawFuncs"
		#define LWPP_NODEEDITORFUNCS_GLOBAL			"NodeEditorFuncs"
		#define LWPP_RASTERFUNCS_GLOBAL				"RasterServices"
		#define LWPP_PARTICLE_SERVICES_GLOBAL		"Particle Services"

		#define LWPP_LIGHTEVALUATIONFUNCS "LW Light Evaluation Funcs"

		// classes
		#define LWPP_IMAGELOADER_VERSION			2
		#define LWPP_IMAGESAVER_VERSION				2
		#define LWPP_MASTER_VERSION					4
		#define LWPP_TEXTURE_VERSION				5
		#define LWPP_SHADER_VERSION					4
		#define LWPP_IMAGEFILTER_VERSION			4
		#define LWPP_PIXELFILTER_VERSION 6
		#define LWPP_TEXTURE_VERSION				5
		#define LWPP_SHADER_VERSION					4

		#define LWPP_DISPLACEMENT_VERSION			5
		#define LWPP_CUSTOMOBJECT_VERSION			5
		#define LWPP_VOLUMETRIC_VERSION				7
		#define LWPP_NODE_VERSION 					2
		#define LWPP_MASTER_VERSION 4
		#define LWPP_COLORPICK_VERSION 4

		#define LWPP_LIGHT_VERSION 1
#elif LW_SDK==100

// globals
                #define LWPP_TEXTUREFUNCS_GLOBAL                        "Texture Functions 3"
                #define LWPP_TXTREDFUNCS_GLOBAL                         "Texture Editor 2"
 
                #define LWPP_DIRINFOFUNC_GLOBAL                         "Directory Info"
                #define LWPP_FILETYPEFUNC_GLOBAL                        "File Type Pattern"
                #define LWPP_FILEACTIVATEFUNC_GLOBAL            "File Request 2"
                #define LWPP_MESSAGEFUNCS_GLOBAL                        "Info Messages 2"
                #define LWPP_LMONFUNCS_GLOBAL                           "LWLMonFuncs"
                #define LWPP_OBJECTINFO_GLOBAL                          "LW Object Info 4"
                #define LWPP_OBJECTFUNCS_GLOBAL                         "Scene Objects 4"
                #define LWPP_INTERFACEINFO_GLOBAL                       "LW Interface Info 5"
 
                #define LWPP_PANEL_SERVICES_NAME                        "LWPanelServices"       
                #define LWPP_PANELFUNCS_GLOBAL                          "LWPanelServices"        
                #define LWPP_RASTERFUNCS_GLOBAL                         "RasterServices"        
                #define LWPP_CONTEXTMENU_GLOBAL                         "ContextMenuServices" 
                #define LWPP_XPANELFUNCS_GLOBAL                         "LWXPanel"
                #define LWPP_LMONFUNCS_GLOBAL                           "LWLMonFuncs"
                #define LWPP_PANELS_API_VERSION                         21
                
                //updated from 3 to 4
                #define LWPP_ITEMINFO_GLOBAL                            "LW Item Info 6"
                #define LWPP_BONEINFO_GLOBAL                            "LW Bone Info 3"
                #define LWPP_LIGHTINFO_GLOBAL                           "LW Light Info 3"
                
                //Updated LWCam info from 2 to 5
                #define LWPP_CAMERAINFO_GLOBAL                          "LW Camera Info 5"
                
                //Added evaluator funcs
                #define LWPP_CAMERAEVALUATIONFUNCS_GLOBAL       "LW Camera Evaluation Funcs 2"
                
                //Updated from 3 to 4
                #define LWPP_SCENEINFO_GLOBAL                           "LW Scene Info 7"
                #define LWPP_TIMEINFO_GLOBAL                            "LW Time Info"
                #define LWPP_COMPINFO_GLOBAL                            "LW Compositing Info"
                #define LWPP_BACKDROPINFO_GLOBAL                        "LW Backdrop Info 2"
                #define LWPP_FOGINFO_GLOBAL                                     "LW Fog Info"
 
                #define LWPP_VIEWPORTINFO_GLOBAL                        "LW Viewport Info"
                #define LWPP_GLOBALPOOL_RENDER_GLOBAL           "Global Render Memory"
                #define LWPP_GLOBALPOOL_GLOBAL                          "Global Memory"
                #define LWPP_PREVIEWFUNCS_GLOBAL                        "Preview Functions"
 
                #define LWPP_COMRING_GLOBAL                                     "LW Communication Ring"
                #define LWPP_SERVERINFO_GLOBAL                          "LW Server Info"
                #define LWPP_SURFACEFUNCS_GLOBAL                        "Surface Functions 3"
 
                #define LWPP_SYSTEMID_GLOBAL                            "System ID"
                #define LWPP_PRODUCTINFO_GLOBAL                         "Product Info"
                #define LWPP_MTUTILFUNCS_GLOBAL                         "MultiThreading Utilities"
 
                #define LWPP_MTUTILFUNCS_GLOBAL_9                       "MultiThreading Utilities"
 
                #define LWPP_VPARMFUNCS_GLOBAL                          "LWVParmFuncs 2"
                #define LWPP_ENVELOPEFUNCS_GLOBAL                       "Animation Envelopes"
                #define LWPP_CHANNELINFO_GLOBAL                         "Channel Info 2"
                #define LWPP_XPANELFUNCS_GLOBAL                         "LWXPanel"
                
                #define LWPP_INSTUPDATE_GLOBAL                          "Instance Update"
 
                #define LWPP_FILEIOFUNCS_GLOBAL                         "File IO"
 
                #define LWPP_IMAGELIST_GLOBAL                           "LW Image List 3"
                #define LWPP_IMAGEUTIL_GLOBAL                           "Image Utility"
 
                // added in LW 9.0 - only load in LW 9.0+ as well
 
                #define LWPP_NODEFUNCS_GLOBAL                           "NodeFuncs"
                #define LWPP_NODEINPUTFUNCS_GLOBAL                      "NodeInputFuncs"
                #define LWPP_NODEOUTPUTFUNCS_GLOBAL                     "NodeOutputFuncs"
                #define LWPP_NODEUTILITYFUNCS_GLOBAL            "NodeUtilityFuncs"
                #define LWPP_NODEDRAWFUNCS_GLOBAL                       "NodeDrawFuncs"
                #define LWPP_NODEEDITORFUNCS_GLOBAL                     "NodeEditorFuncs"
                #define LWPP_RASTERFUNCS_GLOBAL                         "RasterServices" 
                #define LWPP_PARTICLE_SERVICES_GLOBAL           "Particle Services"
 
                // classes
                #define LWPP_IMAGELOADER_VERSION                        2
                #define LWPP_IMAGESAVER_VERSION                         2
                #define LWPP_MASTER_VERSION                                     4
                #define LWPP_TEXTURE_VERSION                            5
                #define LWPP_SHADER_VERSION                                     4
                #define LWPP_PIXELFILTER_VERSION                        7
                #define LWPP_IMAGEFILTER_VERSION                        5
                #define LWPP_TEXTURE_VERSION                            5
                
                #define LWPP_DISPLACEMENT_VERSION                       5
                #define LWPP_CUSTOMOBJECT_VERSION                       5
                #define LWPP_VOLUMETRIC_VERSION                         7
                #define LWPP_NODE_VERSION                                       3
                
	#elif LW_SDK==115

// globals
                #define LWPP_TEXTUREFUNCS_GLOBAL                        "Texture Functions 4"
                #define LWPP_TXTREDFUNCS_GLOBAL                         "Texture Editor 2"
 
                #define LWPP_DIRINFOFUNC_GLOBAL                         "Directory Info"
                #define LWPP_FILETYPEFUNC_GLOBAL                        "File Type Pattern"
                #define LWPP_FILEACTIVATEFUNC_GLOBAL					"File Request 2"
                #define LWPP_MESSAGEFUNCS_GLOBAL                        "Info Messages 3"
                #define LWPP_LMONFUNCS_GLOBAL                           "LWLMonFuncs"
                #define LWPP_OBJECTINFO_GLOBAL                          "LW Object Info 9"
                #define LWPP_OBJECTFUNCS_GLOBAL                         "Scene Objects 7"
                #define LWPP_INTERFACEINFO_GLOBAL                       "LW Interface Info 7"
 
                #define LWPP_PANEL_SERVICES_NAME                        "LWPanelServices 2"       
                #define LWPP_PANELFUNCS_GLOBAL                          "LWPanelServices 2"        
                #define LWPP_RASTERFUNCS_GLOBAL                         "RasterServices"        
                #define LWPP_CONTEXTMENU_GLOBAL                         "ContextMenuServices" 
                #define LWPP_XPANELFUNCS_GLOBAL                         "LWXPanel 2"
                #define LWPP_LMONFUNCS_GLOBAL                           "LWLMonFuncs"
                #define LWPP_PANELS_API_VERSION                         21
                
                #define LWPP_ITEMINFO_GLOBAL                            "LW Item Info 6"
                #define LWPP_BONEINFO_GLOBAL                            "LW Bone Info 5"
                #define LWPP_LIGHTINFO_GLOBAL                           "LW Light Info 5"
                
                //Updated LWCam info from 5 to 5
                #define LWPP_CAMERAINFO_GLOBAL                          "LW Camera Info 7"
                
                //Added evaluator funcs
                #define LWPP_CAMERAEVALUATIONFUNCS_GLOBAL				"LW Camera Evaluation Funcs 2"
                
                //Updated from 3 to 4
                #define LWPP_SCENEINFO_GLOBAL                           "LW Scene Info 9"
                #define LWPP_TIMEINFO_GLOBAL                            "LW Time Info"
                #define LWPP_COMPINFO_GLOBAL                            "LW Compositing Info"
                #define LWPP_BACKDROPINFO_GLOBAL                        "LW Backdrop Info 2"
                #define LWPP_FOGINFO_GLOBAL                             "LW Fog Info"
 
                #define LWPP_VIEWPORTINFO_GLOBAL                        "LW Viewport Info 6"
                #define LWPP_GLOBALPOOL_RENDER_GLOBAL					"Global Render Memory"
                #define LWPP_GLOBALPOOL_GLOBAL                          "Global Memory"
                #define LWPP_PREVIEWFUNCS_GLOBAL                        "Preview Functions"
				#define LWPP_COLORACTIVATEFUNC_GLOBAL					"Color Picker"

                #define LWPP_COMRING_GLOBAL                             "LW Communication Ring"
                #define LWPP_SERVERINFO_GLOBAL                          "LW Server Info"
                #define LWPP_SURFACEFUNCS_GLOBAL                        "Surface Functions 4"
 
                #define LWPP_SYSTEMID_GLOBAL                            "System ID"
                #define LWPP_PRODUCTINFO_GLOBAL                         "Product Info"
                #define LWPP_MTUTILFUNCS_GLOBAL                         "MultiThreading Utilities 3"
 
                #define LWPP_MTUTILFUNCS_GLOBAL_9                       "MultiThreading Utilities 3"
 
                #define LWPP_VPARMFUNCS_GLOBAL                          "LWVParmFuncs 2"
                #define LWPP_ENVELOPEFUNCS_GLOBAL                       "Animation Envelopes"
                #define LWPP_CHANNELINFO_GLOBAL                         "Channel Info 2"
                
                #define LWPP_INSTUPDATE_GLOBAL                          "Instance Update"
 
                #define LWPP_FILEIOFUNCS_GLOBAL                         "File IO 3"
 
                #define LWPP_IMAGELIST_GLOBAL                           "LW Image List 6"
                #define LWPP_IMAGEUTIL_GLOBAL                           "Image Utility 4"
 
                // added in LW 9.0 - only load in LW 9.0+ as well
 
                #define LWPP_NODEFUNCS_GLOBAL                           "NodeFuncs"
                #define LWPP_NODEINPUTFUNCS_GLOBAL                      "NodeInputFuncs 3"
                #define LWPP_NODEOUTPUTFUNCS_GLOBAL                     "NodeOutputFuncs 2"
                #define LWPP_NODEUTILITYFUNCS_GLOBAL					"NodeUtilityFuncs"
                #define LWPP_NODEDRAWFUNCS_GLOBAL                       "NodeDrawFuncs"
                #define LWPP_NODEEDITORFUNCS_GLOBAL                     "NodeEditorFuncs 3"
                #define LWPP_PARTICLE_SERVICES_GLOBAL					"Particle Services"

				// 11.0 Instancer defines
				#define LWPP_ITEMINSTANCERFUNCS_GLOBAL					"ItemInstancerFuncs"
				#define LWPP_ITEMINSTANCEINFO_GLOBAL					"ItemInstanceInfo"

 
                // classes
                #define LWPP_IMAGELOADER_VERSION                        2
                #define LWPP_IMAGESAVER_VERSION                         2
                #define LWPP_MASTER_VERSION                             4
                #define LWPP_TEXTURE_VERSION                            5
                #define LWPP_SHADER_VERSION                             4
                #define LWPP_PIXELFILTER_VERSION                        7
                #define LWPP_IMAGEFILTER_VERSION                        5               
                #define LWPP_DISPLACEMENT_VERSION                       5
                #define LWPP_CUSTOMOBJECT_VERSION                       5
                #define LWPP_VOLUMETRIC_VERSION                         7
                #define LWPP_NODE_VERSION                               3
                #define LWPP_INSTANCER_VERSION                          1
                

	#endif // LW_SDK Version

#endif // LW_SDK

#endif // LW_VERSION