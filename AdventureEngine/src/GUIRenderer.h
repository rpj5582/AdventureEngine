#pragma once
#include "Renderer.h"

#include "AssetManager.h"

#include "GUIComponent.h"

namespace AdventureEngine
{
	class GUIRenderer : public Renderer
	{
	public:
		GUIRenderer();
		~GUIRenderer();

		virtual bool load() override;
		void render(const std::vector<Object*> guiObjects) const;

	private:
		void handleShaders(const GUIComponent* guiComponent) const;
		void handleTextures(const GUIComponent* guiComponent) const;
		void handleQuad(const glm::mat4 modelMatrix) const;

		const Model* m_quad;
	};
}