#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <al.h>
#include <alc.h>

#include "Input.h"
#include "TestScene.h"

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
		static Engine* m_instance;

		static void glfwErrorCallback(int error, const char* description);

		void gameLoop();

		static void windowResizeCallback(GLFWwindow* window, int windowWidth, int windowHeight);
		GLFWwindow* m_window;
		int m_windowWidth;
		int m_windowHeight;

		double m_currentTime;
		double m_prevTime;
		float m_deltaTime;

		float m_frameTimer;
		unsigned int m_frameCount;

		Scene* m_scene;
	};
}
