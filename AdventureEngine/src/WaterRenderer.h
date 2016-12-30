#pragma once
#include "Renderer.h"

#include "WaterComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"

#define REFLECTION_WIDTH 1280
#define REFLECTION_HEIGHT 720

#define REFRACTION_WIDTH 1280
#define REFRACTION_HEIGHT 720

namespace AdventureEngine
{
	class WaterRenderer : public Renderer
	{
	public:
		WaterRenderer(const int* windowWidth, const int* windowHeight);
		~WaterRenderer();

		void bindReflectionFBO();
		void bindRefractionFBO();
		void unbindFBOs();

		Texture* getReflectionTexture() const;
		Texture* getRefractionTexture() const;

		virtual bool load() override;
		void render(const WaterComponent* waterComponent, const std::vector<LightComponent*> lights, const CameraComponent* mainCamera, const glm::vec3 fogColor) const;

	private:
		GLuint createReflectionFBO();
		GLuint createRefractionFBO();
		GLuint generateFBO();

		void bindFBO(GLuint fbo, GLuint width, GLuint height);

		Texture* attachTextureToFBO(GLuint fbo, GLuint width, GLuint height, GLenum colorAttachment);
		Texture* attachDepthTextureToFBO(GLuint fbo, GLuint width, GLuint height);
		GLuint attachDepthRenderBufferToFBO(GLuint fbo, GLuint width, GLuint height);

		void handleShaders(const WaterComponent* waterComponent) const;
		void handleMaterials(const WaterComponent* waterComponent) const;
		void handleLights(const std::vector<LightComponent*> lights) const;
		void handleQuad(const glm::mat4 modelMatrix, const CameraComponent* mainCamera) const;

		GLuint m_reflectionFBO;
		Texture* m_reflectionTexture;
		GLuint m_reflectionDepthRenderBuffer;

		GLuint m_refractionFBO;
		Texture* m_refractionTexture;
		Texture* m_refractionDepthTexture;

		const int* m_windowWidth;
		const int* m_windowHeight;

		const Model* m_quad;
	};
}