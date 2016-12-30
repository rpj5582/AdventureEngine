#include "WaterComponent.h"

namespace AdventureEngine
{
	WaterComponent::WaterComponent() : WaterComponent(nullptr)
	{
	}

	WaterComponent::WaterComponent(const Texture* dudvMap) : WaterComponent(dudvMap, nullptr)
	{
	}

	WaterComponent::WaterComponent(const Texture* dudvMap, const Texture* normalMap) : WaterComponent(dudvMap, normalMap, 0)
	{
	}

	WaterComponent::WaterComponent(const Texture* dudvMap, const Texture* normalMap, float reflectivity) : WaterComponent(dudvMap, normalMap, reflectivity, 1)
	{
	}

	WaterComponent::WaterComponent(const Texture* dudvMap, const Texture* normalMap, float reflectivity, float specularDamping) : WaterComponent(dudvMap, normalMap, reflectivity, specularDamping, glm::vec4(1, 1, 1, 0.25f))
	{
	}

	WaterComponent::WaterComponent(const Texture* dudvMap, const Texture* normalMap, float reflectivity, float specularDamping, glm::vec4 color) : WaterComponent(dudvMap, normalMap, reflectivity, specularDamping, color, nullptr)
	{
	}

	WaterComponent::WaterComponent(const Texture* dudvMap, const Texture* normalMap, float reflectivity, float specularDamping, glm::vec4 color, const Shader* shader)
	{
		this->dudvMap = dudvMap;
		this->normalMap = normalMap;
		this->reflectivity = reflectivity;
		this->specularDamping = specularDamping;
		this->color = color;
		this->shader = shader;
	}

	WaterComponent::~WaterComponent()
	{	
	}

	void WaterComponent::init()
	{
	}

	void WaterComponent::update(float deltaTime)
	{
		waveMovement += 0.01f * deltaTime;
	}
}