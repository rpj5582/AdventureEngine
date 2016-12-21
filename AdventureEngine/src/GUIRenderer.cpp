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
		m_defaultShader = new Shader("guiVertexShader.glsl", "guiFragmentShader.glsl");
		if (!m_defaultShader->load())
		{
			std::cout << "Failed to load default shader for the GUI renderer" << std::endl;
			return false;
		}

		m_quad = AssetManager::loadModel("guiQuad");
		return true;
	}

	void GUIRenderer::render(const std::vector<Object*> guiObjects) const
	{
		glDisable(GL_DEPTH_TEST);

		for (unsigned int i = 0; i < guiObjects.size(); i++)
		{
			Object* guiObject = guiObjects[i];
			GUIComponent* guiComponent = guiObject->getComponent<GUIComponent>();
			if (!guiComponent) continue;

			handleShaders(guiComponent);
			handleTextures(guiComponent);
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

	void GUIRenderer::handleTextures(const GUIComponent* guiComponent) const
	{
		GLuint prevTextureID = 0;
		GLuint textureID = 0;
		for (unsigned int i = 0; i < guiComponent->textures.size(); i++)
		{
			// Gets the texture and texture ID
			const Texture* texture = guiComponent->textures[i];
			textureID = texture->getID();

			// Gets the texture atlas index
			unsigned int atlasIndex = texture->getAtlasIndex();

			// Uploads the atlas size, used for atlas uv calculation
			glm::uvec2 atlasSize = texture->getAtlasSize();
			glUniform2ui(1, atlasSize.x, atlasSize.y);

			// Calculates the x-offset
			float xOffset = atlasIndex % atlasSize.x / (float)atlasSize.x;

			// Flips the y-offset so that texture index 0 is in the top left, not the bottom left
			unsigned int column = atlasIndex % atlasSize.x;
			unsigned int row = atlasSize.y - atlasIndex / atlasSize.x - 1;
			float yOffset = (column + atlasSize.x * row) / atlasSize.y / (float)atlasSize.y;

			// Uploads the x and y atlas offset, used for altas uv calculation
			glUniform2f(2, xOffset, yOffset);

			// If the texture ID is the same as the last one, don't bother binding it
			if (prevTextureID == textureID) continue;

			// Activates and binds the textures
			glActiveTexture(GL_TEXTURE0);
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