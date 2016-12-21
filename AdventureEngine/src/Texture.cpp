#include "Texture.h"

namespace AdventureEngine
{
	Texture::Texture(GLuint id, FIBITMAP* imageData, unsigned int atlasIndex, glm::uvec2 atlasSize)
	{
		m_id = id;
		m_imageData = imageData;
		m_atlasIndex = atlasIndex;
		m_atlasSize = atlasSize;
	}

	Texture::~Texture()
	{
	}

	GLuint Texture::getID() const
	{
		return m_id;
	}

	FIBITMAP* Texture::getImageData() const
	{
		return m_imageData;
	}

	glm::vec4 Texture::getPixel(unsigned int x, unsigned int y) const
	{
		RGBQUAD color;
		FreeImage_GetPixelColor(m_imageData, x, y, &color);

		return glm::vec4(color.rgbRed, color.rgbGreen, color.rgbBlue, color.rgbReserved);
	}

	unsigned int Texture::getWidth() const
	{
		return FreeImage_GetWidth(m_imageData);
	}

	unsigned int Texture::getHeight() const
	{
		return FreeImage_GetHeight(m_imageData);
	}

	unsigned int Texture::getAtlasIndex() const
	{
		return m_atlasIndex;
	}

	glm::uvec2 Texture::getAtlasSize() const
	{
		return m_atlasSize;
	}
}
