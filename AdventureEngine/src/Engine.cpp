#include "Engine.h"

#include <iostream>
#include <string>
#include <fstream>

namespace AdventureEngine
{
	Engine* Engine::m_instance = nullptr;

	Engine::Engine() : Engine(1280, 720)
	{
		m_instance = this;
	}

	Engine::Engine(int windowWidth, int windowHeight)
	{
		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;

		m_scene = nullptr;
	}

	Engine::~Engine()
	{
		delete m_scene;
	}

	void Engine::start()
	{
		std::cout << "Starting the engine..." << std::endl;

		// Sets the error function
		glfwSetErrorCallback(Engine::glfwErrorCallback);

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

		ALCcontext* ctx = nullptr;
		ALCdevice* dev = alcOpenDevice(nullptr);
		if (dev)
		{
			ctx = alcCreateContext(dev, nullptr);
			alcMakeContextCurrent(ctx);
		}
		else
		{
			std::cout << "Failed to initialize OpenAL." << std::endl;
			return;
		}

		glfwSetWindowSizeCallback(m_window, Engine::windowResizeCallback);
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

		//glEnable(GL_ALPHA_TEST);
		//glAlphaFunc(GL_GREATER, 0.5f);

		std::cout << "Engine started." << std::endl;

		m_scene = new TestScene(&m_windowWidth, &m_windowHeight);

		// Loads the first scene
		if (!m_scene->load())
		{ 
			return;
		}

		// Starts the game loop
		gameLoop();

		// Destroys the OpenAL library
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(ctx);
		alcCloseDevice(dev);

		// Destroys the window and terminates the GLFW library
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void Engine::quit()
	{
		// Closes the game next update call
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);
	}

	void Engine::glfwErrorCallback(int error, const char* description)
	{
		std::cout << "GLFW FATAL ERROR " << error << ": " << description << std::endl;
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

			// Updates the scene
			m_scene->update(m_deltaTime);

			// Draws the scene
			m_scene->render();
			glfwSwapBuffers(m_window);

			m_frameTimer += m_deltaTime;
			m_frameCount++;
		}
	}

	void Engine::windowResizeCallback(GLFWwindow* window, int windowWidth, int windowHeight)
	{
		m_instance->m_windowWidth = windowWidth;
		m_instance->m_windowHeight = windowHeight;
		glViewport(0, 0, windowWidth, windowHeight);
	}
}
