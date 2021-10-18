#pragma once

#include "Core.h"
#include "Window.h"
#include "Phoenix/Layer/LayerStack.h"
#include "Phoenix/Events/Event.h"
#include "Phoenix/Events/WindowEvent.h"

#include "Phoenix/ImGui/ImGuiLayer.h"

#include "Phoenix/Time/DeltaTime.h"

#include "Phoenix/Renderer/Shader.h"
#include "Phoenix/Renderer/Buffer.h"
#include "Phoenix/Renderer/VertexArray.h"
#include "Phoenix/Renderer/Camera.h"

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

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

		virtual void OnInit() {}
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		float m_DeltaTime = 0.0f;
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}
	
