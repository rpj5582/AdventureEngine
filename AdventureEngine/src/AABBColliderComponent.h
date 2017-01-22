#pragma once

#include "Component.h"

namespace AdventureEngine
{
	class AABBColliderComponent : public Component
	{
	public:
		AABBColliderComponent(glm::vec3 min, glm::vec3 max);
		~AABBColliderComponent();

		bool collidesWithAABB(const AABBColliderComponent& other) const;

	private:
		virtual void init() override;
		virtual void update(float deltaTime) override;

		glm::vec3 m_min;
		glm::vec3 m_max;
	};
}