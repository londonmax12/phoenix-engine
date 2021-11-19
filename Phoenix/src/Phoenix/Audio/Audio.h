#pragma once

#include "SoundBuffer.h"
#include "SoundDevice.h"

#include <filesystem>

#include <../vendor/OpenAL/include/AL/al.h>

namespace phx {
	class Audio {
	public:
		Audio();
		~Audio();

		uint32_t CreateSound(std::filesystem::path filepath);
	private:
		SoundDevice* m_Device;
	};
}