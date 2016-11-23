#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include <unordered_map>

#include "ModelAsset.h"
#include "TextureAsset.h"

namespace AdventureEngine
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		ModelAsset* loadModel(std::string name, std::string filepath);
		TextureAsset* loadTexture(std::string name, std::string filepath);

		ModelAsset* getModel(std::string name);
		TextureAsset* getTexture(std::string name);

	private:
		void generatePrimitives();
		void bufferModel(std::string name, const GLfloat* modelData, unsigned int vertexCount);

		std::unordered_map<std::string, TextureAsset> m_textures;
		std::unordered_map<std::string, ModelAsset> m_models;
	};
}
