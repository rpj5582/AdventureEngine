#include "GUIRenderer.h"

#include <iostream>

namespace AdventureEngine
{
	GUIRenderer::GUIRenderer()
	{
		m_quad = nullptr;
	}

	GUIRenderer::~GUIRenderer()
	{
	}

	bool GUIRenderer::load()
	{
		m_defaultShader = AssetManager::loadShaderProgram("guiVertexShader.glsl", "guiFragmentShader.glsl");
		if (!m_defaultShader)
		{
			std::cout << "Failed to load default shader for the GUI renderer" << std::endl;
			return false;
		}

		m_quad = AssetManager::getModelPrimitive(ModelPrimitive::GUIQUAD);
		return true;
	}

	void GUIRenderer::render(const std::vector<GUIComponent*> guiComponents) const
	{
		glDisable(GL_DEPTH_TEST);

		for (unsigned int i = 0; i < guiComponents.size(); i++)
		{
			GUIComponent* guiComponent = guiComponents[i];
			Object* guiObject = guiComponent->getObject();

			handleShaders(guiComponent);
			handleMaterials(guiComponent);
			handleQuad(guiObject->getModelMatrix());
		}

		glEnable(GL_DEPTH_TEST);
	}

	void GUIRenderer::handleShaders(const GUIComponent* guiComponent) const
	{
		if (guiComponent->shader)
		{
			guiComponent->shader->use();
		}
		else
		{
			m_defaultShader->use();
		}
	}

	void GUIRenderer::handleMaterials(const GUIComponent* guiComponent) const
	{
		GLuint prevTextureID = 0;
		GLuint textureID = 0;
		for (unsigned int i = 0; i < guiComponent->materials.size(); i++)
		{
			// Gets the current material
			const Material* material = guiComponent->materials[i];

			// Gets the texture and texture ID
			const Texture* texture = material->getTexture();
			textureID = texture->getID();

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

	void GUIRenderer::handleQuad(const glm::mat4 modelMatrix) const
	{
		glUniformMatrix4fv(0, 1, GL_FALSE, &modelMatrix[0][0]);

		// Renders the model
		glBindVertexArray(m_quad->getID());
		glDrawArrays(GL_TRIANGLES, 0, m_quad->getVertexCount());
	}
}