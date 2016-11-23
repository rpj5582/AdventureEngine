#pragma once
#include "Component.h"

#include <GL\glew.h>

#include "AssetManager.h"

namespace AdventureEngine
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent(Object* object, const ModelAsset* model, const TextureAsset* texture);
		~RenderComponent();

		void update() override;

		const ModelAsset* model;
		const TextureAsset* texture;
	};
}
