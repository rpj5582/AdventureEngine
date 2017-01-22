#include "AudioListenerComponent.h"

namespace AdventureEngine
{
	AudioListenerComponent::AudioListenerComponent()
	{
	}

	AudioListenerComponent::~AudioListenerComponent()
	{
	}

	void AudioListenerComponent::init()
	{
	}

	void AudioListenerComponent::update(float deltaTime)
	{
		glm::vec3 forward = object->getForward();
		glm::vec3 up = object->getUp();
		ALfloat orientation[] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };

		alListener3f(AL_POSITION, object->position.x, object->position.y, object->position.z);
		alListenerfv(AL_ORIENTATION, orientation);
	}
}