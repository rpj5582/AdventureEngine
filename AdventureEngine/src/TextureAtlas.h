#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <FreeImage.h>

namespace AdventureEngine
{
	class TextureAtlas
	{
	public:
		friend class AssetManager;

		GLuint getID() const;

		FIBITMAP* getImageData() const;

		glm::vec4 getPixel(unsigned int x, unsigned int y) const;

		unsigned int getWidth() const;
		unsigned int getHeight() const;

		glm::uvec2 getAtlasSize() const;

	private:
		TextureAtlas(GLuint id, FIBITMAP* imageData, glm::uvec2 atlasSize);
		~TextureAtlas();

		GLuint m_id;
		FIBITMAP* m_imageData;

		glm::uvec2 m_atlasSize;
	};
}