#include "TextureAtlas.h"

namespace AdventureEngine
{
	TextureAtlas::TextureAtlas(GLuint id, FIBITMAP* imageData, glm::uvec2 atlasSize)
	{
		m_id = id;
		m_imageData = imageData;

		m_atlasSize = atlasSize;
	}

	TextureAtlas::~TextureAtlas()
	{
		FreeImage_Unload(m_imageData);
		glDeleteTextures(1, &m_id);
	}

	GLuint TextureAtlas::getID() const
	{
		return m_id;
	}

	FIBITMAP* TextureAtlas::getImageData() const
	{
		return m_imageData;
	}

	glm::vec4 TextureAtlas::getPixel(unsigned int x, unsigned int y) const
	{
		RGBQUAD color;
		FreeImage_GetPixelColor(m_imageData, x, y, &color);

		return glm::vec4(color.rgbRed, color.rgbGreen, color.rgbBlue, color.rgbReserved);
	}

	unsigned int TextureAtlas::getWidth() const
	{
		return FreeImage_GetWidth(m_imageData);
	}

	unsigned int TextureAtlas::getHeight() const
	{
		return FreeImage_GetHeight(m_imageData);
	}

	glm::uvec2 TextureAtlas::getAtlasSize() const
	{
		return m_atlasSize;
	}
}