#include "AABBColliderComponent.h"

namespace AdventureEngine
{
	AABBColliderComponent::AABBColliderComponent(glm::vec3 min, glm::vec3 max)
	{
		m_min = min;
		m_max = max;
	}

	AABBColliderComponent::~AABBColliderComponent()
	{
	}

	bool AABBColliderComponent::collidesWithAABB(const AABBColliderComponent& other) const
	{
		glm::vec3 min = m_min + object->position;
		glm::vec3 max = m_max + object->position;

		glm::vec3 otherMin = other.m_min + other.object->position;
		glm::vec3 otherMax = other.m_max + other.object->position;

		if (max.x >= otherMin.x && min.x <= otherMax.x)
		{
			if (max.y >= otherMin.y && min.y <= otherMax.y)
			{
				if (max.z >= otherMin.z && min.z <= otherMax.z)
				{
					return true;
				}
			}
		}

		return false;
	}

	void AABBColliderComponent::init()
	{
	}

	void AABBColliderComponent::update(float deltaTime)
	{
	}
}
