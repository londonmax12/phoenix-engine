#include "phxpch.h"
#include "Application.h"
#include "Phoenix/Logging/Log.h"
#include "discord_rpc.h"
#include "discord_register.h"
#include <Glad/glad.h>
#include "Phoenix/Input/Input.h"

namespace phx {

	Application* Application::s_Instance = nullptr;
	static int64_t eptime;
	char* discordStatus;

	Application::Application()
	{
		s_Instance = this;

		#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		// Initialize Discord RPC
		
		Application::SetupDiscord();
		PHX_CORE_INFO("Discord RPC Initialized");
		Application::UpdateDiscord();
		Application::UpdateDiscordStatus("In Menu");
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::SetupDiscord()
	{
		if (gRPC)
		{
			DiscordEventHandlers handlers;
			memset(&handlers, 0, sizeof(handlers));
			Discord_Initialize("878503814603345930", &handlers, 1, "0");
			eptime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		}
		else
		{
			Discord_Shutdown();
		}
	}

	void Application::UpdateDiscordStatus(char* status)
	{
		discordStatus = status;
		UpdateDiscord();
		PHX_CORE_INFO("Discord status updated to: {0}", discordStatus);
	}

	void Application::UpdateDiscord()
	{
		DiscordRichPresence discordPresence;
		memset(&discordPresence, 0, sizeof(discordPresence));
		discordPresence.details = ("{0}", discordStatus);
		discordPresence.startTimestamp = eptime;
		discordPresence.largeImageKey = "phoenix_new";
		discordPresence.largeImageText = "Editing On Phoenix Engine";
		discordPresence.partyId = "ae488379-351d-4a4f-ad32-2b9b01c91657";
		Discord_UpdatePresence(&discordPresence);
	}
}