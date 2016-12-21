#pragma once

#include <iostream>

#include "AssetManager.h"

#include "SceneRenderer.h"
#include "GUIRenderer.h"

namespace AdventureEngine
{
	class Scene
	{
	public:
		Scene(float* aspectRatio);
		virtual ~Scene();

		virtual bool load();
		void update(float deltaTime);
		void render() const;

	protected:
		std::vector<Object*> objects;
		std::vector<Object*> lights;
		std::vector<Object*> guiObjects;

		CameraComponent* mainCamera;
		float* aspectRatio;

	private:
		SceneRenderer* m_sceneRenderer;
		GUIRenderer* m_guiRenderer;
	};
}
