#include "Input.h"

namespace AdventureEngine
{
	glm::vec2 Input::m_mousePos = glm::vec2();
	std::unordered_map<int, int> Input::m_buttonState = std::unordered_map<int, int>();

	Input::Input()
	{
	}

	Input::~Input()
	{
	}

	glm::vec2 Input::getMousePosition()
	{
		return m_mousePos;
	}

	bool Input::getButton(int button)
	{
		if (m_buttonState.count(button) == 1)
		{
			return m_buttonState.at(button) == GLFW_REPEAT || m_buttonState.at(button) == GLFW_PRESS;
		}

		return false;
	}

	bool Input::getButtonDown(int button)
	{
		if (m_buttonState.count(button) == 1)
		{
			return m_buttonState.at(button) == GLFW_PRESS;
		}

		return false;
	}

	bool Input::getButtonUp(int button)
	{
		if (m_buttonState.count(button) == 1)
		{
			return m_buttonState.at(button) == GLFW_RELEASE;
		}

		return false;
	}

	void Input::mouseMoveCallback(GLFWwindow * window, double x, double y)
	{
		m_mousePos = glm::vec2(x, y);
	}

	void Input::mouseClickCallback(GLFWwindow* window, int button, int action, int mods)
	{
		m_buttonState[button] = action;
	}

	void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		m_buttonState[key] = action;
	}
}
