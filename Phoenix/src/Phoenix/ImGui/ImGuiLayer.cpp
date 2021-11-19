#include "phxpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "Phoenix/Application/Application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "ImGuizmo.h"

namespace phx {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		PHX_PROFILE_FUNCTION();
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Medium.ttf", 16.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Light.ttf", 16.0f);

		ImGuiLayer::SetDarkThemeColors();
		
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		PHX_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		PHX_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& style = ImGui::GetStyle();
		style.WindowMenuButtonPosition = ImGuiDir_None;
		style.Colors[ImGuiCol_Text] = ImVec4(1.000000f, 1.000000f, 1.000000f, 1.000000);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.500000f, 0.500000f, 0.500000f, 1.000000);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.058824f, 0.062745f, 0.062745f, 1.000000);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.000000f, 0.000000f, 0.000000f, 0.000000);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.080000f, 0.080000f, 0.080000f, 0.940000);
		style.Colors[ImGuiCol_Border] = ImVec4(0.431373f, 0.431373f, 0.501961f, 0.062745);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.000000f, 0.000000f, 0.000000f, 0.000000);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.070588f, 0.070588f, 0.070588f, 1.000000);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.300000f, 0.305000f, 0.310000f, 1.000000);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.150000f, 0.150500f, 0.151000f, 1.000000);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.058824f, 0.062745f, 0.062745f, 1.000000);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.058824f, 0.062745f, 0.062745f, 1.000000);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.058824f, 0.062745f, 0.062745f, 1.000000);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.047393f, 0.047393f, 0.047393f, 1.000000);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.019608f, 0.019608f, 0.019608f, 0.000000);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.284360f, 0.284357f, 0.284357f, 0.345972);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.284360f, 0.284357f, 0.284357f, 0.345972);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.286275f, 0.286275f, 0.286275f, 0.113725);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.419608f, 0.000000f, 1.000000f, 1.000000);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.350710f, 0.000000f, 1.000000f, 1.000000);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.350710f, 0.000000f, 1.000000f, 1.000000);
		style.Colors[ImGuiCol_Button] = ImVec4(0.227696f, 0.000000f, 0.649289f, 1.000000);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.290839f, 0.000000f, 0.829384f, 1.000000);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.227696f, 0.000000f, 0.649289f, 1.000000);
		style.Colors[ImGuiCol_Header] = ImVec4(0.213268f, 0.213269f, 0.213270f, 0.582938);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.213268f, 0.213269f, 0.213270f, 0.582938);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.150000f, 0.150500f, 0.151000f, 1.000000);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.099525f, 0.099525f, 0.099526f, 0.317536);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.275908f, 0.000000f, 0.786730f, 1.000000);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.275908f, 0.000000f, 0.786730f, 1.000000);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.349020f, 0.000000f, 1.000000f, 0.580392);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.350710f, 0.000000f, 1.000000f, 1.000000);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.300000f, 0.305000f, 0.310000f, 1.000000);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.058824f, 0.062745f, 0.062745f, 1.000000);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.058824f, 0.062745f, 0.062745f, 1.000000);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.109804f, 0.109804f, 0.109804f, 1.000000);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.058824f, 0.062745f, 0.062745f, 1.000000);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.109804f, 0.109804f, 0.109804f, 1.000000);
		style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.260000f, 0.590000f, 0.980000f, 0.700000);
		style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.200000f, 0.200000f, 0.200000f, 1.000000);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.610000f, 0.610000f, 0.610000f, 1.000000);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000000f, 0.430000f, 0.350000f, 1.000000);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.900000f, 0.700000f, 0.000000f, 1.000000);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000000f, 0.600000f, 0.000000f, 1.000000);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.260000f, 0.590000f, 0.980000f, 0.350000);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.000000f, 1.000000f, 0.000000f, 0.905213);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.260000f, 0.590000f, 0.980000f, 1.000000);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000000f, 1.000000f, 1.000000f, 0.700000);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000f, 0.800000f, 0.800000f, 0.200000);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000f, 0.800000f, 0.800000f, 0.350000);
	}
}