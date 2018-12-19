#pragma once

#ifndef LWPP_SURFED_H
#define LWPP_SURFED_H

#include <lwpp/global.h>
#include <lwsurfed.h>
#include <lwpp/surface.h>
#include <set>
#include <lwpp/wrapper.h>

namespace lwpp {

	class SurfaceEditor : protected GlobalBase<LWSurfEdFuncs>
	{
	public:
		void Open(int i) { globPtr->open(i); }
		void Close() { globPtr->close(); }
		bool isOpen() { return (globPtr->isOpen() != 0); }
		void SetSurface(LWSurfaceID id) { globPtr->setSurface(id); }
		void SetSurface(LWSurface &surf) { globPtr->setSurface(surf.getID()); }
		void SetPosition(int  x, int  y) { globPtr->setPosition(x, y); }
		void GetPosition(int  *x, int *y, int *w, int *h) { globPtr->getPosition(x, y, w, h); }
		void AddLib(LWSurfLibID lib) { globPtr->addLib(lib); }

		LWSurfaceID GetSelection(int index = LWSURFEDFUNCS_PRIMARYINDEX) { return globPtr->getSelection(index); }
	};
}

#endif // LWPP_SURFED_H
