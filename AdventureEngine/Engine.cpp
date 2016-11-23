#include "Engine.h"

#include <iostream>
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

		// Enables backface culling
		glEnable(GL_CULL_FACE);

		// Enables transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (!loadShaderProgram())
		{
			std::cout << "Failed to load shader program." << std::endl;
		}

		std::cout << "Engine started." << std::endl;

		m_level = new Level(&aspectRatio);

		const char* level1 = "Levels/level1.json";
		std::cout << "Loading " << level1 << "..." << std::endl;

		// Loads the first level
		if (!m_level->load(level1)) { return; }

		std::cout << level1 << " loaded." << std::endl;

		// Starts the game loop
		gameLoop();

		// Deletes the shader program
		glDeleteProgram(m_programID);

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
			// Polls the input
			glfwPollEvents();

			// Updates the level
			m_level->update();

			// Draws the level
			glClearColor(0, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_level->draw();
			glfwSwapBuffers(m_window);
		}
	}

	bool Engine::loadShaderProgram()
	{
		// Loads the vertex and fragment shaders.
		int vertexShaderIndex = loadShader("shaders/vertexShader.glsl", GL_VERTEX_SHADER);
		if (vertexShaderIndex == 0) { return false; }

		int fragmentShaderIndex = loadShader("shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER);
		if (fragmentShaderIndex == 0) { return false; }

		// Creates a program.
		m_programID = glCreateProgram();

		// Attaches the vertex shader to the program.
		glAttachShader(m_programID, vertexShaderIndex);

		// Attaches the fragment shader to the program.
		glAttachShader(m_programID, fragmentShaderIndex);

		// Links the program.
		glLinkProgram(m_programID);

		// Checks the link status.
		GLint linkOutput;
		glGetProgramiv(m_programID, GL_LINK_STATUS, &linkOutput);

		// Linking failed.
		if (!linkOutput)
		{
			// Gets the length of the error log
			GLint logLength;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &logLength);
			GLchar* errorLog = new GLchar[logLength];
			glGetProgramInfoLog(m_programID, logLength, NULL, errorLog);

			// Prints the log to the console.
			std::cout << "Error linking program " << m_programID << ": " << errorLog << std::endl;

			// Deletes the error log and the program.
			delete[] errorLog;
			errorLog = nullptr;

			glDetachShader(m_programID, vertexShaderIndex);
			glDetachShader(m_programID, fragmentShaderIndex);
			glDeleteShader(vertexShaderIndex);
			glDeleteShader(fragmentShaderIndex);
			glDeleteProgram(m_programID);
			return false;
		}

		// Deteaches and deletes the shaders
		glDetachShader(m_programID, vertexShaderIndex);
		glDetachShader(m_programID, fragmentShaderIndex);
		glDeleteShader(vertexShaderIndex);
		glDeleteShader(fragmentShaderIndex);

		// Use the program for rendering
		glUseProgram(m_programID);
		return true;
	}

	GLuint Engine::loadShader(const char * filepath, GLenum shaderType)
	{
		std::ifstream ifs(filepath, std::ios::binary);

		if (ifs.is_open())
		{
			// Gets the size of the file.
			ifs.seekg(0, std::ios::end);
			unsigned int length = (unsigned int)ifs.tellg();
			ifs.seekg(0, std::ios::beg);

			// Saves the shader code into the char array and adds a null terminator at the end.
			GLchar* shaderCode = new GLchar[length + 1];
			ifs.read(shaderCode, length);
			shaderCode[length] = 0;

			// Closes the file.
			ifs.close();

			// Creates the shader.
			GLuint shaderIndex = glCreateShader(shaderType);

			// Sets the source code for the shader.
			glShaderSource(shaderIndex, 1, &shaderCode, 0);

			// Compiles the shader program.
			glCompileShader(shaderIndex);

			// Deletes the source code from memory.
			delete[] shaderCode;
			shaderCode = nullptr;

			// Checks the status of the compilation.
			GLint compileOutput;
			glGetShaderiv(shaderIndex, GL_COMPILE_STATUS, &compileOutput);

			// Compilation failed.
			if (!compileOutput)
			{
				// Gets the length of the error log
				GLint logLength;
				glGetShaderiv(shaderIndex, GL_INFO_LOG_LENGTH, &logLength);
				GLchar* errorLog = new GLchar[logLength];
				glGetShaderInfoLog(shaderIndex, logLength, NULL, errorLog);

				// Prints the log to the console.
				std::cout << "Error compiling shader " << shaderIndex << ": " << errorLog << std::endl;

				// Deletes the error log and the shader.
				delete[] errorLog;
				errorLog = nullptr;

				glDeleteShader(shaderIndex);
				return 0;
			}

			return shaderIndex;
		}
		else
		{
			std::cout << "Problem opening the file at " << filepath << std::endl;
			return 0;
		}
	}
}
