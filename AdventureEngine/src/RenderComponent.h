#pragma once
#include "Component.h"

namespace AdventureEngine
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent(const Model* model);
		RenderComponent(const Model* model, const Material* material);
		RenderComponent(const Model* model, const Shader* shader);
		RenderComponent(const Model* model, const Material* material, const Shader* shader);
		RenderComponent(const Model* model, std::vector<const Material*> materials);
		RenderComponent(const Model* model, std::vector<const Material*> materials, const Shader* shader);
		~RenderComponent();

		const Model* model;
		std::vector<const Material*> materials;
		const Shader* shader;

	private:
		void init() override;
		void update(float deltaTime) override;
	};
}
