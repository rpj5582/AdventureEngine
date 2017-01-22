#include "SkyboxComponent.h"

namespace AdventureEngine
{
	SkyboxComponent::SkyboxComponent() : SkyboxComponent(nullptr)
	{
	}

	SkyboxComponent::SkyboxComponent(Cubemap* cubeMapTexture) : SkyboxComponent(cubeMapTexture, nullptr)
	{
		
	}

	SkyboxComponent::SkyboxComponent(Cubemap* cubeMapTexture, const Shader* shader)
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