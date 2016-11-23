#pragma once
#include "Component.h"

#include <glm\gtc\matrix_transform.hpp>

namespace AdventureEngine
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(Object* object, float* aspectRatio);
		CameraComponent(Object* object, float* aspectRatio, float size);
		~CameraComponent();

		void update() override;

		glm::mat4 calculateCameraMatrix() const;

	private:
		float m_size;
		float* m_aspectRatio;
	};
}
