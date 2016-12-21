#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include <unordered_map>

#include "Model.h"
#include "Material.h"

namespace AdventureEngine
{
	class AssetManager
	{
	public:
		static void init();
		static void clean();

		static Model* genTerrainModel(const Texture* heightMap);
		static Model* loadModel(std::string file);

		static Texture* loadTexture(std::string file);
		static Texture* loadTexture(std::string file, unsigned int atlasIndex, glm::uvec2 atlasSize);

		static Material* createMaterial(Texture* texture);
		static Material* createMaterial(Texture* texture, float reflectivity);
		static Material* createMaterial(Texture* texture, float reflectivity, float specularDamping);

	private:
		static void generatePrimitives();

		static Model* bufferModel(const std::vector<GLfloat> data, unsigned int vertexCount);
		static Model* bufferModel(const std::vector<GLfloat> positionData, const std::vector<GLfloat> uvData, const std::vector<GLfloat> normalData, const std::vector<GLuint> indices);
		static GLuint createVBO(GLuint attributeIndex, GLsizei attributeSize, GLsizei dataSize, const GLfloat* data);
		static GLuint createIndicesVBO(const GLuint* indices, GLsizei size);

		static std::unordered_map<std::string, GLuint> m_textureIDs;
		static std::unordered_map<GLuint, std::vector<Texture*>> m_textures;

		static std::unordered_map<std::string, GLuint> m_modelPrimitiveIDs;
		static std::unordered_map<std::string, GLuint> m_modelIDs;
		static std::unordered_map<GLuint, Model*> m_models;

		static std::vector<Material*> m_materials;
	};
}
