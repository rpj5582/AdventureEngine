#include "AssetManager.h"

#include <GL\glew.h>

#include <iostream>
#include <fstream>
#include <sstream>

namespace AdventureEngine
{
	std::unordered_map<GLuint, Texture*> AssetManager::m_textures;
	std::unordered_map<GLuint, TextureAtlas*> AssetManager::m_textureAtlases;
	std::unordered_map<GLuint, Cubemap*> AssetManager::m_cubeMaps;
	std::unordered_map<GLuint, Model*> AssetManager::m_models;
	std::vector<Material*> AssetManager::m_materials;
	std::unordered_map<GLuint, Shader*> AssetManager::m_shaders;

	void AssetManager::init()
	{
		FreeImage_SetOutputMessage(AssetManager::freeImageOutputMessage);
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
			// Delete the texture object
			delete it->second;
		}
		m_textures.clear();

		for (auto it = m_textureAtlases.begin(); it != m_textureAtlases.end(); it++)
		{
			delete it->second;
		}
		m_textureAtlases.clear();

		for (auto it = m_cubeMaps.begin(); it != m_cubeMaps.end(); it++)
		{
			delete it->second;
		}
		m_cubeMaps.clear();

		for (auto it = m_models.begin(); it != m_models.end(); it++)
		{
			delete it->second;
		}
		m_models.clear();

		for (auto it = m_shaders.begin(); it != m_shaders.end(); it++)
		{
			delete it->second;
		}
		m_shaders.clear();
	}

	Model* AssetManager::getModelPrimitive(ModelPrimitive primitive)
	{
		return m_models.at(primitive + 1);
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
		std::string filepath = "res/models/" + file;

		std::ifstream ifs(filepath);
		if (ifs.is_open())
		{
			struct Vertex
			{
				glm::vec3 position;
				glm::vec2 uv;
				glm::vec3 normal;

				bool allDataSet = false;
			};

			std::vector<Vertex> vertices;
			std::vector<glm::vec2> unorderedUVs;
			std::vector<glm::vec3> unorderedNormals;
			std::vector<GLuint> indices;

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

					// Creates a new vertex and assigns its position
					Vertex vertex;
					vertex.position = position;
					vertices.push_back(vertex);
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
					
						// Gets the vertex referred to by the face data
						Vertex& vertex = vertices.at(posIndex);

						// If the vertex's uv and normal data is empty, fill it in
						if (!vertex.allDataSet)
						{
							vertex.uv = unorderedUVs.at(uvIndex);
							vertex.normal = unorderedNormals.at(normIndex);
							vertex.allDataSet = true;

							// Saves the index of the vertex
							indices.push_back(posIndex);
						}
						else // The vertex has already been defined, do the following
						{
							// If the new vertex being parsed has different non-positional data, handle vertex duplication
							if (vertex.uv != unorderedUVs.at(uvIndex) || vertex.normal != unorderedNormals.at(normIndex))
							{
								Vertex duplicateVertex;
								duplicateVertex.position = vertex.position;
								duplicateVertex.uv = unorderedUVs.at(uvIndex);
								duplicateVertex.normal = unorderedNormals.at(normIndex);
								duplicateVertex.allDataSet = true;

								// Adds the vertex to the list
								vertices.push_back(duplicateVertex);

								// Saves the index of the vertex not as the position index from file, but as the last element in the vertices list
								indices.push_back(vertices.size() - 1);
							}
							else
							{
								// The same vertex is simply reused, so just add the index
								indices.push_back(posIndex);
							}
						}
					}
				}
			}

			// Close the file
			ifs.close();

			// Convert the vertices into data lists
			std::vector<GLfloat> positions = std::vector<GLfloat>(vertices.size() * 3);
			std::vector<GLfloat> uvs = std::vector<GLfloat>(vertices.size() * 2);
			std::vector<GLfloat> normals = std::vector<GLfloat>(vertices.size() * 3);

			for (unsigned int i = 0; i < vertices.size(); i++)
			{
				positions[i * 3] = vertices[i].position.x;
				positions[i * 3 + 1] = vertices[i].position.y;
				positions[i * 3 + 2] = vertices[i].position.z;

				uvs[i * 2] = vertices[i].uv.x;
				uvs[i * 2 + 1] = vertices[i].uv.y;

				normals[i * 3] = vertices[i].normal.x;
				normals[i * 3 + 1] = vertices[i].normal.y;
				normals[i * 3 + 2] = vertices[i].normal.z;
			}

			// Creates the model from the data arrays
			Model* model = bufferModel(positions, uvs, normals, indices);
			m_models[model->getID()] = model;

			return model;
		}

		std::cout << "Couldn't open file " << filepath << std::endl;
		return nullptr;
	}

	Texture* AssetManager::createEmptyTexture(GLuint width, GLuint height)
	{
		// Creates the texture
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Uploads the image data to the graphics card's VRAM
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Unbinds the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		// Creates the texture object
		Texture* texture = new Texture(textureID, nullptr);

		// Save the texture id in the map
		m_textures[textureID] = texture;

		return texture;
	}

	Texture* AssetManager::createEmptyDepthTexture(GLuint width, GLuint height)
	{
		// Generates a new texture
		GLuint depthTextureID;
		glGenTextures(1, &depthTextureID);

		// Binds the texture for use
		glBindTexture(GL_TEXTURE_2D, depthTextureID);

		// Creates an empty depth texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);

		// Sets the texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Unbinds the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		// Creates the texture object
		Texture* texture = new Texture(depthTextureID, nullptr);

		// Save the texture id in the map
		m_textures[depthTextureID] = texture;

		return texture;
	}

	Texture* AssetManager::loadTexture(std::string file)
	{
		return loadTexture(file, false);
	}

	Texture* AssetManager::loadTexture(std::string file, bool generateMipMaps)
	{
		std::string filepath = "res/textures/" + file;

		// Loads the image from the filepath
		FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType(filepath.c_str());
		FIBITMAP* rawImage = FreeImage_Load(imageFormat, filepath.c_str());

		// Returns false if the image could not be loaded
		if (!rawImage) { return nullptr; }

		// Converts to a 32 bit image to be consistent
		FIBITMAP* image = FreeImage_ConvertTo32Bits(rawImage);

		// Creates the texture
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Uploads the image data to the graphics card's VRAM
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));

		// Generates mipmaps for the texture
		if (generateMipMaps)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		// Unbinds the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		// Creates the texture object
		Texture* texture = new Texture(textureID, image);

		// Save the texture in the map
		m_textures[textureID] = texture;

		return texture;
	}

	TextureAtlas* AssetManager::loadTextureAtlas(std::string file, glm::uvec2 atlasSize)
	{
		return loadTextureAtlas(file, atlasSize, false);
	}

	TextureAtlas* AssetManager::loadTextureAtlas(std::string file, glm::uvec2 atlasSize, bool generateMipMaps)
	{
		std::string filepath = "res/textures/" + file;

		// Loads the image from the filepath
		FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType(filepath.c_str());
		FIBITMAP* rawImage = FreeImage_Load(imageFormat, filepath.c_str());

		// Returns false if the image could not be loaded
		if (!rawImage) { return nullptr; }

		// Converts to a 32 bit image to be consistent
		FIBITMAP* image = FreeImage_ConvertTo32Bits(rawImage);

		// Creates the texture
		GLuint textureAtlasID;
		glGenTextures(1, &textureAtlasID);
		glBindTexture(GL_TEXTURE_2D, textureAtlasID);

		// Uploads the image data to the graphics card's VRAM
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));

		// Generates mipmaps for the texture
		if (generateMipMaps)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		// Unbinds the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		// Creates the texture object
		TextureAtlas* textureAtlas = new TextureAtlas(textureAtlasID, image, atlasSize);

		// Save the texture in the map
		m_textureAtlases[textureAtlasID] = textureAtlas;

		return textureAtlas;
	}

	Cubemap* AssetManager::loadCubeMap(std::vector<std::string> files)
	{
		// Creates the texture
		GLuint cubemapID;
		glGenTextures(1, &cubemapID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

		std::vector<FIBITMAP*> images(6);

		for (unsigned int i = 0; i < 6; i++)
		{
			std::string filepath = "res/textures/" + files[i];

			// Loads the image from the filepath
			FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType(filepath.c_str());
			FIBITMAP* rawImage = FreeImage_Load(imageFormat, filepath.c_str());

			// Returns false if the image could not be loaded
			if (!rawImage) { continue; }

			// Converts to a 32 bit image to be consistent
			FIBITMAP* image = FreeImage_ConvertTo32Bits(rawImage);

			images[i] = image;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Unbinds the texture
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		Cubemap* cubemap = new Cubemap(cubemapID, images);
		m_cubeMaps[cubemapID] = cubemap;

		return cubemap;
	}

	Material* AssetManager::createMaterial(const Texture* texture)
	{
		return createMaterial(texture, 0.0f);
	}

	Material* AssetManager::createMaterial(const Texture* texture, float reflectivity)
	{
		return createMaterial(texture, reflectivity, 1.0f);
	}

	Material* AssetManager::createMaterial(const Texture* texture, float reflectivity, float specularDamping)
	{
		Material* material = new Material(texture, reflectivity, specularDamping);
		m_materials.push_back(material);
		return material;
	}

	Material* AssetManager::createMaterial(const TextureAtlas* atlas, unsigned int atlasIndex)
	{
		return createMaterial(atlas, atlasIndex, 0.0f);
	}

	Material* AssetManager::createMaterial(const TextureAtlas* atlas, unsigned int atlasIndex, float reflectivity)
	{
		return createMaterial(atlas, atlasIndex, reflectivity, 1.0f);
	}

	Material* AssetManager::createMaterial(const TextureAtlas* atlas, unsigned int atlasIndex, float reflectivity, float specularDamping)
	{
		Material* material = new Material(atlas, atlasIndex, reflectivity, specularDamping);
		m_materials.push_back(material);
		return material;
	}

	Material* AssetManager::createMaterial(const Cubemap* cubemap)
	{
		return createMaterial(cubemap, 0.0f);
	}

	Material* AssetManager::createMaterial(const Cubemap* cubemap, float reflectivity)
	{
		return createMaterial(cubemap, reflectivity, 1.0f);
	}

	Material* AssetManager::createMaterial(const Cubemap* cubemap, float reflectivity, float specularDamping)
	{
		Material* material = new Material(cubemap, reflectivity, specularDamping);
		m_materials.push_back(material);
		return material;
	}

	Shader* AssetManager::loadShaderProgram(std::string vertexFile, std::string fragmentFile)
	{
		// Loads the vertex and fragment shaders.
		GLuint vertexShaderID = loadShaderFile("res/shaders/" + vertexFile, GL_VERTEX_SHADER);
		if (vertexShaderID == 0) return nullptr;

		GLuint fragmentShaderID = loadShaderFile("res/shaders/" + fragmentFile, GL_FRAGMENT_SHADER);
		if (fragmentShaderID == 0) return nullptr;

		// Creates a program.
		GLuint programID = glCreateProgram();

		// Attaches the vertex shader to the program.
		glAttachShader(programID, vertexShaderID);

		// Attaches the fragment shader to the program.
		glAttachShader(programID, fragmentShaderID);

		// Links the program.
		glLinkProgram(programID);

		// Checks the link status.
		GLint linkOutput;
		glGetProgramiv(programID, GL_LINK_STATUS, &linkOutput);

		// Linking failed.
		if (!linkOutput)
		{
			// Gets the length of the error log
			GLint logLength;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
			GLchar* errorLog = new GLchar[logLength];
			glGetProgramInfoLog(programID, logLength, NULL, errorLog);

			// Prints the log to the console.
			std::cout << "Error linking program " << programID << ": " << errorLog << std::endl;

			// Deletes the error log and the program.
			delete[] errorLog;
			errorLog = nullptr;

			glDetachShader(programID, vertexShaderID);
			glDetachShader(programID, fragmentShaderID);
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);
			glDeleteProgram(programID);
			return nullptr;
		}

		// Deteaches and deletes the shaders
		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		// Creates the Shader object
		Shader* shader = new Shader(programID);
		m_shaders[programID] = shader;
		
		return shader;
	}

	void AssetManager::generatePrimitives()
	{
		const std::vector<GLfloat> quadPositions = {
			-0.5f, 0.0f, 0.5f,
			-0.5f, 0.0f, -0.5f,
			0.5f, 0.0f, 0.5f,
			0.5f, 0.0f, -0.5f
		};

		const std::vector<GLfloat> quadUVs = {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 1.0f
		};

		const std::vector<GLfloat> quadNormals = {
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		};

		const std::vector<GLuint> quadIndices = {
			0, 2, 1,
			1, 2, 3
		};

		Model* quad = bufferModel(quadPositions, quadUVs, quadNormals, quadIndices);
		m_models[quad->getID()] = quad;

		const std::vector<GLfloat> guiQuadData =
		{
			-1.0f, 1.0f, 0.0f, // Top left
			-1.0f, -1.0f, 0.0f, // Bottom left
			1.0f, 1.0f, 0.0f, // Top right

			1.0f, 1.0f, 0.0f, // Top right
			-1.0f, -1.0f, 0.0f, // Bottom left
			1.0f, -1.0f, 0.0f // Bottom right
		};

		Model* guiQuad = bufferModel(guiQuadData, 6);
		m_models[guiQuad->getID()] = guiQuad;

		const std::vector<GLfloat> cubePositions = {
			-0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			-0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, 0.5f,
			-0.5f, 0.5f, -0.5f,
			-0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f
		};

		const std::vector<GLfloat> cubeUVs = {
			0.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			0.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			0.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			0.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 1.0f
		};

		const std::vector<GLfloat> cubeNormals = {
			0.0f, 0.0f, 1.0f, 
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f
		};

		const std::vector<GLuint> cubeIndices = {
			0, 1, 2,
			2, 1, 3,
			8, 9, 4,
			4, 10, 5,
			11, 12, 6,
			6, 13, 7,
			14, 15, 16,
			17, 18, 19,
			20, 21, 22,
			23, 24, 25,
			26, 27, 28,
			29, 30, 31
		};

		Model* cube = bufferModel(cubePositions, cubeUVs, cubeNormals, cubeIndices);
		m_models[cube->getID()] = cube;

		const GLfloat SIZE = 0.5f;
		const std::vector<GLfloat> skyboxData = {
			// Right face
			SIZE, -SIZE, SIZE, // Right, bottom, front
			SIZE, -SIZE, -SIZE, // Right, bottom, back
			SIZE, SIZE, SIZE, // Right, top, front
			SIZE, SIZE, SIZE, // Right, top, front
			SIZE, -SIZE, -SIZE, // Right, bottom, back
			SIZE, SIZE, -SIZE, // Right, top, back

			// Left face
			-SIZE, -SIZE, -SIZE, // Left, bottom, back
			-SIZE, -SIZE, SIZE, // Left, bottom, front
			-SIZE, SIZE, -SIZE, // Left, top, back
			-SIZE, SIZE, -SIZE, // Left, top, back
			-SIZE, -SIZE, SIZE, // Left, bottom, front
			-SIZE, SIZE, SIZE, // Left, top, front

			// Top face
			-SIZE, SIZE, SIZE, // Left, top, front
			SIZE, SIZE, SIZE, // Right, top, front
			-SIZE, SIZE, -SIZE, // Left, top, back
			-SIZE, SIZE, -SIZE, // Left, top, back
			SIZE, SIZE, SIZE, // Right, top, front
			SIZE, SIZE, -SIZE, // Right, top, back

			// Bottom face
			-SIZE, -SIZE, -SIZE, // Left, bottom, back
			SIZE, -SIZE, -SIZE, // Right, bottom, back
			SIZE, -SIZE, SIZE, // Right, bottom, front
			SIZE, -SIZE, SIZE, // Right, bottom, front
			-SIZE, -SIZE, SIZE, // Left, bottom, front
			-SIZE, -SIZE, -SIZE, // Left, bottom, back

			// Back face
			SIZE, -SIZE, -SIZE, // Right, bottom, back
			-SIZE, -SIZE, -SIZE, // Left, bottom, back
			SIZE, SIZE, -SIZE, // Right, top, back
			SIZE, SIZE, -SIZE, // Right, top, back
			-SIZE, -SIZE, -SIZE, // Left, bottom, back
			-SIZE, SIZE, -SIZE, // Left, top, back

			// Front face
			-SIZE, -SIZE, SIZE, // Left, bottom, front
			SIZE, -SIZE, SIZE, // Right, bottom, front
			-SIZE, SIZE, SIZE, // Left, top, front
			-SIZE, SIZE, SIZE, // Left, top, front
			SIZE, -SIZE, SIZE, // Right, bottom, front
			SIZE, SIZE, SIZE, // Right, top, front
		};

		Model* skybox = bufferModel(skyboxData, 36);
		m_models[skybox->getID()] = skybox;
	}

	Model* AssetManager::bufferModel(const std::vector<GLfloat> postitionData, unsigned int vertexCount)
	{
		GLuint vao; // The opengl vertex array object index

		glGenVertexArrays(1, &vao); // Generates the vertex array object
		glBindVertexArray(vao); // Binds the vertex array object

		GLuint vbo = createVBO(0, 3, sizeof(GLfloat) * postitionData.size(), postitionData.data());

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

	GLuint AssetManager::loadShaderFile(std::string filepath, GLenum shaderType)
	{
		std::ifstream ifs(filepath, std::ios::binary);

		if (ifs.is_open())
		{
			// Gets the size of the file.
			ifs.seekg(0, std::ios::end);
			unsigned int length = (unsigned int)ifs.tellg();
			ifs.seekg(0, std::ios::beg);

			// Saves the shader code into the char array and adds a null terminator at the end.
			GLchar* shaderCode = new GLchar[length + 1];
			ifs.read(shaderCode, length);
			shaderCode[length] = 0;

			// Closes the file.
			ifs.close();

			// Creates the shader.
			GLuint shaderIndex = glCreateShader(shaderType);

			// Sets the source code for the shader.
			glShaderSource(shaderIndex, 1, &shaderCode, 0);

			// Compiles the shader program.
			glCompileShader(shaderIndex);

			// Deletes the source code from memory.
			delete[] shaderCode;
			shaderCode = nullptr;

			// Checks the status of the compilation.
			GLint compileOutput;
			glGetShaderiv(shaderIndex, GL_COMPILE_STATUS, &compileOutput);

			// Compilation failed.
			if (!compileOutput)
			{
				// Gets the length of the error log
				GLint logLength;
				glGetShaderiv(shaderIndex, GL_INFO_LOG_LENGTH, &logLength);
				GLchar* errorLog = new GLchar[logLength];
				glGetShaderInfoLog(shaderIndex, logLength, NULL, errorLog);

				// Prints the log to the console.
				std::cout << "Error compiling shader " << shaderIndex << ": " << errorLog << std::endl;

				// Deletes the error log and the shader.
				delete[] errorLog;
				errorLog = nullptr;

				glDeleteShader(shaderIndex);
				return 0;
			}

			return shaderIndex;
		}
		else
		{
			std::cout << "Could not open file " << filepath.c_str() << std::endl;
			return 0;
		}
	}

	void AssetManager::freeImageOutputMessage(FREE_IMAGE_FORMAT fif, const char* message)
	{
		if (fif != FIF_UNKNOWN)
		{
			std::cout << "Format: " << FreeImage_GetFormatFromFIF(fif) << std::endl;
		}
		
		std::cout << message << std::endl;
	}
}
