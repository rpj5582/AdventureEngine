#pragma once

#include "Component.h"

namespace AdventureEngine
{
	class FogComponent : public Component
	{
	public:
		FogComponent();
		FogComponent(glm::vec3 color);
		FogComponent(glm::vec3 color, float density);
		FogComponent(glm::vec3 color, float density, float gradient, float skyboxStart, float skyboxEnd);

		glm::vec3 getColor() const;
		float getDensity() const;
		float getGradient() const;
		float getSkyboxGradientStart() const;
		float getSkyboxGradientEnd() const;

	private:
		virtual void init() override;
		virtual void update(float deltaTime) override;

		glm::vec3 m_color;
		float m_density;
		float m_gradient;
		float m_skyboxStart;
		float m_skyboxEnd;
	};
}