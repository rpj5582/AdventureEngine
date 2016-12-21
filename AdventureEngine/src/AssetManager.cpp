#include "AssetManager.h"

#include <GL\glew.h>

#include <iostream>
#include <fstream>
#include <sstream>

namespace AdventureEngine
{
	std::unordered_map<std::string, GLuint> AssetManager::m_textureIDs;
	std::unordered_map<GLuint, std::vector<Texture*>> AssetManager::m_textures;

	std::unordered_map<std::string, GLuint> AssetManager::m_modelPrimitiveIDs;
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
			for (unsigned int i = 0; i < it->second.size(); i++)
			{
				// Only delete the texture data if it is the last texture in the list
				if (i == it->second.size() - 1)
				{
					GLuint textureID = it->second[i]->getID();

					FreeImage_Unload(it->second[i]->getImageData());
					glDeleteTextures(1, &textureID);
				}

				// Delete the texture object
				delete it->second[i];
			}
			it->second.clear();
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

	Model* AssetManager::genTerrainModel(const Texture* heightMap)
	{
		if (!heightMap) return nullptr;

		const unsigned int maxPixelSize = 256 * 256 * 256;

		const unsigned int heightMapWidth = heightMap->getWidth();
		const unsigned int heightMapHeight = heightMap->getHeight();

		const unsigned int vertexCount = heightMapWidth * heightMapHeight;

		std::vector<GLfloat> heights(vertexCount);
		for (unsigned int y = 0; y < heightMapHeight; y++)
		{
			for (unsigned int x = 0; x < heightMapWidth; x++)
			{
				glm::vec3 color = heightMap->getPixel(x, y);

				float height = (float)((int)color.r << 16 | (int)color.g << 8 | (int)color.b << 0);

				height /= maxPixelSize;

				heights[x + heightMapWidth * y] = height - 0.5f;
			}
		}

		// Initializes vectors to hold the model data
		std::vector<GLfloat> positions = std::vector<GLfloat>(vertexCount * 3);
		std::vector<GLfloat> uvs = std::vector<GLfloat>(vertexCount * 2);
		std::vector<GLfloat> normals = std::vector<GLfloat>(vertexCount * 3);
		std::vector<GLuint> indices = std::vector<GLuint>((heightMapWidth - 1) * (heightMapHeight - 1) * 6);

		// Populates the position, uv, and normal data for each vertex
		for (unsigned int y = 0; y < heightMapHeight; y++)
		{
			for (unsigned int x = 0; x < heightMapWidth; x++)
			{
				glm::vec3 position = glm::vec3(x / (float)heightMapWidth - 0.5f, 0, y / (float)heightMapHeight - 0.5f);
				glm::vec2 uv = glm::vec2((float)x / heightMapWidth, -(float)y / heightMapHeight);

				// Calculates the normal based on the height
				int xInt = x;
				int yInt = y;
				unsigned int leftIndex = xInt - 1 < 0 ? 0 : xInt - 1 + heightMapWidth * yInt;
				unsigned int rightIndex = xInt + 1 >= (int)heightMapWidth ? heightMapWidth : xInt + 1 + heightMapWidth * yInt;
				unsigned int bottomIndex = yInt - 1 < 0 ? 0 : xInt + heightMapWidth * yInt - 1;
				unsigned int topIndex = yInt + 1 >= (int)heightMapHeight ? heightMapHeight : xInt + heightMapWidth * yInt + 1;

				GLfloat leftHeight = heights[leftIndex];
				GLfloat rightHeight = heights[rightIndex];
				GLfloat bottomHeight = heights[bottomIndex];
				GLfloat topHeight = heights[topIndex];

				glm::vec3 normal = glm::vec3(leftHeight - rightHeight, 2, bottomHeight - topHeight);
				normal = glm::normalize(normal);

				unsigned int index = x + heightMapWidth * y;

				position.y = heights[index];

				positions[index * 3] = position.x;
				positions[index * 3 + 1] = position.y;
				positions[index * 3 + 2] = position.z;
				uvs[index * 2] = uv.x;
				uvs[index * 2 + 1] = uv.y;

				normals[index * 3] = normal.x;
				normals[index * 3 + 1] = normal.y;
				normals[index * 3 + 2] = normal.z;
			}
		}

		// Populates the face data
		unsigned int index1D = 0;
		for (unsigned int y = 0; y < heightMapHeight - 1; y++)
		{
			for (unsigned int x = 0; x < heightMapWidth - 1; x++)
			{

				unsigned int topLeft = x + heightMapWidth * y;
				unsigned int topRight = topLeft + 1;
				unsigned int bottomLeft = x + heightMapWidth * (y + 1);
				unsigned int bottomRight = bottomLeft + 1;

				indices[index1D] = topLeft;
				indices[index1D + 1] = bottomLeft;
				indices[index1D + 2] = topRight;
				indices[index1D + 3] = topRight;
				indices[index1D + 4] = bottomLeft;
				indices[index1D + 5] = bottomRight;

				index1D += 6;
			}
		}

		Model* terrainModel = bufferModel(positions, uvs, normals, indices);
		m_models[terrainModel->getID()] = terrainModel;

		return terrainModel;
	}

	Model* AssetManager::loadModel(std::string file)
	{
		if (m_modelPrimitiveIDs.find(file) != m_modelPrimitiveIDs.end())
		{
			return m_models.at(m_modelPrimitiveIDs.at(file));
		}

		if (m_modelIDs.find(file) != m_modelIDs.end())
		{
			return m_models.at(m_modelIDs.at(file));
		}

		std::string filepath = "res/models/" + file;

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

			Model* model = bufferModel(positions, uvs, normals, indices);

			if (file != "")
			{
				m_modelIDs[file] = model->getID();
			}

			m_models[model->getID()] = model;

			return model;
		}

		std::cout << "Couldn't open file " << filepath << std::endl;
		return nullptr;
	}

	Texture* AssetManager::loadTexture(std::string file)
	{
		return loadTexture(file, 0, glm::uvec2(1, 1));
	}

	Texture* AssetManager::loadTexture(std::string file, unsigned int atlasIndex, glm::uvec2 atlasSize)
	{
		// Checks if the texture has already been loaded
		if (m_textureIDs.find(file) != m_textureIDs.end())
		{
			GLuint textureID = m_textureIDs.at(file);
			std::vector<Texture*> textures = m_textures.at(textureID);
			for (unsigned int i = 0; i < textures.size(); i++)
			{
				Texture* testTexture = textures[i];

				if (testTexture->getAtlasIndex() == atlasIndex && testTexture->getAtlasSize() == atlasSize)
				{
					return testTexture;
				}
			}

			// None of the textures were exactly the same, so make a new one
			Texture* texture = new Texture(textureID, textures[0]->getImageData(), atlasIndex, atlasSize);

			// Adds the texture copy to the texture map
			m_textures[textureID].push_back(texture);

			return texture;
			
		}

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
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Unbinds the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		// Save the texture id in the set
		Texture* texture = new Texture(textureID, image, atlasIndex, atlasSize);

		m_textureIDs[file] = textureID;
		m_textures[textureID].push_back(texture);

		return texture;
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
		Material* material = new Material(texture, reflectivity, specularDamping);
		m_materials.push_back(material);
		return material;
	}

	void AssetManager::generatePrimitives()
	{
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

		const std::vector<GLfloat> guiQuadData =
		{
			-1.0f, 1.0f, 0.0f, // Top left
			-1.0f, -1.0f, 0.0f, // Bottom left
			1.0f, 1.0f, 0.0f, // Top right

			1.0f, 1.0f, 0.0f, // Top right
			-1.0f, -1.0f, 0.0f, // Bottom left
			1.0f, -1.0f, 0.0f // Bottom right
		};

		Model* quad = bufferModel(quadPositions, quadUVs, quadNormals, quadIndices);
		m_modelPrimitiveIDs["quad"] = quad->getID();
		m_models[quad->getID()] = quad;

		Model* guiQuad = bufferModel(guiQuadData, 6);
		m_modelPrimitiveIDs["guiQuad"] = guiQuad->getID();
		m_models[guiQuad->getID()] = guiQuad;
	}

	Model* AssetManager::bufferModel(const std::vector<GLfloat> data, unsigned int vertexCount)
	{
		GLuint vao; // The opengl vertex array object index

		glGenVertexArrays(1, &vao); // Generates the vertex array object
		glBindVertexArray(vao); // Binds the vertex array object

		GLuint vbo = createVBO(0, 3, sizeof(GLfloat) * data.size(), data.data());

		// Unbinds the vertex array object
		glBindVertexArray(0);

		GLuint vbos[] = { vbo };

		return new Model(vao, 1, vbos, vertexCount);
	}

	Model* AssetManager::bufferModel(const std::vector<GLfloat> positionData, const std::vector<GLfloat> uvData, const std::vector<GLfloat> normalData, const std::vector<GLuint> indices)
	{
		GLuint vao; // The opengl vertex array object index

		glGenVertexArrays(1, &vao); // Generates the vertex array object
		glBindVertexArray(vao); // Binds the vertex array object

		GLuint positionVBO = createVBO(0, 3, sizeof(GLfloat) * positionData.size(), positionData.data());
		GLuint uvVBO = createVBO(1, 2, sizeof(GLfloat) * uvData.size(), uvData.data());
		GLuint normalVBO = createVBO(2, 3, sizeof(GLfloat) * normalData.size(), normalData.data());

		GLuint indicesVBO = createIndicesVBO(indices.data(), sizeof(GLuint) * indices.size());

		// Unbinds the vertex array object
		glBindVertexArray(0);

		GLuint vbos[] = { positionVBO, uvVBO, normalVBO, indicesVBO };

		return new Model(vao, 4, vbos, indices.size());
	}

	GLuint AssetManager::createVBO(GLuint attributeIndex, GLsizei attributeSize, GLsizei dataSize, const GLfloat* data)
	{
		GLuint vbo;
		glGenBuffers(1, &vbo); // Creates a vertex buffer object
		glBindBuffer(GL_ARRAY_BUFFER, vbo); // Binds the vertex buffer object

		// Fills the vertex buffer with the data
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);

		// Sets the attribute
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
