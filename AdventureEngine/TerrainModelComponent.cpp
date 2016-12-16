#include "TerrainModelComponent.h"

#include <FreeImage.h>

namespace AdventureEngine
{
	TerrainModelComponent::TerrainModelComponent() : TerrainModelComponent(nullptr)
	{
	}

	TerrainModelComponent::TerrainModelComponent(Texture* heightMap)
	{
		m_heightMap = heightMap;
	}

	TerrainModelComponent::~TerrainModelComponent()
	{
	}

	void TerrainModelComponent::init()
	{
		m_model = generateModel();
	}

	Model* TerrainModelComponent::generateModel()
	{
		if (!m_heightMap) return nullptr;

		const unsigned int maxPixelSize = 256 * 256 * 256;

		const unsigned int heightMapWidth = m_heightMap->getWidth();
		const unsigned int heightMapHeight = m_heightMap->getHeight();

		const unsigned int vertexCount = heightMapWidth * heightMapHeight;

		std::vector<GLfloat> heights(vertexCount);
		for (unsigned int y = 0; y < heightMapHeight; y++)
		{
			for (unsigned int x = 0; x < heightMapWidth; x++)
			{
				glm::vec3 color = m_heightMap->getPixel(x, y);

				float height = (float)((int)color.r << 16 | (int)color.g << 8 | (int)color.b << 0);

				height /= maxPixelSize;

				heights[x + heightMapWidth * y] = height * object->scale.y - object->scale.y / 2.0f;
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
				glm::vec3 position = glm::vec3(x / (float)heightMapWidth, 0, y / (float)heightMapHeight) * object->scale - object->scale / 2.0f;
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

				/*indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(topRight);
				indices.push_back(topRight);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);*/

				indices[index1D] = topLeft;
				indices[index1D + 1] = bottomLeft;
				indices[index1D + 2] = topRight;
				indices[index1D + 3] = topRight;
				indices[index1D + 4] = bottomLeft;
				indices[index1D + 5] = bottomRight;

				index1D += 6;
			}
		}

		return AssetManager::bufferModel(positions, uvs, normals, indices);
	}
}
