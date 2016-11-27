#pragma once

#include "AssetManager.h"

#include "Object.h"

#include "ComponentRegistry.h"
#include "RenderComponent.h"
#include "CameraComponent.h"
#include "TestComponent.h"

namespace AdventureEngine
{
	class Level
	{
	public:
		Level(float* aspectRatio);
		virtual ~Level();

		bool load(const char* levelPath);
		void update();
		void draw();

	private:
		float* m_aspectRatio;

		AssetManager* m_assetManager;

		std::vector<Object*> m_objects;
		Object* m_mainCamera;
	};
}
