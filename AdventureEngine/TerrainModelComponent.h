#pragma once
#include "ModelComponent.h"

namespace AdventureEngine
{
	class TerrainModelComponent : public ModelComponent
	{
	public:
		TerrainModelComponent();
		TerrainModelComponent(Texture* heightMap);
		~TerrainModelComponent();

		virtual void init() override;

	private:
		Model* generateModel();

		Texture* m_heightMap;
	};
}
