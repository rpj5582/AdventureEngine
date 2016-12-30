#pragma once
#include "Renderer.h"

#include "RenderComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"

#include <GL\glew.h>

namespace AdventureEngine
{
	class SceneRenderer : public Renderer
	{
	public:
		SceneRenderer();
		~SceneRenderer();

		virtual bool load() override;
		void render(const std::vector<RenderComponent*> renderComponent, const std::vector<LightComponent*> lights, const CameraComponent* mainCamera, const glm::vec3 fogColor) const;
		void render(const std::vector<RenderComponent*> renderComponent, const std::vector<LightComponent*> lights, const CameraComponent* mainCamera, const glm::vec3 fogColor, glm::vec4 clipPlane) const;

	private:
		void handleShader(const RenderComponent* renderComponent) const;
		void handleMaterials(const RenderComponent* renderComponent) const;
		void handleLights(const std::vector<LightComponent*> lights) const;
		void handleModel(const glm::mat4 modelMatrix, const Model* model, const CameraComponent* mainCamera) const;
	};
}