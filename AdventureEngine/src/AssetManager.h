#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include <unordered_map>

#include "TextureAtlas.h"
#include "Cubemap.h"
#include "Model.h"
#include "Material.h"
#include "Shader.h"

namespace AdventureEngine
{
	enum ModelPrimitive
	{
		QUAD,
		GUIQUAD,
		CUBE,
		SKYBOX
	};

	class AssetManager
	{
	public:
		static void init();
		static void clean();

		// Models

		static Model* getModelPrimitive(ModelPrimitive primitive);
		static Model* genTerrainModel(const Texture* heightMap);
		static Model* loadModel(std::string file);

		// Textures

		static Texture* createEmptyTexture(GLuint width, GLuint height);
		static Texture* createEmptyDepthTexture(GLuint width, GLuint height);
		static Texture* loadTexture(std::string file);
		static Texture* loadTexture(std::string file, bool generateMipMaps);

		// Texture Atlas
		
		static TextureAtlas* loadTextureAtlas(std::string file, glm::uvec2 atlasSize);
		static TextureAtlas* loadTextureAtlas(std::string file, glm::uvec2 atlasSize, bool generateMipMaps);

		// Cubemaps

		static Cubemap* loadCubeMap(std::vector<std::string> files);

		// Materials

		static Material* createMaterial(const Texture* texture);
		static Material* createMaterial(const Texture* texture, float reflectivity);
		static Material* createMaterial(const Texture* texture, float reflectivity, float specularDamping);

		static Material* createMaterial(const TextureAtlas* atlas, unsigned int atlasIndex);
		static Material* createMaterial(const TextureAtlas* atlas, unsigned int atlasIndex, float reflectivity);
		static Material* createMaterial(const TextureAtlas* atlas, unsigned int atlasIndex, float reflectivity, float specularDamping);

		static Material* createMaterial(const Cubemap* cubemap);
		static Material* createMaterial(const Cubemap* cubemap, float reflectivity);
		static Material* createMaterial(const Cubemap* cubemap, float reflectivity, float specularDamping);

		// Shaders

		static Shader* loadShaderProgram(std::string vertexFile, std::string fragmentFile);

	private:
		// Models
		
		static void generatePrimitives();

		static Model* bufferModel(const std::vector<GLfloat> data, unsigned int vertexCount);
		static Model* bufferModel(const std::vector<GLfloat> positionData, const std::vector<GLfloat> uvData, const std::vector<GLfloat> normalData, const std::vector<GLuint> indices);
		static GLuint createVBO(GLuint attributeIndex, GLsizei attributeSize, GLsizei dataSize, const GLfloat* data);
		static GLuint createIndicesVBO(const GLuint* indices, GLsizei size);

		// Shaders

		static GLuint loadShaderFile(std::string filepath, GLenum shaderType);

		// FreeImage error message callback
		static void freeImageOutputMessage(FREE_IMAGE_FORMAT fif, const char* message);

		// Lists of assets

		static std::unordered_map<GLuint, Texture*> m_textures;
		static std::unordered_map<GLuint, TextureAtlas*> m_textureAtlases;
		static std::unordered_map<GLuint, Cubemap*> m_cubeMaps;
		static std::unordered_map<GLuint, Model*> m_models;
		static std::vector<Material*> m_materials;
		static std::unordered_map<GLuint, Shader*> m_shaders;
	};
}
