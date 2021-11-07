#pragma once

#include "Phoenix/Application/Base.h"
#include "Phoenix/Application/Window.h"
#include "Phoenix/Layer/LayerStack.h"
#include "Phoenix/Events/Event.h"
#include "Phoenix/Events/WindowEvent.h"

#include "Phoenix/ImGui/ImGuiLayer.h"

#include "Phoenix/Time/DeltaTime.h"

#include "Phoenix/Renderer/Shader.h"
#include "Phoenix/Renderer/Buffer.h"
#include "Phoenix/Renderer/VertexArray.h"
#include "Phoenix/Renderer/OrthographicCamera.h"

namespace phx {
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			PHX_CORE_ASSERT(index < Count, "");
			return Args[index];
		}
	};

	class Application
	{
	public:
		Application(const std::string& name = "Phoenix Application", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() { return *m_Window; }

		void Close();
		
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		static Application& Get() { return *s_Instance; }

		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }

		virtual void OnInit() {}
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		ApplicationCommandLineArgs m_CommandLineArgs;

		float m_DeltaTime = 0.0f;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication(ApplicationCommandLineArgs args);

}
	
