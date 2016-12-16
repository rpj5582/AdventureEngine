#pragma once

#include <GL\glew.h>

namespace AdventureEngine
{
	class Model
	{
	public:
		friend class AssetManager;

		~Model();

		GLuint getID() const;
		unsigned int getVertexCount() const;

	private:
		Model(GLuint vao, GLuint vboCount, GLuint* vbos, GLuint vertexCount);

		GLuint m_vao;
		GLuint m_vboCount;
		GLuint* m_vbos;
		GLuint m_vertexCount;
	};
}

