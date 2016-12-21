#pragma once
#include "Component.h"

#include <glm\gtc\matrix_transform.hpp>

namespace AdventureEngine
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(float* aspectRatio);
		CameraComponent(float* aspectRatio, float perspectiveFOV);
		CameraComponent(float* aspectRatio, int orthoSize, int maxZ);
		CameraComponent(float* aspectRatio, float perspectiveFOV, int orthoSize, int maxZ, bool usePerspective);
		~CameraComponent();

		glm::mat4 getProjectionMatrix() const;
		glm::mat4 getViewMatrix() const;

	private:
		void init() override;
		void update(float deltaTime) override;

		float* m_aspectRatio;
		float m_fov;
		int m_orthoSize;
		int m_maxZ;
		bool m_usePerspective;
	};
}
