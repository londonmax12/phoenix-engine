#pragma once

#include <../vendor/OpenAL/include/AL/al.h>
#include <../vendor/OpenAL/include/AL/alc.h>

namespace phx {
	class SoundDevice
	{
	public:
		static SoundDevice* New();
	private:
		SoundDevice();
		~SoundDevice();

		ALCdevice* m_ALCDevice;
		ALCcontext* m_ALCContext;
	};
}