#pragma once
#include "Component.h"

#include "Shader.h"

namespace AdventureEngine
{
	class GUIComponent : public Component
	{
	public:
		GUIComponent();
		GUIComponent(const Material* material);
		GUIComponent(const Material* material, const Shader* shader);
		GUIComponent(std::vector<const Material*> materials);
		GUIComponent(std::vector<const Material*> materials, const Shader* shader);
		~GUIComponent();

		std::vector<const Material*> materials;
		const Shader* shader;

	private:
		virtual void init() override;
		virtual void update(float deltaTime) override;
	};
}