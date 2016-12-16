#include "LightComponent.h"

AdventureEngine::LightComponent::LightComponent() : LightComponent(1)
{
}

AdventureEngine::LightComponent::LightComponent(float radius) : LightComponent(radius, glm::vec3(1, 1, 1))
{
}

AdventureEngine::LightComponent::LightComponent(float radius, glm::vec3 color) : LightComponent(radius, color, false)
{
}

AdventureEngine::LightComponent::LightComponent(float radius, glm::vec3 color, bool isDirectionalLight)
{
	this->radius = radius;
	this->color = color;
	this->isDirectionalLight = isDirectionalLight;
}

AdventureEngine::LightComponent::~LightComponent()
{
}

void AdventureEngine::LightComponent::init()
{
}

void AdventureEngine::LightComponent::update()
{
}
