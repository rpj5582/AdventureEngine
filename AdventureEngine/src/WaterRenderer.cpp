#include "WaterRenderer.h"

#include <iostream>

namespace AdventureEngine
{
	WaterRenderer::WaterRenderer(const int* windowWidth, const int* windowHeight)
	{
		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;

		m_quad = nullptr;
	}

	WaterRenderer::~WaterRenderer()
	{
		// Deletes the reflection render buffer and frame buffer
		glDeleteRenderbuffers(1, &m_reflectionDepthRenderBuffer);
		glDeleteFramebuffers(1, &m_reflectionFBO);

		// Deletes the refraction frame buffer
		glDeleteFramebuffers(1, &m_refractionFBO);
	}

	void WaterRenderer::bindReflectionFBO()
	{
		bindFBO(m_reflectionFBO, REFLECTION_WIDTH, REFLECTION_HEIGHT);
	}

	void WaterRenderer::bindRefractionFBO()
	{
		bindFBO(m_refractionFBO, REFRACTION_WIDTH, REFRACTION_HEIGHT);
	}

	void WaterRenderer::unbindFBOs()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, *m_windowWidth, *m_windowHeight);
	}

	Texture* WaterRenderer::getReflectionTexture() const
	{
		return m_reflectionTexture;
	}

	Texture* WaterRenderer::getRefractionTexture() const
	{
		return m_refractionTexture;
	}

	bool WaterRenderer::load()
	{
		m_quad = AssetManager::getModelPrimitive(ModelPrimitive::QUAD);

		m_reflectionFBO = createReflectionFBO();
		m_refractionFBO = createRefractionFBO();

		m_defaultShader = new Shader("waterVertexShader.glsl", "waterFragmentShader.glsl");
		if (!m_defaultShader->load())
		{
			std::cout << "Failed to load default shader for the water renderer" << std::endl;
			return false;
		}
		return true;
	}

	void WaterRenderer::render(const WaterComponent* waterComponent, const std::vector<LightComponent*> lights, const CameraComponent* mainCamera, const glm::vec3 fogColor) const
	{
		const Object* waterObject = waterComponent->getObject();

		handleShaders(waterComponent);

		// Uploads the fog color
		glUniform3f(7, fogColor.r, fogColor.g, fogColor.b);

		// Uploads the water color and wave movement
		glUniform4f(15, waterComponent->color.r, waterComponent->color.g, waterComponent->color.b, waterComponent->color.a);
		glUniform1f(16, waterComponent->waveMovement);

		handleMaterials(waterComponent);
		handleLights(lights);
		handleQuad(waterObject->getModelMatrix(), mainCamera);
	}

	GLuint WaterRenderer::createReflectionFBO()
	{
		GLuint reflectionFBO = generateFBO();

		// The reflection FBO needs a texture attachment and a depth render buffer attachment
		m_reflectionTexture = attachTextureToFBO(reflectionFBO, REFLECTION_WIDTH, REFLECTION_HEIGHT, GL_COLOR_ATTACHMENT0);
		m_reflectionDepthRenderBuffer = attachDepthRenderBufferToFBO(reflectionFBO, REFLECTION_WIDTH, REFLECTION_HEIGHT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return reflectionFBO;
	}

	GLuint WaterRenderer::createRefractionFBO()
	{
		GLuint refractionFBO = generateFBO();

		// The refraction FBO needs a texture attachment and a depth texture attachment
		m_refractionTexture = attachTextureToFBO(refractionFBO, REFLECTION_WIDTH, REFLECTION_HEIGHT, GL_COLOR_ATTACHMENT0);
		m_refractionDepthTexture = attachDepthTextureToFBO(refractionFBO, REFLECTION_WIDTH, REFLECTION_HEIGHT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return refractionFBO;
	}

	GLuint WaterRenderer::generateFBO()
	{
		// Generates a new frame buffer
		GLuint fbo;
		glGenFramebuffers(1, &fbo);

		// Binds the frame buffer for use
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// Sets the FBO to render to color attachment 0
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		return fbo;
	}

	void WaterRenderer::bindFBO(GLuint fbo, GLuint width, GLuint height)
	{
		// Unbinds any textures, just in case
		glBindTexture(GL_TEXTURE_2D, 0);

		// Binds the frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// Changes the viewport to match the width and height
		glViewport(0, 0, width, height);
	}

	Texture* WaterRenderer::attachTextureToFBO(GLuint fbo, GLuint width, GLuint height, GLenum colorAttachment)
	{
		Texture* texture = AssetManager::createEmptyTexture(width, height);

		// Sets the frame buffer's color attachment to the texture
		glFramebufferTexture(GL_FRAMEBUFFER, colorAttachment, texture->getID(), 0);

		return texture;
	}

	Texture* WaterRenderer::attachDepthTextureToFBO(GLuint fbo, GLuint width, GLuint height)
	{
		Texture* depthTexture = AssetManager::createEmptyDepthTexture(width, height);

		// Sets the frame buffer's depth attachment to the texture
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture->getID(), 0);

		return depthTexture;
	}

	GLuint WaterRenderer::attachDepthRenderBufferToFBO(GLuint fbo, GLuint width, GLuint height)
	{
		// Generates a new render buffer
		GLuint depthRenderBuffer;
		glGenRenderbuffers(1, &depthRenderBuffer);

		// Binds the render buffer for use
		glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);

		// Attaches the frame buffer's depth attachment to the render buffer
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

		// Sets the frame buffer's depth attachment to the render buffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

		return depthRenderBuffer;
	}

	void WaterRenderer::handleShaders(const WaterComponent* waterComponent) const
	{
		if (waterComponent->shader)
		{
			waterComponent->shader->use();
		}
		else
		{
			m_defaultShader->use();
		}
	}

	void WaterRenderer::handleMaterials(const WaterComponent* waterComponent) const
	{
		// Uploads the reflectivity
		glUniform1f(5, waterComponent->reflectivity);

		// Uploads the specular damping
		glUniform1f(6, waterComponent->specularDamping);

		// Uploads the reflection texture unit
		glUniform1i(8, 0);

		// Activates and binds the reflection texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_reflectionTexture->getID());


		// Uploads the refraction texture unit
		glUniform1i(9, 1);

		// Activates and binds the refraction texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_refractionTexture->getID());

		// Uploads the refraction depth texture unit
		glUniform1i(10, 2);

		// Activates and binds the refraction depth texture
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_refractionDepthTexture->getID());

		if (waterComponent->dudvMap)
		{
			// Uploads the dudv map texture unit
			glUniform1i(11, 3);

			// Activates and binds the dudv map
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, waterComponent->dudvMap->getID());
		}

		if (waterComponent->normalMap)
		{
			// Uploads the normal map texture unit
			glUniform1i(12, 4);

			// Activates and binds the normal map
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, waterComponent->normalMap->getID());
		}
	}

	void WaterRenderer::handleLights(const std::vector<LightComponent*> lights) const
	{
		// Uploads each light to the graphics card
		for (unsigned int i = 0; i < lights.size(); i++)
		{
			// Can't upload more than the maximum number of lights
			if (i == MAX_LIGHTS) break;

			const LightComponent* light = lights[i];
			const Object* lightObject = lights[i]->getObject();

			glm::vec3 forward = lightObject->getForward();
			if (light->lightType == Light::DIRECTIONAL)
			{
				glUniform4f(17 + i * 6, forward.x, forward.y, forward.z, 0);
			}
			else
			{
				glUniform4f(17 + i * 6, lightObject->position.x, lightObject->position.y, lightObject->position.z, 1);
			}

			glUniform1f(18 + i * 6, light->intensity);
			glUniform1f(19 + i * 6, light->radius);
			glUniform3f(20 + i * 6, light->color.x, light->color.y, light->color.z);
			glUniform3f(21 + i * 6, forward.x, forward.y, forward.z);
			if (light->lightType == Light::CONE)
			{
				glUniform1f(22 + i * 6, light->coneAngle);
			}
			else
			{
				glUniform1f(22 + i * 6, 180);
			}
		}
	}

	void WaterRenderer::handleQuad(const glm::mat4 modelMatrix, const CameraComponent* mainCamera) const
	{
		// Sends the model matrix to the graphics card
		glUniformMatrix4fv(0, 1, GL_FALSE, &modelMatrix[0][0]);

		glm::mat4 viewMatrix = mainCamera->getViewMatrix();
		glm::mat4 projectionMatrix = mainCamera->getProjectionMatrix();

		// Sends the camera matrices to the graphics card
		glUniformMatrix4fv(1, 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(2, 1, GL_FALSE, &projectionMatrix[0][0]);

		// Renders the model
		glBindVertexArray(m_quad->getID());
		glDrawElements(GL_TRIANGLES, m_quad->getVertexCount(), GL_UNSIGNED_INT, 0);
	}
}