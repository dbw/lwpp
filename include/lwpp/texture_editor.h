/*!
 * @file
 * @brief
 * @version $Id: texture_editor.h 93 2011-01-05 17:27:10Z mwolf $
 */

#ifndef LWPP_TXTEDITOR_H
#define LWPP_TXTEDITOR_H


#include <lwpp/global.h>
#include <lwtxtred.h>

namespace lwpp
{

	//! @ingroup Globals
	/*!
	*  Give access to the editing of textures with the Texture Editor
	*/
	class TextureEditor : protected GlobalBase<LWTxtrEdFuncs>
	{
		private:
			LWTECltID txtrEditorID;
		public:
			public:
			TextureEditor() 
				: txtrEditorID(0)
			{
				;
			}
			//! Constructor
      /*!
			*   @param title Title of the Texture Editor
			*   @param flags Indicate what the user can do with the editor can be:
			*			TEF_USEBTN	Add the Use Texture and Remove Texture buttons at the bottom of the panel.
			*			TEF_OPACITY Add layer opacity settings.
			*			TEF_BLEND	Add blend options to the layer global settings.
			*			TEF_TYPE	Add layer type control on the top of the pane.
			*			TEF_LAYERS	Add layer list pane on the left side of the pane.
			*			TEF_ALL		All of the above flags. This is the standard configuration for the texture editor.
			*	@param  userData Pointer to data created by the user, it can be NULL
			*	@param  txtr_RemoveFunc Callback to the function used when the user removes a texture. It is optional, and can be NULL.
			*	@param  txtr_AutoSizeFunc Callback to the function used when the user whants to autosize a texture. It is optional, and can be NULL.
			*	@param  txtr_EventFunc Callback to the function used modifiy the settings of a texture. It is optional, and can be NULL. 	
			*/
			 TextureEditor (char *title,
							int flags, 
							void *userData = 0,
							LW_TxtrRemoveFunc *txtr_RemoveFunc = 0,
							LW_TxtrAutoSizeFunc *txtr_AutoSizeFunc = 0,
							LW_TxtrEventFunc *txtr_EventFunc = 0)
			{
				txtrEditorID = subscribe( title, flags, userData, txtr_RemoveFunc , txtr_AutoSizeFunc, txtr_EventFunc);
			}
			
			//! Returns the ID of the editor
			LWTECltID getID() const { return txtrEditorID; }
			
			//! Returns the ID of the Texture Editor for later calls.
      /*!
			*   @param title Title of the Texture Editor
			*   @param flags Indicate what the user can do with the editor can be:
			*			TEF_USEBTN	Add the Use Texture and Remove Texture buttons at the bottom of the panel.
			*			TEF_OPACITY Add layer opacity settings.
			*			TEF_BLEND	Add blend options to the layer global settings.
			*			TEF_TYPE	Add layer type control on the top of the pane.
			*			TEF_LAYERS	Add layer list pane on the left side of the pane.
			*			TEF_ALL		All of the above flags. This is the standard configuration for the texture editor.
			*	@param  userData Pointer to data created by the user, it can be NULL
			*	@param  txtr_RemoveFunc Callback to the function used when the user removes a texture. It is optional, and can be NULL.
			*	@param  txtr_AutoSizeFunc Callback to the function used when the user whants to autosize a texture. It is optional, and can be NULL.
			*	@param  txtr_EventFunc Callback to the function used modifiy the settings of a texture. It is optional, and can be NULL. 	
			*/
			LWTECltID subscribe (char *title,
								 int flags,
								 void *userData, 
								 LW_TxtrRemoveFunc *txtr_RemoveFunc = 0, 
								 LW_TxtrAutoSizeFunc *txtr_AutoSizeFunc = 0, 
								 LW_TxtrEventFunc *txtr_EventFunc = 0)
			{
				return globPtr->subscribe( title, flags, userData, txtr_RemoveFunc, txtr_AutoSizeFunc, txtr_EventFunc);
			}
			//! Frees the editor resources acquires with suscribe.
      /*!
      *  this will clear the ID, so you will to call 
			*  suscribe again to be able to use the Texture Editor Functions
			*/
			void unsubscribe( void ) { 	globPtr->unsubscribe(txtrEditorID);	}
			//! Opens the panel for the Texture editor.
      /*!
			*   @param txtr_ID Texture ID to edit in the Texture Editor
			*   @param title Title of the Texture Editor
			*/
			void open ( LWTextureID txtr_ID, char *title = 0) {	globPtr->open(txtrEditorID, txtr_ID, title ); }
			//! Moves the Texture editor window.
      /*!
			*   @param x Horizontal coordinate in pixels of the Texture editor Window
			*   @param y Vertical coordinate in pixels of the Texture editor Window
			*/
			void setPosition (int x, int y) {	globPtr->setPosition(txtrEditorID, x, y); }

			//! Closes the Texture editor window.
			void close () {	globPtr->close(txtrEditorID); }

			//! Returns TRUE if the Texture Editor Window is open.
			bool isOpen ( ) {	return (globPtr->isOpen(txtrEditorID))? true : false;   }

			//! Returns TRUE if the Texture Editor Window has been correctly refresh.			
			bool refresh ()	{	return (globPtr->refresh(txtrEditorID)) ? true : false;	}

			//! Returns the Texture Layer ID of the layer beign edited 
			LWTLayerID  currentLayer ( ) {	return globPtr->currentLayer(txtrEditorID);	}

			//! Add a Texture to a multiselection. Returns true if there is no problems
      /*!
			*   @param txtr_ID Layer ID of the texture to add to the multiselection
			*/
			bool selectAdd ( LWTextureID txtr_ID) {	return (globPtr->selectAdd(txtrEditorID, txtr_ID)) ? true : false; }

			//! Removes a Texture from a multiselection. Returns true if there is no problems
      /*!
			*   @param txtr_ID Layer ID of the texture to remove from the multiselection
			*/
			bool selectRem ( LWTextureID txtr_ID) {	return (globPtr->selectRem(txtrEditorID, txtr_ID)) ? true : false; }

			//! Clears the multiselection. Returns true if there is no problems
			bool selectClr ( ) {  return (globPtr->selectClr(txtrEditorID)) ? true : false; }

			//! Selects the first texture, returning its Texture ID
			LWTextureID selectFirst ( ) {  return globPtr->selectFirst(txtrEditorID); }

			//! Selects the next texture, returning its Texture ID
      /*!
			*   @param txtr_ID Texture ID of the texture to select
			*/
			LWTextureID selectNext ( LWTextureID txtr_ID){ return globPtr->selectNext(txtrEditorID, txtr_ID); }
			//! Designates a callback to perform autosizing
      /*!
			*   @param *txtr_GradAutoSizeFunc Function
			*/
			void setGradientAutoSize ( LW_GradAutoSizeFunc *txtr_GradAutoSizeFunc) {	globPtr->setGradientAutoSize(txtrEditorID, txtr_GradAutoSizeFunc); }
	
	}; // end class TextureEditor

} // end namespace
#endif // LWPP_TXTEDITOR_H