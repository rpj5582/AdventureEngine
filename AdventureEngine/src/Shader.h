#pragma once

#include <GL\glew.h>
#include <string>

namespace AdventureEngine
{
	class Shader
	{
	public:
		Shader(std::string vertexFile, std::string fragmentFile);
		~Shader();

		GLuint getID() const;

		bool load();
		void use() const;

	private:
		GLuint load(std::string filepath, GLenum shaderType);
		void unload();

		std::string m_vertexFile;
		std::string m_fragmentFile;
		GLuint m_programID;
	};
}
