#include "Cubemap.h"

namespace AdventureEngine
{
	Cubemap::Cubemap(GLuint id, std::vector<FIBITMAP*> imageData)
	{
		m_id = id;
		m_imageData = imageData;
	}

	Cubemap::~Cubemap()
	{
		for (unsigned int i = 0; i < m_imageData.size(); i++)
		{
			FreeImage_Unload(m_imageData[i]);
		}

		glDeleteTextures(1, &m_id);
	}

	GLuint Cubemap::getID() const
	{
		return m_id;
	}

	FIBITMAP* Cubemap::getImageData(unsigned int index) const
	{
		return m_imageData[index];
	}
}