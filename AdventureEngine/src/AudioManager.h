#pragma once

#include <string>
#include <unordered_map>

#include "AudioBuffer.h"

namespace AdventureEngine
{
	class AudioManager
	{
	public:
		static void init();
		static void clean();

		static AudioBuffer* loadWAVFile(std::string audioFile);

	private:
		static std::unordered_map<ALuint, AudioBuffer*> m_audioBuffers;
	};
}