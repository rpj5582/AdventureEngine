#pragma once
#include "Component.h"

#include "AudioBuffer.h"

namespace AdventureEngine
{
	class AudioSourceComponent : public Component
	{
	public:
		AudioSourceComponent();
		AudioSourceComponent(AudioBuffer* audioBuffer);
		~AudioSourceComponent();

		void play() const;
		void pause() const;
		void stop() const;

		ALfloat getVolume() const;
		ALfloat getPitch() const;
		bool isPlaying() const;
		bool isPaused() const;
		ALboolean isLooping() const;

		void setAudio(AudioBuffer* audioBuffer);
		void setVolume(ALfloat volume);
		void setPitch(ALfloat pitch);
		void shouldLoop(ALboolean loop);

	private:
		virtual void init() override;
		virtual void update(float deltaTime) override;

		ALuint m_sourceID;
		const AudioBuffer* m_audioBuffer;

		ALfloat m_volume;
		ALfloat m_pitch;
		ALboolean m_looping;
	};
}