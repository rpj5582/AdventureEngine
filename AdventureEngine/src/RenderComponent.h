#pragma once
#include "Component.h"

namespace AdventureEngine
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent();
		RenderComponent(const Material* material);
		RenderComponent(std::vector<const Material*> materials);
		~RenderComponent();

		std::vector<const Material*> materials;

	private:
		void init() override;
		void update(float deltaTime) override;
	};
}
