#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <FreeImage.h>

#include <vector>

namespace AdventureEngine
{
	class Texture
	{
	public:
		friend class AssetManager;

		GLuint getID() const;

		FIBITMAP* getImageData() const;

		glm::vec4 getPixel(unsigned int x, unsigned int y) const;

		unsigned int getWidth() const;
		unsigned int getHeight() const;

	private:
		Texture(GLuint id, FIBITMAP* imageData);
		~Texture();

		GLuint m_id;
		FIBITMAP* m_imageData;
	};
}

