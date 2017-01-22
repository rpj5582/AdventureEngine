#include "SkyboxRenderer.h"

#include <glm\gtc\matrix_access.hpp>
#include <iostream>

namespace AdventureEngine
{
	SkyboxRenderer::SkyboxRenderer()
	{
	}

	SkyboxRenderer::~SkyboxRenderer()
	{
	}

	bool SkyboxRenderer::load()
	{
		m_skyboxModel = AssetManager::getModelPrimitive(ModelPrimitive::SKYBOX);

		m_defaultShader = AssetManager::loadShaderProgram("skyboxVertexShader.glsl", "skyboxFragmentShader.glsl");
		if (!m_defaultShader)
		{
			std::cout << "Failed to load default shader for skybox renderer" << std::endl;
			return false;
		}

		return true;
	}

	void SkyboxRenderer::render(const SkyboxComponent* skybox, const CameraComponent* mainCamera, const FogComponent* fog) const
	{
		// Saves the cull face mode and depth function
		GLint cullFaceMode;
		glGetIntegerv(GL_CULL_FACE_MODE, &cullFaceMode);

		GLint depthFunc;
		glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);

		// Changes the cull face mode to cull front faces and changes the depth function to include depths of 1.
		// This is done to render the skybox cube on the inside and to not cull the skybox when performing the depth test, since the skybox's z value in normalized device coordinates will be 1.
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);

		handleShader(skybox);

		// Upload the camera's near and far planes
		glUniform1f(3, mainCamera->getNearPlane());
		glUniform1f(4, mainCamera->getFarPlane());

		if (fog)
		{
			// Upload the fog color
			glm::vec3 fogColor = fog->getColor();
			glUniform3f(9, fogColor.r, fogColor.g, fogColor.b);

			// Uploads the fog density and gradient
			glUniform1f(10, fog->getDensity());
			glUniform1f(11, fog->getGradient());

			// Uploads the starting and ending points for the skybox fog
			glUniform1f(12, fog->getSkyboxGradientStart());
			glUniform1f(13, fog->getSkyboxGradientEnd());
		}
		else
		{
			glUniform3f(9, 1, 1, 1);
			glUniform1f(10, 0);
			glUniform1f(11, 1);
			glUniform1f(12, 0);
			glUniform1f(13, 0);
		}

		handleCubeMapTexture(skybox);
		handleModel(mainCamera);

		// Changes the cull face mode and the depth test function back to the old ones
		glCullFace(cullFaceMode);
		glDepthFunc(depthFunc);
	}

	void SkyboxRenderer::handleShader(const SkyboxComponent* skybox) const
	{
		if (skybox->shader)
		{
			skybox->shader->use();
		}
		else
		{
			m_defaultShader->use();
		}
	}

	void SkyboxRenderer::handleCubeMapTexture(const SkyboxComponent* skybox) const
	{
		glUniform1i(14, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubeMapTexture->getID());
	}

	void SkyboxRenderer::handleModel(const CameraComponent* mainCamera) const
	{
		glm::mat4 viewMatrix = mainCamera->getViewMatrix();
		glm::mat4 projectionMatrix = mainCamera->getProjectionMatrix();

		// Removes the translation from the camera's view matrix
		viewMatrix = glm::column(viewMatrix, 3, glm::vec4(0, 0, 0, 1));

		glUniformMatrix4fv(0, 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(1, 1, GL_FALSE, &projectionMatrix[0][0]);

		glBindVertexArray(m_skyboxModel->getID());
		glDrawArrays(GL_TRIANGLES, 0, m_skyboxModel->getVertexCount());
	}
}