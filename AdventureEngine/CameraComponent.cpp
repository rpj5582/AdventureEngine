#include "CameraComponent.h"

#include "Object.h"

namespace AdventureEngine
{
	CameraComponent::CameraComponent() : CameraComponent(1.0471975511965977461542144610932f)
	{
	}

	CameraComponent::CameraComponent(float perspectiveFOV) : CameraComponent(perspectiveFOV, 5, 100, true)
	{
	}

	CameraComponent::CameraComponent(int orthoSize, int maxZ) : CameraComponent(1.0471975511965977461542144610932f, orthoSize, maxZ, false)
	{
	}

	CameraComponent::CameraComponent(float perspectiveFOV, int orthoSize, int maxZ, bool usePerspective)
	{
		m_fov = perspectiveFOV;
		m_orthoSize = orthoSize;
		m_maxZ = maxZ;
		m_usePerspective = usePerspective;
	}

	CameraComponent::~CameraComponent()
	{
		
	}

	void CameraComponent::init()
	{
	}

	void CameraComponent::update()
	{
	}

	glm::mat4 CameraComponent::getProjectionMatrix(float aspectRatio) const
	{
		// projection matrix
		glm::mat4 projectionMatrix;
		if (m_usePerspective)
		{
			projectionMatrix = glm::perspective(m_fov, aspectRatio, 0.1f, 1000.0f);
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
		return glm::lookAt(object->position, object->position + rotationMatrix * glm::vec3(0, 0, -1), rotationMatrix * glm::vec3(0, 1, 0));

		
	}
}
