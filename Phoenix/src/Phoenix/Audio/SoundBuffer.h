#pragma once

#include <../vendor/OpenAL/include/AL/al.h>

#include <vector>
#include <filesystem>

namespace phx {
	class SoundBuffer
	{
	public:
		static SoundBuffer* New();

		ALuint AddSoundEffect(std::filesystem::path filename);
		bool RemoveSoundEffect(const ALuint& buffer);

	private:
		SoundBuffer();
		~SoundBuffer();

		std::vector<ALuint> m_SoundEffectBuffers;
	};
}