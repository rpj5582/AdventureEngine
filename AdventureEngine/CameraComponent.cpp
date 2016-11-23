#include "CameraComponent.h"

#include "Object.h"

namespace AdventureEngine
{
	CameraComponent::CameraComponent(Object* object, float* aspectRatio) : CameraComponent(object, aspectRatio, 5)
	{
	}

	CameraComponent::CameraComponent(Object* object, float* aspectRatio, float size) : Component(object)
	{
		m_aspectRatio = aspectRatio;
		m_size = size;
	}

	CameraComponent::~CameraComponent()
	{
		m_aspectRatio = nullptr;
	}

	void CameraComponent::update()
	{
	}

	glm::mat4 CameraComponent::calculateCameraMatrix() const
	{
		// view matrix
		glm::mat4 viewMatrix = glm::lookAt(object->position, object->position + glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

		// projection matrix
		float xSize = m_size;
		float ySize = m_size;
		if (*m_aspectRatio > 1)
		{
			xSize *= *m_aspectRatio;
		}
		else
		{
			ySize = xSize / *m_aspectRatio;
		}
		glm::mat4 projectionMatrix = glm::ortho(-xSize, xSize, -ySize, ySize, -1.0f, 1.0f);

		return projectionMatrix * viewMatrix;
	}
}
