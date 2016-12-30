#pragma once
#include "Renderer.h"

#include "SkyboxComponent.h"
#include "CameraComponent.h"

#include "AssetManager.h"

namespace AdventureEngine
{
	class SkyboxRenderer : public Renderer
	{
	public:
		SkyboxRenderer();
		~SkyboxRenderer();

		virtual bool load() override;
		void render(const SkyboxComponent* skybox, const CameraComponent* mainCamera, const glm::vec3 fogColor) const;

	private:
		void handleShader(const SkyboxComponent* skybox) const;
		void handleCubeMapTexture(const SkyboxComponent* skybox) const;
		void handleModel(const CameraComponent* mainCamera) const;

		Model* m_skyboxModel;
	};
}