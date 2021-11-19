#include "phxpch.h"

#include "SoundDevice.h"

#include "Phoenix/Logging/Log.h"

namespace phx {
	SoundDevice* SoundDevice::New()
	{
		static SoundDevice* soundDevice = new SoundDevice();
		return soundDevice;
	}
	SoundDevice::SoundDevice()
	{
		m_ALCDevice = alcOpenDevice(nullptr); // Get the default audio device
		if (!m_ALCDevice)
			PHX_CORE_ERROR("OpenAL failed to get audio device");

		m_ALCContext = alcCreateContext(m_ALCDevice, nullptr);
		if (!m_ALCContext)
			PHX_CORE_ERROR("OpenAL failed set context");

		if (!alcMakeContextCurrent(m_ALCContext))
			PHX_CORE_ERROR("OpenAL failed make context");

		const ALCchar* name = nullptr;
		if (alcIsExtensionPresent(m_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
			name = alcGetString(m_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
		if (!name || alcGetError(m_ALCDevice) != AL_NO_ERROR)
			name = alcGetString(m_ALCDevice, ALC_DEVICE_SPECIFIER);
		PHX_CORE_INFO("AL opened {0}", name);
	}
	SoundDevice::~SoundDevice()
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(m_ALCContext);
		alcCloseDevice(m_ALCDevice);
	}
}