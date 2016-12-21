#include "SceneRenderer.h"

#include <iostream>

namespace AdventureEngine
{
	SceneRenderer::SceneRenderer() : SceneRenderer(glm::vec3(0.8f, 0.8f, 0.8f))
	{
	}

	SceneRenderer::SceneRenderer(glm::vec3 fogColor)
	{
		m_fogColor = fogColor;
	}

	SceneRenderer::~SceneRenderer()
	{
		delete m_defaultShader;
	}

	bool SceneRenderer::load()
	{
		m_defaultShader = new Shader("vertexShader.glsl", "fragmentShader.glsl");
		if (!m_defaultShader->load())
		{
			std::cout << "Failed to load default shader for the scene renderer" << std::endl;
			return false;
		}

		// Sets the clear color to the fog color
		glClearColor(m_fogColor.r, m_fogColor.g, m_fogColor.b, 1);

		return true;
	}

	void SceneRenderer::render(const std::vector<Object*> objects, const std::vector<Object*> lights, const CameraComponent* mainCamera) const
	{
		// Clears the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (unsigned int i = 0; i < objects.size(); i++)
		{
			// Gets the object from the list
			const Object* object = objects[i];

			// Gets the render component for the object
			const RenderComponent* renderComponent = objects[i]->getComponent<RenderComponent>();
			if (!renderComponent) continue;

			// Handles the shader
			handleShader(renderComponent);

			// Upload the fog color
			glUniform3f(7, m_fogColor.r, m_fogColor.g, m_fogColor.b);

			// Uploads the UV scale for terrain tiling
			glUniform2f(13, objects[i]->scale.x, objects[i]->scale.z);

			// Handles the materials
			handleMaterials(renderComponent);

			// Handles the lights
			handleLights(lights);

			// Handles the model
			glm::mat4 modelMatrix = object->getModelMatrix();
			handleModel(modelMatrix, renderComponent->model, mainCamera);

			// Cleans up the texture binds
			glBindTexture(GL_TEXTURE_2D, 0);
		}	
	}

	void SceneRenderer::handleShader(const RenderComponent* renderComponent) const
	{
		// Uses the object's shader
		if (renderComponent->shader)
		{
			renderComponent->shader->use();
		}
		else
		{
			m_defaultShader->use();
		}
	}

	void SceneRenderer::handleMaterials(const RenderComponent* renderComponent) const
	{
		GLuint prevTextureID = 0;
		GLuint textureID = 0;
		for (unsigned int i = 0; i < renderComponent->materials.size(); i++)
		{
			// Gets the material
			const Material* material = renderComponent->materials[i];

			// Uploads the material properties
			glUniform1f(5, material->getReflectivity());
			glUniform1f(6, material->getSpecularDamping());

			// Gets the texture and texture ID
			const Texture* texture = material->getTexture();
			textureID = texture->getID();

			// Gets the texture atlas index
			unsigned int atlasIndex = texture->getAtlasIndex();

			// Uploads the atlas size, used for atlas uv calculation
			glm::uvec2 atlasSize = texture->getAtlasSize();
			glUniform2ui(3, atlasSize.x, atlasSize.y);

			// Calculates the x-offset
			float xOffset = atlasIndex % atlasSize.x / (float)atlasSize.x;

			// Flips the y-offset so that texture index 0 is in the top left, not the bottom left
			unsigned int column = atlasIndex % atlasSize.x;
			unsigned int row = atlasSize.y - atlasIndex / atlasSize.x - 1;
			float yOffset = (column + atlasSize.x * row) / atlasSize.y / (float)atlasSize.y;

			// Uploads the x and y atlas offset, used for altas uv calculation
			glUniform2f(4, xOffset, yOffset);

			// Uploads the texture unit
			glUniform1i(8 + i, i);

			// If the texture ID is the same as the last one, don't bother binding it
			if (prevTextureID == textureID) continue;

			// Activates and binds the textures
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textureID);

			// Updates the previous texture ID
			prevTextureID = textureID;
		}
	}

	void SceneRenderer::handleLights(const std::vector<Object*> lights) const
	{
		// Uploads each light to the graphics card
		for (unsigned int i = 0; i < lights.size(); i++)
		{
			// Can't upload more than the maximum number of lights
			if (i == MAX_LIGHTS) break;

			const LightComponent* lightComponent = lights[i]->getComponent<LightComponent>();
			if (lightComponent)
			{
				glm::vec3 forward = lights[i]->getForward();
				if (lightComponent->lightType == Light::DIRECTIONAL)
				{

					glUniform4f(14 + i * 7, forward.x, forward.y, forward.z, 0);
				}
				else
				{
					glUniform4f(14 + i * 7, lights[i]->position.x, lights[i]->position.y, lights[i]->position.z, 1);
				}

				glUniform1f(15 + i * 7, lightComponent->intensity);
				glUniform1f(16 + i * 7, lightComponent->radius);
				glUniform3f(17 + i * 7, lightComponent->color.x, lightComponent->color.y, lightComponent->color.z);
				glUniform3f(18 + i * 7, forward.x, forward.y, forward.z);
				if (lightComponent->lightType == Light::CONE)
				{
					glUniform1f(19 + i * 7, lightComponent->coneAngle);
				}
				else
				{
					glUniform1f(19 + i * 7, 180);
				}
			}
		}
	}

	void SceneRenderer::handleModel(const glm::mat4 modelMatrix, const Model* model, const CameraComponent* mainCamera) const
	{
		// Sends the model matrix to the graphics card
		glUniformMatrix4fv(0, 1, GL_FALSE, &modelMatrix[0][0]);

		glm::mat4 viewMatrix = mainCamera->getViewMatrix();
		glm::mat4 projectionMatrix = mainCamera->getProjectionMatrix();

		// Sends the camera matrices to the graphics card
		glUniformMatrix4fv(1, 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(2, 1, GL_FALSE, &projectionMatrix[0][0]);

		// Renders the model
		glBindVertexArray(model->getID());
		glDrawElements(GL_TRIANGLES, model->getVertexCount(), GL_UNSIGNED_INT, 0);
	}
}