#include "FogComponent.h"

namespace AdventureEngine
{
	FogComponent::FogComponent() : FogComponent(glm::vec3(1, 1, 1))
	{
	}

	FogComponent::FogComponent(glm::vec3 color) : FogComponent(color, 0.0f)
	{
	}

	FogComponent::FogComponent(glm::vec3 color, float density) :FogComponent(color, density, 1.0f, 0, 0)
	{
	}

	FogComponent::FogComponent(glm::vec3 color, float density, float gradient, float skyboxStart, float skyboxEnd)
	{
		m_color = color;
		m_density = density;
		m_gradient = gradient;
		m_skyboxStart = skyboxStart;
		m_skyboxEnd = skyboxEnd;
	}

	glm::vec3 FogComponent::getColor() const
	{
		return m_color;
	}

	float FogComponent::getDensity() const
	{
		return m_density;
	}

	float FogComponent::getGradient() const
	{
		return m_gradient;
	}

	float FogComponent::getSkyboxGradientStart() const
	{
		return m_skyboxStart;
	}

	float FogComponent::getSkyboxGradientEnd() const
	{
		return m_skyboxEnd;
	}

	void FogComponent::init()
	{
	}

	void FogComponent::update(float deltaTime)
	{
	}
}