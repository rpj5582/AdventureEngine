#include "Level.h"

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include <fstream>

namespace AdventureEngine
{
	Level::Level(float* aspectRatio)
	{
		m_aspectRatio = aspectRatio;
	}

	Level::~Level()
	{
		delete m_defaultShader;

		AssetManager::clean();

		for (unsigned int i = 0; i < objects.size(); i++)
		{
			delete objects[i];
		}

		m_aspectRatio = nullptr;
	}

	bool Level::load()
	{
		m_defaultShader = new Shader("vertexShader.glsl", "fragmentShader.glsl");
		if (!m_defaultShader->load())
		{
			return false;
		}

		m_defaultShader->use();

		AssetManager::init();
		return true;
	}

	void Level::update(float deltaTime)
	{
		for (unsigned int i = 0; i < objects.size(); i++)
		{
			/// UPDATE OBJECTS
			objects[i]->update(deltaTime);

			/// HANDLE COLLISIONS
			// NOT YET IMPLEMENTED
		}
	}

	void Level::render() const
	{
		// Defines the sky color
		glm::vec3 skyColor = glm::vec3(0.8f, 0.8f, 0.8f);

		// Clears the screen with the sky color
		glClearColor(skyColor.r, skyColor.g, skyColor.b, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draws each object
		for (unsigned int i = 0; i < objects.size(); i++)
		{
			// Uses the model's shader
			const ShaderComponent* shaderComponent = objects[i]->getComponent<ShaderComponent>();
			if (shaderComponent)
			{
				shaderComponent->m_shader->use();
			}
			else
			{
				m_defaultShader->use();
			}

			// Upload the sky color
			glUniform3f(7, skyColor.r, skyColor.g, skyColor.b);

			// Handles the materials and textures
			const RenderComponent* renderComponent = objects[i]->getComponent<RenderComponent>();
			if (renderComponent)
			{
				// Uploads the UV scale for terrain tiling
				glUniform2f(13, objects[i]->scale.x, objects[i]->scale.z);

				GLuint prevTextureID = 0;
				GLuint textureID = 0;
				for (unsigned int j = 0; j < renderComponent->materials.size(); j++)
				{
					// Gets the material
					const Material* material = renderComponent->materials[j];

					// Uploads the material properties
					glUniform1f(5, material->getReflectivity());
					glUniform1f(6, material->getSpecularDamping());

					// Gets the texture and texture ID
					const Texture* texture = material->getTexture();
					textureID = texture->getID();

					// Gets the texture atlas index
					unsigned int atlasIndex = material->getAtlasIndex();

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
					glUniform1i(8 + j, j);

					// If the texture ID is the same as the last one, don't bother binding it
					if (prevTextureID == textureID) continue;

					// Activates and binds the textures
					glActiveTexture(GL_TEXTURE0 + j);
					glBindTexture(GL_TEXTURE_2D, textureID);

					// Updates the previous texture ID
					prevTextureID = textureID;
				}
			}

			// Handles rendering the model
			const ModelComponent* modelComponent = objects[i]->getComponent<ModelComponent>();
			if (modelComponent)
			{
				// Upload the necessary matrices to the shader
				uploadMatrices(objects[i]);

				// Renders the model
				renderModel(modelComponent->getModel());

				// Cleans up the texture binds
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			// Uploads each light to the graphics card
			for (unsigned int i = 0; i < lights.size(); i++)
			{
				// Can't upload more than the maximum number of lights
				if (i == MAX_LIGHTS) break;

				LightComponent* lightComponent = lights[i]->getComponent<LightComponent>();
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

	void Level::uploadMatrices(const Object* const object) const
	{
		// Model matrix
		glm::mat4 modelMatrix = object->getModelMatrix();

		// Sends the model matrix to the graphics card
		glUniformMatrix4fv(0, 1, GL_FALSE, &modelMatrix[0][0]);

		if (mainCamera)
		{
			// model-view-projection matrix
			CameraComponent* cameraComponent = mainCamera->getComponent<CameraComponent>();
			if (cameraComponent)
			{
				glm::mat4 viewMatrix = cameraComponent->getViewMatrix();
				glm::mat4 projectionMatrix = cameraComponent->getProjectionMatrix(*m_aspectRatio);

				// Sends the camera matrices to the graphics card
				glUniformMatrix4fv(1, 1, GL_FALSE, &viewMatrix[0][0]);
				glUniformMatrix4fv(2, 1, GL_FALSE, &projectionMatrix[0][0]);
			}
		}
	}

	void Level::renderModel(const Model* model) const
	{
		glBindVertexArray(model->getID());
		glDrawElements(GL_TRIANGLES, model->getVertexCount(), GL_UNSIGNED_INT, 0);
	}

	GLuint Level::getDefaultShaderID() const
	{
		return m_defaultShader->getID();
	}
}
