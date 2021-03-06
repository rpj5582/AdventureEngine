#include "Texture.h"

namespace AdventureEngine
{
	Texture::Texture(GLuint id, FIBITMAP* imageData)
	{
		m_id = id;
		m_imageData = imageData;
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
}
