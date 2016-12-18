#include "AssetManager.h"

#include <GL\glew.h>

#include <fstream>
#include <sstream>

namespace AdventureEngine
{
	std::unordered_map<std::string, GLuint> AssetManager::m_textureIDs;
	std::unordered_map<GLuint, Texture*> AssetManager::m_textures;

	std::unordered_map<std::string, GLuint> AssetManager::m_modelPrimitives;
	std::unordered_map<std::string, GLuint> AssetManager::m_modelIDs;
	std::unordered_map<GLuint, Model*> AssetManager::m_models;

	std::vector<Material*> AssetManager::m_materials;

	void AssetManager::init()
	{
		generatePrimitives();
	}

	void AssetManager::clean()
	{
		for (unsigned int i = 0; i < m_materials.size(); i++)
		{
			delete m_materials[i];
		}
		m_materials.clear();

		for (auto it = m_textures.begin(); it != m_textures.end(); it++)
		{
			delete it->second;
		}
		m_textures.clear();
		m_textureIDs.clear();

		for (auto it = m_models.begin(); it != m_models.end(); it++)
		{
			delete it->second;
		}
		m_models.clear();
		m_modelIDs.clear();
	}

	Model* AssetManager::loadModel(std::string file)
	{
		std::string filepath = "res/models/" + file;

		if (m_modelIDs.find(file) == m_modelIDs.end())
		{
			std::ifstream ifs(filepath);
			if (ifs.is_open())
			{
				std::vector<GLfloat> positions;
				std::vector<GLfloat> uvs;
				std::vector<GLfloat> normals;
				std::vector<GLuint> indices;

				std::vector<glm::vec2> unorderedUVs;
				std::vector<glm::vec3> unorderedNormals;

				std::string line;
				while (std::getline(ifs, line))
				{
					std::istringstream iss(line);

					std::string id;
					iss >> id;

					if (id == "v")
					{
						glm::vec3 position;

						iss >> position.x;
						iss >> position.y;
						iss >> position.z;

						positions.push_back(position.x);
						positions.push_back(position.y);
						positions.push_back(position.z);
					}
					else if (id == "vt")
					{
						glm::vec2 uv;

						iss >> uv.x;
						iss >> uv.y;

						unorderedUVs.push_back(uv);
					}
					else if (id == "vn")
					{
						glm::vec3 normal;

						iss >> normal.x;
						iss >> normal.y;
						iss >> normal.z;

						unorderedNormals.push_back(normal);
					}
					else if (id == "f")
					{
						uvs.resize(positions.size());
						normals.resize(positions.size());
						break;
					}
				}

				while (std::getline(ifs, line))
				{
					std::istringstream iss(line);

					char f;
					iss >> f;

					// Processes 3 vertices per line
					for (int i = 0; i < 3; i++)
					{
						char backSlash;
						int posIndex;
						int uvIndex;
						int normIndex;

						// Reads in all the data from one vertex
						iss >> posIndex;
						iss >> backSlash;
						iss >> uvIndex;
						iss >> backSlash;
						iss >> normIndex;

						// Subtracts one from the indices so they index properly into an array
						posIndex--;
						uvIndex--;
						normIndex--;

						indices.push_back(posIndex);

						uvs[posIndex * 2] = unorderedUVs.at(uvIndex).x;
						uvs[posIndex * 2 + 1] = unorderedUVs.at(uvIndex).y;

						normals[posIndex * 3] = unorderedNormals.at(normIndex).x;
						normals[posIndex * 3 + 1] = unorderedNormals.at(normIndex).y;
						normals[posIndex * 3 + 2] = unorderedNormals.at(normIndex).z;
					}
				}

				ifs.close();

				return bufferModel(file, positions, uvs, normals, indices);
			}
		}

		return getModel(file);
	}

	Texture* AssetManager::loadTexture(std::string file)
	{
		return loadTexture(file, glm::uvec2(1, 1));
	}

	Texture* AssetManager::loadTexture(std::string file, glm::uvec2 atlasSize)
	{
		if (m_textureIDs.find(file) == m_textureIDs.end())
		{
			std::string filepath = "res/textures/" + file;

			// Loads the image from the filepath
			FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType(filepath.c_str());
			FIBITMAP* rawImage = FreeImage_Load(imageFormat, filepath.c_str());

			// Returns false if the image could not be loaded
			if (!rawImage) { return nullptr; }

			// Converts to a 32 bit image to be consistent
			FIBITMAP* image = FreeImage_ConvertTo32Bits(rawImage);

			// Creates the opengl texture
			GLuint textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);

			// Uploads the image data to the graphics card's VRAM
			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));

			// Generates mipmaps for the texture
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1);

			// Unbinds the texture
			glBindTexture(GL_TEXTURE_2D, 0);

			// Save the texture id in the set
			Texture* texture = new Texture(textureID, image, atlasSize);

			m_textureIDs[file] = textureID;
			m_textures[textureID] = texture;

			return texture;
		}

		return getTexture(file);
	}

	Model* AssetManager::getModel(std::string file)
	{
		if (m_modelPrimitives.find(file) != m_modelPrimitives.end())
		{
			return m_models.at(m_modelPrimitives.at(file));
		}

		if (m_modelIDs.find(file) != m_modelIDs.end())
		{
			return m_models.at(m_modelIDs.at(file));
		}

		return nullptr;
	}

	Texture* AssetManager::getTexture(std::string file)
	{
		if (m_textureIDs.find(file) != m_textureIDs.end())
		{
			return m_textures.at(m_textureIDs.at(file));
		}

		return nullptr;
	}

	Material* AssetManager::createMaterial(Texture* texture)
	{
		return createMaterial(texture, 0.0f);
	}

	Material* AssetManager::createMaterial(Texture* texture, float reflectivity)
	{
		return createMaterial(texture, reflectivity, 1.0f);
	}

	Material* AssetManager::createMaterial(Texture* texture, float reflectivity, float specularDamping)
	{
		return createMaterial(texture, 0, reflectivity, specularDamping);
	}

	Material* AssetManager::createMaterial(Texture* texture, unsigned int atlasIndex)
	{
		return createMaterial(texture, atlasIndex, 0.0f);
	}

	Material* AssetManager::createMaterial(Texture* texture, unsigned int atlasIndex, float reflectivity)
	{
		return createMaterial(texture, atlasIndex, reflectivity, 1.0f);
	}

	Material* AssetManager::createMaterial(Texture* texture, unsigned int atlasIndex, float reflectivity, float specularDamping)
	{
		Material* material = new Material(texture, atlasIndex, reflectivity, specularDamping);
		m_materials.push_back(material);
		return material;
	}

	void AssetManager::generatePrimitives()
	{
		// The vertices, uvs, and normals that make up a quad
		const GLfloat quadData[] =
		{
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Top left
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom left
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Top right

			0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom right
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Top right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f // Bottom left
		};

		const std::vector<GLfloat> quadPositions = {
			-0.5f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f
		};

		const std::vector<GLfloat> quadUVs = {
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
		};

		const std::vector<GLfloat> quadNormals = {
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f
		};

		const std::vector<GLuint> quadIndices = {
			0, 1, 2,
			2, 1, 3
		};

		Model* quad = bufferModel("", quadPositions, quadUVs, quadNormals, quadIndices);
		m_modelPrimitives["quad"] = quad->getID();
	}

	Model* AssetManager::bufferModel(const std::vector<GLfloat> positionData, const std::vector<GLfloat> uvData, const std::vector<GLfloat> normalData, const std::vector<GLuint> indices)
	{
		return bufferModel("", positionData, uvData, normalData, indices);
	}

	Model* AssetManager::bufferModel(std::string file, const std::vector<GLfloat> positionData, const std::vector<GLfloat> uvData, const std::vector<GLfloat> normalData, const std::vector<GLuint> indices)
	{
		GLuint vao; // The opengl vertex array object index

		glGenVertexArrays(1, &vao); // Generates the vertex array object
		glBindVertexArray(vao); // Binds the vertex array

		GLuint positionVBO = createVBO(0, 3, sizeof(GLfloat) * positionData.size(), positionData.data());
		GLuint uvVBO = createVBO(1, 2, sizeof(GLfloat) * uvData.size(), uvData.data());
		GLuint normalVBO = createVBO(2, 3, sizeof(GLfloat) * normalData.size(), normalData.data());

		GLuint indicesVBo = createIndicesVBO(indices.data(), sizeof(GLuint) * indices.size());

		// Unbinds the vertex array object
		glBindVertexArray(0);

		GLuint vbos[] = { positionVBO, uvVBO, normalVBO, indicesVBo };

		Model* model = new Model(vao, 4, vbos, indices.size());

		if (file != "")
		{
			m_modelIDs[file] = vao;
		}

		m_models[vao] = model;

		return model;
	}

	GLuint AssetManager::createVBO(GLuint attributeIndex, GLsizei attributeSize, GLsizei dataSize, const GLfloat* data)
	{
		GLuint vbo;
		glGenBuffers(1, &vbo); // Creates a vertex buffer object
		glBindBuffer(GL_ARRAY_BUFFER, vbo); // Binds the vertex buffer object

		// Fills the arrray buffer with the data
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);

		// Sets the attribute to the model's vertex location
		glEnableVertexAttribArray(attributeIndex);
		glVertexAttribPointer(attributeIndex, attributeSize, GL_FLOAT, GL_FALSE, 0, 0);

		// Unbinds the vertex buffer object
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return vbo;
	}
	GLuint AssetManager::createIndicesVBO(const GLuint* indices, GLsizei size)
	{
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);

		return vbo;
	}
}
