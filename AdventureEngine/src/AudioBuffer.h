#pragma once

#include <al.h>
#include <alc.h>

namespace AdventureEngine
{
	class AudioBuffer
	{
	public:
		friend class AudioManager;

		~AudioBuffer();

		ALuint getBufferID() const;

	private:
		AudioBuffer(ALuint bufferID);

		ALuint m_bufferID;
	};
}