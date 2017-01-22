#pragma once

#include <GL\glew.h>
#include <FreeImage.h>

#include <vector>

namespace AdventureEngine
{
	class Cubemap
	{
	public:
		friend class AssetManager;

		GLuint getID() const;
		FIBITMAP* getImageData(unsigned int index) const;

	private:
		Cubemap(GLuint id, std::vector<FIBITMAP*> imageData);
		~Cubemap();

		GLuint m_id;
		std::vector<FIBITMAP*> m_imageData;
	};
}