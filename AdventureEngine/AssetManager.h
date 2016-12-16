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

		static Model* loadModel(std::string filepath);

		static Texture* loadTexture(std::string filepath);
		static Texture* loadTexture(std::string filepath, glm::uvec2 atlasSize);

		static Model* getModel(std::string filepath);
		static Texture* getTexture(std::string filepath);

		static Material* createMaterial(Texture* texture);
		static Material* createMaterial(Texture* texture, float reflectivity);
		static Material* createMaterial(Texture* texture, float reflectivity, float specularDamping);
		static Material* createMaterial(Texture* texture, unsigned int atlasIndex);
		static Material* createMaterial(Texture* texture, unsigned int atlasIndex, float reflectivity);
		static Material* createMaterial(Texture* texture, unsigned int atlasIndex, float reflectivity, float specularDamping);

		static Model* bufferModel(const std::vector<GLfloat> positionData, const std::vector<GLfloat> uvData, const std::vector<GLfloat> normalData, const std::vector<GLuint> indices);

	private:
		static void generatePrimitives();
		static Model* bufferModel(std::string filepath, const std::vector<GLfloat> positionData, const std::vector<GLfloat> uvData, const std::vector<GLfloat> normalData, const std::vector<GLuint> indices);
		static GLuint createVBO(GLuint attributeIndex, GLsizei attributeSize, GLsizei dataSize, const GLfloat* data);
		static GLuint createIndicesVBO(const GLuint* indices, GLsizei size);

		static std::unordered_map<std::string, GLuint> m_textureIDs;
		static std::unordered_map<GLuint, Texture*> m_textures;

		static std::unordered_map<std::string, GLuint> m_modelPrimitives;
		static std::unordered_map<std::string, GLuint> m_modelIDs;
		static std::unordered_map<GLuint, Model*> m_models;

		static std::vector<Material*> m_materials;
	};
}
