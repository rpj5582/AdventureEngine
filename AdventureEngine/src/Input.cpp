#include "Input.h"

#include <iostream>

namespace AdventureEngine
{
	glm::vec2 Input::m_mousePos;
	glm::vec2 Input::m_prevMousePos;
	glm::vec2 Input::m_mouseDelta;
	std::unordered_map<int, int> Input::m_buttonState;
	std::unordered_map<int, bool> Input::m_activeButtons = std::unordered_map<int, bool>(512);

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

	glm::vec2 Input::getMouseDelta()
	{
		return m_mouseDelta;
	}

	bool Input::getButton(int button)
	{
		return m_activeButtons[button];
	}

	bool Input::getButtonDown(int button)
	{
		if (m_buttonState.count(button) == 1)
		{
			return m_buttonState[button] == GLFW_PRESS;
		}

		return false;
	}

	bool Input::getButtonUp(int button)
	{
		if (m_buttonState.count(button) == 1)
		{
			return m_buttonState[button] == GLFW_RELEASE;
		}

		return false;
	}

	void Input::mouseMoveCallback(GLFWwindow* window, double x, double y)
	{
		m_prevMousePos = m_mousePos;
		m_mousePos = glm::vec2(x, y);
		m_mouseDelta = m_mousePos - m_prevMousePos;
	}

	void Input::mouseClickCallback(GLFWwindow* window, int button, int action, int mods)
	{
		m_buttonState[button] = action;
		m_activeButtons[button] = action != GLFW_RELEASE;
	}

	void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		m_buttonState[key] = action;
		m_activeButtons[key] = action != GLFW_RELEASE;
	}
	void Input::clear()
	{
		m_buttonState.clear();
	}
}
