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
	std::set <LWSurfaceID> SceneObjects::getSurfaces(int idx)
	{
		std::set <LWSurfaceID> ret;
		if (idx != -1)
		{
			lwpp::LWSurface surf;
			std::string name(refName(idx));
			while (surf.exists())
			{
				if (name == surf.sceneObject()) ret.insert(surf.getID());
				surf.next();
			}
		}
		return ret;
	}

}