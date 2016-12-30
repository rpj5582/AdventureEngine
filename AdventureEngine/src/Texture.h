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

		~Texture();

		GLuint getID() const;

		FIBITMAP* getImageData() const;
		FIBITMAP* getImageData(unsigned int index) const;
		unsigned int getImageDataSize() const;

		glm::vec4 getPixel(unsigned int x, unsigned int y) const;
		glm::vec4 getPixel(unsigned int index, unsigned int x, unsigned int y) const;

		unsigned int getWidth() const;
		unsigned int getWidth(unsigned int index) const;
		unsigned int getHeight() const;
		unsigned int getHeight(unsigned int index) const;

		unsigned int getAtlasIndex() const;
		glm::uvec2 getAtlasSize() const;

	private:
		Texture(GLuint id, std::vector<FIBITMAP*> imageData, unsigned int atlasIndex, glm::uvec2 atlasSize);

		GLuint m_id;
		std::vector<FIBITMAP*> m_imageData;

		unsigned int m_atlasIndex;
		glm::uvec2 m_atlasSize;
	};
}

