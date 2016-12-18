#pragma once
#include "Component.h"

#include "Shader.h"

namespace AdventureEngine
{
	class ShaderComponent : public Component
	{
	public:
		ShaderComponent();
		ShaderComponent(Shader* shader);
		~ShaderComponent();

		Shader* m_shader;

	private:
		virtual void init() override;
		virtual void update(float deltaTime) override;
	};
}
