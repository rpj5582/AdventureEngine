#pragma once
#include "Renderer.h"

#include "RenderComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"

#include <GL\glew.h>
#include <vector>

#define MAX_LIGHTS 16

namespace AdventureEngine
{
	class SceneRenderer : public Renderer
	{
	public:
		SceneRenderer();
		SceneRenderer(glm::vec3 fogColor);
		~SceneRenderer();

		virtual bool load() override;
		void render(const std::vector<Object*> objects, const std::vector<Object*> lights, const CameraComponent* mainCamera) const;

	private:
		void handleShader(const RenderComponent* renderComponent) const;
		void handleMaterials(const RenderComponent* renderComponent) const;
		void handleLights(const std::vector<Object*> lights) const;
		void handleModel(const glm::mat4 modelMatrix, const Model* model, const CameraComponent* mainCamera) const;

		glm::vec3 m_fogColor;
	};
}