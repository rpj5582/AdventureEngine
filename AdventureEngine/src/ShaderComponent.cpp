#include "ShaderComponent.h"

namespace AdventureEngine
{
	ShaderComponent::ShaderComponent() : ShaderComponent(nullptr)
	{
	}

	ShaderComponent::ShaderComponent(Shader* shader)
	{
		m_shader = shader;
	}

	ShaderComponent::~ShaderComponent()
	{
	}

	void ShaderComponent::init()
	{
		if (m_shader)
		{
			m_shader->load();
		}
	}

	void ShaderComponent::update(float deltaTime)
	{
	}
}
