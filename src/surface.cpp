#include <lwpp/surface.h>
#include <lwpp/objectinfo.h>

namespace lwpp
{
	LWSurface::SurfaceSet LWSurface::byName(std::string name, std::string obj) const
	{
		LWSurfaceID *ids = globPtr->byName(name.c_str(), obj.c_str());
		SurfaceSet ret;
		while (*ids)
		{
			ret.insert(LWSurface(*ids));
			ids++;
		}
		return ret;
	}

	LWSurface::SurfaceSet LWSurface::byName(std::string name) const
	{
		SurfaceSet ret;
		
		LWSurfaceID *ids = globPtr->byName(name.c_str(), NULL);
		
		while (*ids)
		{
			ret.insert(LWSurface(*ids));
			ids++;
		}
		/*
		LWSurface surf;
		while (surf.exists())
		{
			if (name == surf.name())
			{
				ret.insert(surf);
			}
			surf.next();
		}
		*/
		return ret;
	}

	LWSurface::SurfaceSet LWSurface::byObject(std::string name) const
	{
		LWSurfaceID *ids = globPtr->byObject(name.c_str());
		SurfaceSet ret;
		while (*ids)
		{
			ret.insert(LWSurface(*ids));
			ids++;
		}
		return ret;
	}

	LWSurface::SurfaceSet LWSurface::getAll() 
	{
		SurfaceSet ret;
		LWSurface surf;
		if (!surf.exists()) return ret;
		do {
			ret.insert(surf);				
		} while (surf.next());

		return ret;
	}
}

