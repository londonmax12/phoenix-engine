#pragma once

#include <../vendor/OpenAL/include/AL/al.h>

namespace phx {
	class SoundSource
	{
	public:
		SoundSource();
		~SoundSource();

		void Play(const ALuint buffer);

	private:
		ALuint m_Source;
		float m_Pitch = 1.0f;
		float m_Gain = 1.0f;
		float m_Position[3] = { 0,0,0 };
		float m_Velocity[3] = { 0,0,0 };
		bool m_LoopSound = false;
		ALuint m_Buffer = 0;
	};
}