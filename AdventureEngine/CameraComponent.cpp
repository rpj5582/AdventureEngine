#include "CameraComponent.h"

#include "Object.h"

namespace AdventureEngine
{
	CameraComponent::CameraComponent(Object* object) : CameraComponent(object, 60, 5, 100, true)
	{
	}

	CameraComponent::CameraComponent(Object* object, float perspectiveFOV) : CameraComponent(object, perspectiveFOV, 5, 100, true)
	{
	}

	CameraComponent::CameraComponent(Object* object, float orthoSize, float maxZ) : CameraComponent(object, 60.0f, orthoSize, maxZ, false)
	{
	}

	CameraComponent::CameraComponent(Object* object, float perspectiveFOV, float orthoSize, float maxZ, bool usePerspective) : Component(object)
	{
		m_fov = perspectiveFOV;
		m_orthoSize = orthoSize;
		m_maxZ = maxZ;
		m_usePerspective = usePerspective;
	}

	CameraComponent::~CameraComponent()
	{
		
	}

	void CameraComponent::initFromJSON(AssetManager* assetManager, json assets, json args)
	{
		m_fov = args[0];
		m_orthoSize = args[1];
		m_maxZ = args[2];
		m_usePerspective = args[3];
	}

	void CameraComponent::update()
	{
	}

	glm::mat4 CameraComponent::calculateCameraMatrix(float aspectRatio) const
	{
		// gets the camera's rotation matrix to use for calculating the view matrix
		glm::mat3 rotationMatrix = object->getRotationMatrix();

		// view matrix
		glm::mat4 viewMatrix = glm::lookAt(object->position, object->position + rotationMatrix * glm::vec3(0, 0, -1), rotationMatrix * glm::vec3(0, 1, 0));

		// projection matrix
		glm::mat4 projectionMatrix;
		if (m_usePerspective)
		{
			projectionMatrix = glm::perspective(m_fov, aspectRatio, 0.1f, 100.0f);
		}
		else
		{
			float xSize = m_orthoSize;
			float ySize = m_orthoSize;
			if (aspectRatio > 1)
			{
				xSize *= aspectRatio;
			}
			else
			{
				ySize = xSize / aspectRatio;
			}
			projectionMatrix = glm::ortho(-xSize, xSize, -ySize, ySize, 0.0f, m_maxZ);
		}

		return projectionMatrix * viewMatrix;
	}
}
