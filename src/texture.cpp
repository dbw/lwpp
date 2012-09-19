#include <lwpp/texture.h>

namespace lwpp
{
	TextureLayer Texture::layerAdd (int type)
	{
		return TextureLayer(layerAddID(type));
	}

	TextureLayer Texture::firstLayer()
	{
		return TextureLayer(firstLayerID());
	}

	TextureLayer Texture::lastLayer()
	{
		return TextureLayer(lastLayerID());
	}

	TextureLayer Texture::nextLayer(TextureLayer layer)
	{
		return TextureLayer(nextLayerID(layer.getID()));
	}

} // lwpp