#pragma once

#include <Phoenix.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <string>

namespace phx {
	struct LauncherProperties
	{
		std::string InstallPath;
		std::function<void(std::string)> ProjectOpenedCallback;
	};

	class LauncherLayer : public Layer
	{
	public:
		LauncherLayer();
		virtual ~LauncherLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void OnUpdate(phx::DeltaTime dt) override;
		virtual void OnImGuiRender() override;
		void OnEvent(phx::Event& e) override;

		void CreateProject(std::string name, std::string path);
	private:
		int m_MousePrevX;
		int m_MousePrevY;

		int m_WindowPrevX;
		int m_WindowPrevY;

		bool m_Dragging;

		//std::string m_DefaultProjectPath = FileDialogs::GetDocumentsPath() + "\\Phoenix\\Projects\\";

		LauncherProperties m_Properties;
		ImGuiID m_HoveredProjectID;
		Ref<Texture2D> m_PhoenixLogoTexture;
	};
}
