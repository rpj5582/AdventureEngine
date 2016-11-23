#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Input.h"
#include "Level.h"

namespace AdventureEngine
{
	class Engine
	{
	public:
		Engine();
		Engine(int windowWidth, int windowHeight);
		~Engine();

		void start();
		void quit();

	private:
		void gameLoop();

		GLFWwindow* m_window;

		int m_windowWidth;
		int m_windowHeight;

		bool loadShaderProgram();
		GLuint loadShader(const char* filepath, GLenum shaderType);
		GLuint m_programID;

		GLuint m_quadVAO;

		Level* m_level;
	};
}
