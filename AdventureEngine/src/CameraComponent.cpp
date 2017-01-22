#include "CameraComponent.h"

#include "Object.h"

namespace AdventureEngine
{
	CameraComponent::CameraComponent(const int* windowWidth, const int* windowHeight, float nearPlane, float farPlane) : CameraComponent(windowWidth, windowHeight, nearPlane, farPlane, 60.0f)
	{
	}

	CameraComponent::CameraComponent(const int* windowWidth, const int* windowHeight, float nearPlane, float farPlane, float perspectiveFOV) : CameraComponent(windowWidth, windowHeight, nearPlane, farPlane, perspectiveFOV, 5, 100, true)
	{
	}

	CameraComponent::CameraComponent(const int* windowWidth, const int* windowHeight, float nearPlane, float farPlane, int orthoSize, int maxZ) : CameraComponent(windowWidth, windowHeight, nearPlane, farPlane, 60.0f, orthoSize, maxZ, false)
	{
	}

	CameraComponent::CameraComponent(const int* windowWidth, const int* windowHeight, float nearPlane, float farPlane, float perspectiveFOV, int orthoSize, int maxZ, bool usePerspective)
	{
		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;
		m_nearPlane = nearPlane;
		m_farPlane = farPlane;
		m_fov = perspectiveFOV;
		m_orthoSize = orthoSize;
		m_maxZ = maxZ;
		m_usePerspective = usePerspective;
	}

	CameraComponent::~CameraComponent()
	{
	}

	float CameraComponent::getNearPlane() const
	{
		return m_nearPlane;
	}

	float CameraComponent::getFarPlane() const
	{
		return m_farPlane;
	}

	void CameraComponent::init()
	{
	}

	void CameraComponent::update(float deltaTime)
	{
	}

	glm::mat4 CameraComponent::getProjectionMatrix() const
	{
		// Dereferences the aspect ratio pointer to get its value
		float aspectRatio = *m_windowWidth / (float)*m_windowHeight;

		// projection matrix
		glm::mat4 projectionMatrix;
		if (m_usePerspective)
		{
			projectionMatrix = glm::perspective(glm::radians(m_fov), aspectRatio, m_nearPlane, m_farPlane);
		}
		else
		{
			float xSize = (float)m_orthoSize;
			float ySize = (float)m_orthoSize;
			if (aspectRatio > 1)
			{
				xSize *= aspectRatio;
			}
			else
			{
				ySize = xSize / aspectRatio;
			}
			projectionMatrix = glm::ortho(-xSize, xSize, -ySize, ySize, 0.0f, (float)m_maxZ);
		}

		return projectionMatrix;
	}

	glm::mat4 CameraComponent::getViewMatrix() const
	{
		// gets the camera's rotation matrix to use for calculating the view matrix
		glm::mat3 rotationMatrix = object->getRotationMatrix();

		// view matrix
		return glm::lookAt(object->position, object->position + -object->getForward(), object->getUp());

		
	}
}
