#include "GUIComponent.h"

namespace AdventureEngine
{
	GUIComponent::GUIComponent() : GUIComponent(nullptr)
	{
	}

	GUIComponent::GUIComponent(const Texture* texture) : GUIComponent(texture, nullptr)
	{
	}

	GUIComponent::GUIComponent(const Texture* texture, const Shader* shader) : GUIComponent(std::vector<const Texture*> { texture }, shader)
	{
	}

	GUIComponent::GUIComponent(std::vector<const Texture*> textures) : GUIComponent(textures, nullptr)
	{
	}

	GUIComponent::GUIComponent(std::vector<const Texture*> textures, const Shader* shader)
	{
		this->textures = textures;
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