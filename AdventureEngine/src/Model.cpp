#include "Model.h"

namespace AdventureEngine
{
	Model::Model(GLuint vao, GLuint vboCount, GLuint* vbos, GLuint vertexCount)
	{
		m_vao = vao;
		m_vboCount = vboCount;
		m_vbos = vbos;
		m_vertexCount = vertexCount;
	}

	Model::~Model()
	{
		glDeleteBuffers(m_vboCount, m_vbos);
		glDeleteVertexArrays(1, &m_vao);
	}

	GLuint Model::getID() const
	{
		return m_vao;
	}

	unsigned int Model::getVertexCount() const
	{
		return m_vertexCount;
	}
}
