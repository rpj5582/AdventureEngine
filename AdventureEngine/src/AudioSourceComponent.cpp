#include "AudioSourceComponent.h"

namespace AdventureEngine
{
	AudioSourceComponent::AudioSourceComponent()
	{
		m_volume = 1;
		m_pitch = 1;
		m_looping = false;
	}

	AudioSourceComponent::AudioSourceComponent(AudioBuffer* audioBuffer)
	{
		this->m_audioBuffer = audioBuffer;
	}

	AudioSourceComponent::~AudioSourceComponent()
	{
		stop();
	}

	void AudioSourceComponent::play() const
	{
		stop();
		alSourcePlay(m_sourceID);
	}

	void AudioSourceComponent::pause() const
	{
		alSourcePause(m_sourceID);
	}

	void AudioSourceComponent::stop() const
	{
		alSourceStop(m_sourceID);
	}

	ALfloat AudioSourceComponent::getVolume() const
	{
		return m_volume;
	}

	ALfloat AudioSourceComponent::getPitch() const
	{
		return m_pitch;
	}

	bool AudioSourceComponent::isPlaying() const
	{
		ALint state;
		alGetSourcei(m_sourceID, AL_SOURCE_STATE, &state);
		return state == AL_PLAYING;
	}

	bool AudioSourceComponent::isPaused() const
	{
		ALint state;
		alGetSourcei(m_sourceID, AL_SOURCE_STATE, &state);
		return state == AL_PAUSED;
	}

	ALboolean AudioSourceComponent::isLooping() const
	{
		return m_looping;
	}

	void AudioSourceComponent::setAudio(AudioBuffer* audioBuffer)
	{
		m_audioBuffer = audioBuffer;
		alSourcei(m_sourceID, AL_BUFFER, m_audioBuffer->getBufferID());
	}

	void AudioSourceComponent::setVolume(ALfloat volume)
	{
		m_volume = volume;
		alSourcef(m_sourceID, AL_GAIN, m_volume);
	}

	void AudioSourceComponent::setPitch(ALfloat pitch)
	{
		m_pitch = pitch;
		alSourcef(m_sourceID, AL_PITCH, m_pitch);
	}

	void AudioSourceComponent::shouldLoop(ALboolean loop)
	{
		m_looping = loop;
		alSourcei(m_sourceID, AL_LOOPING, m_looping);
	}

	void AudioSourceComponent::init()
	{
		alGenSources(1, &m_sourceID);
		alSourcei(m_sourceID, AL_BUFFER, m_audioBuffer->getBufferID());
	}

	void AudioSourceComponent::update(float deltaTime)
	{
		alSource3f(m_sourceID, AL_POSITION, object->position.x, object->position.y, object->position.z);
	}
}
