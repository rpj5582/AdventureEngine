#pragma once
#include "Component.h"

#include <GL\glew.h>

#include "AssetManager.h"

namespace AdventureEngine
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent(Object* object);
		~RenderComponent();

		void initFromJSON(AssetManager* assetManager, json assets, json args) override;
		void update() override;

		const ModelAsset* model;
		const TextureAsset* texture;
	};
}
