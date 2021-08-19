#pragma once

#include "Core.h"
#include"Window.h"
#include"Events/WindowEvent.h"

namespace phx {
	class PHX_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};
	// Define in client
	Application* CreateApplication();
}
	
