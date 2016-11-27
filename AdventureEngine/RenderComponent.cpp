#include "RenderComponent.h"

namespace AdventureEngine
{
	RenderComponent::RenderComponent(Object* object) : Component(object)
	{
	}

	RenderComponent::~RenderComponent()
	{
	}

	void RenderComponent::initFromJSON(AssetManager* assetManager, json assets, json args)
	{
		for (unsigned int i = 0; i < assets.size(); i++)
		{
			json asset = assets[i];

			std::string assetType = asset["type"];
			if (assetType == "model")
			{
				model = assetManager->loadModel(asset["name"], asset["filepath"]);
			}
			else if (assetType == "texture")
			{
				texture = assetManager->loadTexture(asset["name"], asset["filepath"]);
			}
		}
	}

	void RenderComponent::update()
	{
	}
}
