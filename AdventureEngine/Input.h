#pragma once

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <unordered_map>

namespace AdventureEngine
{
	class Input
	{
	public:
		Input();
		~Input();

		static glm::vec2 getMousePosition();
		static glm::vec2 getMouseDelta();

		static bool getButton(int button);
		static bool getButtonDown(int button);
		static bool getButtonUp(int button);

		static void mouseMoveCallback(GLFWwindow* window, double x, double y);
		static void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		static void clear();

	private:
		static std::unordered_map<int, int> m_buttonState;
		static std::unordered_map<int, bool> m_activeButtons;
		static glm::vec2 m_mousePos;
		static glm::vec2 m_prevMousePos;
		static glm::vec2 m_mouseDelta;
	};
}
