#pragma once

#include "alc.h"

namespace phx {
	class Audio {
	public:
		~Audio();

		static void Init();

	private:
		bool CheckAlErrors();

		static ALCdevice* m_Device;
		static ALCcontext* m_Context;
	};
}