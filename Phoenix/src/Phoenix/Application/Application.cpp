#include "phxpch.h"
#include "Phoenix/Application/Application.h"

#include "Phoenix/Input/Input.h"
#include "Phoenix/Renderer/Buffer.h"
#include "Phoenix/Renderer/Renderer.h"

#include "Phoenix/Application/DiscordRPC.h"

#include <GLFW/glfw3.h>

namespace phx {

	Application* Application::s_Instance = nullptr;

	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application(const std::string& name)
		
	{
		PHX_PROFILE_FUNCTION();

		PHX_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		PHX_PROFILE_FUNCTION();
	}

	void Application::PushLayer(Layer* layer)
	{
		PHX_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		PHX_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		PHX_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(PHX_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(PHX_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			if (e.Handled)
				break;
			(*--it)->OnEvent(e);		
		}
	}

	void Application::Run()
	{
		PHX_PROFILE_FUNCTION();
		while (m_Running)
		{		
			PHX_PROFILE_SCOPE("RunLoop");
			float time = (float)glfwGetTime();
			DeltaTime deltaTime = time - m_DeltaTime;
			m_DeltaTime = time;
			if (!m_Minimized)
			{
				PHX_PROFILE_SCOPE("LayerStack OnUpdate");
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(deltaTime);
			}		
			m_ImGuiLayer->Begin();
			{
				PHX_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}			
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		PHX_PROFILE_FUNCTION();
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}