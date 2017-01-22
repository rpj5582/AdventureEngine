#pragma once

#include <GL\glew.h>
#include <string>

namespace AdventureEngine
{
	class Shader
	{
	public:
		friend class AssetManager;

		GLuint getID() const;

		void use() const;

	private:
		Shader(GLuint programID);
		~Shader();

		GLuint m_programID;
	};
}
