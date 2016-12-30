#pragma once
#include "Component.h"

#include <glm\gtc\matrix_transform.hpp>

namespace AdventureEngine
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(const int* windowWidth, const int* windowHeight);
		CameraComponent(const int* windowWidth, const int* windowHeight, float perspectiveFOV);
		CameraComponent(const int* windowWidth, const int* windowHeight, int orthoSize, int maxZ);
		CameraComponent(const int* windowWidth, const int* windowHeight, float perspectiveFOV, int orthoSize, int maxZ, bool usePerspective);
		~CameraComponent();

		glm::mat4 getProjectionMatrix() const;
		glm::mat4 getViewMatrix() const;

	private:
		void init() override;
		void update(float deltaTime) override;

		const int* m_windowWidth;
		const int* m_windowHeight;
		float m_fov;
		int m_orthoSize;
		int m_maxZ;
		bool m_usePerspective;
	};
}
