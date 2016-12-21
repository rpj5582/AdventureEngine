#pragma once
#include "Component.h"

#include "Shader.h"

namespace AdventureEngine
{
	class GUIComponent : public Component
	{
	public:
		GUIComponent();
		GUIComponent(const Texture* texture);
		GUIComponent(const Texture* texture, const Shader* shader);
		GUIComponent(std::vector<const Texture*> textures);
		GUIComponent(std::vector<const Texture*> textures, const Shader* shader);
		~GUIComponent();

		std::vector<const Texture*> textures;
		const Shader* shader;

	private:
		virtual void init() override;
		virtual void update(float deltaTime) override;
	};
}