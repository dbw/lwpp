#include <lwpp/objectinfo.h>
#include <lwpp/surface.h>

namespace lwpp
{
	int SceneObjects::findObject(const std::string &objectName)
	{
		int num_objects = numObjects();
		if (lwpp::LightWave::isModeler())
		{
			for (int i = 0; i < num_objects; ++i)
			{
				if (objectName == refName(i))
				{
					return i;
				}
			}
		}
		for (int i = 0; i < num_objects; ++i)
		{
			if (objectName == filename(i))
			{
				return i;
			}
		}
		return -1;
	}
  LWSurface::SurfaceSet SceneObjects::getSurfaces(int idx)
	{
		LWSurface::SurfaceSet ret;
		if (idx != -1)
		{
			lwpp::LWSurface surf;
			std::string name;
      if (lwpp::LightWave::isModeler())
      {
        name = (refName(idx));
      }
      else
      {
        name = filename(idx);
      }
			while (surf.exists())
			{
        std::string so = surf.sceneObject();
				if (name == so) ret.insert(surf.getID());
				surf.next();
			}
		}
		return ret;
	}

}