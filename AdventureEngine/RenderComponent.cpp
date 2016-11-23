#include "RenderComponent.h"

namespace AdventureEngine
{
	RenderComponent::RenderComponent(Object* object, const ModelAsset* model, const TextureAsset* texture) : Component(object)
	{
		this->model = model;
		this->texture = texture;
	}

	RenderComponent::~RenderComponent()
	{
	}

	void RenderComponent::update()
	{
	}
}
