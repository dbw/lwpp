#ifndef LWPP_PREVIEW_H
#define LWPP_PREVIEW_H

#include <lwpreview.h>

namespace lwpp
{
	class PreviewCallbacks
	{
		private:
			static PreviewCallbacks* getCast(void *userData) {return static_cast<PreviewCallbacks *>(userData);}
		protected:
			virtual void previewClose(void) {;}
			virtual void previewOptions(int option) {;}
			virtual void previewPreset(LWImageID image) {;}
			virtual int previewClick(int count, PvSample *pixel) {return 0;}
			virtual int previewInit(bool manual) {return 0;}
			virtual void previewCleanup() {;}
			virtual int previewEvaluate(int w, int h, PvSample *pixel) {return 0;}
		public:
			static void closeFunc(void *userData)	{	if (userData) getCast(userData)->previewClose();}
			static void optionsFunc(int option, void *userData)	{	if (userData) getCast(userData)->previewOptions(option); }
			static void presetFunc(void *userData, LWImageID image) { if (userData) getCast(userData)->previewPreset(image); }
			static int clickFunc(int count, void *userData, PvSample *pixel)
			{ 
				if (userData) return getCast(userData)->previewClick(count, pixel);
				return 0;
			}
			static int initFunc(void *renderData, int manual)
			{
				if (renderData) return (getCast(renderData)->previewInit(manual != 0)); 
				return 0;
			}
			static void cleanupFunc(void *renderData) { if (renderData) getCast(renderData)->previewCleanup(); }
			static int evaluateFunc(void *renderData, int w, int h, PvSample *pixel)
			{ 
				if (renderData) return (getCast(renderData)->previewEvaluate(w, h, pixel)); 
				return 0;
			}
	};

	class Preview : protected GlobalBase<LWPreviewFuncs>
	{
		PvContextID id;
		PvContextID subscribe (char *title, void *userData, closeFunc *cFunc)
		{
			return globPtr->subscribe(title, userData, cFunc);
		}
		void unsubscribe(PvContextID id) { globPtr->unsubscribe(id); }
	public:
		Preview(const char *title, PreviewCallbacks *prevCB) : GlobalBase<LWPreviewFuncs>(), id(0)
		{
			if (GlobalBase<LWPreviewFuncs>::available())
			{
				id = subscribe(const_cast<char *>(title), prevCB, prevCB->closeFunc);
			}
		}
		~Preview()
		{
			if (id) unsubscribe(id);
		}
		bool isValid(void){return (id !=0);}
		void open(void) { globPtr->open(id); }
		void close(void) { globPtr->close(); }
		bool isOpen(void) { return (globPtr->isOpen() != 0); }
		void setContext(void) {globPtr->setContext(id);}
		void setClick(clickFunc *cf) {globPtr->setClick(id, cf);}
		void setRender(PreviewCallbacks *prevCB)
		{
			globPtr->setRender(id, prevCB, prevCB->initFunc, prevCB->cleanupFunc, prevCB->evaluateFunc);
			setClick(PreviewCallbacks::clickFunc);
		}
		void setOptions(const char **list, PreviewCallbacks *prevCB, int selection)
		{
			globPtr->setOptions(id, const_cast<char **>(list), prevCB->optionsFunc, selection);
		}
		void startRender() {globPtr->startRender(id);}
		void stopRender() {globPtr->stopRender(id);}
		void getPixel(PvSample *pixel) {globPtr->getPixel(pixel);}
		LWImageID getBitmap(int *width, int *height) {return globPtr->getBitmap(width, height);}
		LWItemID getCamera(double pos[3], double rot[3], double *zoomFactor) {return globPtr->getCamera(pos, rot, zoomFactor);}
		void getView(int *width, int *height, double *pixelAspect) {globPtr->getView(width, height, pixelAspect);}
		void setPreset(PreviewCallbacks *prevCB) {globPtr->setPreset(id, prevCB->presetFunc);}
	};
}

#endif