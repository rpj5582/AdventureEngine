#include "AssetManager.h"

#include <GL\glew.h>
#include <FreeImage.h>

#include <fstream>
#include <sstream>

namespace AdventureEngine
{
	AssetManager::AssetManager()
	{
		generatePrimitives();
	}

	AssetManager::~AssetManager()
	{
		for (auto it = m_textures.begin(); it != m_textures.end(); it++)
		{
			delete it->second;
		}

		for (auto it = m_models.begin(); it != m_models.end(); it++)
		{
			delete it->second;
		}
	}

	ModelAsset* AssetManager::loadModel(std::string name, std::string filepath)
	{
		if (m_models.find(name) == m_models.end())
		{
			// WIP
			std::ifstream ifs(filepath);
			if (ifs.is_open())
			{
				std::vector<glm::vec3> positions;
				std::vector<glm::vec2> uvs;
				std::vector<glm::vec3> normals;
				std::vector<GLfloat> modelData;
				int vertexCount = 0;

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
						positions.push_back(position);
					}
					else if (id == "vt")
					{
						glm::vec3 uv;
						iss >> uv.x;
						iss >> uv.y;
						uvs.push_back(uv);
					}
					else if (id == "vn")
					{
						glm::vec3 normal;
						iss >> normal.x;
						iss >> normal.y;
						iss >> normal.z;
						normals.push_back(normal);
					}
					else if (id == "f")
					{
						// Processes 3 vertices per line
						for (int i = 0; i < 2; i++)
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

							// Adds the position data to the buffer
							modelData.push_back(positions[posIndex].x);
							modelData.push_back(positions[posIndex].y);
							modelData.push_back(positions[posIndex].z);

							// Adds the uv data to the buffer
							modelData.push_back(uvs[uvIndex].x);
							modelData.push_back(uvs[uvIndex].y);

							// Adds the normal data to the buffer
							modelData.push_back(normals[normIndex].x);
							modelData.push_back(normals[normIndex].y);
							modelData.push_back(normals[normIndex].z);

							vertexCount++;
						}
					}
				}
				
				
				ifs.close();
				GLfloat* model = &modelData[0];


				return bufferModel(name, model, vertexCount);
			}

			return nullptr;
		}

		return m_models.at(name);
	}

	TextureAsset* AssetManager::loadTexture(std::string name, std::string filepath)
	{
		// Checks if the texture is already in the texture map
		if (m_textures.find(name) == m_textures.end())
		{
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

			// Sets the min filter to linear
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			// Unloads the image from RAM
			FreeImage_Unload(image);

			// Unbinds the texture
			glBindTexture(GL_TEXTURE_2D, 0);

			// Save the texture id in the map
			m_textures[name] = new TextureAsset(textureID);
		}

		return m_textures.at(name);
	}

	ModelAsset* AssetManager::getModel(std::string name)
	{
		if (m_models.find(name) != m_models.end())
		{
			return m_models.at(name);
		}

		return nullptr;
	}

	TextureAsset* AssetManager::getTexture(std::string name)
	{
		if (m_textures.find(name) != m_textures.end())
		{
			return m_textures.at(name);
		}

		return nullptr;
	}

	void AssetManager::generatePrimitives()
	{
		// The vertices, uvs, and normals that make up a quad
		const GLfloat m_quad[] =
		{
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

			1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
		};

		bufferModel("quad", m_quad, 6);
	}

	ModelAsset* AssetManager::bufferModel(std::string name, const GLfloat* modelData, unsigned int vertexCount)
	{
		GLuint vbo; // The opengl vertex buffer object index
		GLuint vao; // The opengl vertex array object index

		glGenVertexArrays(1, &vao); // Generates the vertex array object
		glGenBuffers(1, &vbo); // Generates the vertex buffer object

		glBindBuffer(GL_ARRAY_BUFFER, vbo); // Binds the array buffer to the vertex buffer object
		glBindVertexArray(vao); // Binds the vertex array to the quad

		// Fills the arrray buffer with the quad's data
		glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3)) * vertexCount, modelData, GL_STATIC_DRAW);

		// Sets the first attribute to the quad's vertex location
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3), 0);

		// Sets the second attribute to the quad's vertex uv
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));

		// Sets the third attribute to the quad's vertex normal
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

		m_models[name] = new ModelAsset(vao, vertexCount);
		return m_models[name];
	}
}
