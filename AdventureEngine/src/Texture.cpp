#include "Texture.h"

namespace AdventureEngine
{
	Texture::Texture(GLuint id, std::vector<FIBITMAP*> imageData, unsigned int atlasIndex, glm::uvec2 atlasSize)
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
		return getImageData(0);
	}

	FIBITMAP* Texture::getImageData(unsigned int index) const
	{
		return m_imageData[index];
	}

	unsigned int Texture::getImageDataSize() const
	{
		return m_imageData.size();
	}

	glm::vec4 Texture::getPixel(unsigned int x, unsigned int y) const
	{
		return getPixel(0, x, y);
	}

	glm::vec4 Texture::getPixel(unsigned int index, unsigned int x, unsigned int y) const
	{
		RGBQUAD color;
		FreeImage_GetPixelColor(m_imageData[index], x, y, &color);

		return glm::vec4(color.rgbRed, color.rgbGreen, color.rgbBlue, color.rgbReserved);
	}

	unsigned int Texture::getWidth() const
	{
		return getWidth(0);
	}

	unsigned int Texture::getWidth(unsigned int index) const
	{
		return FreeImage_GetWidth(m_imageData[index]);
	}

	unsigned int Texture::getHeight() const
	{
		return getHeight(0);
	}

	unsigned int Texture::getHeight(unsigned int index) const
	{
		return FreeImage_GetHeight(m_imageData[index]);
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
