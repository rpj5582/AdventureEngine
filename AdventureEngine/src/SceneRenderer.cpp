#include "SceneRenderer.h"

#include <iostream>

namespace AdventureEngine
{
	SceneRenderer::SceneRenderer()
	{
	}

	SceneRenderer::~SceneRenderer()
	{
		
	}

	bool SceneRenderer::load()
	{
		m_defaultShader = AssetManager::loadShaderProgram("vertexShader.glsl", "fragmentShader.glsl");
		if (!m_defaultShader)
		{
			std::cout << "Failed to load default shader for the scene renderer" << std::endl;
			return false;
		}

		return true;
	}

	void SceneRenderer::render(const std::vector<RenderComponent*> renderComponent, const std::vector<LightComponent*> lights, const CameraComponent* mainCamera, const FogComponent* fog) const
	{
		render(renderComponent, lights, mainCamera, fog, glm::vec4());
	}

	void SceneRenderer::render(const std::vector<RenderComponent*> renderComponent, const std::vector<LightComponent*> lights, const CameraComponent* mainCamera, const FogComponent* fog, glm::vec4 clipPlane) const
	{
		// Clears the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (unsigned int i = 0; i < renderComponent.size(); i++)
		{
			// Gets the render component from the list
			const RenderComponent* renderable = renderComponent[i];
			
			// Gets the object from the render component
			const Object* renderObject = renderable->getObject();

			// Handles the shader
			handleShader(renderable);

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
			}
			else
			{
				glUniform3f(9, 1, 1, 1);
				glUniform1f(10, 0);
				glUniform1f(11, 1);
			}

			// Uploads the UV scale for terrain tiling
			glUniform2f(19, renderObject->scale.x, renderObject->scale.z);

			// Uploads the clipping plane height for water
			glUniform4f(20, clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);

			// Handles the materials
			handleMaterials(renderable);

			// Handles the lights
			handleLights(lights);

			// Handles the model
			glm::mat4 modelMatrix = renderObject->getModelMatrix();
			handleModel(modelMatrix, renderable->model, mainCamera);

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
			glUniform1f(7, material->getReflectivity());
			glUniform1f(8, material->getSpecularDamping());

			// Check which texture type the union has active
			TextureType textureType = material->getTextureType();
			if (textureType == TextureType::TEXTURE_ALTAS)
			{
				// Gets the texture and texture ID
				const TextureAtlas* textureAtlas = material->getTextureAtlas();
				textureID = textureAtlas->getID();

				// Gets the texture atlas index
				unsigned int atlasIndex = material->getAtlasIndex();

				// Uploads the atlas size, used for atlas uv calculation
				glm::uvec2 atlasSize = textureAtlas->getAtlasSize();
				glUniform2ui(5, atlasSize.x, atlasSize.y);

				// Calculates the x-offset
				float xOffset = atlasIndex % atlasSize.x / (float)atlasSize.x;

				// Flips the y-offset so that texture index 0 is in the top left, not the bottom left
				unsigned int column = atlasIndex % atlasSize.x;
				unsigned int row = atlasSize.y - atlasIndex / atlasSize.x - 1;
				float yOffset = (column + atlasSize.x * row) / atlasSize.y / (float)atlasSize.y;

				// Uploads the x and y atlas offset, used for altas uv calculation
				glUniform2f(6, xOffset, yOffset);
			}
			else
			{
				if (textureType == TextureType::TEXTURE)
				{
					// Gets the texture and texture ID
					const Texture* texture = material->getTexture();
					textureID = texture->getID();
				}
				else if (textureType == TextureType::CUBEMAP)
				{
					// Gets the texture and texture ID
					const Cubemap* cubemap = material->getCubemap();
					textureID = cubemap->getID();
				}

				// Upload the default size for non atlas textures
				glUniform2ui(5, 1, 1);

				// Upload the default offset for non atlas textures
				glUniform2f(6, 0, 0);
			}

			// Uploads the texture unit
			glUniform1i(14 + i, i);

			// If the texture ID is the same as the last one, don't bother binding it
			if (prevTextureID == textureID) continue;

			// Activates and binds the textures
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textureID);

			// Updates the previous texture ID
			prevTextureID = textureID;
		}
	}

	void SceneRenderer::handleLights(const std::vector<LightComponent*> lights) const
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
				glUniform4f(23 + i * 6, forward.x, forward.y, forward.z, 0);
			}
			else
			{
				glUniform4f(23 + i * 6, lightObject->position.x, lightObject->position.y, lightObject->position.z, 1);
			}

			glUniform1f(24 + i * 6, light->intensity);
			glUniform1f(25 + i * 6, light->radius);
			glUniform3f(26 + i * 6, light->color.x, light->color.y, light->color.z);
			glUniform3f(27 + i * 6, forward.x, forward.y, forward.z);
			if (light->lightType == Light::CONE)
			{
				glUniform1f(28 + i * 6, light->coneAngle);
			}
			else
			{
				glUniform1f(28 + i * 6, 180);
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