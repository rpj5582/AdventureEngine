#pragma once

#include <GL\glew.h>

namespace AdventureEngine
{
	class Shader
	{
	public:
		Shader(const char* vertexFile, const char* fragmentFile);
		~Shader();

		GLuint getID() const;

		bool load();
		void use() const;

	private:
		GLuint load(const char* filepath, GLenum shaderType);
		void unload();

		const char* m_vertexFile;
		const char* m_fragmentFile;
		GLuint m_programID;
	};
}
