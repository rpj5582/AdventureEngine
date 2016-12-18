#include "Shader.h"

#include <fstream>
#include <iostream>

namespace AdventureEngine
{
	Shader::Shader(std::string vertexFile, std::string fragmentFile)
	{
		m_vertexFile = vertexFile;
		m_fragmentFile = fragmentFile;
	}

	Shader::~Shader()
	{
		unload();
	}

	GLuint Shader::getID() const
	{
		return m_programID;
	}

	bool Shader::load()
	{
		// Unloads the previous shader program, if any
		unload();

		// Loads the vertex and fragment shaders.
		GLuint vertexShaderID = load("src/shaders/" + m_vertexFile, GL_VERTEX_SHADER);
		if (vertexShaderID == 0) return false;

		GLuint fragmentShaderID = load("src/shaders/" + m_fragmentFile, GL_FRAGMENT_SHADER);
		if (fragmentShaderID == 0) return false;

		// Creates a program.
		m_programID = glCreateProgram();

		// Attaches the vertex shader to the program.
		glAttachShader(m_programID, vertexShaderID);

		// Attaches the fragment shader to the program.
		glAttachShader(m_programID, fragmentShaderID);

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

			glDetachShader(m_programID, vertexShaderID);
			glDetachShader(m_programID, fragmentShaderID);
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);
			glDeleteProgram(m_programID);
			return false;
		}

		// Deteaches and deletes the shaders
		glDetachShader(m_programID, vertexShaderID);
		glDetachShader(m_programID, fragmentShaderID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		return true;
	}

	void Shader::use() const
	{
		glUseProgram(m_programID);
	}

	GLuint Shader::load(std::string filepath, GLenum shaderType)
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
			std::cout << "Could not open file " << filepath.c_str() << std::endl;
			return 0;
		}
	}

	void Shader::unload()
	{
		glDeleteProgram(m_programID);
	}
}
