#pragma once

#include "Core.h"

namespace phx {
	class PHX_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};
	// Define in client
	Application* CreateApplication();
}
	
