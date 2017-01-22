#pragma once
#include "Component.h"

namespace AdventureEngine
{
	class SkyboxComponent : public Component
	{
	public:
		SkyboxComponent();
		SkyboxComponent(Cubemap* cubeMapTexture);
		SkyboxComponent(Cubemap* cubeMapTexture, const Shader* shader);
		~SkyboxComponent();

		const Cubemap* cubeMapTexture;
		const Shader* shader;

	private:
		virtual void init() override;
		virtual void update(float deltaTime) override;
	};
}