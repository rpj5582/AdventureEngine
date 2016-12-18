#include "RenderComponent.h"

namespace AdventureEngine
{
	RenderComponent::RenderComponent() : RenderComponent(nullptr)
	{
	}

	RenderComponent::RenderComponent(const Material* material) : RenderComponent(std::vector<const Material*> { material })
	{
	}

	RenderComponent::RenderComponent(std::vector<const Material*> materials)
	{
		this->materials = materials;
	}

	RenderComponent::~RenderComponent()
	{
	}

	void RenderComponent::init()
	{
	}

	void RenderComponent::update(float deltaTime)
	{
	}
}
