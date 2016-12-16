#include "Texture.h"

namespace AdventureEngine
{
	Texture::Texture(GLuint id, FIBITMAP* imageData, glm::uvec2 atlasSize)
	{
		m_id = id;
		m_imageData = imageData;
		m_atlasSize = atlasSize;
	}

	Texture::~Texture()
	{
		FreeImage_Unload(m_imageData);
		glDeleteTextures(1, &m_id);
	}

	GLuint Texture::getID() const
	{
		return m_id;
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

	glm::uvec2 Texture::getAtlasSize() const
	{
		return m_atlasSize;
	}
}
