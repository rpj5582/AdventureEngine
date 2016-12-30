#pragma once
#include "Component.h"

#include "Shader.h"

namespace AdventureEngine
{
	class SkyboxComponent : public Component
	{
	public:
		SkyboxComponent();
		SkyboxComponent(Texture* cubeMapTexture);
		SkyboxComponent(Texture* cubeMapTexture, const Shader* shader);
		~SkyboxComponent();

		const Texture* cubeMapTexture;
		const Shader* shader;

	private:
		virtual void init() override;
		virtual void update(float deltaTime) override;
	};
}