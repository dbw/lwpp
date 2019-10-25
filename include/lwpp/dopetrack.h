#pragma once

#ifndef LWPP_DOPETRACK_H
#define LWPP_DOPETRACK_H

#include "lwdopetrack.h"
#include <lwpp/global.h>
#include <lwpp/debug.h>
#include <vector>
namespace lwpp
{
	// since only one tool can be active at any one time, and there is no userData for dopetracks,
	// we're going to use a global pointer to the plugin instanve which needs to be set
	class DTCallbacks;
	extern DTCallbacks *dopeTrackInstance;

	class DTCallbacks
	{
	public:
		virtual void dtUserEvent(DTEventParamID param) {  }
		virtual int dtAllow(DTKeyAction action) { return 1; }
		virtual void dtMouse(DTMouseParamID mouseParam) { }
		virtual int dtMenuCount(int){return 0;}
		virtual int dtMenuSub(int, int)	{return 0;}
		virtual int* dtMenuSep(int)	{return 0;}
		virtual int dtMenuEnable(int, int){	return 0;}
		virtual const char* dtMenuItem(int, int){return 0;}
		virtual void dtMenuSelect(int, int){}
		virtual int dtMenuInitial(void)	{return 0;}

		static void cbUserEvent(DTEventParamID param)
		{		
			if (dopeTrackInstance) dopeTrackInstance->dtUserEvent(param);			
		}
		static int cbAllow(DTKeyAction action)
		{
			if (dopeTrackInstance) return dopeTrackInstance->dtAllow(action);
			return 0;
		}
		static void cbMouse(DTMouseParamID mouseParam)
		{
			if (dopeTrackInstance) dopeTrackInstance->dtMouse(mouseParam);
		}
		static int cb_dtMenuCount(int men)
		{
			if (dopeTrackInstance) return dopeTrackInstance->dtMenuCount(men);
			return 0;
		}
		static int cb_dtMenuSub(int men, int it)
		{
			if (dopeTrackInstance) return dopeTrackInstance->dtMenuSub(men, it);
			return 0;
		}
		static int *cb_dtMenuSep(int men)
		{
			if (dopeTrackInstance) return dopeTrackInstance->dtMenuSep(men);
			return nullptr;
		}
		static int cb_dtMenuEnable(int men, int it)
		{
			if (dopeTrackInstance) return dopeTrackInstance->dtMenuEnable(men, it);
			return 0;
		}
		static const char *cb_dtMenuItem(int men, int it)
		{
			if (dopeTrackInstance) return dopeTrackInstance->dtMenuItem(men, it);
			return 0;
		}
		static void cb_dtMenuSelect(int men, int it)
		{
			if (dopeTrackInstance) dopeTrackInstance->dtMenuSelect(men, it);
		}
		static int cb_dtMenuInitial()
		{
			if (dopeTrackInstance) return dopeTrackInstance->dtMenuInitial();
			return 0;
		}

	};

	enum {DTFL_MENU = 1 << 0,
		DTFL_ACTIONS = 1 << 1,
		DTFL_MOUSE = 1 << 1,
	};

	template <int tFlags = 0>
	class Dopetrack : protected GlobalBase<LWDopetrackProxy>
	{
		DTToolCallbacks callbacks =
		{
			&DTCallbacks::cbUserEvent,
			(tFlags& DTFL_ACTIONS ? &DTCallbacks::cbAllow: nullptr),
			(tFlags& DTFL_MOUSE ? &DTCallbacks::cbMouse:nullptr),
			{
				(tFlags & DTFL_MENU ? &DTCallbacks::cb_dtMenuCount : nullptr),
				(tFlags& DTFL_MENU ? &DTCallbacks::cb_dtMenuSub : nullptr),
				(tFlags& DTFL_MENU ? &DTCallbacks::cb_dtMenuSep : nullptr),
				(tFlags& DTFL_MENU ? &DTCallbacks::cb_dtMenuEnable : nullptr),
				(tFlags& DTFL_MENU ? &DTCallbacks::cb_dtMenuItem : nullptr),
				(tFlags& DTFL_MENU ? &DTCallbacks::cb_dtMenuSelect : nullptr),
				(tFlags& DTFL_MENU ? &DTCallbacks::cb_dtMenuInitial : nullptr)
			}
		};
	public:
		
		Dopetrack(DTCallbacks* instance = nullptr)
		{		
			if (instance)
			{
				dopeTrackInstance = instance;
				globPtr->toolRegister(&callbacks);
			}
		}
		void Aquire(DTCallbacks* instance)
		{
			if (dopeTrackInstance == nullptr)
			{
				dopeTrackInstance = instance;
				globPtr->toolRegister(&callbacks);
			}
		}
		void Release()
		{
			if (dopeTrackInstance != nullptr)
			{
				globPtr->toolRelease();
				dopeTrackInstance = nullptr;
			}
		}
		
		virtual ~Dopetrack()
		{
			if (dopeTrackInstance != nullptr)
			{
				globPtr->toolRelease();
				dopeTrackInstance = nullptr;			
			}
		}
		
		void  exposeEnvelopes(LWEnvelopeID* envs, const char** channel_names /* utf8 encoded? */, int* states)
		{
			globPtr->exposeEnvelopes(envs, channel_names, states);
		}
		void  exposeEnvelopes(std::vector< LWEnvelopeID> &envs)
		{
			static const char* channel_names[] = { "X", "Y", "Z", 0 };
			static int          channel_states[] = { 1,1,1 };
			globPtr->exposeEnvelopes(&envs[0], channel_names, channel_states);
		}
		void refreshDisplay()
		{
			globPtr->refreshDisplay();
		}

		// selection
		DTKeySelectID querySelectedKeys()
		{
			return globPtr->querySelectedKeys();
		}

		void querySelection(LWTime* start, LWTime* end)
		{
			globPtr->querySelection(start, end);
		}

		// marker
		const LWTime* queryMarkers()
		{
			return globPtr->queryMarkers();
		}
		void addMarker(LWTime t)
		{
			globPtr->addMarker(t);
		}
		void remMarker(LWTime t)
		{
			globPtr->remMarker(t);
		}

		DTBakeZoneID queryBakeZones()
		{
			return globPtr->queryBakeZones();
		}

		void addBakeZone(LWTime start, LWTime end)
		{
			globPtr->addBakeZone(start, end);
		}
		void remBakeZone(LWTime start, LWTime end)
		{
			globPtr->addBakeZone(start, end);
		}
		DTOperatingMode queryOpMode()
		{
			return globPtr->queryOpMode();
		}

		void displayMenu(DTMenuCallbacksID cb)
		{
			globPtr->displayMenu(cb);
		}

		DTDrawFuncs& draw()
		{
			return globPtr->drawfuncs;
		}

		bool isVisible() { return globPtr->visible() != 0; }
		
	};
}
#endif //LWPP_DOPETRACK_H
