#include "GUIComponent.h"

namespace AdventureEngine
{
	GUIComponent::GUIComponent() : GUIComponent(nullptr)
	{
	}

	GUIComponent::GUIComponent(const Material* material) : GUIComponent(material, nullptr)
	{
	}

	GUIComponent::GUIComponent(const Material* material, const Shader* shader) : GUIComponent(std::vector<const Material*> { material }, shader)
	{
	}

	GUIComponent::GUIComponent(std::vector<const Material*> materials) : GUIComponent(materials, nullptr)
	{
	}

	GUIComponent::GUIComponent(std::vector<const Material*> materials, const Shader* shader)
	{
		this->materials = materials;
		this->shader = shader;
	}

	GUIComponent::~GUIComponent()
	{
	}

	void GUIComponent::init()
	{
	}

	void GUIComponent::update(float deltaTime)
	{
	}
}