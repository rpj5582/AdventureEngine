#include "AudioBuffer.h"

namespace AdventureEngine
{
	AudioBuffer::AudioBuffer(ALuint bufferID)
	{
		m_bufferID = bufferID;
	}

	AudioBuffer::~AudioBuffer()
	{
		alDeleteBuffers(1, &m_bufferID);
	}

	ALuint AudioBuffer::getBufferID() const
	{
		return m_bufferID;
	}
}