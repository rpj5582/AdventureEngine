#include "LightComponent.h"

namespace AdventureEngine
{
	LightComponent::LightComponent() : LightComponent(Light::POINT)
	{
	}

	LightComponent::LightComponent(Light lightType) : LightComponent(lightType, 1)
	{
	}

	LightComponent::LightComponent(Light lightType, float intensity) : LightComponent(lightType, intensity, 1)
	{
	}

	LightComponent::LightComponent(Light lightType, float intensity, float radius) : LightComponent(lightType, intensity, radius, glm::vec3(1, 1, 1))
	{
	}

	LightComponent::LightComponent(Light lightType, float intensity, float radius, glm::vec3 color) : LightComponent(lightType, intensity, radius, color, lightType == Light::CONE ? 30.0f : 180.0f)
	{
	}

	LightComponent::LightComponent(Light lightType, float intensity, float radius, float coneAngle) : LightComponent(lightType, intensity, radius, glm::vec3(1, 1, 1), coneAngle)
	{
	}

	LightComponent::LightComponent(Light lightType, float intensity, float radius, glm::vec3 color, float coneAngle)
	{
		this->lightType = lightType;
		this->intensity = intensity;
		this->radius = radius;
		this->color = color;
		this->coneAngle = coneAngle;
	}

	LightComponent::~LightComponent()
	{
	}

	void LightComponent::init()
	{
	}

	void LightComponent::update(float deltaTime)
	{
	}
}