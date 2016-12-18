#include "ModelComponent.h"

namespace AdventureEngine
{
	ModelComponent::ModelComponent() : ModelComponent(nullptr)
	{
	}

	ModelComponent::ModelComponent(Model* model)
	{
		m_model = model;
	}

	ModelComponent::~ModelComponent()
	{
	}

	void ModelComponent::init()
	{
	}
	void ModelComponent::update(float deltaTime)
	{
	}

	const Model* ModelComponent::getModel() const
	{
		return m_model;
	}
}
