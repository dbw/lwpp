#ifndef LWPP_INTERFACE_H
#define LWPP_INTERFACE_H

#include <lwpp/global.h>

// For the schematicPosition function
#include <lwpp/point3d.h>

// Standard C++ Library includes
#include <vector>

namespace lwpp
{
	class LWItem; // Forward declaration
    
    DEFINE_GLOBAL(LWInterfaceInfo)

	//! @ingroup Globals
	//! Use this class to get several information about the state of LightWave's Layout Interface
	//! Usage:
	//!	On your header declare a member
	//!	lwpp::InterfaceInfo intInfo;
	//! Initialize it like this: (for example in the constructor of your plugin class)
	//! intInfo = lwpp::InterfaceInfo();
	//! NOTE: InterfaceInfo need to be re-created EVERY time before it's used, to be
	//! sure it contains updated information about Lightwave's Interface
	class InterfaceInfo : protected GlobalBase<LWInterfaceInfo>
	{
		private:
			std::vector<LWItem *> selItems;

		public:
			InterfaceInfo( void ) { ; }

			// CURRENT TIME
			//! Returns the current time indicated by the frame slider on Layout
			LWTime currentTime(void)
			{
				if (globPtr) return globPtr->curTime;
				else return 0;
			}

			// SELECTED ITEMS
			//! Returns the lwpp::LWItem of the selected Item in Layout, or the first selected item of a multiple selection
			// const LWItem selectedItem( void );
			lwpp::LWItem selectedItem( ) const;

			//! Returns a std::vector<lwpp::LWItem> with 1 or multiple elements depending the selected Items in Layout
			std::vector<lwpp::LWItem> selectedItems() const;

			//ITEM FLAGS
			//! Returns the flags associated with the LWItemID
			unsigned int itemFlags(LWItemID item)
			{
				if (globPtr) return globPtr->itemFlags(item);
				return 0;
			}

			// PREVIEW OPTIONS
			//! Returns the Start Frame defined for previews on Layout Preview options
			LWFrame previewStart(void)
			{
				if (globPtr) return globPtr->previewStart;
				return 0;
			}
			//! Returns the End Frame defined for previews on Layout Preview options
			LWFrame previewEnd(void)
			{
				if (globPtr) return globPtr->previewEnd;
				return 0;
			}
			//! Returns the Step Frame defined for previews on Layout Preview options
			LWFrame previewStep(void)
			{
				if (globPtr) return globPtr->previewStep;
				return 0;
			}

			// DYNAMIC UPDATE
			//! Returns an int indicating one of the three posible states of Dynamic Update
			//! in Layout Display Options
			int dynamicUpdate(void)
			{
				if (globPtr) return (globPtr->dynaUpdate);
				return 0;
			}
			//! Returns true if the Dynamic Update on Layout Display Options is off
			bool isDynamicUpdateOff(void)
			{
				return (dynamicUpdate() & LWDYNUP_OFF);
			}
			//! Returns true if the Dynamic Update on Layout Display Options is delayed
			bool isDynamicUpdateDelayed(void)
			{
				return (dynamicUpdate() & LWDYNUP_DELAYED) != 0;
			}
			//! Returns true if the Dynamic Update on Layout Display Options is interactive
			bool isDynamicUpdateInteractive(void)
			{
				return (dynamicUpdate() & LWDYNUP_INTERACTIVE)!= 0;
			}

			// SCHEMATIC POSITION
			//! Returns a lwpp::Point3d with it's x and y members indicating
			//! it's position inside the Schematic View
			Point3d schematicPosition(LWItemID item)
			{
				if (globPtr)
				{
					double x, y;
					globPtr->schemaPos(item, &x, &y);
					return Point3d (x,y,0);
				}
				return Point3d();
			}

			// ITEM DISPLAY
			//! Retirns an int indicating one of the eight
			//! possible shading states of an Item
			int itemVisibility(LWItemID item)
			{
				if (globPtr) return globPtr->itemVis(item);
				return 0;
			}
			//! Returns true if the item is hidden on Layout's interface
			bool isItemHidden(LWItemID item)
			{
				return (itemVisibility(item) & LWOVIS_HIDDEN);
			}
			//! Returns true if the item is showed as a Bounding Box on Layout's interface
			bool isItemBBox(LWItemID item)
			{
				return (itemVisibility(item) & LWOVIS_BOUNDINGBOX) !=0;
			}
			//! Returns true if the item is showed as Vertices on Layout's interface
			bool isItemVertices(LWItemID item)
			{
				return (itemVisibility(item) & LWOVIS_VERTICES) !=0;
			}
			//! Returns true if the item is showed as Wireframe on Layout's interface
			bool isItemWireframe(LWItemID item)
			{
				return (itemVisibility(item) & LWOVIS_WIREFRAME) !=0;
			}
			//! Returns true if the item is showed as Front Face Wireframe on Layout's interface
			bool isItemFrontFace(LWItemID item)
			{
				return (itemVisibility(item) & LWOVIS_FFWIREFRAME) !=0;
			}
			//! Returns true if the item is showed as Shaded on Layout's interface
			bool isItemShaded(LWItemID item)
			{
				return (itemVisibility(item) & LWOVIS_SHADED) !=0;
			}
			//! Returns true if the item is showed as Textured on Layout's interface
			bool isItemTextured(LWItemID item)
			{
				return (itemVisibility(item) & LWOVIS_TEXTURED) !=0;
			}
			//! Returns true if the item is showed as Textured Shaded on Layout's interface
			bool isItemTexturedShaded(LWItemID item)
			{
				return (itemVisibility(item) & LWOVIS_TEXTURED_WIRE) !=0;
			}

			// DISPLAY FLAGS
			unsigned int displayFlags(void)
			{
				if (globPtr) return globPtr->displayFlags;
				return 0;
			}
			//! Returns true if Motion Paths are displayed on Layout's interface
			bool isMotionPathsDisplayed( void )
			{
				return (displayFlags()  & LWDISPF_MOTIONPATHS) !=0;
			}
			//! Returns true if Handles are displayed on Layout's interface
			bool isHandlesDisplayed( void )
			{
				return (displayFlags()  & LWDISPF_HANDLES) !=0;
			}
			//! Returns true if IK Chains are displayed on Layout's interface
			bool isIKChainsDisplayed( void )
			{
				return (displayFlags()  & LWDISPF_IKCHAINS) !=0;
			}
			//! Returns true if sub-patch cages are displayed on Layout's interface
			bool isCagesDisplayed( void )
			{
				return (displayFlags()  & LWDISPF_CAGES) !=0;
			}
			//! Returns true if Safe Areas are displayed on Layout's Camera View
			bool isSafeAreasDisplayed( void )
			{
				return (displayFlags()  & LWDISPF_SAFEAREAS) !=0;
			}
			//! Returns true if the Field Chart is displayed on Layout's Camera View
			bool isFieldChartDisplayed( void )
			{
				return (displayFlags()  & LWDISPF_FIELDCHART) !=0;
			}

			// GENERAL FLAGS
			//! Returns the value of several general flags state
			unsigned int generalFlags(void)
			{
				if (globPtr) return globPtr->generalFlags;
				return 0;
			}
			//! Returns true if the Tool Bar is hidden on Layout's Interface
			bool isToolBarHidden( void )
			{
				return (generalFlags()  & LWGENF_HIDETOOLBAR) !=0;
			}
			//! Returns true if the Tool Bar is displayed on the right side of Layout's Interface
			bool isToolBarRight( void )
			{
				return (generalFlags()  & LWGENF_RIGHTTOOLBAR) !=0;
			}
			//! Returns true if Parent in Place is active
			bool isParentInPlace( void )
			{
				return (generalFlags()  & LWGENF_PARENTINPLACE) !=0;
			}
			//! Returns true if Fractional Frames is active
			bool isFractionalFrames( void )
			{
				return (generalFlags()  & LWGENF_FRACTIONALFRAME) !=0;
			}
			//! Returns true if Keys are visible on the timeline
			bool isKeyFramesVisible( void )
			{
				return (generalFlags()  & LWGENF_KEYSINSLIDER) !=0;
			}
			//! Returns true if Play At Exact Rate is active on Layout General Preferences
			bool isPlayAtExactRate( void )
			{
				return (generalFlags()  & LWGENF_PLAYEXACTRATE) !=0;
			}
			//! Returns true if Auto Key is active on Layout Interface
			bool isAutoKeyOn( void )
			{
				return (generalFlags()  & LWGENF_AUTOKEY) !=0;
			}
			//! Returns true if Inverse Kinematics is active on Layout Interface
			bool isIKOn( void )
			{
				return (generalFlags()  & LWGENF_IKENABLE) !=0;
			}
			//! Returns true if Left button Item Select is active on Layout General Preferences
			bool isLMBSelectionOn( void )
			{
				return (generalFlags()  & LWGENF_LMBITEMSELECT) !=0;
			}
#if (LW_SDK >= 96)
			//! Returns true if Dbl. Click Bone Mode is active on Layout General Preferences
			bool isDBLClickBoneOn( void )
			{
				return (generalFlags()  & LWGENF_DOUBLECLICKBONEMODE) !=0;
			}
			//! Returns true if Motion Controllers is active on Layout Interface
			bool isMotionControllersOn( void )
			{
				return (generalFlags()  & LWGENF_MCENABLE) !=0;
			}
#endif
			// BOUNDINGBOX THRESHOLD
			//! Returns the Bounding Box value on Layout Display options
			int	boxThreshold()
			{
				if (globPtr) return (globPtr->boxThreshold);
				return 0;
			}

			// ALERT LEVELS
			//! Returns the Alert Level specified on LightWave's Display Options
			int alertLevel(void)
			{
				if (globPtr) return globPtr->alertLevel;
				else return 0;
			}
			//! Returns true if the alert level is beginner on LightWave's Display Options
			bool showInfo(void) {return (alertLevel() <= LWALERT_BEGINNER);}
			//! Returns true if the alert level is intermediate on LightWave's Display Options
			bool showWarning(void) {return (alertLevel() <= LWALERT_INTERMEDIATE);}
			//! Returns true if the alert level is expert on LightWave's Display Options
			bool showError(void) {return (alertLevel() <= LWALERT_EXPERT);}

			//AUTO KEY
			//! Returns one of the three possible states of the Auto Create Key
			//! setting in Layout's General Preferences
			int autoKeyCreate()
			{
				if (globPtr) return globPtr->autoKeyCreate;
				return 0;
			}
			//! Returns true if Auto Key Create in Layout's General Preferences
			bool isAutoKeyCreateOff(void)
			{
				return (autoKeyCreate() == LWAKC_OFF);
			}
			//! Returns true if Auto Key Create is Only Modificed Channels in Layout's General Preferences
			bool isAutoKeyCreateModChannels(void)
			{
				return (autoKeyCreate() == LWAKC_MODIFIED);
			}
			//! Returns true if Auto Key Create is All Channels in Layout's General Preferences
			bool isAutoKeyCreateAllChannels(void)
			{
				return (autoKeyCreate() == LWAKC_ALL);
			}

			// ITEM COLORS
			//! Returns the index of the color used on Wireframe to draw the item in Layout Viewports
			int  itemColor  (LWItemID item)
			{
				if (globPtr) return globPtr->itemColor(item);
				else return 0;
			}
			//! Changes the color to the item to one of the indexed colors
			/*!
       * @param[in] item the itemid of the item to change
       * @param[in] color the new colour as an index or a a 24bit color if you set the upper byte of a 32bit color to 0x01. I.e. Red will be 0x01FF0000
       */
			void setItemColorIndex(LWItemID item, int color)
			{
				if (globPtr) globPtr->setItemColorIndex(item, color);
			}
			//! Changes the color to the item using 0.0 to 1.0 values for each R,G,B.
			/*!
       * @param[in] item the itemid of the item to change
       * @param[in] color the new colour as a float RGB triplet
       */
			void setItemColorCustom(LWItemID item, double *color)
			{
				if (globPtr) globPtr->setItemColorCustom(item, color);
			}
#if (LW_SDK >= 92)
			//! Changes the default color for draw objects on Layout's Viewports
			/*!
       * @param[out] color will contain the colour as a float RGB triplet
       */
			void setObjectsDefaultColor(float color)
			{
				if (globPtr) globPtr->defaultItemTypeColor(LWI_OBJECT, &color, 1);
			}
			//! Changes the default color for Lights on Layout's Viewports
			/*!
       * @param[out] color will contain the colour as a float RGB triplet
       */
			void setLightsDefaultColor(float color)
			{
				if (globPtr) globPtr->defaultItemTypeColor(LWI_LIGHT, &color, 1);
			}
			//! Changes the default color for Cameras on Layout's Viewports
			/*!
       * @param[out] color will contain the colour as a float RGB triplet
       */
			void setCamerasDefaultColor(float color[3])
			{
				if (globPtr) globPtr->defaultItemTypeColor(LWI_CAMERA, color, 1);
			}
			//! Changes the default color for Bones on Layout's Viewports
			/*!
       * @param[out] color will contain the colour as a float RGB triplet
       */
			void setBonesDefaultColor(float color[3])
			{
				if (globPtr) globPtr->defaultItemTypeColor(LWI_BONE, color, 1);
			}
			//! Gets the default color for draw Objects on Layout's Viewports
			/*!
       * @param[out] color will contain the colour as a float RGB triplet
       */
			void getObjectsDefaultColor(float color[3])
			{
				if (globPtr) globPtr->defaultItemTypeColor(LWI_OBJECT, color, 0);
			}
			//! Gets the default drawing color for Lights in Viewports
			/*!
       * @param[out] color will contain the colour as a float RGB triplet
       */
			void getLightsDefaultColor(float color[3])
			{
				if (globPtr) globPtr->defaultItemTypeColor(LWI_LIGHT, color, 0);
			}
			//! Gets the default drawing color for Cameras in viewports
			/*!
       * @param[out] color will contain the colour as a float RGB triplet
       */
			void getCamerasDefaultColor(float color[3])
			{
				if (globPtr) globPtr->defaultItemTypeColor(LWI_CAMERA, color, 0);
			}
			//! Gets the default drawing color for Bones in viewports
			/*!
       * @param[out] color will contain the colour as a float RGB triplet
       */
			void getBonesDefaultColor(float color[3])
			{
				if (globPtr) globPtr->defaultItemTypeColor(LWI_BONE, color, 0);
			}
#if (LW_SDK >= 96)
			void getItemColour( LWItemID item, unsigned int state, float rgba[4])
			{
				if (globPtr) globPtr->itemColorRgba(item, state, rgba);
			}
			void getItemColour( LWItemID item, float rgba[4])
			{
				getItemColour(item, LWITEMCOL_CURRENT, rgba);
			}
#else
			void getItemColour( LWItemID item, float rgba[4])
			{
				if (globPtr) globPtr->itemColorRgba(item, rgba);
			}
#endif

#if (LW_SDK >= 96)
			//! Gets the color of Items on it's current state on Layout's Viewports
			//! @parameter color is an float[4] array
			//! For example Red will be { 1.0, 0.0, 0.0}
			void getItemsCurrentColor( LWItemID item, float rgba[4])
			{
				if (globPtr) globPtr->itemColorRgba(item, LWITEMCOL_CURRENT, rgba);
			}
			//! Gets the color of Items on it's normal state on Layout's Viewports
			//! @parameter color is an float[4] array
			//! For example Red will be { 1.0, 0.0, 0.0}
			void getItemsNormalColor( LWItemID item, float rgba[4])
			{
				if (globPtr) globPtr->itemColorRgba(item, LWITEMCOL_NORMAL, rgba);
			}
			//! Gets the color of Items on it's selected state on Layout's Viewports
			//! @parameter color is an float[4] array
			//! For example Red will be { 1.0, 0.0, 0.0}
			void getItemsSelectedColor( LWItemID item, float rgba[4])
			{
				if (globPtr) globPtr->itemColorRgba(item, LWITEMCOL_SELECTED, rgba);
			}
#endif // LW_SDK 96+
			//! Returns the icon scale of the Item in Layout's Viewports
			/*!
       * It's used to size the icon of the item on them.
       */
			float getItemIconScale( LWItemID item)
			{
				if (globPtr) return globPtr->itemIconScale(item);
        return 1.0;
			}
#endif
	};
}

#endif

// OLD VERSION //
//#ifndef LWPP_INTERFACE_H
//#define LWPP_INTERFACE_H
//
//#include <lwpp/global.h>
//
//namespace lwpp
//{
//
//	class LWItem; // Forward declaration
//
//	//! @ingroup Globals
//	class InterfaceInfo : protected GlobalBase<LWInterfaceInfo>
//	{
//		public:
//			InterfaceInfo( void ) { ; }
//
//			// CURRENT TIME
//			//! Returns the current time indicated by the frame slider on Layout
//			LWTime currentTime(void)
//			{
//				if (globPtr) return globPtr->curTime;
//				else return 0;
//			}
//
//			// SELECTED ITEMS
//			//! Returns the lwpp::LWItem of the selected Item in Layout, or the first selected item of a multiple selection
//			void * selectedItem( void );
//
//
//			int alertLevel(void)
//			{
//				if (globPtr) return globPtr->alertLevel;
//				else return 0;
//			}
//			bool showInfo() {return (alertLevel() <= LWALERT_BEGINNER);}
//			bool showWarning() {return (alertLevel() <= LWALERT_INTERMEDIATE);}
//			bool showError() {return (alertLevel() <= LWALERT_EXPERT);}
//			int  itemColor  (LWItemID mID)
//			{
//				return globPtr->itemColor(mID);
//			}
//	};
//
//}
//
///*
//typedef struct st_LWInterfaceInfo {
//        LWTime            curTime;
//        const LWItemID   *selItems;
//        unsigned int    (*itemFlags)     (LWItemID);
//        LWFrame           previewStart;
//        LWFrame           previewEnd;
//        LWFrame           previewStep;
//        int               dynaUpdate;
//        void            (*schemaPos)     (LWItemID, double *x, double *y);
//        int             (*itemVis)       (LWItemID);
//        unsigned int      displayFlags;
//        unsigned int      generalFlags;
//        int               boxThreshold;
//        int             (*itemColor)     (LWItemID);
//        int               alertLevel;
//        int               autoKeyCreate;
//} LWInterfaceInfo;
//*/
//
//#endif