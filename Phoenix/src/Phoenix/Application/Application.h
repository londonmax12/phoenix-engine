#pragma once

#include "Core.h"
#include "Window.h"
#include "Phoenix/Layer/LayerStack.h"
#include "Phoenix/Events/WindowEvent.h"

namespace phx {
	class PHX_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};
	// Define in client
	Application* CreateApplication();
}
	
