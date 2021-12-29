#pragma once

#include <Phoenix.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <string>

#include <Phoenix/Project/Preferences.h>

namespace phx {
	static std::string GetDateTimeString(const time_t& input_time, const std::locale& loc, char fmt)
	{
		const std::time_put<char>& tmput = std::use_facet <std::time_put<char> >(loc);

		std::stringstream s;
		s.imbue(loc);

		tm time;
		localtime_s(&time, &input_time);
		tmput.put(s, s, ' ', &time, fmt);

		return s.str();
	}

	static std::string FormatDateAndTime(time_t dateTime) { return GetDateTimeString(dateTime, std::locale(""), 'R') + " " + GetDateTimeString(dateTime, std::locale(""), 'x'); }

	struct LauncherProperties
	{
		Ref<UserPreferences> UserPreferences;
		std::string InstallPath;
		//std::function<void(std::string)> ProjectOpenedCallback;
	};

	class LauncherLayer : public Layer
	{
	public:
		LauncherLayer(LauncherProperties launcherProperties);
		virtual ~LauncherLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void OnUpdate(phx::DeltaTime dt) override;
		virtual void OnImGuiRender() override;
		void OnEvent(phx::Event& e) override;

		void CreateProject(std::string name, std::string path);
		void AddProjectToRecents(const std::filesystem::path& projectFile);
		void SerializePrefs();
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
		Ref<Texture2D> m_New;
		Ref<Texture2D> m_Open;
		Ref<Texture2D> m_Settings;
	};
}
