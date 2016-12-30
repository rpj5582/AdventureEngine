#include "SkyboxComponent.h"

namespace AdventureEngine
{
	SkyboxComponent::SkyboxComponent() : SkyboxComponent(nullptr)
	{
	}

	SkyboxComponent::SkyboxComponent(Texture* cubeMapTexture) : SkyboxComponent(cubeMapTexture, nullptr)
	{
		
	}

	SkyboxComponent::SkyboxComponent(Texture* cubeMapTexture, const Shader* shader)
	{
		this->cubeMapTexture = cubeMapTexture;
		this->shader = shader;
	}

	SkyboxComponent::~SkyboxComponent()
	{
	}

	void SkyboxComponent::init()
	{
	}

	void SkyboxComponent::update(float deltaTime)
	{
	}
}