#include "EditorLayer.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

#include "Panels/ThemeEditor.h"

#include <chrono>

float highfps = 0;

namespace phx
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
	{
	}

	void EditorLayer::OnAttach()
	{
		PHX_PROFILE_FUNCTION();

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
		PHX_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(DeltaTime dt)
	{
		PHX_PROFILE_FUNCTION();

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		deltatimems = dt.GetMilliseconds();

		if (m_ViewportFocused)
			m_CameraController.OnUpdate(dt);

		Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		RenderCommand::ClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		//Renderer2D::BeginScene(m_CameraController.GetCamera());

		m_ActiveScene->OnUpdate(dt);

		//Renderer2D::EndScene();

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		PHX_PROFILE_FUNCTION();

		//ImGui::ShowDemoWindow();

		static bool p_open = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					SaveScene();
				}
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Quit", "Alt+F4"))
				{ 
					Application::Get().Close(); 
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();

	#ifdef PHX_DEBUG_MODE
			ImGui::Begin("Metrics");
			float fps = 1000.0f / deltatimems;

			if (fps > highfps)
				highfps = fps;

			auto stats = Renderer2D::GetStats();
			ImGui::Text("FPS: %.0f", fps);
			ImGui::SameLine();
			ImGui::Text("High: %.0f", highfps);
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
				highfps = 0;
			ImGui::Separator();
			ImGui::Text("Renderer2D Stats");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
			ImGui::Separator();
			ImGui::Text("Scene Stats");
			ImGui::Text("Registry Size: %d", m_ActiveScene->GetRegistrySize());
			ImGui::End();
	#endif

		//renderThemeEditor();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(PHX_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}
	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		//	ShortCuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(PHX_KEY_LEFT_CONTROL) || Input::IsKeyPressed(PHX_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(PHX_KEY_LEFT_SHIFT) || Input::IsKeyPressed(PHX_KEY_RIGHT_SHIFT);

		switch (e.GetKeyCode())
		{
		case (int)PHX_KEY_S:
		{
			if (control && shift)
			{
				SaveSceneAs();
			}
			else if (control)
			{
				SaveScene();
			}
			break;
		}

		case (int)PHX_KEY_O:
		{
			if (control)
			{
				OpenScene();
			}
			break;
		}
		case (int)PHX_KEY_N:
		{
			if (control)
			{
				NewScene();
			}
			break;
		}
		}
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Phoenix Scene (*.phoenix)\0*.phoenix\0", ".phoenix");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
			m_ActiveScene->SetFilePath(filepath);
		}
	}

	void EditorLayer::SaveScene()
	{
		if (m_ActiveScene->GetFilePath().empty())
		{
			std::string filepath = FileDialogs::SaveFile("Phoenix Scene (*.phoenix)\0*.phoenix\0", ".phoenix");
			if (!filepath.empty())
			{
				SceneSerializer serializer(m_ActiveScene);
				serializer.Serialize(filepath);
			}
		}
		else
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(m_ActiveScene->GetFilePath());
		}

	}

	void EditorLayer::OpenScene()
	{

		std::string filepath = FileDialogs::OpenFile("Phoenix Scene (*.phoenix)\0*.phoenix\0");
		if (!filepath.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
			m_ActiveScene->SetFilePath(filepath);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}

}

