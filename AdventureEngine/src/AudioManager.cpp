#include "AudioManager.h"

#include <fstream>
#include <iostream>

namespace AdventureEngine
{
	std::unordered_map<ALuint, AudioBuffer*> AudioManager::m_audioBuffers;

	void AudioManager::init()
	{
	}

	void AudioManager::clean()
	{
		for (auto it = m_audioBuffers.begin(); it != m_audioBuffers.end(); it++)
		{
			delete it->second;
		}
		m_audioBuffers.clear();
	}

	AudioBuffer* AudioManager::loadWAVFile(std::string audioFile)
	{
		// Most of this code was taken from http://www.dunsanyinteractive.com/blogs/oliver/?p=72

		struct RIFF_Header
		{
			char chunkID[4];
			int chunkSize; // Size, not including chunkSize or chunkID
			char format[4];
		} riff_header;

		struct WAV_Format
		{
			char subChunkID[4];
			int subChunkSize;
			short audioFormat;
			short numChannels;
			int sampleRate;
			int byteRate;
			short blockAlign;
			short bitsPerSample;
		} wav_format;

		struct WAV_Data
		{
			char subChunkID[4]; //should contain the word data
			int subChunk2Size; //Stores the size of the data block
		} wav_data;

		std::string filepath = "res/audio/" + audioFile;

		std::ifstream ifs(filepath, std::ios::binary);
		if (ifs.is_open())
		{
			// Reads in the riff header data
			ifs.read((char*)&riff_header, sizeof(RIFF_Header));

			// Checks the RIFF header is valid
			if (riff_header.chunkID[0] != 'R' || riff_header.chunkID[1] != 'I' || riff_header.chunkID[2] != 'F' || riff_header.chunkID[3] != 'F' ||
				riff_header.format[0] != 'W' || riff_header.format[1] != 'A' || riff_header.format[2] != 'V' || riff_header.format[3] != 'E')
			{
				std::cout << "Unable to load WAV file: Invalid RIFF header" << std::endl;
				ifs.close();
				return nullptr;
			}

			// Reads in the wave format data
			ifs.read((char*)&wav_format, sizeof(WAV_Format));

			if (wav_format.subChunkID[0] != 'f' || wav_format.subChunkID[1] != 'm' || wav_format.subChunkID[2] != 't' || wav_format.subChunkID[3] != ' ')
			{
				std::cout << "Unable to load WAV file: Invalid format" << std::endl;
				ifs.close();
				return nullptr;
			}

			// Determines the format based on the number of channels and bits per sample
			ALenum format = NULL;
			if (wav_format.numChannels == 1)
			{
				if (wav_format.bitsPerSample == 8)
				{
					format = AL_FORMAT_MONO8;
				}
					
				else if (wav_format.bitsPerSample == 16)
				{
					format = AL_FORMAT_MONO16;
				}
			}
			else if (wav_format.numChannels == 2)
			{
				if (wav_format.bitsPerSample == 8)
				{
					format = AL_FORMAT_STEREO8;
				}
				else if (wav_format.bitsPerSample == 16)
				{
					format = AL_FORMAT_STEREO16;
				}
			}

			// If there are extra parameters in the format chunk, just skip them
			if (wav_format.subChunkSize > 16)
			{
				ifs.seekg(sizeof(short), std::ios::cur);
			}

			// Reads in the wave data
			ifs.read((char*)&wav_data, sizeof(WAV_Data));

			if (wav_data.subChunkID[0] != 'd' || wav_data.subChunkID[1] != 'a' || wav_data.subChunkID[2] != 't' || wav_data.subChunkID[3] != 'a')
			{
				std::cout << "Unable to load WAV file: Invalid data header" << std::endl;
				ifs.close();
				return nullptr;
			}

			// Reads in the sound data into an array
			char* audioData = new char[wav_data.subChunk2Size];
			ifs.read(audioData, wav_data.subChunk2Size);

			ifs.close();

			// Load buffer into OpenAL
			ALuint bufferID;
			alGenBuffers(1, &bufferID);

			alBufferData(bufferID, format, audioData, wav_data.subChunk2Size, wav_format.sampleRate);

			// Deletes the audio data, as it is now taken care of by OpenAL
			delete[] audioData;

			// Creates the AudioBuffer object and adds it to the map
			AudioBuffer* audioBuffer = new AudioBuffer(bufferID);
			m_audioBuffers[bufferID] = audioBuffer;

			return audioBuffer;
		}

		std::cout << "Couldn't open file " << filepath << std::endl;
		return nullptr;
	}
}