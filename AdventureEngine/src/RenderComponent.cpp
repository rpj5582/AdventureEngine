#include "RenderComponent.h"

namespace AdventureEngine
{
	RenderComponent::RenderComponent(const Model* model) : RenderComponent(model, (Material*)nullptr)
	{
	}

	RenderComponent::RenderComponent(const Model* model, const Material* material) : RenderComponent(model, material, nullptr)
	{
	}

	RenderComponent::RenderComponent(const Model* model, const Shader* shader) : RenderComponent(model, nullptr, shader)
	{
	}

	RenderComponent::RenderComponent(const Model* model, const Material* material, const Shader* shader) : RenderComponent(model, std::vector<const Material*> { material }, shader)
	{
	}

	RenderComponent::RenderComponent(const Model* model, std::vector<const Material*> materials) : RenderComponent(model, materials, nullptr)
	{
	}

	RenderComponent::RenderComponent(const Model* model, std::vector<const Material*> materials, const Shader* shader)
	{
		this->model = model;
		this->materials = materials;
		this->shader = shader;
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
