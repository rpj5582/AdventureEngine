#include "Engine.h"

#include <iostream>
#include <string>
#include <fstream>

namespace AdventureEngine
{
	static float aspectRatio = 0;

	static void glfwErrorCallback(int error, const char* description)
	{
		std::cout << "GLFW FATAL ERROR " << error << ": " << description << std::endl;
	}

	static void glfwWindowResizeCallback(GLFWwindow* window, int windowWidth, int windowHeight)
	{
		aspectRatio = windowWidth / (float)windowHeight;
		glViewport(0, 0, windowWidth, windowHeight);
		std::cout << "Window resized to " << windowWidth << "x" << windowHeight << std::endl;
	}

	Engine::Engine() : Engine(1280, 720)
	{
	}

	Engine::Engine(int windowWidth, int windowHeight)
	{
		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;
		aspectRatio = windowWidth / (float)windowHeight;

		m_level = nullptr;
	}

	Engine::~Engine()
	{
		delete m_level;
	}

	void Engine::start()
	{
		std::cout << "Starting the engine..." << std::endl;

		// Sets the error function
		glfwSetErrorCallback(glfwErrorCallback);

		// Initializes the GLFW library
		if (glfwInit() == GLFW_FALSE) return;

		// Sets the min version of OpenGL to v3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		// Creates the window and makes it the current context
		m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Adventure Engine", NULL, NULL);
		if (m_window)
		{
			glfwMakeContextCurrent(m_window);
		}
		else
		{
			glfwTerminate();
			return;
		}

		// Initialies the GLEW library
		if (glewInit() != GLEW_OK)
		{
			glfwTerminate();
			return;
		}

		glfwSetWindowSizeCallback(m_window, glfwWindowResizeCallback);
		glfwSetKeyCallback(m_window, Input::keyCallback);
		glfwSetMouseButtonCallback(m_window, Input::mouseClickCallback);
		glfwSetCursorPosCallback(m_window, Input::mouseMoveCallback);

		// Enables backface culling
		glEnable(GL_CULL_FACE);

		// Enables depth testing
		glEnable(GL_DEPTH_TEST);

		// Enables transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.75f);

		std::cout << "Engine started." << std::endl;

		m_level = new TestLevel(&aspectRatio);

		// Loads the first level
		if (!m_level->load())
		{ 
			return;
		}

		// Starts the game loop
		gameLoop();

		// Destroys the window and terminates the GLFW library
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void Engine::quit()
	{
		// Closes the game next update call
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);
	}

	void Engine::gameLoop()
	{
		while (!glfwWindowShouldClose(m_window))
		{
			m_currentTime = glfwGetTime();
			m_deltaTime = (float)(m_currentTime - m_prevTime);
			m_prevTime = m_currentTime;

			if (m_frameTimer >= 1)
			{
				std::string fpsString = "Adventure Engine - " + std::to_string(m_frameCount) + " FPS";
				const char* title = fpsString.c_str();
				glfwSetWindowTitle(m_window, title);

				m_frameTimer = 0;
				m_frameCount = 0;
			}

			// Clears the event queue
			Input::clear();

			// Polls the input
			glfwPollEvents();

			// Updates the level
			m_level->update(m_deltaTime);

			// Draws the level
			m_level->render();
			glfwSwapBuffers(m_window);

			m_frameTimer += m_deltaTime;
			m_frameCount++;
		}
	}
}
