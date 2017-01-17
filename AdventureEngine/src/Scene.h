#pragma once

#include <iostream>

#include "AssetManager.h"
#include "AudioManager.h"

#include "SceneRenderer.h"
#include "WaterRenderer.h"
#include "SkyboxRenderer.h"
#include "GUIRenderer.h"

#include "AudioSourceComponent.h"

namespace AdventureEngine
{
	class Scene
	{
	public:
		Scene(const int* windowWidth, const int* windowHeight);
		virtual ~Scene();

		void addObject(Object* object);
		void addRenderable(RenderComponent* render);
		void setWater(WaterComponent* water);
		void setSkybox(SkyboxComponent* skybox);
		void addLight(LightComponent* light);
		void addGUI(GUIComponent* gui);
		void setMainCamera(CameraComponent* mainCamera);

		virtual bool load();
		void update(float deltaTime);
		void render() const;

	protected:
		const Texture* waterReflectionTexture;
		const Texture* waterRefractionTexture;

		glm::vec3 fogColor;

		const int* windowWidth;
		const int* windowHeight;

	private:
		void renderWaterFBOs() const;

		SceneRenderer* m_sceneRenderer;
		WaterRenderer* m_waterRenderer;
		SkyboxRenderer* m_skyboxRenderer;
		GUIRenderer* m_guiRenderer;

		std::vector<Object*> m_objects;

		std::vector<RenderComponent*> m_renderables;
		WaterComponent* m_water;
		SkyboxComponent* m_skybox;
		std::vector<LightComponent*> m_lights;
		std::vector<GUIComponent*> m_guiObjects;
		CameraComponent* m_mainCamera;
	};
}
