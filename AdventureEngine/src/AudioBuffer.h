#pragma once

#include <al.h>
#include <alc.h>

namespace AdventureEngine
{
	class AudioBuffer
	{
	public:
		friend class AudioManager;

		ALuint getBufferID() const;

	private:
		AudioBuffer(ALuint bufferID);
		~AudioBuffer();

		ALuint m_bufferID;
	};
}