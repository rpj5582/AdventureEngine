#include "Shader.h"

#include <fstream>
#include <iostream>

namespace AdventureEngine
{
	Shader::Shader(GLuint programID)
	{
		m_programID = programID;
	}

	Shader::~Shader()
	{
	}

	GLuint Shader::getID() const
	{
		return m_programID;
	}

	void Shader::use() const
	{
		glUseProgram(m_programID);
	}
}
