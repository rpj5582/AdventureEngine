#pragma once
#include "Component.h"

#include <glm\gtc\matrix_transform.hpp>

namespace AdventureEngine
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent();
		CameraComponent(float perspectiveFOV);
		CameraComponent(int orthoSize, int maxZ);
		CameraComponent(float perspectiveFOV, int orthoSize, int maxZ, bool usePerspective);
		~CameraComponent();

		glm::mat4 getProjectionMatrix(float aspectRatio) const;
		glm::mat4 getViewMatrix() const;

	private:
		void init() override;
		void update(float deltaTime) override;

		float m_fov;
		int m_orthoSize;
		int m_maxZ;
		bool m_usePerspective;
	};
}
